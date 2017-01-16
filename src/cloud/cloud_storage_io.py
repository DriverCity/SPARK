import json
from google.cloud import storage
from utils import TimeUtils


class CloudStorageIO():

    def __init__(self):
        storage_client = storage.Client()
        bucket = storage.Bucket(storage_client, 'parking-event-store')
        self.blob = storage.Blob('parking-event-blob-' + TimeUtils.get_local_datestamp(), bucket)

    def upload_to_parking_event_store(self, json_values):
        # TODO: compress stored json or turn into csv
        self.blob.upload_from_string(json.dumps(json_values), 'application/json')
