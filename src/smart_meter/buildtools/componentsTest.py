
import unittest
import components

class ComponentsTest(unittest.TestCase):
   
   def test_ConstructorWithDefaultParameters(self):
      c = components.Component("ComponentName")
      self.assertEquals("ComponentName", c.name)
      self.assertIsNone(c.runTestsCmd)
      self.assertEquals(0, len(components.componentList))

   def test_ConstructorWithTestSpec(self):
      c = components.Component("ComponentName", "./ComponentName/tests")
      self.assertEquals("ComponentName", c.name)
      self.assertEquals("./ComponentName/tests", c.runTestsCmd)
      self.assertEquals(0, len(components.componentList))

if __name__ == "__main__":
   unittest.main()
