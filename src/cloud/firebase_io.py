import json
from pyrebase import pyrebase
from utils import TimeUtils


class FirebaseIO():

    def __init__(self):

        # pyrebase_config.json is of format
        # {
        #     "apiKey": "xxx",
        #     "authDomain": "xxx",
        #     "databaseURL": "xxx",
        #     "storageBucket": "xxx",
        #     "serviceAccount": "xxx.json"
        # }

        with open('pyrebase_config.json') as fp:
            config = json.load(fp)

        firebase = pyrebase.initialize_app(config)
        self.db = firebase.database()

    def store_parking_event(self, request_json):
        register_number = request_json['registerNumber']
        parking_context_type = request_json['parkingContextType']
        parking_event_json = {
            'timestamp': TimeUtils.get_local_timestamp(),
            'parkingType': parking_context_type
        }

        if parking_context_type == 'PAID':
            parking_area_id = request_json['parkingAreaId']
            parking_event_json['parkingDurationInMinutes'] = request_json['parkingDurationInMinutes']
        elif parking_context_type == 'PARKING_DISC':
            parking_area_id = 'PARKING_DISC_AREA'

        results = self.db\
            .child('parkingAreaParkingEvent')\
            .child(parking_area_id)\
            .child(register_number)\
            .push(parking_event_json)

        # Store notification about the event for event consumption
        # > Notifications are stored in a flattened format
        # > Better use of indexing for server side event consumers
        notification_json = {
            'parkingAreaId': parking_area_id,
            'registerNumber': register_number,
            'parkingEventId': results['name'],
            'isConsumedByOccupancyAnalysis': False,
            'isConsumedByLongTermDataStore': False,
            'liveUntilTime': TimeUtils.get_epoch_timestamp_plus_seconds(60*60*24*7)
        }

        notification_result = self.db\
            .child('parkingEventNotification')\
            .push(notification_json)

        return json.dumps(results)

    def remove_dead_events(self):
        notifications_ref = self.db.child('parkingEventNotification')
        dead_notifications = notifications_ref\
            .order_by_child('liveUntilTime')\
            .start_at(TimeUtils.get_epoch_timestamp_plus_seconds(-365*24*60*60))\
            .end_at(TimeUtils.get_epoch_timestamp_plus_seconds(0)).get()
        dead_notifications = [(dn.key(), dn.val()) for dn in dead_notifications.each()
                              if all([dn.val()['isConsumedByOccupancyAnalysis'], dn.val()['isConsumedByLongTermDataStore']])]

        for dn_id, dn in dead_notifications:

            # Remove dead events
            self.db.child('parkingAreaParkingEvent')\
                .child(dn['parkingAreaId'])\
                .child(dn['registerNumber'])\
                .child(dn['parkingEventId'])\
                .remove()

            # Remove dead notifications
            self.db.child('parkingEventNotification')\
                .child(dn_id)\
                .remove()

    # consumer is either LongTermDataStore or OccupancyAnalysis
    def consume_new_parking_events_by(self, consumer):
        consumed_notifications = self.db\
            .child('parkingEventNotification')\
            .order_by_child('isConsumedBy' + consumer)\
            .start_at(False).end_at(False)\
            .get()

        result = []

        for cn in consumed_notifications.each():

            # Get parking event for the result set
            parking_event = self.db\
                .child('parkingAreaParkingEvent')\
                .child(cn.val()['parkingAreaId'])\
                .child(cn.val()['registerNumber'])\
                .child(cn.val()['parkingEventId'])\
                .get()

            result.append(parking_event.val())

            # TODO: notifications may be checked even if the following processes fail
            # TODO: form transaction
            # Set parking event as consumed
            self.db\
                .child('parkingEventNotification')\
                .child(cn.key())\
                .update({'isConsumedBy'+consumer:True})

        return result
