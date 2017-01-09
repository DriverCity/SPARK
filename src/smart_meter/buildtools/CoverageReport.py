
import os
import sys
import shutil
import components
import glob

BUILD_DIR = os.environ["BUILD_DIR"]
REPORT_DIR = BUILD_DIR + "/report"


def getComponents(componentFile):
    sys.path.append("buildtools/components")
    exec("from " + componentFile + " import *")
    return components.componentList


def getTestName(comp):
    return comp.runTestsCmd[2:]


def getAppInfoDir(comp):
    return BUILD_DIR + "/" + comp.name + "/cov"


def getAppInfoFile(comp):
    return BUILD_DIR + "/" + comp.name + "/cov/app.info"


def getComponentBuildDir(comp):
    return BUILD_DIR + "/" + comp.name + "/tests/CMakeFiles/" + getTestName(comp) + ".dir"


def run_lcov(componentList):
    for comp in componentList:

        appInfoDir = getAppInfoDir(comp)
        if os.path.exists(appInfoDir):
            shutil.rmtree(appInfoDir)
        os.mkdir(appInfoDir)

        appInfo = getAppInfoFile(comp)
        os.system("lcov --directory " + getComponentBuildDir(comp) + " --capture --output-file " + appInfo + " --test-name " + comp.name + "_UnitTests" )
        os.system("lcov --directory " + getComponentBuildDir(comp) + " --remove " + appInfo + " c++/\\* --output-file " + appInfo )
        os.system("lcov --directory " + getComponentBuildDir(comp) + " --remove " + appInfo + " \\*src/"+comp.name+"/tests/\\* --output-file " + appInfo )
        os.system("lcov --directory " + getComponentBuildDir(comp) + " --remove " + getAppInfoFile(comp) + " \\*gtest/\\* --output-file " + appInfo )
        os.system("lcov --directory " + getComponentBuildDir(comp) + " --remove " + getAppInfoFile(comp) + " \\*/include/\\* --output-file " + appInfo )


def generate_html(componentList):
    os.chdir(REPORT_DIR)
    appFiles = []
    for comp in componentList:
        appFiles.append(getAppInfoFile(comp))
    os.system("genhtml " + " ".join(appFiles))


if __name__ == "__main__":
    componentList = getComponents( sys.argv[1])
    run_lcov(componentList)
    generate_html(componentList)
    print("Coverage report has been created. Open " + REPORT_DIR + "/index.html in web browser to view it.")
    sys.exit(0)
