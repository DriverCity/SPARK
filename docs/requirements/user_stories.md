
# User stories
This document contains all the **functional requirements** in the form of user stories. The document does not dive too deep into technical details - it focuses only on what the end users of SPARK can do with the applications and assumes that the implementation itself is merely a technicality that evolves through the project.

If some **required functionality** is missing, please give comments about it. We can discuss about adding them, though we think that the scope of the stories described below is enough for the scope of the project course and what will be implemented during that. If necessary, we may conceptualise (i.e. define the stories) even further than what is presented in this document, even though we would not implement all that.

## Priorisation
As mentioned, this document contains only the user stories related to **the required functionality**. The stories may refer to sections that are not required by the customer, but would be necessary for the product to be actually functional and usable in practice. Those sections can be found in the document **additional_functionalities.md**, namely:
* DRIVER: Paid outside parking without SPARK
* ATTENDANT: Paid outside parking without SPARK 1b
* ATTENDANT: Parking disc outside parking without SPARK 1b

## Glossary
* ATTENDANT - Parking attendant patrolling parking areas.
* BEACON - BLE-BEACON gadget positioned in cars whose drivers use SPARK. Always broadcasts CAR's register number configured by DRIVER.
* BLE - Bluetooth Low Energy, wireless communication technology. Quite short distances only.
* CAR - A vehicle that gets parked. Either has a BEACON or not.
* CLOUD - Google Cloud Platform that receives data from SMART METERs and DRIVER APP and provides MAP information to DRIVER APP.
* DRIVER - A person parking CAR and using DRIVER APP.
* DRIVER APP - A mobile application used by DRIVER. Communicates with SMART METER in the paid outside parking context and with CLOUD in the parking disc context. Displays MAP.
* GEAR - A handheld smart tool wielded by ATTENDANT. Connected to CLOUD. Displays MAP.
* MAP - A functionality in DRIVER APP and GEAR. A Google Maps -map with a heat map overlay that presents the user the predicted availabilities of parking lots.
* METADATA - Used vaguely to stand for “all metadata necessary at this point”. Specified in detail whenever purposeful.
* SMART METER - A parking meter which accepts payments both traditionally (i.e. the person parking goes at the meter, pays physically with cash and the meter prints a parking permit for the parker) and using SPARK. When requested, sends METADATA about the parking context to DRIVER APP. Sends METADATA about payments to CLOUD.
* SPARK - This very system (of many applications). A system consisting of SMART METERS, BEACONS, DRIVER APPS, GEARS and CLOUD.

## DRIVER: Configuration
1. DRIVER configures his CAR’s register number to the DRIVER APP and bonds the BEACON with the DRIVER APP. DRIVER APP may have multiple sets of register numbers and BEACON bonds, since DRIVER may have multiple CARS. Likewise, BEACONS may be bonded with multiple DRIVER APPs, since multiple people may be using the same car. DRIVER APP configuration needs to be done only once per CAR-BEACON pair.

## DRIVER: Parking lot availability map
1. DRIVER opens the DRIVER APP and goes to the map view with the DRIVER APP’s menu
2. DRIVER APP pulls a view of Google Maps -map from CLOUD with a custom heat map overlay that has been built based on the latest available parking lot availability information received from SMART METERs, GEARs and DRIVER APPs. This is called the MAP
    * In the overlay, green areas mean that the CLOUD analysis has deduced that in these areas it is likely that there are free parking lots. As the color turns from green to red, the likelihood of finding a free parking lot decreases.
        * These exact colors are mere placeholders. I.e. in later phases they may be changed to some other colors for UX reasons.
3. DRIVER searches through the MAP for a place around which there would be free parking lots by zooming and moving the MAP view
4. Having an idea about where to park, DRIVER closes the DRIVER APP.

## ATTENDANT: Parking lot availability map
ATTENDANT has access to the same MAP with the GEAR as was described in **DRIVER: Parking lot availability map**.

## DRIVER: Paid outside parking with SPARK
1. DRIVER drives his CAR to a paid outside parking lot and parks his CAR. BEACON is broadcasting CAR's register number.
2. DRIVER exits the CAR and goes to a SMART METER 20 metres away from the CAR
3. DRIVER opens the DRIVER APP on his way to the SMART METER and configures the time he would like to park his CAR for into the DRIVER APP
4. When the DRIVER is at the SMART METER, he confirms the configured time with the DRIVER APP by tapping a “Confirm time”-button on the DRIVER APP. The DRIVER APP connects with the SMART METER by bluetooth and receives METADATA about the parking area. This METADATA would include at least
    * Parking area identifier
    * Parking price per hour
    * The price that the DRIVER has to pay for staying for the configured amount of time.
5. DRIVER APP presents this information to the DRIVER and DRIVER confirms the payment by tapping “Confirm payment”
6. SMART METER sends METADATA about the parking situation to CLOUD. This event is called “parking registration to CLOUD”. Connection to CLOUD is secured. The parking event gets put into a message queue on CLOUD, thus providing the SMART METER a near immediate response. The METADATA would include at least
    * Parking area identifier
    * Parking time
    * CAR’s register number.
9. DRIVER APP instructs the DRIVER that the payment has been completed.
10. DRIVER closes the DRIVER APP and leaves the parking area.

