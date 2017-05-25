# Demo mode

Smart meter has a demo mode that is intended to make demonstrating the product and system-level testing easier. 
Demo mode allows creating situations that would be hard to create otherwise (e.g. connection to cloud is lost between 
price request and parking event registering). Demo mode is to be used in development only.
This document instructs how to configure, enable and use demo mode.

## Enable demo mode

To enable demo mode, add line **demoMode;1** to smart meter configuration (config/global.txt). To disable demo mode, 
change this to **demoMode;0** or remove the line.

## Configure demo mode

Demomode has it's own configuration file, that will be installed to **config/demoConfig.txt** in deploy directory after 
successful build. Edit this file to change demo mode settings. The configuration file consists of following lines:

- Empty lines, which are ignored.
- Comments starting with '#', which are ignored.
- Price specification.
- Key-value pairs separated with semicolon.

Price specification sets a predefined value for parking price information. Give price information in format **price;price per hour (EUR):time limit (min):resolution (min)**,
e.g. **price;1.2:120:1**. If price information is left unspecified, price is fetched from firebase like normally.

Every other key-value pair will specify the number of parking event registering request and it's demonstrated value. The key
represents the number of parking registering request. The first parking event reqistering request is indexed as '0', 
and each request after that will increase the index by one. Not all requests need to be defined. 
If an index is missing from configuration, smart meter will handle requests as in normal mode.

The second part (value) is an integer representing the situation to be demonstrated on request specified by the key.
The possible values are listed in the table below.

Value | Explanation
---|---
0 | Replace the actual payment token in request with a valid test token. Request is then forwarded to the cloud, and handled as normally.
1 | Request to the cloud times out. The request is not actually sent to the cloud, but smart meter will write a timeout error message to BLENode's response fifo.
2 | Payment token is invalid. The request is not actually sent to the cloud, but smart meter will write invalid token error message to BLENode's response fifo.
3 | Other error in the cloud. The request is not actually sent to the cloud, but smart meter will write error message to BLENode's response fifo.
4 | Forced Ok response. The request is not actually sent to the cloud, but smart meter will write OK message to BLENode's response fifo.
any other | Unknown command. Smart meter responds with error message about unknown command.

Example configuration:
```
# My demo config (config/demoConfig.txt)

# Price is not fetched from cloud.
price;1.2:120:1

# First request's payment token will be replaced with a valid test token
0;0

# Second request will time out.
1;1

# Third request is not defined, and is therefore handled normally.

# Fourh request will have an invalid payment token, even if the actual token was valid.
3;2

# End of configuration. All requests after 4th are handled as normally.
```

## Using demo mode

Other than separate configuration, demo mode needs no user guidance. Use smart meter as normally. Requests defined in 
configuration will be handled according to specified action, and others are handled normally.

