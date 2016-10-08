
# User stories
This document contains all the **functional requirements** in the form of user stories. The document does not dive too deep into technical details - it focuses only on what the end users of SPARK can do with the applications and assumes that the implementation itself is merely a technicality that evolves through the project.

If some **required functionality** is missing, please give comments about it. We can discuss about adding them, though we think that the scope of the stories described below is enough for the scope of the project course and what will be implemented during that. If necessary, we may conceptualise (i.e. define the stories) even further than what is presented in this document, even though we would not implement all that.

## Glossary
* ATTENDANT - Parking attendant patrolling parking areas.
* BEACON - BLE-BEACON gadget positioned in cars whose drivers use SPARK. Responds with METADATA about the parking situation of CAR.
* BLE - Bluetooth Low Energy, wireless communication technology. Quite short distances only.
* CAR - A vehicle that gets parked. Either has a BEACON or not.
* CLOUD - Google Cloud Platform receiving data from SMART METERs and providing parking map information to DRIVER APP.
* DRIVER - A person parking CAR and using DRIVER APP.
* DRIVER APP - A mobile application that, at the time of parking CAR, communicates with SMART METER and BEACON respectively.
* GEAR - A handheld smart tool wielded by ATTENDANT. Connected to CLOUD.
* MAP - A functionality in DRIVER APP and GEAR. A Google Maps -map with a heat map overlay that presents the user the predicted availabilities of parking lots.
* METADATA - Used vaguely to stand for “all metadata necessary at this point”. Specified in detail whenever purposeful.
* SMART METER - A parking meter, which accepts payments both traditionally (i.e. the person parking goes at the meter, pays physically with cash and the meter prints a parking permit for the parker) and using SPARK. When requested, sends METADATA about the parking context to DRIVER APP. Sends METADATA about payments to CLOUD.
* SPARK - This application (of many applications). A system consisting of SMART METERS, BEACONS, DRIVER APPS, GEARS and CLOUD.
* TOKEN - A mean of authentication between multiple parties without shared memory. In our context, TOKEN is used for validating parking permit advertised by BEACON, making sure that permits cannot be forged.

## DRIVER: Configuration
1. DRIVER configures his CAR’s register number to the DRIVER APP and bonds the BEACON with the DRIVER APP. DRIVER APP may have multiple sets of register numbers and BEACON bonds, since DRIVER may have multiple CARS. Configuration needs to be only once per CAR-BEACON pair.

## DRIVER: Parking lot availability map
1. DRIVER opens the DRIVER APP and goes to the map view with the DRIVER APP’s menu
2. DRIVER APP pulls from CLOUD a view of Google Maps -map with a custom heat map overlay that has been built based on the latest available parking lot availability information received from SMART METERs and GEARs. This is called the MAP
    * In the overlay, green areas mean that the CLOUD analysis has deduced that in these areas it is likely that there are free parking lots. As the color turns from green to red, the likelihood of finding a free parking lot decreases.
        * These exact colors are mere placeholders. I.e. in later phases they may be changed to some other colors for UX reasons.
3. DRIVER searches through the MAP for a place around which there are likely free parking lots by zooming and moving the MAP view
4. Having an idea about where to park, DRIVER closes the DRIVER APP.

## ATTENDANT: Parking lot availability map
ATTENDANT has access to the same MAP with the GEAR as was described in **DRIVER: Parking lot availability map**.

## DRIVER: Paid outside parking with SPARK
1. DRIVER drives his CAR to a paid outside parking lot and parks his CAR
2. DRIVER exits the CAR and goes to a SMART METER 20 metres away from the CAR
3. DRIVER opens the DRIVER APP on his way to the SMART METER and configures the time he would like to park his CAR for into the DRIVER APP
4. When the DRIVER is at the SMART METER, he confirms the configured time with the DRIVER APP by tapping a “Confirm time”-button on the DRIVER APP. The DRIVER APP connects with the SMART METER by bluetooth and receives METADATA about the parking area. This METADATA would include at least
    * Parking area identifier
    * Parking price per hour
    * The price that the DRIVER has to pay for staying for the configured amount of time.
5. DRIVER APP presents this information to the DRIVER and DRIVER confirms the payment by tapping “Confirm payment”
6. SMART METER sends DRIVER APP a TOKEN that authorises the DRIVER to park his CAR for the allotted amount of time
7. SMART METER sends METADATA about the parking situation to CLOUD. This event is called “parking registration to CLOUD”. The METADATA would include at least
    * Parking area identifier
    * Parking time
    * CAR’s register number
    * TOKEN.
8. DRIVER APP instructs the DRIVER to walk back at his CAR. The DRIVER walks back to his CAR with his smart device in his hand
9. Back at the CAR, the DRIVER taps “Send parking info to car”-button on the DRIVER APP (without necessarily entering the CAR)
10. DRIVER APP sends the TOKEN to the BEACON inside the CAR
11. BEACON receives the TOKEN and starts broadcasting it
12. DRIVER APP informs the DRIVER that parking has been completed
13. DRIVER closes the DRIVER APP and leaves the CAR.

## DRIVER: Parking disc outside parking with SPARK
1. DRIVER drives his CAR to an outside parking disc parking lot and parks the CAR
2. DRIVER opens the DRIVER APP with his smart device, chooses the parking disc parking -option and taps “Park here”
3. DRIVER APP sends a point of time when parking was initiated to the BEACON
4. BEACON begins broadcasting the point of time when parking was initiated
5. DRIVER closes the DRIVER APP and leaves the CAR.

