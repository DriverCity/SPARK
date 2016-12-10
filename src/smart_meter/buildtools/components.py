
import os
import shutil

"""List of components to be built."""
componentList = []

"""Represents single component to be buildt."""
class Component(object):
   
   """ Constructor """
   def __init__(self, name, srcDir=None, runTestsCmd=None):
      self.name = name			# Component name.
      self.runTestsCmd = runTestsCmd  	# run tests command. If empty, tests are not run.
      self.srcDir = srcDir
      if srcDir is None:
         self.srcDir = os.environ["SOURCE_DIR"] + "/" + name + "/src"



   """Build component"""
   def build(self, clean=False, build=True):
      result = 0

      # Clean
      comp_build_dir = os.environ["BUILD_DIR"] + "/" + self.name
      if clean:
         if os.path.exists(comp_build_dir):
            shutil.rmtree(comp_build_dir)

      # Create build dir and change to source dir
      if not os.path.exists(comp_build_dir):
         os.mkdir(comp_build_dir)
      pwd = os.getcwd()
      os.chdir(self.srcDir)

      #clean only?
      if not build:
         os.chdir(pwd)
         return True

      # Build
      print("Building component " + self.name)
      result = os.system("cmake -H. -B" + comp_build_dir)
      if (result == 0):
         os.chdir(comp_build_dir)
         result = os.system("make")

      # Install
      result = os.system("make install")

      os.chdir(pwd)
      if result==0:
         print("Building component " + self.name + " succeeded.")
      return result == 0


   """Run tests."""
   def runTests(self, clean):
      print("Building tests for " + self.name)

      comp_build_root = os.environ["BUILD_DIR"] + "/" + self.name
      if not os.path.exists(comp_build_root):
         os.mkdir(comp_build_root)

      comp_test_dir = comp_build_root + "/tests"
      if self.runTestsCmd is None:
         return True
      result = 0
      if clean:
         if os.path.exists(comp_test_dir):
            shutil.rmtree(comp_build_root + "/tests")

      if not os.path.exists(comp_test_dir):
         os.mkdir(comp_test_dir)
      pwd = os.getcwd()
      os.chdir(comp_test_dir)
      result = os.system("cmake " + os.environ["SOURCE_DIR"] + "/" + self.name + "/tests")

      if result == 0:
         result = os.system("make")

      if result == 0:
          # Copy test files.
          os.system("cp -r " + os.environ["SOURCE_DIR"] + "/" + self.name + "/tests/data " + comp_test_dir)


      if result == 0:
         print("Building tests succeeded. Running tests for " + self.name)
         result = os.system(self.runTestsCmd)
         if result==0:
            print("Completed tests for " + self.name)

      os.chdir(pwd)
      return result == 0


def addComponent(comp):
   componentList.append(comp)
