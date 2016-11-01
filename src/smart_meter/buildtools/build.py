import sys
sys.path.append("buildtools/variants")
sys.path.append("buildtools/components")

import os
import shutil
from BuildConfig import BuildConfig
import components

BUILD_ROOT = os.environ["BUILD_ROOT"]
BUILD_DIR = os.environ["BUILD_DIR"]
DEPLOY_ROOT = os.environ["DEPLOY_ROOT"]
DEPLOY_DIR = os.environ["DEPLOY_DIR"]
SOURCE_DIR = os.environ["SOURCE_DIR"]


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
   config = None
   try:
      config = BuildConfig(argc, argv)
   except Exception, e:
      print("Build configuration error!")
      print(e.message)
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

   # Build components
   for i in range(config.firstComponent, len(config.components)):
      comp = config.components[i]

      # Run test step (only in linux debug build)
      if (config.runTests and os.environ["BUILD_NAME"] == "linux_debug"):
         if not comp.runTests(config.cleanBeforeBuild):
            return 1

      # Build step
      if (config.buildComponent):
         if not comp.build(config.cleanBeforeBuild):
            return 1

   return 0



"""Copy binary files to deploy directory."""
def deploy(config):
   # Create deploy directory structure
   if not os.path.exists(DEPLOY_ROOT):
      os.makedirs(DEPLOY_ROOT)
   if not os.path.exists(DEPLOY_DIR):
      os.makedirs(DEPLOY_DIR)
   if not os.path.exists(DEPLOY_DIR + "/bin"):
      os.makedirs(DEPLOY_DIR + "/bin")

   # Copy binaries.
   for comp in config.components:
      for binary in comp.binaries:
         SOURCE = BUILD_DIR + "/" + comp.name + "/" + binary
         TARGET = DEPLOY_DIR + "/bin/" + binary
         print("Deploying "+ binary)
         os.system("cp " + SOURCE + " " + TARGET)

   return 0


if __name__ == "__main__":
   result = main(len(sys.argv), sys.argv)
   if result != 0:
      print ("BUILD FAILED!")
   else:
      print ("Build succeeded.")
   sys.exit(result)
