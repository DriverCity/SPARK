import json
from firebase_io import FirebaseIO
from utils import TimeUtils


def filter_to_register_number(self, register_number):
    return self\
        .order_by_child('registerNumber')\
        .start_at(register_number).end_at(register_number)


class ParkingEventRepository(FirebaseIO):

    __parking_event_ODS_node_name = 'parkingAreaParkingEvent'
    __parking_event_ODS_lookup_node_name = 'parkingEventLookup'
    __parking_event_notification_store_node_name = 'parkingEventNotification'

    def __init__(self):
        FirebaseIO.__init__(self)

    def __remove_parking_event_from_ods(self, register_number):
        events = self.db \
            .child(ParkingEventRepository.__parking_event_ODS_lookup_node_name) \
            .filter_to_register_number(register_number) \
            .get().each()

        for e in events:
            k = e.key()
            v = e.val()

            # Remove event from the ODS
            self.db \
                .child(ParkingEventRepository.__parking_event_ODS_node_name) \
                .child(v['parkingAreaId']) \
                .child(v['parkingAreaParkingEventId']) \
                .remove()

            # Remove event from the lookup
            self.db \
                .child(ParkingEventRepository.__parking_event_ODS_lookup_node_name) \
                .child(k) \
                .remove()

    def __add_parking_event_to_ods(self, parking_area_id, parking_event_json):
        add_to_ods_results = self.db\
            .child(ParkingEventRepository.__parking_event_ODS_node_name)\
            .child(parking_area_id)\
            .push(parking_event_json)

        lookup_json = {
            'registerNumber': parking_event_json['registerNumber'],
            'parkingAreaId': parking_area_id,
            'parkingAreaParkingEventId': add_to_ods_results['name']
        }

        add_to_lookup_results = self.db\
            .child(ParkingEventRepository.__parking_event_ODS_lookup_node_name)\
            .push(lookup_json)

        add_results = {
            'odsId': add_to_ods_results['name'],
            'odsLookupId': add_to_lookup_results['name']
        }

        return add_results

    def __add_parking_event_to_notification_store(self, parking_event_notification_json):
        return self.db \
            .child(ParkingEventRepository.__parking_event_notification_store_node_name) \
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
        self.__remove_parking_event_from_ods(register_number)

        # Store the incoming event to ODS
        add_results = self.__add_parking_event_to_ods(parking_area_id, parking_event_json)

        # Store notification about the event for event consumption
        # > Notifications are stored in a flattened format
        # > Better use of indexing for server side event consumers
        notification_json = {
            'parkingAreaId': parking_area_id,
            'registerNumber': register_number,
            'parkingEventId': add_results['name'],
            'isConsumedByOccupancyAnalysis': False,
            'isConsumedByLongTermDataStore': False,
            'liveUntilTime': TimeUtils.get_epoch_timestamp_plus_seconds(60*60*24*7) # TODO make configurable
        }
        notification_add_results = self.__add_parking_event_notification(notification_json)

        return json.dumps(add_results) # TODO: fix swagger specs for the response

    def remove_dead_events(self):
        notifications_ref = self.db.child(ParkingEventRepository.__parking_event_notification_store_node_name)
        # TODO make time configurable
        dead_notifications = notifications_ref\
            .order_by_child('liveUntilTime')\
            .start_at(TimeUtils.get_epoch_timestamp_plus_seconds(-365*24*60*60))\
            .end_at(TimeUtils.get_epoch_timestamp_plus_seconds(0)).get()
        dead_notifications = [(dn.key(), dn.val()) for dn in dead_notifications.each()
                              if all([dn.val()['isConsumedByOccupancyAnalysis'], dn.val()['isConsumedByLongTermDataStore']])]

        for dn_id, dn in dead_notifications:

            # Remove dead events
            self.db.child(ParkingEventRepository.__parking_event_ODS_node_name)\
                .child(dn['parkingAreaId'])\
                .child(dn['registerNumber'])\
                .child(dn['parkingEventId'])\
                .remove()

            # TODO: Remove from ODS registry

            # Remove dead notifications
            self.db.child(ParkingEventRepository.__parking_event_notification_store_node_name)\
                .child(dn_id)\
                .remove()

    # consumer is either LongTermDataStore or OccupancyAnalysis
    def consume_new_parking_events_by(self, consumer):
        consumed_notifications = self.db\
            .child(ParkingEventRepository.__parking_event_notification_store_node_name)\
            .order_by_child('isConsumedBy' + consumer)\
            .start_at(False).end_at(False)\
            .get()

        result = []

        for cn in consumed_notifications.each():

            # Get parking event for the result set
            parking_event = self.db\
                .child(ParkingEventRepository.__parking_event_ODS_node_name)\
                .child(cn.val()['parkingAreaId'])\
                .child(cn.val()['registerNumber'])\
                .child(cn.val()['parkingEventId'])\
                .get()

            result.append(parking_event.val())

            # TODO: notifications may be checked even if the following processes fail
            # TODO: form transaction
            # Set parking event as consumed
            self.db\
                .child(ParkingEventRepository.__parking_event_notification_store_node_name)\
                .child(cn.key())\
                .update({'isConsumedBy'+consumer:True})

        return result
