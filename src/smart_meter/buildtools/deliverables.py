import os

includedToPkg = []
excludedFromPkg = []

class Deliverable(object):
   
   def __init__(self, source, target, excludeFromPkg=False, addExecRights=False):
      self.source = source
      self.target = target
      self.excludeFromPkg = excludeFromPkg
      self.addExecRights = addExecRights


   def deploy(self):
      path = self.target.split('/')
      self._createDirectories(path)
      fullSource = os.environ['CFG_DIR'] + "/" + self.source
      fullTarget = os.environ['DEPLOY_DIR'] + "/" + self.target
      os.system("cp -r " + fullSource + " " + fullTarget)
      if self.addExecRights:
         os.system("chmod a+x " + fullTarget)


   def _createDirectories(self, path):
      baseDir = os.environ['DEPLOY_DIR'] + "/"
      for i in range(len(path)-1):
         baseDir += path[i] + '/'
         if not os.path.exists(baseDir):
            os.makedirs(baseDir)



def addDeliverable(d):
   if (d.excludeFromPkg):
      excludedFromPkg.append(d)
   else:
      includedToPkg.append(d)
