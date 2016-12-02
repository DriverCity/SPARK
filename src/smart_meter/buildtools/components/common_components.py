
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
                                                     runTestsCmd="./ParkingDatabaseTest"))

components.componentList.append(components.Component(name="CloudService",
                                                     runTestsCmd=None))

components.componentList.append(components.Component(name="PriceProvider",
                                                     runTestsCmd="./PriceProviderTest"))

components.componentList.append(components.Component(name="VerifyParking",
                                                     runTestsCmd="./VerifyParkingTest"))

components.componentList.append(components.Component(name="BLEService",
                                                     runTestsCmd="./BLEServiceTest"))

components.componentList.append(components.Component(name="application",
                                                     runTestsCmd=None))

components.componentList.append(components.Component(name="FakeBLETool",
                                                     runTestsCmd=None))
