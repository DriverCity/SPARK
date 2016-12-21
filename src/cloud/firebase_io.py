import json
from flask import jsonify
from pyrebase import pyrebase
import utils

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

config = {
    "apiKey": "AIzaSyDhjWe4lzQBToEiVRTp98nTp09xKi7LxEM",
    "authDomain": "spark2-150308.firebaseapp.com",
    "databaseURL": "https://spark2-150308.firebaseio.com",
    "storageBucket": "staging.spark2-150308.appspot.com",
    "serviceAccount": "serviceaccount.json"
}

firebase = pyrebase.initialize_app(config)
db = firebase.database()


def store_parking_event(request_json):

    parking_context_type = request_json['parkingContextType']
    register_number = request_json['registerNumber']

    parking_event_json = {
        'timestamp': utils.get_local_timestamp(),
        'parkingType': parking_context_type
    }

    if parking_context_type == 'PAID':
        parking_area_id = request_json.get['parkingAreaId']
        parking_event_json['parkingDurationInMinutes'] = request_json['parkingDurationInMinutes']
    elif parking_context_type == 'PARKING_DISC':
        parking_area_id = 'PARKING_DISC_AREA'

    results = db\
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
        'liveUntilTime': utils.get_epoch_timestamp_plus_seconds(60*60*24*7)
    }

    if parking_context_type == 'PAID':
        notification_json['isConsumedByOccupancyAnalysis'] = False
        notification_json['isConsumedByLongTermDataStore'] = False

    notification_result = db\
        .child('parkingEventNotification')\
        .push(notification_json)

    return jsonify(results)


def remove_dead_events():

    notifications_ref = db.child('parkingEventNotification')
    dead_notifications = notifications_ref\
        .order_by_child('liveUntilTime')\
        .start_at(utils.get_epoch_timestamp_plus_seconds(-365*24*60*60))\
        .end_at(utils.get_epoch_timestamp_plus_seconds(0)).get()
    dead_notifications = [(dn.key(), dn.val()) for dn in dead_notifications.each()
                          if all([dn.val()['isConsumedByOccupancyAnalysis'], dn.val()['isConsumedByLongTermDataStore']])]

    for dn_id, dn in dead_notifications:

        # Remove dead events
        db.child('parkingAreaParkingEvent')\
        .child(dn['parkingAreaId'])\
        .child(dn['registerNumber'])\
        .child(dn['parkingEventId'])\
        .remove()

        # Remove dead notifications
        db.child('parkingEventNotification')\
        .child(dn_id)\
        .remove()


def get_new_events_for(consumer):
    """

    :param consumer is either LongTermDataStore or OccupancyAnalysis:
    :return:
    """

    consumed_notifications = db\
    .child('parkingEventNotification')\
    .order_by_child('isConsumedBy' + consumer)\
    .start_at(False).end_at(False)\
    .get()

    result = []

    for cn in consumed_notifications.each():

        # Get parking event for the result set
        parking_event = db\
            .child('parkingAreaParkingEvent')\
            .child(cn.val()['parkingAreaId'])\
            .child(cn.val()['registerNumber'])\
            .child(cn.val()['parkingEventId'])\
            .get()

        result.append((parking_event.val(), cn.key()))

    return result


def mark_events_consumed_by(consumer, event_keys):

    for key in event_keys:

        db\
        .child('parkingEventNotification')\
        .child(key)\
        .update({'isConsumedBy'+consumer:True})