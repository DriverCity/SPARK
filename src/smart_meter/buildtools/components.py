
import os

BUILD_DIR = "builds"

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
   def build(self, clean=False):
      result = 0
      if clean:
         os.system("rm -rf " + BUILD_DIR + "/" + self.name)
      result = os.system("cmake " + self.srcDir)
      if (result == 0):
         result = os.system("make")
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
