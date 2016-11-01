
import os
import shutil

"""List of components to be built."""
componentList = []

"""Represents single component to be buildt."""
class Component(object):
   
   """ Constructor """
   def __init__(self, name, srcDir, binaries, testSrcDir=None, runTestsCmd=None):
      self.name = name			# Component name.
      self.srcDir = srcDir		# source directory
      self.testSrcDir = testSrcDir	# Test source directory.
      self.runTestsCmd = runTestsCmd  	# run tests command. If empty, tests are not run.
      self.binaries = binaries 		# Binary files to be deployed


   """Build component"""
   def build(self, clean=False, build=True):
      result = 0
      # Clean
      comp_build_dir = os.environ["BUILD_DIR"] + "/" + self.name
      comp_src_dir = os.environ["SOURCE_DIR"] + "/" + self.srcDir
      if clean:
         if os.path.exists(comp_build_dir):
            shutil.rmtree(comp_build_dir)

      # Create build dir and change to source dir
      if not os.path.exists(comp_build_dir):
         os.mkdir(comp_build_dir)
      pwd = os.getcwd()
      os.chdir(comp_src_dir)
      print (comp_src_dir)

      #clean only?
      if not build:
         os.chdir(pwd)
         return True

      # Build
      result = os.system("cmake -H. -B" + comp_build_dir)
      if (result == 0):
         os.chdir(comp_build_dir)
         result = os.system("make")

      os.chdir(pwd)
      return result == 0


   """Run tests."""
   def runTests(self, clean):
      if self.testSrcDir is None or self.runTestCmd is None:
         return True
      result = 0
      if clean:
         os.system("rm -rf " + BUILD_DIR + "/tests")
      result = os.system("cmake " + self.testSrcDir)
      if result == 0:
         os.system("make")
      result = os.system(self.runTestCmd)
      return result == 0
