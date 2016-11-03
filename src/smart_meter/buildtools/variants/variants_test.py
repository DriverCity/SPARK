import os
import unittest

class VariantsTest(unittest.TestCase):
   
   def __init__(self, testCaseNames):
      super(VariantsTest, self).__init__(testCaseNames)


   def test_common_variants(self):
      exec('from common_variants import *')
      self.assertEquals(CFLAGS, 'CFLAGS')
      self.assertEquals(CXXFLAGS, 'CXXFLAGS')
      self.assertEquals(CC, 'CC')
      self.assertEquals(CXX, 'CXX')
      self.assertEquals(os.environ[CFLAGS], '-Wall -Werror')
      self.assertEquals(os.environ[CXXFLAGS], '-Wall -Werror -std=c++11')
      self.assertIsNone(os.environ.get(CC))
      self.assertIsNone(os.environ.get(CXX), '')


   def test_linux_variant(self):
      exec('from variant_linux import *')
      self.assertEquals(CFLAGS, 'CFLAGS')
      self.assertEquals(CXXFLAGS, 'CXXFLAGS')
      self.assertEquals(CC, 'CC')
      self.assertEquals(CXX, 'CXX')
      self.assertEquals(os.environ[CFLAGS], '-Wall -Werror')
      self.assertEquals(os.environ[CXXFLAGS], '-Wall -Werror -std=c++11')
      self.assertEquals(os.environ[CC], '/usr/bin/gcc')
      self.assertEquals(os.environ[CXX], '/usr/bin/g++')

   def test_raspberrypi_variant(self):
      exec('from variant_raspberrypi import *')
      self.assertEquals(CFLAGS, 'CFLAGS')
      self.assertEquals(CXXFLAGS, 'CXXFLAGS')
      self.assertEquals(CC, 'CC')
      self.assertEquals(CXX, 'CXX')
      self.assertEquals(os.environ[CFLAGS], '-Wall -Werror')
      self.assertEquals(os.environ[CXXFLAGS], '-Wall -Werror -std=c++11')
      self.assertEquals(os.environ[CC], os.environ['HOME'] + '/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-gcc')
      self.assertEquals(os.environ[CXX], os.environ['HOME'] + '/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-g++')



if __name__ == "__main__":
   unittest.main()
