import os

IGNORED_FILES = [".pyc", "~"]

def createQtProject(files, includes, projFileName):
   proFile = open(projFileName, 'w')

   proFile.write("INCLUDEPATH += \\ \n")
   for i in includes:
      proFile.write(i + "\\ \n")
   proFile.write("\n")

   proFile.write("SOURCES += \\ \n")
   for f in files:
      proFile.write(f + " \\ \n")



def findFiles(directory, includes):
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
         if fullPath.endswith("include"):
            includes.append(fullPath)
         files += findFiles(fullPath, includes)
   
   return files


if __name__ == "__main__":
   files = []

   includes = ["builds/linux_debug/include",
               "builds/raspberrypi_debug/include",
               "builds/raspberrypi_release/include"]

   files += findFiles( os.path.join(os.getcwd(), "src"), includes )
   files += findFiles( os.path.join(os.getcwd(), "buildtools"), includes )
   files += findFiles( os.path.join(os.getcwd(), "config"), includes)
   files += findFiles( os.path.join(os.getcwd(), "3rd_party"), includes)
   createQtProject(files, includes, os.path.join(os.getcwd(), "qt.pro") )

