from common_deliverables import *

# Rasperry Pi degub -specific deliverables here.

addDeliverable(Deliverable(source="install/raspberrypi/install_raspberrypi_debug.sh", target="install_debug.sh", excludeFromPkg=True))

addDeliverable(Deliverable(source="install/raspberrypi/uninstall_raspberrypi_debug.sh", target="uninstall_debug.sh", excludeFromPkg=True))

addDeliverable(Deliverable(source="install/raspberrypi/start_raspberrypi_debug.sh", target="start_debug.sh", excludeFromPkg=True))

addDeliverable(Deliverable(source="install/raspberrypi/upload_raspberrypi_debug.sh", target="upload_raspberrypi_debug.sh", excludeFromPkg=True, addExecRights=True))
