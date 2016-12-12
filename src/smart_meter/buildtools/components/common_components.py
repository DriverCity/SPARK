
import os
import sys
sys.path.append("../")
from components import addComponent, Component

# Add components in order of dependency.

addComponent(Component(name="Logger", runTestsCmd="./LoggerTest"))

addComponent( Component(name="misc", runTestsCmd="./misctest") )

addComponent( Component(name="GlobalConfiguration", runTestsCmd="./GlobalConfigurationTest") )

addComponent( Component(name="ParkingDatabase", runTestsCmd="./ParkingDatabaseTest") )

addComponent( Component(name="CloudService", runTestsCmd="./CloudServiceTest") )

addComponent( Component(name="PriceProvider", runTestsCmd="./PriceProviderTest") )

addComponent( Component(name="VerifyParking", runTestsCmd="./VerifyParkingTest") )

addComponent( Component(name="BLEService", runTestsCmd="./BLEServiceTest") )

addComponent( Component(name="application", runTestsCmd=None) )

addComponent( Component(name="FakeBLETool", runTestsCmd=None) )

addComponent( Component(name="BLENode", runTestsCmd=None) )

addComponent( Component(name="pkg", srcDir=os.environ["SOURCE_DIR"]+"/pkg") )
