
import sys
sys.path.append("../")
import components

# Add components in order of dependency.

components.componentList.append(components.Component(name="Logger",
                                                     runTestsCmd="./LoggerTest"))

components.componentList.append(components.Component(name="GlobalConfiguration",
                                                     runTestsCmd="./GlobalConfigurationTest"))

components.componentList.append(components.Component(name="application",
                                                     runTestsCmd=None))
