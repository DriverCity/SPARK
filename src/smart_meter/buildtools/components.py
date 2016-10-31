
import os

BUILD_DIR = "builds"

"""List of components to be built."""
componentList = []

"""Represents single component to be buildt."""
class Component(object):
   
   """ Constructor """
   def __init__(self, name, srcDir, testSrcDir=None, runTestsCmd=None):
      self.name = name			# Component name.
      self.srcDir = srcDir		# source directory
      self.testSrcDir = testSrcDir	# Test source directory.
      self.runTestsCmd = runTestsCmd  	# run tests command. If empty, tests are not run.

   """Build component"""
   def build(self, clean=False):
      if clean:
         os.system("rm -rf " + BUILD_DIR + "/" + self.name)
      os.system("cmake " + self.srcDir)
      os.system("make")


   """Run tests."""
   def runTests(self):
      os.system("cmake " + self.testSrcDir)
      os.system("make")
      os.system(self.runTestCmd)
