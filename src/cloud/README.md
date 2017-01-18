# SPARK Google Cloud implementation
Application's backend responsible or data storage and analysis functionalities take place in [Google Cloud Platform](http://cloud.google.com). 

## TODO: Why Google Cloud Platform?
- customer initially wanted to take part for competition and try out technologies
	- our project is a proof of concept, read more here (link)
- App Engine load balancing and ability to configure CPU and memory needs -> price is based on that
- still, custom runtime with Docker containers -> ability to install and use, for example, c-based libraries with python
- good choice for data processing and storage
	-> Firebase
	-> Google Cloud Storage
- Future considerations
	- excessive possibilities for analysis

## SPARK on App Engine: Modules
This section describes the most important modules of SPARK Google Cloud implementation on App Engine.
- `main.py`
  - Contains a [Flask API](http://flask.pocoo.org/), which provides parking event storing functionality for **smart meters** and **mobile apps**. It also contains API for [CRON](https://en.wikipedia.org/wiki/Cron)-scheduled jobs. See [Scheduling Tasks With Cron for Python](https://cloud.google.com/appengine/docs/python/config/cron). **TODO** Explain what each API call does
- `firebase_repo.py`
  - Contains a base class for accessing Firebase using [Pyrebase](https://github.com/thisbejim/Pyrebase) - a Firebase API wrapper for Python. The base class is inherited by `parking_event_repo.py` and `occupancy_rates_repo.py`.
- `parking_event_repo.py`
  - Contains a repository for storing and receiving parking events to/from Firebase
- `occupancy_rates_repo.py`
  - Contains a repository for forming and storing parking area occupancy levels to Firebase
- `cloud_storage_io.py`
  - Contains the functionality for storing parking events into to a [Cloud Storage Bucket](https://cloud.google.com/storage/docs/key-terms#buckets). The bucket is used as a long term datastore for parking events.
- `swagger_specs/parkingEvent.yml`
  - The API docs for parking event storage call consumed by [Flasgger](https://github.com/rochacbruno/flasgger) - a [Swagger](http://swagger.io/) API docs creator for Flask.

## How to install and deploy
For these instructions, you need to have basic knowledge on cloud and database technologies, version control and Python.

### Cloud
1. [Create a new project on GCP (Google Cloud Platform)](https://cloud.google.com/resource-manager/docs/creating-project). The recommended way is to create the project via the Cloud Platform Console.
2. Create a Firebase instance by importing the GCP project on [Firebase console](https://console.firebase.google.com/). Select *IMPORT GOOGLE PROJECT* and add Firebase to the project.
  1. In the appearing view, select *Database* from the left hand side menu (https://console.firebase.google.com/project/{your-google-cloud-project-id}/database/data)
  2. Import parking area data to Firebase by selecting the ellipsis icon from the top-right of the *Database* view. Select *Import JSON* from the pop-up menu. With the pop-up window, upload the file (`TampereOpenDataTransformed.json`)[https://github.com/DriverCity/SPARK/blob/master/data/TampereOpenDataTransformed.json]. This populates Firebase with Tampere city parking area information.
  3. Configure indexes and access permissions by navigating to the *Rules*-tab and copy-pasting **TODO: Add rules and indexes to github**
  4. **Optional**: Configure backups according to the [*Setup* instructions](https://firebase.google.com/docs/database/ios/backups)
  5. Generate a private key for the Firebase service account according to the [*Add Firebase to your app* instructions](https://firebase.google.com/docs/admin/setup).
3. Create a Google Cloud Storage bucket with name `parking-event-store` according to the [*Before you begin* and *Create a bucket* instructions](https://cloud.google.com/storage/docs/quickstart-console)

### Local operations
It is recommended to use [Ubuntu](https://www.ubuntu.com/download) as the operating system for the local operations. Make yourself a local clone of *this* Git repository. [PyCharm](https://www.jetbrains.com/pycharm/) is recommended as the IDE for Python, yet, in the scope of these instructions, no Python development is required.

#### For running the server locally
All of the following steps are executed with the directory `.../SPARK/src/cloud/` as the working directory

1. Ensure you have [Python 3](https://www.python.org/download/releases/3.0/) installed
2. [Instantiate an isolated Python 3 environment](http://docs.python-guide.org/en/latest/dev/virtualenvs/) and activate it
  - The instantiation command can be e.g. `virtualenv -p python3 spark_venv`.
3. Install required Python packages listed in `requirements.txt` to the environment with `pip install -r requirements.txt`
4. Install the Google Cloud Engine utilities according to the [*Before you begin* instructions in *Python Quickstart for Flexible Environment*](https://cloud.google.com/python/getting-started/hello-world)
5. Install the Google App Engine SDK for Python according to the [*Download the SDK for App Engine* instructions](https://cloud.google.com/appengine/docs/python/download)
6. Create files `pyrebase_config.json` and `serviceaccount.json`
  - `pyrebase_config.json` should be of format: 
  
  ```json  
  {
    "apiKey": "apiKey",
    "authDomain": "projectId.firebaseapp.com",
    "databaseURL": "https://databaseName.firebaseio.com",
    "storageBucket": "projectId.appspot.com",
    "serviceAccount": "serviceaccount.json"
  }
  ```
  - `serviceaccount.json` should be the service account file from firebase **TODO**
  - These files are included in `.gitignore`.
7. Execute command `python main.py`. This instantiates the builtin Python server with a Flask API. Now you should be able to execute the API commands described in `swagger specs` **TODO: Make link**. The default port is **TODO**. The Swagger API descriptions can be found in **TODO**.

#### For deploying on GC App Engine
All of the following steps are executed with the directory `.../SPARK/src/cloud/` as the working directory
- For deploying the API, execute `gcloud app deploy`
  - API is running in **TODO**
  - Swagger in **TODO**
- For deploying the CRON jobs, execute `gcloud app deploy cron.xml`.
