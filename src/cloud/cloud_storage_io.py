import json
from google.cloud import storage

storage_client = storage.Client()
bucket = storage.Bucket(storage_client, 'parking-event-store')
blob = storage.Blob('parking-event-blob', bucket) # TODO: set blob name by date

def upload_json_to_parking_event_store(json_values):
    # TODO: compress stored json
    blob.upload_from_string(json.dumps(json_values), 'application/json')
