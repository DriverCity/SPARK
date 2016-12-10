
import components

"""Class containing build configuration."""
class BuildConfig(object):

   """Constructor. Reads configuration from commandline arguments."""
   def __init__(self, argc, argv):
      self.firstComponent = 0		# Index of first compiled component in component list.
      self.compileAllComponents = True  # Should all following components be compiled
      self.runTests = False 		# Should tests be run during build?
      self.buildComponent = True	# Should component it self be built.
      self.cleanBeforeBuild = False	# Should component be cleaned before building?
      self.cleanAll = False		# Should whole project be cleaned before build?
      self.variant = ""			# Variant file
      self.components = []		# Components list.
      self.readArguments(argc, argv)


   """Read build configuration from commandline arguments."""
   def readArguments(self, argc, argv):

      # Args should contain at least component file and variant file.
      if argc < 4:
         raise Exception("Wrong number of arguments. Define at least components and variant (in this order).")
      
      # Get components
      componentFile = argv[1]
      exec('from ' + componentFile + ' import *')
      self.components = components.componentList
      print ("Components read.")

      # Get variant
      self.variant = argv[2]
      exec('from ' + self.variant + ' import *')
      print ("Variant read.")

      #Get deliverables
      exec('from ' + argv[3] + ' import *')
      print ("Deliverables read.")

      # Check for conflicting options
      for option in ['-c', '--continue', '--continue-after']:
         if 'clean-all' in argv and option in argv:
            raise Exception("Invalid arguments: '" + option + "' and 'clean-all' cannot both be used.")
      
      # Clean-all step.
      if 'clean-all' in argv:
         self.cleanAll = True
         if not ('all' in argv or 'build' in argv or 'test' in argv):
            # only clean.
            self.buildComponent = False
            self.components = []
            return
      
      # clean component step
      if 'clean' in argv:
         self.cleanBeforeBuild = True
      
      # build and run tests.
      if 'test' in argv or 'all' in argv:
         self.runTests = True

      # Only build and run tests.
      if 'test' in argv and ('build' not in argv and 'all' not in argv):
         self.buildComponent = False
      
      # Only clean
      if ('clean' in argv and not('build' in argv or 'test' in argv or 'all' in argv)):
         self.buildComponent = False

      # Compile all components?
      if '-c' in argv and not ('--continue' in argv or '--continue-after' in argv):
         self.compileAllComponents = False

      # Start from component...
      if '-c' in argv:
         self.findFirstComponent(argc, argv)


   """Find index of first component to be compiled based of commandline arguments and component list."""
   def findFirstComponent(self, argc, argv):
      compIndex = argv.index('-c') + 1
      first = ""
      if compIndex < argc:
         first = argv[compIndex]
      
      # Illegal component names
      if first in ['', 'build', '-c', 'clean', '--continue', '--continue-after', 'test', 'all']:
         raise Exception("Invalid arguments: specify component name right after -c option.")

      # Find index from component list.
      for c in self.components:
         if c.name == first:
            break
         self.firstComponent += 1
      
      if self.firstComponent == len(self.components):
         raise Exception("Unknown component '" + first + "'.")

      if '--continue-after' in argv:
         self.firstComponent += 1
      elif '--continue' not in argv:
         self.components = [self.components[self.firstComponent]]
         self.firstComponent = 0
         


