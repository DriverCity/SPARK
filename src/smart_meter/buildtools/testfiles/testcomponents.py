
import sys
sys.path.append("../")
import components

components.componentList.append(components.Component("Foo"))

components.componentList.append(components.Component("Bar", "./builds/Bar/tests/tests"))

components.componentList.append(components.Component("Baz"))
