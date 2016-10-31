
import sys
sys.path.append("../")
import components

components.componentList.append(components.Component("Foo", "Foo/src"))

components.componentList.append(components.Component("Bar", "Bar/src", "Bar/tests", "./builds/Bar/tests/tests"))

components.componentList.append(components.Component("Baz", "Baz/src"))
