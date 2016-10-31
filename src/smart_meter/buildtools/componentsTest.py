
import unittest
import components

class ComponentsTest(unittest.TestCase):
   
   def test_ConstructorWithDefaultParameters(self):
      c = components.Component("ComponentName", "ComponentName/src")
      self.assertEquals("ComponentName", c.name)
      self.assertEquals("ComponentName/src", c.srcDir)
      self.assertIsNone(c.testSrcDir)
      self.assertIsNone(c.runTestsCmd)
      self.assertEquals(0, len(components.componentList))

   def test_ConstructorWithTestSpec(self):
      c = components.Component("ComponentName", "ComponentName/src", "ComponentName/tests", "./ComponentName/tests")
      self.assertEquals("ComponentName", c.name)
      self.assertEquals("ComponentName/src", c.srcDir)
      self.assertEquals("ComponentName/tests", c.testSrcDir)
      self.assertEquals("./ComponentName/tests", c.runTestsCmd)
      self.assertEquals(0, len(components.componentList))

if __name__ == "__main__":
   unittest.main()
