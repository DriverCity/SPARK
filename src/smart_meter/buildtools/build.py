import sys
sys.path.append("buildtools/variants")
sys.path.append("buildtools/components")
sys.path.append("buildtools/deliverables")

import os
import shutil
from BuildConfig import BuildConfig
import components
import deliverables

BUILD_ROOT = os.environ["BUILD_ROOT"]
BUILD_DIR = os.environ["BUILD_DIR"]
DEPLOY_ROOT = os.environ["DEPLOY_ROOT"]
DEPLOY_DIR = os.environ["DEPLOY_DIR"]
SOURCE_DIR = os.environ["SOURCE_DIR"]
BUILD_NAME = os.environ["BUILD_NAME"]


def main(argc, argv):

   pwd = os.getcwd()

   result = 0
   # Get build configuration
   config = getConfig(argc, argv)
   if config is None:
      result = 1
   else:
      # Build and test
      result = runBuildSteps(config)

   # Deploy
   if result == 0:
      result = deploy(config)

   # Change back to original directory
   os.chdir(pwd)
   return result


""" Read components, options and variants."""
def getConfig(argc, argv):
   config = BuildConfig(argc, argv)
   return config


"""Build all configured components."""
def runBuildSteps(config):

   # Clean-all
   if config.cleanAll:
      print ("Cleaning build and deploy directories...")
      if os.path.exists(BUILD_DIR):
         shutil.rmtree(BUILD_DIR)
      if os.path.exists(DEPLOY_DIR):
         shutil.rmtree(DEPLOY_DIR)
      print ("Done")
      if (not config.compileAllComponents):
         return 0
   
   # Create build dir
   if not os.path.exists(BUILD_ROOT):
      os.makedirs(BUILD_ROOT)
   if not os.path.exists(BUILD_DIR):
      os.makedirs(BUILD_DIR)

   #build 3rd party
   if (config.firstComponent==0 and config.compileAllComponents):
       if build3rdPartyComponents(config) != 0:
          return 1

   # Build components
   for i in range(config.firstComponent, len(config.components)):
      comp = config.components[i]

      print("================================================================================")
      print("Starting component " + comp.name)

      # Run test step (only in linux debug build)
      if (config.runTests and BUILD_NAME == "linux_debug"):
         if not comp.runTests(config.cleanBeforeBuild):
            return 1

      # Build step
      if (config.buildComponent):
         if not comp.build(config.cleanBeforeBuild):
            return 1

      print("Component " + comp.name + " finished")
      print("================================================================================")
      print("\n")

   return 0



def build3rdPartyComponents(config):

   os.environ["SOURCE_DIR"] = SOURCE_DIR + "/../3rd_party"
   for comp in components.thirdParty:
       print("================================================================================")
       print("Starting component " + comp.name)

       # Run test step (only in linux debug build)
       if (config.runTests and BUILD_NAME == "linux_debug"):
          if not comp.runTests(config.cleanBeforeBuild):
             return 1

       # Build step
       if (config.buildComponent):
          if not comp.build(config.cleanBeforeBuild):
             return 1

       print("Component " + comp.name + " finished")
       print("================================================================================")
       print("\n")

   os.environ["SOURCE_DIR"] = SOURCE_DIR
   return 0


"""Copy binary files to deploy directory."""
def deploy(config):
   print("================================================================================")
   print("Deploying...")

   # Create deploy directory structure
   if not os.path.exists(DEPLOY_ROOT):
      os.makedirs(DEPLOY_ROOT)
   if not os.path.exists(DEPLOY_DIR):
      os.makedirs(DEPLOY_DIR)
   if not os.path.exists(DEPLOY_DIR + "/bin"):
      os.makedirs(DEPLOY_DIR + "/bin")

   # Copy binaries.
   for comp in components.thirdParty:
      SOURCE = BUILD_DIR + "/" + comp.name + "/bin/*"
      TARGET = DEPLOY_DIR + "/bin/"
      print ("Copying binaries for " + comp.name)
      os.system("cp " + SOURCE + " " + TARGET)

   # Copy binaries.
   for comp in config.components:
      SOURCE = BUILD_DIR + "/" + comp.name + "/bin/*"
      TARGET = DEPLOY_DIR + "/bin/"
      print ("Copying binaries for " + comp.name)
      os.system("cp " + SOURCE + " " + TARGET)

   # Copy files included to archive.
   copyDeliverables(deliverables.includedToPkg)

   # Create zip for raspberry builds
   if (BUILD_NAME != "linux_debug"):
      createArchive()

   # Copy files excluded from archive.
   copyDeliverables(deliverables.excludedFromPkg)
   return 0


def createArchive():
   print("Creating archive " + BUILD_NAME + ".zip")
   pwd = os.getcwd()
   os.chdir(DEPLOY_DIR)
   os.system("zip -r --symlinks " + BUILD_NAME + ".zip .")
   os.chdir(pwd)


def copyDeliverables(delivList):
   for d in delivList:
      d.deploy()


if __name__ == "__main__":
   result = main(len(sys.argv), sys.argv)
   if result != 0:
      print ("BUILD FAILED!")
   else:
      print ("Build succeeded.")
   sys.exit(result)
