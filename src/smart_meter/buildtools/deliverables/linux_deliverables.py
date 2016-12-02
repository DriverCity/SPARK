
from common_deliverables import *

# Linux-specific deliverables here.
addDeliverable(Deliverable(source="install/linux/install_linux.sh", target="install.sh", excludeFromPkg=True, addExecRights=True))
