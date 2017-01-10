# TODO: test invalid cases
# TODO: test timestamps
# TODO: test cloud storaging
# TODO: test scheduled tasks

import json
from unittest import TestCase
from mock import mock
from datetime import datetime

from parking_event_repo import ParkingEventRepository
from utils import TokenUtils, TimeUtils

# Constants used in testing
root_event_node = ParkingEventRepository._parking_event_ODS_node_name
root_lookup_node = ParkingEventRepository._parking_event_ODS_lookup_node_name
root_notification_note = ParkingEventRepository._parking_event_notification_store_node_name

register_number = 'ABC-123'

paid_parking_area_id = '777'
paid_parking_payment_method_type = 'SERVICE_1'
paid_parking_payment_receipt = 'valid_test_hash'

parking_disc_parking_area_id = 'PARKING_DISC_AREA'

secondary_area_id = '888'


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
        expected_token = json.loads(result)['odsId']
        actual_token, actual_event = self.parking_event_repo.db \
            .get_single_event_key_and_value(parking_disc_parking_area_id)
        self.assertEqual(expected_token, actual_token)
        self.assertEqual(expected_parking_context_type, actual_event['parkingType'])

        # > parkingEventLookup
        self.__assert_lookup_matches(result, actual_token)

        # > parkingEventNotification
        actual_notification = self.parking_event_repo.db.get_notification(parking_disc_parking_area_id, expected_token)
        self.assertEqual(True, actual_notification['isConsumedByLongTermDataStore'])
        #self.assertEqual(False, actual_notification['isConsumedByOccupancyAnalysis'])
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
        expected_token = json.loads(result)['odsId']
        actual_token, actual_event = self.parking_event_repo.db.\
            get_single_event_key_and_value(paid_parking_area_id)
        self.assertEqual(expected_token, actual_token)
        self.assertEqual(expected_parking_duration_in_minutes, actual_event['parkingDurationInMinutes'])
        self.assertEqual(expected_parking_context_type, actual_event['parkingType'])

        # > parkingEventLookup
        self.__assert_lookup_matches(result, actual_token)

        # > parkingEventNotification
        actual_notification = self.parking_event_repo.db.get_notification(paid_parking_area_id, expected_token)
        self.assertEqual(False, actual_notification['isConsumedByLongTermDataStore'])
        self.assertEqual(False, actual_notification['isConsumedByOccupancyAnalysis'])
        self.assertEqual(paid_parking_area_id, actual_notification['parkingAreaId'])
        self.assertEqual(expected_token, actual_notification['parkingEventId'])
        self.assertEqual(register_number, actual_notification['registerNumber'])

    def test_store_paid_parking_over_previous(self):

        # Arrange
        self.parking_event_repo.db = MockDb().with_paid_init()

        expected_parking_context_type = 'PAID'
        expected_parking_duration_in_minutes = 123

        request_json = TestUtils.build_request(
            parking_context_type=expected_parking_context_type,
            parking_duration_in_minutes=expected_parking_duration_in_minutes,
            parking_area_id=secondary_area_id,
            payment_method_type=paid_parking_payment_method_type,
            payment_receipt=paid_parking_payment_receipt
        )

        # Act
        result_0 = self.parking_event_repo.store_parking_event(request_json)

        # Add over existing
        request_json['parkingAreaId'] = paid_parking_area_id
        result = self.parking_event_repo.store_parking_event(request_json)

        # Assert
        # > parkingAreaParkingEvent
        expected_token = json.loads(result)['odsId']
        actual_token, actual_event = self.parking_event_repo.db. \
            get_single_event_key_and_value(paid_parking_area_id)
        self.assertEqual(expected_token, actual_token)
        self.assertEqual(expected_parking_duration_in_minutes, actual_event['parkingDurationInMinutes'])
        self.assertEqual(expected_parking_context_type, actual_event['parkingType'])

        # > parkingEventLookup
        self.__assert_lookup_matches(result, actual_token)

        # > parkingEventNotifications
        actual_notification = self.parking_event_repo.db.get_notification(paid_parking_area_id, expected_token)
        self.assertEqual(False, actual_notification['isConsumedByLongTermDataStore'])
        self.assertEqual(False, actual_notification['isConsumedByOccupancyAnalysis'])
        self.assertEqual(paid_parking_area_id, actual_notification['parkingAreaId'])
        self.assertEqual(expected_token, actual_notification['parkingEventId'])
        self.assertEqual(register_number, actual_notification['registerNumber'])
        secondary_notification = self.parking_event_repo.db.get_notification(secondary_area_id, expected_token)
        self.assertEqual(secondary_area_id, secondary_notification['parkingAreaId'])

    def test_duration_timestamp(self):

        # Assign
        mock_now = datetime(2016, 1, 1, 1, 1, 1, 1)
        with mock.patch('datetime.datetime') as dt_mock:
            dt_mock.now.return_value = mock_now
            self.parking_event_repo.db = MockDb().with_paid_init()

            expected_parking_context_type = 'PAID'
            expected_parking_duration_in_minutes = 60

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
            _, actual_event = self.parking_event_repo.db. \
                get_single_event_key_and_value(paid_parking_area_id)

            _, lookup = self.parking_event_repo.db \
                .get_single_lookup_key_and_value(parking_disc_parking_area_id)
            actual_end_timestamp = lookup['durationEndTimestamp']
            expected_end_timestamp = TimeUtils.get_local_timestamp(actual_event['parkingDurationInMinutes'])
            self.assertEqual(expected_end_timestamp, actual_end_timestamp)


    def __assert_lookup_matches(self, result, expected_token):
        expected_lookup_id = json.loads(result)['odsLookupId']
        actual_lookup_id, lookup = self.parking_event_repo.db \
            .get_single_lookup_key_and_value(parking_disc_parking_area_id)
        actual_lookup_token = lookup['parkingAreaParkingEventId']
        self.assertEqual(expected_lookup_id, actual_lookup_id)
        self.assertEqual(expected_token, actual_lookup_token)


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
            if 'paymentMethodInformation' not in request:
                request['paymentMethodInformation'] = {}
            request['paymentMethodInformation']['paymentMethodType'] = payment_method_type

        if payment_receipt is not None:
            if 'paymentMethodInformation' not in request:
                request['paymentMethodInformation'] = {}
            request['paymentMethodInformation']['paymentReceipt'] = payment_receipt

        return request