## DRIVER: Parking disc outside parking with SPARK
1. DRIVER drives his CAR to an outside parking disc parking lot and parks the CAR. BEACON is broadcasting CAR's register number.
2. DRIVER opens the DRIVER APP with his smart device, chooses the parking disc parking -option and taps “Park here”
3. DRIVER APP sends METADATA to CLOUD. METADATA would include at least
   * CAR's register number.
4. CLOUD registers that the CAR (with the given register number) has been parked into a parking disc area at the time when the message with the METADATA was received. DRIVER APP instructs the DRIVER that the registration has been completed.
5. DRIVER closes the DRIVER APP and leaves the CAR.

## ATTENDANT: Paid outside parking
1. ATTENDANT arrives to the parking area and chooses the current parking area name from a list of parking area names
2. ATTENDANT navigates to the register number list view on GEAR. Register number list view shows each register number that GEAR can hear being broadcasted by BEACONs in the chosen area. The register numbers are sorted in an alphabetical order. Each heard register number is checked with CLOUD, making CLOUD provide METADATA about each CAR. The METADATA would include at least
    * Point of time when parking registration to CLOUD was done
    * Allotted time (i.e. time-span that was actually paid for).
3. GEAR visualises the list to the ATTENDANT in such a way, that
    * Valid parkings are highlighted with green color
    * Invalid parkings (i.e. allotted time has been surpassed) are highlighted with red color.
4. ATTENDANT patrols through the area. For each CAR, ATTENDANT compares the register number on CAR's license plate to the register numbers in the register number list view, although, ATTENDANT may at the same time check if there is a traditional parking permit on the windshield of CAR.
    * IF a traditional parking permit exists or CAR's register number is not found on the list, then the user story continues in **ATTENDANT: Paid outside parking without SPARK 1b**.
    * IF NOT, then the user story continues in **ATTENDANT: Paid outside parking with SPARK 1a**
    
## ATTENDANT: Paid outside parking with SPARK 1a
1. ATTENDANT looks at GEAR
    * IF CAR’s staying time has exceeded the allotted time, then the user story continues in **ATTENDANT: Paid outside parking with SPARK 2a**
    * IF NOT, then the user story continues in **ATTENDANT: Paid outside parking with SPARK 2b**.

## ATTENDANT: Paid outside parking with SPARK 2a
1. GEAR informs the ATTENDANT about CAR’s staying time having exceeded the allotted time and offers the ATTENDANT METADATA about the CAR. This METADATA would include at least
    * Register number
    * Point of time when parking registration to CLOUD was done
    * Allotted time
    * Amount of time exceeded.
2. ATTENDANT fines the CAR
3. ATTENDANT has an option to remove the CAR's register number from the register number list view
4. ATTENDANT continues patrolling.

## ATTENDANT: Paid outside parking with SPARK 2b
1. GEAR informs the ATTENDANT that CAR has not exceeded the allotted time
2. ATTENDANT has an option to remove the CAR's register number from the register number list view
3. ATTENDANT continues patrolling.

## ATTENDANT: Parking disc outside parking
1. ATTENDANT arrives to the parking area and configures the maximum allowed parking time to the GEAR
2. ATTENDANT navigates to the register number list view on GEAR. Register number list view shows each register number of parking disc parked CARs that GEAR can hear being broadcasted by BEACONs. The register numbers are sorted in an alphabetical order. Each heard register number is checked with CLOUD, making CLOUD provide METADATA about each CAR. The METADATA would include at least
    * Point of time when parking registration to CLOUD was done. 
3. GEAR visualises the list to the ATTENDANT in such a way, that
    * Valid parkings are highlighted with green color
    * Invalid parkings (i.e. allotted time has been surpassed) are highlighted with red color.
4. ATTENDANT patrols through the area. For each CAR, ATTENDANT compares the register number on CAR's license plate to the register numbers in the register number list view, although, ATTENDANT may at the same time check if there is a traditional parking disc on the windshield of CAR.
    * IF a traditional parking disc exists or CAR's register number is not found on the list, then the user story continues in **ATTENDANT: Parking disc outside parking without SPARK 1b**.
    * IF NOT, then the user story continues in **ATTENDANT: Parking disc outside parking with SPARK 1a**.
   
## ATTENDANT: Parking disc outside parking with SPARK 1a
1. ATTENDANT looks at GEAR
    * IF CAR’s staying time has exceeded the allotted time, then the user story continues in **ATTENDANT: Parking disc outside parking with SPARK 2a**
    * IF NOT, then the user story continues in **ATTENDANT: Parking disc outside parking with SPARK 2b**.

## ATTENDANT: Parking disc outside parking with SPARK 2a
1. GEAR informs the ATTENDANT about CAR’s staying time having exceeded the allotted time and offers the ATTENDANT METADATA about the CAR. This METADATA would include at least
    * Allotted time
    * Amount of time exceeded.
2. ATTENDANT fines the CAR
3. ATTENDANT has an option to remove the CAR's register number from the register number list view
4. ATTENDANT continues patrolling.

## ATTENDANT: Parking disc outside parking with SPARK 2b
1. GEAR informs the ATTENDANT that CAR has not exceeded the allotted time
2. ATTENDANT has an option to remove the CAR's register number from the register number list view
3. ATTENDANT continues patrolling.
