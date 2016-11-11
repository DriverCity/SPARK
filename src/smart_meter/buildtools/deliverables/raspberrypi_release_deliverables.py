from common_deliverables import *

# Rasperry Pi release -specific deliverables here.

addDeliverable(Deliverable(source="install/raspberrypi/install_raspberrypi_release.sh", target="install_release.sh", excludeFromPkg=True))

addDeliverable(Deliverable(source="install/raspberrypi/uninstall_raspberrypi_release.sh", target="uninstall_release.sh", excludeFromPkg=True))

addDeliverable(Deliverable(source="install/raspberrypi/start_raspberrypi_release.sh", target="start_release.sh", excludeFromPkg=True))

addDeliverable(Deliverable(source="install/raspberrypi/upload_raspberrypi_release.sh", target="upload_raspberrypi_release.sh", excludeFromPkg=True, addExecRights=True))
