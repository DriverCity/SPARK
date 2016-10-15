# Additional functionalities

This document cover those proposed functionalities, whose conceptualization and/or implementation is not part of the required functionalities, and to which the project team will come back to in case there is enough time. Whether or not these functionalities are conceptualised and/or implemented does not affect on team's, customer's or course personnel's views on whether the project was successful or not.

## ATTENDANT's additional MAP features
ATTENDANT could have some additional features in his version of the MAP, for example the absolute numbers of SPARK using CAR's in certain areas.

## DRIVER: Paid outside parking without SPARK
1. DRIVER drives his CAR to a paid outside parking lot and parks the CAR
2. DRIVER exists the CAR and goes to the SMART METER
3. DRIVER purchases a ticket from the SMART METER and METADATA about the purchase is sent to CLOUD. This METADATA would include at least
    * Parking area identifier
    * Parking time
4. DRIVER receives a parking ticket from the SMART METER
5. DRIVER walks back to his CAR, puts the parking ticket on the dashboard and leaves the CAR.

## ATTENDANT: Paid outside parking without SPARK 1b
1. GEAR informs the ATTENDANT that the scanning found no SPARK using CARs
2. ATTENDANT checks if there’s a valid parking permit on the windshield of the CAR
    * IF THERE is, then ATTENDANT continues patrolling.
    * IF NOT, then ATTENDANT fines the CAR and continues patrolling.

## ATTENDANT: Parking disc outside parking without SPARK 1b
1. GEAR informs the ATTENDANT that the scanning found no SPARK using CARs
2. ATTENDANT checks if there’s a valid parking disc on the windshield of the CAR
    * IF THERE is, then ATTENDANT continues patrolling.
    * IF NOT, then ATTENDANT fines the CAR and continues patrolling.
