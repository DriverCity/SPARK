# 1. Current status of project
- some changes to requirements have come up. Role of the cloud has increased and role of BLE beacons decreased.
- currently testing the input to Google Cloud with a fake sender using the Pub/Sub REST api and a service account (just as it would be in production)
- We have given up with Brillo and Weave for now, but keep eye on it.
- Customer has provided us with commercial BLE-beacons, decreasing required amount of work significantly.

# 2. Problems and improvement

Problem:
- This week was an exam week so members were busy with other things.
- Lack of support for Brillo and Weave.
- BLE’s advertisement data width may be too short to advertise tokens.

Improvement:
- Google Cloud SDK’s Python client libraries seem to work nicely with PyCharm IDE
- Give up with Brillo and Weave. Use commercial iBeacon devices instead.
- BLE advertises only the register number of the car. Token is stored only in the cloud.

# 3. Effort
Person | Hours this week | Cumulative
---|---|---
Aurélien | 6 | 67
Hanning | 3 | 60
Soumya | 0 | 42
Joona | 8 | 90
Perttu | 7,5 | 90
Tanja | 0,5 | 44
Anil | 3 | 40
SUM | 28 | 433


# 4. Weekly meetings
  - Meeting with Antti, Bill and Perttu on 17.10. 14:00-16:00
  - Meeting with Antti, Bill, Joona and Perttu on 21.10. 10:00-12:00