class MockDb():
    class Node():
        class KeyValue():
            def __init__(self, key, value):
                self.__key = key
                self.__value = value
            def key(self):
                return self.__key
            def val(self):
                return self.__value

        def __init__(self, children=None):
            self.parent = None
            self.parent_key = None
            self.__filter_by_child = None
            if children is not None:
                self.json = children
                for k, v in children.items():
                    if isinstance(v, MockDb.Node):
                        v.parent = self
                        v.parent_key = k
            else:
                self.json = {}

        def child(self, name):
            if name not in self.json:
                self.json[name] = MockDb.Node()
            return self.json[name]

        def push(self, inner_node):
            token = TokenUtils.get_uuid()
            self.json[token] = inner_node
            return {'name': token}

        def order_by_child(self, name):
            self.__filter_by_child = name
            return self

        def start_at(self, value):
            children = [(k, v) for k, v in self.json.items() if k == self.__filter_by_child and v == value]
            self.json = {}
            for k, v in children:
                self.json[k] = v
            return self

        def end_at(self, value):
            return self

        def get(self):
            return self

        def set(self, json_value):
            self.json = {}
            for k, v in json_value.items():
                self.json[k] = v

        def each(self):
            return [MockDb.Node.KeyValue(k, v) for k, v in self.json.items()]

        def val(self):
            if any(self.json) is False: return None
            return self.json

        def remove(self):
            del self

    def __init__(self):
        self.__root = None
        pass

    def child(self, name):
        return self.__root.json[name]

    def get_event(self, area, odsId):
        return self.child(root_event_node)\
            .child(area)\
            .child(odsId)

    def get_lookup(self, area, odsId):
        return self.child(root_lookup_node)\
            .child(odsId)

    def get_single_event_key_and_value(self, area):
        event = self.get_event(area, register_number)
        return register_number, event.json

    def get_single_lookup_key_and_value(self, area):
        lookup = self.get_lookup(area, register_number)
        return register_number, lookup.json

    def get_notification(self, areaId, token):
        return [v for k, v in self.child(root_notification_note).json.items() if v['parkingEventId'] == token and v['parkingAreaId'] == areaId][0]

    def with_paid_init(self):
        return self.__with_basic_area_id_init(paid_parking_area_id)

    def with_parking_disc_init(self):
        return self.__with_basic_area_id_init(parking_disc_parking_area_id)

    def __with_basic_area_id_init(self, area_id):
        self.__root = MockDb.Node({
            root_event_node: MockDb.Node({
                area_id: MockDb.Node({
                    register_number: MockDb.Node()
                }),
                secondary_area_id: MockDb.Node({
                    register_number: MockDb.Node()
                })
            }),
            root_notification_note: MockDb.Node(),
            root_lookup_node: MockDb.Node({
                register_number: MockDb.Node()
            })
        })
        return self
