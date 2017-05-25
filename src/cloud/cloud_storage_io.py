import json
from google.cloud import storage
from utils import TimeUtils


class CloudStorageIO():

    def __init__(self, store_name, blob_name_prefix):
        storage_client = storage.Client()
        bucket = storage.Bucket(storage_client, store_name)
        self.blob = storage.Blob(blob_name_prefix + '-' + TimeUtils.get_local_datestamp(), bucket)

    def upload_to_parking_event_store(self, json_values):
        # NOTE: instead of json, events could be stored as csv for efficiency
        self.blob.upload_from_string(json.dumps(json_values), 'application/json')
