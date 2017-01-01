from unittest import TestCase
import json
from utils import TokenUtils
from firebase_io import FirebaseIO

# Constants used in testing
root_event_node = 'parkingAreaParkingEvent'
root_notification_note = 'parkingEventNotification'
register_number = 'ABC-123'
paid_parking_area_id = '777'
paid_parking_payment_method_type = 'SERVICE_1'
paid_parking_payment_receipt = 'valid_test_hash'


class TestFirebaseIO(TestCase):
    def setUp(self):
        self.firebase_io = FirebaseIO()
        self.firebase_io.db = MockDb().with_paid_init()

    def test_store_paid_parking_event(self):

        # Values to test
        expected_parking_context_type = 'PAID'
        expected_parking_duration_in_minutes = 123

        request_json = {
            'parkingAreaId': paid_parking_area_id,
            'parkingContextType': expected_parking_context_type,
            'parkingDurationInMinutes': expected_parking_duration_in_minutes,
            'paymentMethodInformation': {
                'paymentMethodType': paid_parking_payment_method_type,
                'paymentReceipt': paid_parking_payment_receipt
            },
            'registerNumber': register_number
        }

        # Act
        result = self.firebase_io.store_parking_event(request_json)

        # Assert
        # > parkingAreaParkingEvent
        expected_token = json.loads(result)['name']
        actual_token, actual_event = self.firebase_io.db.\
            get_single_paid_event_key_and_value(paid_parking_area_id)
        self.assertEqual(expected_token, actual_token)
        self.assertEqual(expected_parking_duration_in_minutes, actual_event['parkingDurationInMinutes'])
        self.assertEqual(expected_parking_context_type, actual_event['parkingType'])

        # > parkingEventNotification
        actual_notification = self.firebase_io.db.get_notification(expected_token)
        self.assertEqual(False, actual_notification['isConsumedByLongTermDataStore'])
        self.assertEqual(False, actual_notification['isConsumedByOccupancyAnalysis'])
        self.assertEqual(paid_parking_area_id, actual_notification['parkingAreaId'])
        self.assertEqual(expected_token, actual_notification['parkingEventId'])
        self.assertEqual(register_number, actual_notification['registerNumber'])


class MockDb():
    class Node():
        def __init__(self, children=None):
            if children is not None:
                self.json = children
            else:
                self.json = {}

        def child(self, name):
            return self.json[name]

        def push(self, inner_node):
            token = TokenUtils.get_uuid()
            self.json[token] = inner_node
            return {'name': token}

    def __init__(self):
        self.__root = None
        pass

    def child(self, name):
        return self.__root.json[name]

    def get_paid_events(self, area):
        return self.child(root_event_node)\
            .child(area)\
            .child(register_number)\
            .json

    def get_single_paid_event_key_and_value(self, area):
        events = self.get_paid_events(area)
        for k, v in events.items(): return k, v;

    def get_notification(self, token):
        return [v for k, v in self.child(root_notification_note).json.items() if v['parkingEventId'] == token][0]

    def with_paid_init(self):
        self.__root = MockDb.Node({
            root_event_node: MockDb.Node({
                paid_parking_area_id: MockDb.Node({
                    register_number: MockDb.Node()
                })
            }),
            root_notification_note: MockDb.Node()
        })
        return self