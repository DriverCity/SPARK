# TODO: test invalid cases
# TODO: test timestamps
# TODO: test cloud storaging
# TODO: test scheduled tasks

import json
from unittest import TestCase

from firebase_io.parking_event_repo import ParkingEventRepository
from utils import TokenUtils

# Constants used in testing
root_event_node = 'parkingAreaParkingEvent'
root_notification_note = 'parkingEventNotification'
register_number = 'ABC-123'

paid_parking_area_id = '777'
paid_parking_payment_method_type = 'SERVICE_1'
paid_parking_payment_receipt = 'valid_test_hash'

parking_disc_parking_area_id = 'PARKING_DISC_AREA'


class TestParkingEventIO(TestCase):
    def setUp(self):
        self.parking_event_repo = ParkingEventRepository()

    def test_store_parking_disc_parking_event(self):

        # Arrange
        self.parking_event_repo.db = MockDb().with_parking_disc_init()

        expected_parking_context_type = 'PARKING_DISC'

        request_json = TestUtils.build_request(
            parking_context_type=expected_parking_context_type
        )

        # Act
        result = self.parking_event_repo.store_parking_event(request_json)

        # Assert
        # > parkingAreaParkingEvent
        expected_token = json.loads(result)['name']
        actual_token, actual_event = self.parking_event_repo.db. \
            get_single_event_key_and_value(parking_disc_parking_area_id)
        self.assertEqual(expected_token, actual_token)
        self.assertEqual(expected_parking_context_type, actual_event['parkingType'])

        # > parkingEventNotification
        actual_notification = self.parking_event_repo.db.get_notification(expected_token)
        self.assertEqual(False, actual_notification['isConsumedByLongTermDataStore'])
        self.assertEqual(False, actual_notification['isConsumedByOccupancyAnalysis'])
        self.assertEqual(parking_disc_parking_area_id, actual_notification['parkingAreaId'])
        self.assertEqual(expected_token, actual_notification['parkingEventId'])
        self.assertEqual(register_number, actual_notification['registerNumber'])

    def test_store_paid_parking_event(self):

        # Arrange
        self.parking_event_repo.db = MockDb().with_paid_init()

        expected_parking_context_type = 'PAID'
        expected_parking_duration_in_minutes = 123

        request_json = TestUtils.build_request(
            parking_context_type=expected_parking_context_type,
            parking_duration_in_minutes=expected_parking_duration_in_minutes,
            parking_area_id=paid_parking_area_id,
            payment_method_type=paid_parking_payment_method_type,
            payment_receipt=paid_parking_payment_receipt
        )

        # Act
        result = self.parking_event_repo.store_parking_event(request_json)

        # Assert
        # > parkingAreaParkingEvent
        expected_token = json.loads(result)['name']
        actual_token, actual_event = self.parking_event_repo.db.\
            get_single_event_key_and_value(paid_parking_area_id)
        self.assertEqual(expected_token, actual_token)
        self.assertEqual(expected_parking_duration_in_minutes, actual_event['parkingDurationInMinutes'])
        self.assertEqual(expected_parking_context_type, actual_event['parkingType'])

        # > parkingEventNotification
        actual_notification = self.parking_event_repo.db.get_notification(expected_token)
        self.assertEqual(False, actual_notification['isConsumedByLongTermDataStore'])
        self.assertEqual(False, actual_notification['isConsumedByOccupancyAnalysis'])
        self.assertEqual(paid_parking_area_id, actual_notification['parkingAreaId'])
        self.assertEqual(expected_token, actual_notification['parkingEventId'])
        self.assertEqual(register_number, actual_notification['registerNumber'])


class TestUtils():
    @staticmethod
    def build_request(parking_context_type,
                      parking_duration_in_minutes=None,
                      parking_area_id=None,
                      payment_method_type=None,
                      payment_receipt=None):
        request = {
            'parkingContextType': parking_context_type,
            'registerNumber': register_number
        }

        if parking_area_id is not None:
            request['parkingAreaId'] = parking_area_id

        if parking_duration_in_minutes is not None:
            request['parkingDurationInMinutes'] = parking_duration_in_minutes

        if payment_method_type is not None:
            if request.has_key('paymentMethodInformation') is False:
                request['paymentMethodInformation'] = {}
            request['paymentMethodInformation']['paymentMethodType'] = payment_method_type

        if payment_receipt is not None:
            if request.has_key('paymentMethodInformation') is False:
                request['paymentMethodInformation'] = {}
            request['paymentMethodInformation']['paymentReceipt'] = payment_receipt

        return request


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

    def get_events(self, area):
        return self.child(root_event_node)\
            .child(area)\
            .child(register_number)\
            .json

    def get_single_event_key_and_value(self, area):
        events = self.get_events(area)
        for k, v in events.items(): return k, v;

    def get_notification(self, token):
        return [v for k, v in self.child(root_notification_note).json.items() if v['parkingEventId'] == token][0]

    def with_paid_init(self):
        return self.__with_basic_area_id_init(paid_parking_area_id)

    def with_parking_disc_init(self):
        return self.__with_basic_area_id_init(parking_disc_parking_area_id)

    def __with_basic_area_id_init(self, area_id):
        self.__root = MockDb.Node({
            root_event_node: MockDb.Node({
                area_id: MockDb.Node({
                    register_number: MockDb.Node()
                })
            }),
            root_notification_note: MockDb.Node()
        })
        return self