## DRIVER: Paid outside parking without SPARK
1. DRIVER drives his CAR to a paid outside parking lot and parks the CAR
2. DRIVER exists the CAR and goes to the SMART METER
3. DRIVER purchases a ticket from the SMART METER and METADATA about the purchase is sent to CLOUD. This METADATA would include at least
    * Parking area identifier
    * Parking time
4. DRIVER receives a parking ticket from the SMART METER
5. DRIVER walks back to his CAR, puts the parking ticket on the dashboard and leaves the CAR.

## DRIVER: Parking disc parking without SPARK
1. DRIVER drives his CAR to an outside parking disc parking lot and parks the CAR
2. DRIVER sets the parking disc on the windshield
3. DRIVER leaves the CAR.

## ATTENDANT: Paid outside parking
1. ATTENDANT arrives to the parking area
2. ATTENDANT taps “Get parking area information” on the GEAR and GEAR gets METADATA about all the registered-as-parked CARs in the area. A registered-as-parked CAR in this context means a registration of a payment related to a CAR for a certain amount of time. This does not mean that CAR should still be physically in the parking lot, but at least its parking time has not yet exceeded. The METADATA would include at least
    * TOKENs
    * Register numbers of CARs
    * Points of time when parking registration to CLOUD was done
    * Allotted time (i.e. time-spans that were actually paid).
3. ATTENDANT walks by CAR and taps “Scan” button on GEAR
    * IF GEAR receives a broadcasted TOKEN from the BEACON inside the CAR, then the user story continues in **ATTENDANT: Paid outside parking with SPARK 1a**
    * IF NOT, then the user story continues in **ATTENDANT: Paid outside parking without SPARK 1b**.
    
## ATTENDANT: Paid outside parking with SPARK 1a
1. ATTENDANT looks at GEAR.
    * IF CAR’s staying time has exceeded the allotted time, then the user story continues in **ATTENDANT: Paid outside parking with SPARK 2a**
    * IF NOT, then the user story continues in **ATTENDANT: Paid outside parking with SPARK 2b**.

## ATTENDANT: Paid outside parking without SPARK 1b
1. GEAR informs the ATTENDANT that the scanning found no SPARK using CARs
2. ATTENDANT checks if there’s a valid parking permit on the windshield of the CAR
    * IF THERE is, then ATTENDANT continues patrolling.
    * IF NOT, then ATTENDANT fines the CAR and continues patrolling.

## ATTENDANT: Paid outside parking with SPARK 2a
1. GEAR informs the ATTENDANT about CAR’s staying time having exceeded the allotted time and offers the ATTENDANT METADATA about the CAR. This METADATA would include at least
    * Register number
    * Point of time when parking registration to CLOUD was done
    * Allotted time
    * Amount of time exceeded.
2. ATTENDANT checks that the register number of the CAR matches the register number that the GEAR offers, and if they match, the ATTENDANT fines the CAR
3) ATTENDANT continues patrolling.

## ATTENDANT: Paid outside parking with SPARK 2b
1. GEAR informs the ATTENDANT that CAR has not exceeded the allotted time
2. ATTENDANT continues patrolling.

## ATTENDANT: Parking disc outside parking
1. ATTENDANT arrives to the parking area
2. ATTENDANT taps “Get parking area information” on the GEAR and GEAR gets METADATA about the parking area from CLOUD. This METADATA would include at least
    * Parking area identifier
    * Maximum allowed parking time.
3. GEAR informs ATTENDANT that the current parking area is a parking disc area
4. ATTENDANT walks by CAR and taps “Scan” button on GEAR and GEAR sends to CLOUD information about “Scan”-button having been pressed in that parking disc area
    * IF GEAR receives the broadcasted TOKEN from the BEACON inside the CAR, then the user story continues in **ATTENDANT: Parking disc outside parking with SPARK 1a**
    * IF NOT, then the user story continues in **ATTENDANT: Parking disc outside parking without SPARK 1b**.

## ATTENDANT: Parking disc outside parking with SPARK 1a
1. ATTENDANT looks at GEAR.
    * IF CAR’s staying time has exceeded the allotted time, then the user story continues in **ATTENDANT: Parking disc outside parking with SPARK 2a**
    * IF NOT, then the user story continues in **ATTENDANT: Parking disc outside parking with SPARK 2b**.

## ATTENDANT: Parking disc outside parking without SPARK 1b
1. GEAR informs the ATTENDANT that the scanning found no SPARK using CARs
2. ATTENDANT checks if there’s a valid parking disc on the windshield of the CAR
    * IF THERE is, then ATTENDANT continues patrolling.
    * IF NOT, then ATTENDANT fines the CAR and continues patrolling.

## ATTENDANT: Parking disc outside parking with SPARK 2a
1. GEAR informs the ATTENDANT about CAR’s staying time having exceeded the allotted time and offers the ATTENDANT METADATA about the CAR. This METADATA would include at least
    * Allotted time
    * Amount of time exceeded.
2. ATTENDANT fines the CAR and continues patrolling.

## ATTENDANT: Parking disc outside parking with SPARK 2b
1. GEAR informs the ATTENDANT that CAR has not exceeded the allotted time
2. ATTENDANT continues patrolling.
