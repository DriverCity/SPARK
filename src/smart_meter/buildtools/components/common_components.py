
import sys
sys.path.append("../")
import components

# Add components in order of dependency.

components.componentList.append(components.Component(name="Logger",
                                                     runTestsCmd="./LoggerTest"))

components.componentList.append(components.Component(name="misc",
                                                     runTestsCmd="./misctest"))

components.componentList.append(components.Component(name="GlobalConfiguration",
                                                     runTestsCmd="./GlobalConfigurationTest"))

components.componentList.append(components.Component(name="ParkingDatabase",
                                                     runTestsCmd=None))

components.componentList.append(components.Component(name="CloudService",
                                                     runTestsCmd=None))

components.componentList.append(components.Component(name="application",
                                                     runTestsCmd=None))