import os

IGNORED_FILES = [".pyc", "~"]

def createQtProject(files, projFileName):
   proFile = open(projFileName, 'w')
   proFile.write("SOURCES += \\ \n")

   for f in files:
      proFile.write(f + " \\ \n")



def findFiles(directory):
   files = []
   entries = os.listdir(directory)
   subdirs = []
   
   for e in entries:
      fullPath = os.path.join(directory, e)
      if os.path.isfile(fullPath):
         addToFiles = True
         for ignored in IGNORED_FILES:
            if e.endswith(ignored):
               addToFiles = False
               break
         
         if addToFiles:
            files.append(fullPath)
      
      else:
         files += findFiles(fullPath)
   
   return files


if __name__ == "__main__":
   files = [".gitignore", "build_linux_debug.sh", "build_raspberrypi_debug.sh",
            "build_raspberrypi_release.sh", "QtCreatorProject.sh"]
   files += findFiles( os.path.join(os.getcwd(), "src") )
   files += findFiles( os.path.join(os.getcwd(), "buildtools") )
   createQtProject(files, os.path.join(os.getcwd(), "qt.pro") )

