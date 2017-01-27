LOGGER_NAME = 'spark-logger'

# Test receipt is always valid. If do not want to use,
# set PAYMENT_VALIDATION_TEST_METHOD_TYPE and PAYMENT_VALIDATION_TEST_RECEIPT to None
PAYMENT_VALIDATION_TEST_METHOD_TYPE = 'SERVICE_1'
PAYMENT_VALIDATION_TEST_RECEIPT = 'valid_test_hash'

GOOGLE_CLOUD_STORAGE_STORE_NAME = 'parking-event-store'
GOOGLE_CLOUD_STORAGE_BLOB_NAME_PREFIX = 'parking-event-blob'

# Store parking event in Firebase for three days
PARKING_EVENT_FIREBASE_TIME_IN_SECONDS = 60*60*24*3
