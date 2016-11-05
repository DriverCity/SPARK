
import sys
sys.path.append("../")
import components

# Add components in order of dependency.

components.componentList.append(components.Component("Logger", "./LoggerTest"))

components.componentList.append(components.Component("application"))
