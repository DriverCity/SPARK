import json
from itertools import groupby

from firebase_repo import FirebaseRepo
from utils import TimeUtils, TokenUtils


class ParkingEventRepository(FirebaseRepo):

    _parking_event_ODS_node_name = 'parkingAreaParkingEvent'
    _parking_event_ODS_lookup_node_name = 'parkingEventLookup'
    _parking_event_notification_store_node_name = 'parkingEventNotification'

    def __init__(self):
        FirebaseRepo.__init__(self)

    def __remove_lookup_events_by(self, register_number, timestamp=None):
        self.db \
            .child(ParkingEventRepository._parking_event_ODS_lookup_node_name) \
            .child(register_number) \
            .order_by_child('timestamp') \
            .start_at(timestamp or '0').end_at(timestamp or '9') \
            .remove()

    def __remove_parking_events_from_ods_by(self, parking_area_id, register_number, timestamp=None):

        # Remove from the ODS
        parking_event = self.db \
            .child(ParkingEventRepository._parking_event_ODS_node_name) \
            .child(parking_area_id) \
            .child(register_number) \
            .get()

        if timestamp is None or parking_event.val()['timestamp'] == timestamp:
            self.db \
                .child(ParkingEventRepository._parking_event_ODS_node_name) \
                .child(parking_area_id) \
                .child(register_number) \
                .remove()

            # Remove from the lookup
            self.__remove_lookup_events_by(register_number)

    def __remove_parking_event_from_ods_by(self, register_number):
        lookup_event = self.db \
            .child(ParkingEventRepository._parking_event_ODS_lookup_node_name) \
            .child(register_number) \
            .get().val()

        if lookup_event is not None:
            self.__remove_parking_events_from_ods_by(lookup_event['parkingAreaId'], register_number)

    def __add_parking_event_to_ods(self, parking_area_id, parking_event_json):

        register_number = parking_event_json['registerNumber']

        self.db\
            .child(ParkingEventRepository._parking_event_ODS_node_name)\
            .child(parking_area_id)\
            .child(register_number)\
            .set(parking_event_json)

        lookup_json = {
            'registerNumber': register_number,
            'parkingAreaId': parking_area_id,
            'parkingAreaParkingEventId': register_number,
            'timestamp': parking_event_json['timestamp']
        }

        if parking_event_json['parkingType'] == 'PAID':
            lookup_json['durationEndTimestamp'] = TimeUtils.get_local_timestamp(parking_event_json['parkingDurationInMinutes'])

        self.db\
            .child(ParkingEventRepository._parking_event_ODS_lookup_node_name)\
            .child(register_number) \
            .set(lookup_json)

        add_results = {
            'odsId': register_number,
            'odsLookupId': register_number
        }

        return add_results

    def __add_parking_event_to_notification_store(self, parking_event_notification_json):
        return self.db \
            .child(ParkingEventRepository._parking_event_notification_store_node_name) \
            .push(parking_event_notification_json)

    def store_parking_event(self, request_json):
        register_number = request_json['registerNumber']
        parking_context_type = request_json['parkingContextType']
        parking_event_json = {
            'timestamp': TimeUtils.get_local_timestamp(),
            'parkingType': parking_context_type,
            'registerNumber': register_number,
        }

        if parking_context_type == 'PAID':
            parking_area_id = request_json['parkingAreaId']
            parking_event_json['parkingDurationInMinutes'] = request_json['parkingDurationInMinutes']
        elif parking_context_type == 'PARKING_DISC':
            parking_area_id = 'PARKING_DISC_AREA'

        # Remove previous events from the ODS if any exist
        self.__remove_parking_event_from_ods_by(register_number)

        # Store the incoming event to ODS
        add_results = self.__add_parking_event_to_ods(parking_area_id, parking_event_json)

        # Store notification about the event for event consumption and clean up
        # > Notifications are stored in a flattened format
        # > Better use of indexing for server side event consumers
        notification_json = {
            'parkingAreaId': parking_area_id,
            'registerNumber': register_number,
            'parkingEventId': add_results['odsId'],
            'liveUntilTime': TimeUtils.get_epoch_timestamp_plus_seconds(60*60*24*7), # TODO make configurable
            'parkingAreaParkingEvent': parking_event_json
        }

        # Only PAID context events are stored long term, because they are the only
        # types of events that one gets location information of.
        if parking_context_type == 'PAID':
            notification_json['willBeStoredToLongTermDataStore'] = True

        notification_add_results = self.__add_parking_event_to_notification_store(notification_json)

        return json.dumps(add_results) # TODO: fix swagger specs for the response

    def remove_dead_events(self):
        notifications_ref = self.db.child(ParkingEventRepository._parking_event_notification_store_node_name)
        # TODO make time configurable
        dead_notifications = notifications_ref\
            .order_by_child('liveUntilTime')\
            .start_at(TimeUtils.get_epoch_timestamp_plus_seconds(-365*24*60*60))\
            .end_at(TimeUtils.get_epoch_timestamp_plus_seconds(0)).get()
        dead_notifications = [(dn.key(), dn.val()) for dn in dead_notifications.each()
                              if 'willBeStoredToLongTermDataStore' not in dn.val()
                              or dn.val()['willBeStoredToLongTermDataStore'] == False]

        for dn_id, dn in dead_notifications:

            # Remove dead events
            self.__remove_parking_events_from_ods_by(dn['parkingAreaId'], dn['registerNumber'],
                                                     dn['parkingAreaParkingEvent']['timestamp'])

            # Remove dead notifications
            self.db.child(ParkingEventRepository._parking_event_notification_store_node_name)\
                .child(dn_id)\
                .remove()

    def consume_new_parking_events_by(self, flag_name, from_flag_value, to_flag_value):
        consumed_notifications = self.db\
            .child(ParkingEventRepository._parking_event_notification_store_node_name)\
            .order_by_child(flag_name)\
            .start_at(from_flag_value).end_at(from_flag_value)\
            .get()

        result = []

        for cn in consumed_notifications.each():

            parking_event = cn.val()['parkingAreaParkingEvent']
            result.append(parking_event)

            # TODO: notifications may be checked even if the following processes fail
            # TODO: form transaction
            # Set parking event as consumed
            self.db\
                .child(ParkingEventRepository._parking_event_notification_store_node_name)\
                .child(cn.key())\
                .update({flag_name:to_flag_value})

        return result

    def get_occuring_paid_event_counts(self):

        now = TimeUtils.get_local_timestamp()
        counts = {}

        occuring_events = self.db \
            .child('parkingEventLookup') \
            .order_by_child('durationEndTimestamp') \
            .start_at(now).end_at('9') \
            .get()

        occuring_events = self.db.sort(occuring_events, 'parkingAreaId')

        for k, g in groupby(occuring_events.each(), lambda i: i.val()['parkingAreaId']):
            counts[str(k)] = sum(1 for i in g)

        return counts