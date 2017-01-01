import json
from google.cloud import storage


class CloudStorageIO():

    def __init__(self):
        storage_client = storage.Client()
        bucket = storage.Bucket(storage_client, 'parking-event-store')
        self.blob = storage.Blob('parking-event-blob', bucket) # TODO: set blob name by date

    def upload_json_to_parking_event_store(self, json_values):
        # TODO: compress stored json
        self.blob.upload_from_string(json.dumps(json_values), 'application/json')
