
import sys
sys.path.append("../")
from deliverables import *

addDeliverable(Deliverable(source="global.txt", target="config/global.txt"))

addDeliverable(Deliverable(source="../src/BLENode/src", target="bin/BLENode"))
