import json
from itertools import groupby

from firebase_repo import FirebaseRepo
from utils import TimeUtils


class ParkingEventRepository(FirebaseRepo):

    _parking_event_ODS_node_name = 'parkingAreaParkingEvent'
    _parking_event_ODS_lookup_node_name = 'parkingEventLookup'
    _parking_event_notification_store_node_name = 'parkingEventNotification'

    def __init__(self):
        FirebaseRepo.__init__(self)

    def __remove_lookup_events_by_ods_key(self, ods_key):
        self.db \
            .child(ParkingEventRepository._parking_event_ODS_lookup_node_name) \
            .order_by_child('parkingAreaParkingEventId')\
            .start_at(ods_key).end_at(ods_key) \
            .remove()

    def __remove_parking_events_from_ods_by_lookup_events(self, lookup_events):
        for e in lookup_events:
            parking_area_id = e['parkingAreaId']
            ods_key = e['parkingAreaParkingEventId']

            # Remove from the ODS
            self.db \
                .child(ParkingEventRepository._parking_event_ODS_node_name) \
                .child(parking_area_id) \
                .child(ods_key) \
                .remove()

            # Remove from the lookup
            self.__remove_lookup_events_by_ods_key(ods_key)

    def __remove_parking_event_from_ods_by_ods_key(self, ods_key):

        # Remove from the ODS
        self.db \
            .child(ParkingEventRepository._parking_event_ODS_node_name) \
            .child(ods_key) \
            .remove()

        # Remove from the lookup
        self.db \
            .child(ParkingEventRepository._parking_event_ODS_lookup_node_name) \
            .order_by_child('parkingAreaParkingEventId') \
            .start_at(ods_key).end_at(ods_key) \
            .remove()

    def __remove_parking_event_from_ods_by_register_number(self, register_number):
        lookup_event = self.db \
            .child(ParkingEventRepository._parking_event_ODS_lookup_node_name) \
            .child(register_number) \
            .get().val()

        if lookup_event is not None:
            self.__remove_parking_events_from_ods_by_lookup_events([lookup_event])

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
            'parkingAreaParkingEventId': register_number
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
            'registerNumber': register_number
        }

        if parking_context_type == 'PAID':
            parking_area_id = request_json['parkingAreaId']
            parking_event_json['parkingDurationInMinutes'] = request_json['parkingDurationInMinutes']
        elif parking_context_type == 'PARKING_DISC':
            parking_area_id = 'PARKING_DISC_AREA'

        # Remove previous events from the ODS if any exist
        self.__remove_parking_event_from_ods_by_register_number(register_number)

        # Store the incoming event to ODS
        add_results = self.__add_parking_event_to_ods(parking_area_id, parking_event_json)

        is_to_be_consumed_by_occupancy_analysis = (parking_context_type == 'PAID')
        is_to_be_consumed_by_long_term_datastore = (parking_context_type == 'PAID')

        # Store notification about the event for event consumption
        # > Notifications are stored in a flattened format
        # > Better use of indexing for server side event consumers
        notification_json = {
            'parkingAreaId': parking_area_id,
            'registerNumber': register_number,
            'parkingEventId': add_results['odsId'],
            'isConsumedByOccupancyAnalysis': (is_to_be_consumed_by_occupancy_analysis == False),
            'isConsumedByLongTermDataStore': (is_to_be_consumed_by_long_term_datastore == False), #TODO: this is a bit illogical
            'liveUntilTime': TimeUtils.get_epoch_timestamp_plus_seconds(60*60*24*7), # TODO make configurable
            'parkingAreaParkingEvent': parking_event_json
        }
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
                              if all([dn.val()['isConsumedByOccupancyAnalysis'], dn.val()['isConsumedByLongTermDataStore']])]

        for dn_id, dn in dead_notifications:

            # Remove dead events
            self.db.child(ParkingEventRepository._parking_event_ODS_node_name)\
                .child(dn['parkingAreaId'])\
                .child(dn['registerNumber'])\
                .child(dn['parkingEventId'])\
                .remove()

            # TODO: Remove from ODS registry

            # Remove dead notifications
            self.db.child(ParkingEventRepository._parking_event_notification_store_node_name)\
                .child(dn_id)\
                .remove()

    # consumer is either LongTermDataStore or OccupancyAnalysis
    def consume_new_parking_events_by(self, consumer):
        consumed_notifications = self.db\
            .child(ParkingEventRepository._parking_event_notification_store_node_name)\
            .order_by_child('isConsumedBy' + consumer)\
            .start_at(False).end_at(False)\
            .get()

        result = []

        for cn in consumed_notifications.each():

            # Get parking event for the result set
            parking_event = self.db\
                .child(ParkingEventRepository._parking_event_ODS_node_name)\
                .child(cn.val()['parkingAreaId'])\
                .child(cn.val()['registerNumber'])\
                .child(cn.val()['parkingEventId'])\
                .get()

            result.append(parking_event.val())

            # TODO: notifications may be checked even if the following processes fail
            # TODO: form transaction
            # Set parking event as consumed
            self.db\
                .child(ParkingEventRepository._parking_event_notification_store_node_name)\
                .child(cn.key())\
                .update({'isConsumedBy'+consumer:True})

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
            counts[k] = sum(1 for i in g)

        return counts