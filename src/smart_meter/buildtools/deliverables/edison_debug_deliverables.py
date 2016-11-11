
from common_deliverables import *

addDeliverable(Deliverable(source="install/edison/install_edison_debug.sh", target="install_debug.sh", excludeFromPkg=True))

addDeliverable(Deliverable(source="install/edison/uninstall_edison_debug.sh", target="uninstall_debug.sh", excludeFromPkg=True))

addDeliverable(Deliverable(source="install/edison/start_edison_debug.sh", target="start_debug.sh", excludeFromPkg=True))

addDeliverable(Deliverable(source="install/edison/upload_edison_debug.sh", target="upload_edison_debug.sh", excludeFromPkg=True, addExecRights=True))
