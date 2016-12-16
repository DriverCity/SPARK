# Copyright 2016 Team DriverCity. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# [START app]
import logging

from flask import Flask,request,jsonify
from flasgger import Swagger, swag_from, validate, ValidationError

import firebase_io, cloud_storage_io
import payment, parking_area

app = Flask(__name__)
Swagger(app)


@app.route('/api/v1.0/parkingEvent/', methods=['POST'])
@swag_from('swagger_specs/parkingEvent.yml')
def store_parking_event():
    # TODO: logging
    try:
        # validate schema
        validate(request.json, 'ParkingEvent', 'swagger_specs/parkingEvent.yml', root=__file__)

        # validate payment if paid context
        if request.json['parkingContextType'] == 'PAID':
            payment.validate(request.json['paymentMethodInformation']['paymentMethodType'], request.json['paymentMethodInformation'])

        # store valid information
        return firebase_io.store_parking_event(request.json), 201

    except ValidationError as e:
        return jsonify({ 'errorType': 'SCHEMA_VALIDATION_ERROR', 'content': e }), 400
    except payment.PaymentException as e:
        return jsonify({ 'errorType': 'PAYMENT_ERROR', 'content': e}), 400


#@app.route('/api/v1.0/parkingArea/', methods=['GET'])
#@swag_from('swagger_specs/parkingAreaLocation.yml')
def get_parking_area():
    # TODO: logging
    try:
        # validate schema
        validate(request.json, 'ParkingArea', 'swagger_specs/parkingAreaLocation.yml', root=__file__)

        latitude = request.args['latitude']
        longnitude = request.args['longnitude']

        # TODO

        return jsonify({ 'parkingAreaId': latitude }), 200

    except ValidationError as e:
        return jsonify({ 'errorType': 'SCHEMA_VALIDATION_ERROR', 'content': e }), 400
    except parking_area.LocationException as e:
        return jsonify({ 'errorType': 'LOCATION_ERROR', 'content': e }), 400


@app.route('/tasks/occupancy', methods=['GET'])
def update_occupancy_rates():
    """
    An API for updating the occupancy rates. Used by a cron schedule.
    ---
    tags:
      - Occupancy rates analysis task
    """
    # TODO: logging
    try:
        new_events = firebase_io.consume_new_parking_events_by('OccupancyAnalysis')
        # TODO: analysis
        return '', 201
    except Exception as e:
        # TODO: make fault responding better
        return jsonify({'errorType': 'EXCEPTION', 'content': str(e)}), 500


@app.route('/tasks/store', methods=['GET'])
def move_to_long_term_data_store():
    """
    An API for storing the parking events to the long term data store. Used by a cron schedule.
    ---
    tags:
      - Blob storage task
    """
    # TODO: logging
    try:
        new_events = firebase_io.consume_new_parking_events_by('LongTermDataStore')
        cloud_storage_io.upload_json_to_parking_event_store(new_events)
        return '', 201
    except Exception as e:
        return jsonify({'errorType': 'EXCEPTION', 'content': str(e)}), 500


@app.route('/tasks/cleanup', methods=['GET'])
def cleanup_firebase():
    """
    An API for removing dead parking events from firebase. Used by a cron schedule.
    ---
    tags:
      - Cleanup firebase task
    """
    try:
        firebase_io.remove_dead_events()
        return '', 201
    except Exception as e:
        return jsonify({'errorType': 'EXCEPTION', 'content': str(e)}), 500


@app.route('/')
def hello():
    """Return a friendly HTTP greeting."""
    return 'Hello friend :o)'


@app.errorhandler(500)
def server_error(e):
    logging.exception('An error occurred during a request.')
    return """
    An internal error occurred: <pre>{}</pre>
    See logs for full stacktrace.
    """.format(e), 500

if __name__ == '__main__':
    # This is used when running locally. Gunicorn is used to run the
    # application on Google App Engine. See entrypoint in app.yaml.
    app.run(host='127.0.0.1', port=8080, debug=True)
# [END app]
