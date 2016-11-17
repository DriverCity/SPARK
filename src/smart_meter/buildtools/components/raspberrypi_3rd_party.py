import sys
sys.path.append("../")
import components

components.thirdParty.append(components.Component(name="sqlite3",
                                                     runTestsCmd=None))
