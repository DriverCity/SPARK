import sys
import os
sys.path.append("../")
from components import addComponent, Component

addComponent( Component(name="sqlite3", srcDir=os.environ["SOURCE_DIR"]+"/../3rd_party/sqlite3/src") )

addComponent( Component(name="openssl", srcDir=os.environ["SOURCE_DIR"]+"/../3rd_party/openssl") )

addComponent( Component(name="curl", srcDir=os.environ["SOURCE_DIR"]+"/../3rd_party/curl") )

exec('from common_components import *')

# Raspberry Pi specific components here:
