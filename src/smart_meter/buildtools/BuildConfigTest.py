import unittest
import sys
sys.path.append("testfiles")
sys.path.append("variants")
from BuildConfig import *

class BuildConfigTest(unittest.TestCase):
   
   """Test default options (build all, no clean. no tests)."""
   def test_NoExtraParameters(self):
      argc = 3
      argv = ["program_name", "testcomponents", "variant_linux"]	
      conf = BuildConfig(argc, argv)
      self.assertEquals(0, conf.firstComponent)
      self.assertTrue(conf.compileAllComponents)
      self.assertFalse(conf.runTests)
      self.assertTrue(conf.buildComponent)
      self.assertFalse(conf.cleanBeforeBuild)
      self.assertFalse(conf.cleanAll)
      self.assertEquals(conf.variant, "variant_linux")
      self.assertEquals(len(conf.components), 3)

      self.assertEquals("Foo", conf.components[0].name)
      self.assertEquals("Bar", conf.components[1].name)
      self.assertEquals("Baz", conf.components[2].name)

      self.assertEquals("Foo/src", conf.components[0].srcDir)
      self.assertEquals("Bar/src", conf.components[1].srcDir)
      self.assertEquals("Baz/src", conf.components[2].srcDir)

      self.assertIsNone(conf.components[0].testSrcDir)
      self.assertEquals("Bar/tests", conf.components[1].testSrcDir)
      self.assertIsNone(conf.components[2].testSrcDir)

      self.assertIsNone(conf.components[0].testSrcDir)
      self.assertEquals("./builds/Bar/tests/tests", conf.components[1].runTestsCmd)
      self.assertIsNone(conf.components[2].testSrcDir)


   """Test clean-all without building."""
   def test_CleanAllNoBuild(self):
      argc = 4
      argv = ["program_name", "testcomponents", "variant_linux", "clean-all"]	
      conf = BuildConfig(argc, argv)
      self.assertEquals(0, conf.firstComponent)
      self.assertTrue(conf.compileAllComponents)
      self.assertFalse(conf.runTests)
      self.assertFalse(conf.buildComponent)
      self.assertFalse(conf.cleanBeforeBuild)
      self.assertTrue(conf.cleanAll)
      self.assertEquals(conf.variant, "variant_linux")
      self.assertEquals(len(conf.components), 0)


   """Test clean-all and building."""
   def test_CleanAllAndBuild(self):
      argc = 5
      argv = ["program_name", "testcomponents", "variant_linux", "clean-all", "build"]	
      conf = BuildConfig(argc, argv)
      self.assertEquals(0, conf.firstComponent)
      self.assertTrue(conf.compileAllComponents)
      self.assertFalse(conf.runTests)
      self.assertTrue(conf.buildComponent)
      self.assertFalse(conf.cleanBeforeBuild)
      self.assertTrue(conf.cleanAll)
      self.assertEquals(conf.variant, "variant_linux")
      self.assertEquals(len(conf.components), 3)

      self.assertEquals("Foo", conf.components[0].name)
      self.assertEquals("Bar", conf.components[1].name)
      self.assertEquals("Baz", conf.components[2].name)

      self.assertEquals("Foo/src", conf.components[0].srcDir)
      self.assertEquals("Bar/src", conf.components[1].srcDir)
      self.assertEquals("Baz/src", conf.components[2].srcDir)

      self.assertIsNone(conf.components[0].testSrcDir)
      self.assertEquals("Bar/tests", conf.components[1].testSrcDir)
      self.assertIsNone(conf.components[2].testSrcDir)

      self.assertIsNone(conf.components[0].testSrcDir)
      self.assertEquals("./builds/Bar/tests/tests", conf.components[1].runTestsCmd)
      self.assertIsNone(conf.components[2].testSrcDir)


   """Test clean-all and tests."""
   def test_CleanAllAndRunTests(self):
      argc = 5
      argv = ["program_name", "testcomponents", "variant_linux", "clean-all", "test"]	
      conf = BuildConfig(argc, argv)
      self.assertEquals(0, conf.firstComponent)
      self.assertTrue(conf.compileAllComponents)
      self.assertTrue(conf.runTests)
      self.assertFalse(conf.buildComponent)
      self.assertFalse(conf.cleanBeforeBuild)
      self.assertTrue(conf.cleanAll)
      self.assertEquals(conf.variant, "variant_linux")
      self.assertEquals(len(conf.components), 3)

      self.assertEquals("Foo", conf.components[0].name)
      self.assertEquals("Bar", conf.components[1].name)
      self.assertEquals("Baz", conf.components[2].name)

      self.assertEquals("Foo/src", conf.components[0].srcDir)
      self.assertEquals("Bar/src", conf.components[1].srcDir)
      self.assertEquals("Baz/src", conf.components[2].srcDir)

      self.assertIsNone(conf.components[0].testSrcDir)
      self.assertEquals("Bar/tests", conf.components[1].testSrcDir)
      self.assertIsNone(conf.components[2].testSrcDir)

      self.assertIsNone(conf.components[0].testSrcDir)
      self.assertEquals("./builds/Bar/tests/tests", conf.components[1].runTestsCmd)
      self.assertIsNone(conf.components[2].testSrcDir)

   
   """Test clean-all, build and tests."""
   def test_CleanAllAndBuildAndRunTests(self):
      argc = 5
      argv = ["program_name", "testcomponents", "variant_linux", "clean-all", "all"]	
      conf = BuildConfig(argc, argv)
      self.assertEquals(0, conf.firstComponent)
      self.assertTrue(conf.compileAllComponents)
      self.assertTrue(conf.runTests)
      self.assertTrue(conf.buildComponent)
      self.assertFalse(conf.cleanBeforeBuild)
      self.assertTrue(conf.cleanAll)
      self.assertEquals(conf.variant, "variant_linux")
      self.assertEquals(len(conf.components), 3)

      self.assertEquals("Foo", conf.components[0].name)
      self.assertEquals("Bar", conf.components[1].name)
      self.assertEquals("Baz", conf.components[2].name)

      self.assertEquals("Foo/src", conf.components[0].srcDir)
      self.assertEquals("Bar/src", conf.components[1].srcDir)
      self.assertEquals("Baz/src", conf.components[2].srcDir)

      self.assertIsNone(conf.components[0].testSrcDir)
      self.assertEquals("Bar/tests", conf.components[1].testSrcDir)
      self.assertIsNone(conf.components[2].testSrcDir)

      self.assertIsNone(conf.components[0].testSrcDir)
      self.assertEquals("./builds/Bar/tests/tests", conf.components[1].runTestsCmd)
      self.assertIsNone(conf.components[2].testSrcDir)


   """Test clean-all, build and tests with explicit commands."""
   def test_CleanAllAndBuildAndRunTestsExplicit(self):
      argc = 6
      argv = ["program_name", "testcomponents", "variant_linux", "clean-all", "build", "test"]	
      conf = BuildConfig(argc, argv)
      self.assertEquals(0, conf.firstComponent)
      self.assertTrue(conf.compileAllComponents)
      self.assertTrue(conf.runTests)
      self.assertTrue(conf.buildComponent)
      self.assertFalse(conf.cleanBeforeBuild)
      self.assertTrue(conf.cleanAll)
      self.assertEquals(conf.variant, "variant_linux")
      self.assertEquals(len(conf.components), 3)

      self.assertEquals("Foo", conf.components[0].name)
      self.assertEquals("Bar", conf.components[1].name)
      self.assertEquals("Baz", conf.components[2].name)

      self.assertEquals("Foo/src", conf.components[0].srcDir)
      self.assertEquals("Bar/src", conf.components[1].srcDir)
      self.assertEquals("Baz/src", conf.components[2].srcDir)

      self.assertIsNone(conf.components[0].testSrcDir)
      self.assertEquals("Bar/tests", conf.components[1].testSrcDir)
      self.assertIsNone(conf.components[2].testSrcDir)

      self.assertIsNone(conf.components[0].testSrcDir)
      self.assertEquals("./builds/Bar/tests/tests", conf.components[1].runTestsCmd)
      self.assertIsNone(conf.components[2].testSrcDir)


   """Test buiding single component."""
   def test_BuildSingleComponent(self):
      argc = 5
      argv = ["program_name", "testcomponents", "variant_linux", "-c", "Bar"]	
      conf = BuildConfig(argc, argv)
      self.assertEquals(1, conf.firstComponent)
      self.assertFalse(conf.compileAllComponents)
      self.assertFalse(conf.runTests)
      self.assertTrue(conf.buildComponent)
      self.assertFalse(conf.cleanBeforeBuild)
      self.assertFalse(conf.cleanAll)
      self.assertEquals(conf.variant, "variant_linux")
      self.assertEquals(len(conf.components), 3)

      self.assertEquals("Foo", conf.components[0].name)
      self.assertEquals("Bar", conf.components[1].name)
      self.assertEquals("Baz", conf.components[2].name)

      self.assertEquals("Foo/src", conf.components[0].srcDir)
      self.assertEquals("Bar/src", conf.components[1].srcDir)
      self.assertEquals("Baz/src", conf.components[2].srcDir)

      self.assertIsNone(conf.components[0].testSrcDir)
      self.assertEquals("Bar/tests", conf.components[1].testSrcDir)
      self.assertIsNone(conf.components[2].testSrcDir)

      self.assertIsNone(conf.components[0].testSrcDir)
      self.assertEquals("./builds/Bar/tests/tests", conf.components[1].runTestsCmd)
      self.assertIsNone(conf.components[2].testSrcDir)


   def test_ContinueCleanBuild(self):
      argc = 8
      argv = ["program_name", "testcomponents", "variant_linux", "--continue", "-c", "Bar", "clean", "build"]	
      conf = BuildConfig(argc, argv)
      self.assertEquals(1, conf.firstComponent)
      self.assertTrue(conf.compileAllComponents)
      self.assertFalse(conf.runTests)
      self.assertTrue(conf.buildComponent)
      self.assertTrue(conf.cleanBeforeBuild)
      self.assertFalse(conf.cleanAll)
      self.assertEquals(conf.variant, "variant_linux")
      self.assertEquals(len(conf.components), 3)

      self.assertEquals("Foo", conf.components[0].name)
      self.assertEquals("Bar", conf.components[1].name)
      self.assertEquals("Baz", conf.components[2].name)

      self.assertEquals("Foo/src", conf.components[0].srcDir)
      self.assertEquals("Bar/src", conf.components[1].srcDir)
      self.assertEquals("Baz/src", conf.components[2].srcDir)

      self.assertIsNone(conf.components[0].testSrcDir)
      self.assertEquals("Bar/tests", conf.components[1].testSrcDir)
      self.assertIsNone(conf.components[2].testSrcDir)

      self.assertIsNone(conf.components[0].testSrcDir)
      self.assertEquals("./builds/Bar/tests/tests", conf.components[1].runTestsCmd)
      self.assertIsNone(conf.components[2].testSrcDir)


   def test_ContinueAfterCleanTest(self):
      argc = 8
      argv = ["program_name", "testcomponents", "variant_linux", "--continue-after", "-c", "Bar", "clean", "test"]	
      conf = BuildConfig(argc, argv)
      self.assertEquals(2, conf.firstComponent)
      self.assertTrue(conf.compileAllComponents)
      self.assertTrue(conf.runTests)
      self.assertFalse(conf.buildComponent)
      self.assertTrue(conf.cleanBeforeBuild)
      self.assertFalse(conf.cleanAll)
      self.assertEquals(conf.variant, "variant_linux")
      self.assertEquals(len(conf.components), 3)

      self.assertEquals("Foo", conf.components[0].name)
      self.assertEquals("Bar", conf.components[1].name)
      self.assertEquals("Baz", conf.components[2].name)

      self.assertEquals("Foo/src", conf.components[0].srcDir)
      self.assertEquals("Bar/src", conf.components[1].srcDir)
      self.assertEquals("Baz/src", conf.components[2].srcDir)

      self.assertIsNone(conf.components[0].testSrcDir)
      self.assertEquals("Bar/tests", conf.components[1].testSrcDir)
      self.assertIsNone(conf.components[2].testSrcDir)

      self.assertIsNone(conf.components[0].testSrcDir)
      self.assertEquals("./builds/Bar/tests/tests", conf.components[1].runTestsCmd)
      self.assertIsNone(conf.components[2].testSrcDir)


   def test_TooFewArguments(self):
      try:
         argc = 2
         argv = ["program_name", "testcomponents"]	
         conf = BuildConfig(argc, argv)
         self.fail("No exception for invalid arguments")
      except Exception, e:
	 self.assertEquals(e.message, "Wrong number of arguments. Define at least components and variant (in this order).")


   def test_UnknownComponent(self):
      try:
         argc = 5
         argv = ["program_name", "testcomponents", "variant_raspberrypi", "-c", "DoesNotExist"]	
         conf = BuildConfig(argc, argv)
         self.fail("No exception for invalid arguments")
      except Exception, e:
	 self.assertEquals(e.message, "Unknown component 'DoesNotExist'.")


   def test_MissingComponentName(self):
      try:
         argc = 4
         argv = ["program_name", "testcomponents", "variant_raspberrypi", "-c"]	
         conf = BuildConfig(argc, argv)
         self.fail("No exception for invalid arguments")
      except Exception, e:
	 self.assertEquals(e.message, "Invalid arguments: specify component name right after -c option.")


   def test_InvalidComponentName(self):
      for name in ['all', 'build', 'clean', 'test', '-c', '--continue', '--continue-after']:
         try:
            argc = 5
            argv = ["program_name", "testcomponents", "variant_raspberrypi", "-c", name]	
            conf = BuildConfig(argc, argv)
            print(name)
            self.fail("No exception for invalid arguments")
         except Exception, e:
	    self.assertEquals(e.message, "Invalid arguments: specify component name right after -c option.")


   def test_ConflictingOptions(self):
      argc = 5
      for option in ['-c', '--continue', '--continue-after']:
         try:
            argv = ["program_name", "testcomponents", "variant_linux", "clean-all", option]
            conf = BuildConfig(argc, argv)
            print(option)
            self.fail("No exception for invalid arguments.")
         except Exception, e:
            self.assertEquals(e.message, "Invalid arguments: '" + option + "' and 'clean-all' cannot both be used.")



if __name__ == "__main__":
   unittest.main()
