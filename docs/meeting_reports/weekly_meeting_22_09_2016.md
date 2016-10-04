Meeting report 22.9 Customer

General decisions:
Use Agilefant for project and sprint management.

Things to solve in near future:
Connect Gitter to repo (Perttu and Bill?)
Access to Google Cloud (Joona)
Shopping list before October

Decisions related to requirements:
Give examples of parking contexts in the requirements document (photos)
The project will not cover free parking
The fining process is not in the scope of our project.

Paid parking context decisions:
Driver:
Meter does not have to be a meter. Can be replaced by any device (e.g a beacon) in the parking area
There will be no beacons on every sign (We cannot locate a specific parking lot by using beacons on every parking lot)
The parking fee is paid when parking
During parking there will be no connection to the cloud from the driver app 
-But the meter may be connected to cloud
Meter is assumed to be connected to cloud at least asynchronously, sending batches to cloud
During parking the driver app communicates to meter
What is the role of the beacon at when parking?
 We can use tokens: Meter gives a token to app, App gives the token to the beacon and beacon broadcasts it. Tokens would make the validity checking easier.
We need to make sure car is parked before the payment. (Token helps with this).
The beacon should send the signal until it runs out of battery.
What happens if the car leaves before the time expires? What if the car goes to another parking area?


Attendant:
Goal is to find out which car has exceeded the time limit.
Finds cars by chance
Attendants app communicates with the meter and the beacon
How to find out which car is broadcasting?
Signal strength not trustworthy
Token
Register plate
Read documents given beforehand


Parking disc decisions:
Includes the app and the beacon. There will be no meter.
Of course there could still be a some kind of proxy (i.e. not a meter but some much simpler gateway) if one wanted to gather parking information to cloud for better accuracy on the app’s map, but still, it is not mandatory
How to prove violations?
Two attendants prove by signing?
We will get no data

Cloud:
Cloud only gets data from paid parking contexts
If needed, it can be assumed that ticket information without personally identifiable information is available

Before next meeting:
Update the story and the component map. Take pictures of example parking lots for each context.
