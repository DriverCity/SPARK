import time, datetime, json
from flask import jsonify
from pyrebase import pyrebase

# pyrebase_config.json is of format
# {
#     "apiKey": "xxx",
#     "authDomain": "xxx",
#     "databaseURL": "xxx",
#     "storageBucket": "xxx",
#     "serviceAccount": "xxx.json"
# }
with open('pyrebase_config.json') as config_str:
    config = json.load(config_str)
firebase = pyrebase.initialize_app(config)
db = firebase.database()


def get_timestamp_plus_seconds(seconds):
    return datetime.datetime.fromtimestamp(time.time() + seconds).strftime('%Y-%m-%d %H:%M:%S')


def store_parking_event(request_json):
    register_number = request_json['registerNumber']
    parking_event_json = {
        'timestamp': get_timestamp_plus_seconds(0),
        'parkingType': request_json['parkingContextType'],
        'parkingDurationInMinutes': request_json['parkingDurationInMinutes']
    }
    results = db\
        .child('parkingAreaParkingEvent')\
        .child(request_json['parkingAreaId'])\
        .child(register_number)\
        .push(parking_event_json)

    # Store notification about the event for event consumption
    # > Notifications are stored in a flattened format
    # > Better use of indexing for server side event consumers
    notification_json = {
        'parkingAreaId': request_json['parkingAreaId'],
        'registerNumber': register_number,
        'parkingEventId': results['name'],
        'isConsumedByOccupancyAnalysis': False,
        'isConsumedByLongTermDataStore': False,
        'liveUntilTime': get_timestamp_plus_seconds(60*60*24*7)
    }

    notification_result = db\
        .child('parkingEventNotification')\
        .push(notification_json)

    return jsonify(results)


def remove_dead_events():

    notifications_ref = db.child('parkingEventNotification')
    dead_notifications = notifications_ref\
        .order_by_child('liveUntilTime')\
        .start_at(get_timestamp_plus_seconds(-365*24*60*60))\
        .end_at(get_timestamp_plus_seconds(0)).get()
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


# consumer is either LongTermDataStore or OccupancyAnalysis
def consume_new_parking_events_by(consumer):

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

        result.append(parking_event.val())

        # TODO: notifications may be checked even if the following processes fail
        # TODO: form transaction
        # Set parking event as consumed
        db\
        .child('parkingEventNotification')\
        .child(cn.key())\
        .update({'isConsumedBy'+consumer:True})

    return result