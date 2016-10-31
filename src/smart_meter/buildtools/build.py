import sys
import os
from BuildConfig import BuildConfig
import components

BUILD_DIR = os.environ["BUILD_DIR"]
DEPLOY_DIR = os.environ["DEPLOY_DIR"]
SOURCE_DIR = os.environ["SOURCE_DIR"]


def main(argc, argv):
   result = 0
   # Get build configuration
   config = getConfig(argc, argv)
   if config is None:
      result = 1
   else:
      # Build
      result = runBuildSteps(config)

   # Deploy
   if result == 0:
      result = deploy(config)

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
      os.system("rm -rf " + BUILD_DIR)
      os.system("rm -rf " + DEPLOY_DIR)
      if (not config.compileAllComponents):
         return 0
   
   # Build components
   for i in range(config.firstComponent, len(config.components)):
      comp = config.components[i]

      # Run test step (skip in cross-compilation)
      if (config.runTests and config.variant != "variant_raspberrypi"):
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
   os.system("mkdir " + DEPLOY_DIR)
   os.system("mkdir " + DEPLOY_DIR + "/bin")

   # Copy binaries.
   for comp in config.components:
      for binary in comp.binaries:
         SOURCE = BUILD_DIR + "/" + comp.name + "/" + binary
         TARGET = DEPLOY_DIR + "/bin/" + binary
         os.system("cp " + SOURCE + " " + TARGET)

   return 0


if __name__ == "__main__":
   result = main(len(sys.argv), sys.argv)
   if result != 0:
      print ("BUILD FAILED!")
   else:
      print ("Build succeeded.")
   sys.exit(result)
