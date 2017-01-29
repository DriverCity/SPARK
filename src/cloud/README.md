# SPARK on Google Cloud Platform
Application's backend responsible or data storage and analysis functionalities take place in [Google Cloud Platform](http://cloud.google.com). 

## TODO: Why Google Cloud Platform?

<p>
The customer of the project initially wanted the project to participate the [Google IoT Technology Research Awards Pilot](https://research.googleblog.com/2016/02/announcing-google-internet-of-things.html). Since, in addition, the project focused on implementing a cloud backed proof of concept system, GCP (Google Cloud Platform) was a natural choice as the cloud backend of our project.
</p>
<p>
GCP offers [a number of platforms](https://cloud.google.com/docs/choosing-a-compute-option) for deploying your application on. Out of these, the project team decided to go with [Google App Engine](https://cloud.google.com/appengine/). This was due to the example use cases listed in App Engine documentation being quite similar with the one of the project. Also, App Engine promises to provide a highly reliable and scalable platform without the project developers having to implement it themselves, making it possible for developers to focus more on implementing the functional requirements. For the project, scalability is definetily an important concern, because the project is essentially a tool for parking that could very easily scale from Tampere to other areas and locations.
</p>
<p>
Despite providing a scalable platform, App Engine still supports custom runtime with custom [Docker containers](https://www.docker.com/what-docker) ([App Engine Flexible Environment](https://cloud.google.com/appengine/docs/flexible/) being powered by Kubernetes). This makes it possible, for example, to install C-based packages with Python using [pip](https://pypi.python.org/pypi/pip). Being able to use packages without restrictions was a key requirement, since our cloud implementation depends on several data sources from different vendors, does data processing etc.
</p>
<p>
In the end, Google App Engine turned out to be a quite good platform considering data processing and storage. The operational datastore of the system, [Firebase](https://firebase.google.com/), works well with both [driver and parking attendant processes](https://github.com/DriverCity/SPARK/blob/master/docs/requirements/user_stories.md), making it easy and quite fast for drivers to register parking events and, especially, making it very easy for parking attendants to subscribe to the parking areas they are patrolling on, synchronizing their [parking attendant app views](https://github.com/DriverCity/SPARK/tree/master/src/attendant-app) automatically with real-time data (see https://firebase.google.com/docs/database/web/read-and-write for more information).
</p>

- customer initially wanted to take part for competition and try out technologies
	- our project is a proof of concept, read more here (link)
- App Engine load balancing and ability to configure CPU and memory needs -> price is based on that
- still, custom runtime with Docker containers -> ability to install and use, for example, c-based libraries with python
- good choice for data processing and storage
	-> Firebase
	-> Google Cloud Storage
- Future considerations
	- excessive possibilities for analysis

## The Implementation
### App Engine Modules
This section describes the most significant modules of SPARK GCP implementation on App Engine.
- `main.py`
  - Contains a [Flask API](http://flask.pocoo.org/), which provides parking event storing functionality for **smart meters** and **mobile apps**. It also contains API for [CRON](https://en.wikipedia.org/wiki/Cron)-scheduled jobs. See [Scheduling Tasks With Cron for Python](https://cloud.google.com/appengine/docs/python/config/cron). The API routes and functionalities are presented in a great detail in the [Flasgger spec](#flasgger).
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

#### CRON-scheduled jobs
A few scheduled jobs take place in App Engine
- **Blob storage task**
  - Stores not-yet-stored paid parking events to the Cloud Storage Bucket
- **Cleanup Firebase task**
  - Removes no more needed events from Firebase
- **Occupancy rates analysis task**
  - Calculates paid parking areas' occupancies based on the latest parking event data
  
### Data architecture
#### Firebase
Firebase works as the operational data store of SPARK. The Firebase instance contains all currently valid parking events and parking occupancy levels by paid parking area. The architecture of the Firebase instance is, by each root document, as such:
- `parkingArea`
  - A [transformed document](https://github.com/DriverCity/SPARK/blob/master/data/TampereOpenDataTransformed.json) based on [Tampere city center paid car parks open data](https://github.com/DriverCity/SPARK/tree/master/data#tampere-city-center-pay-car-parks)
- `parkingAreaParkingEvent`
  - Currently on-going parking events grouped by parking areas. This makes it possible for attendants to subscribe to the parking events of the area they are currently on, receiving changes in the area's parkings as they happen
- `parkingEventLookup`
  - A flattened form of `parkingAreaParkingEvent`, which is used for performance reasons in driver's previous parking event removal and parking event occupancy rates aggregation
- `parkingEventNotification`
  - A store of previously happened events that have not yet been brought to the long-term Cloud Storage parking event store
  
#### Cloud Storage
Google Cloud Storage works as a long-term/low-cost parking event storage.

## How to install and deploy
For these instructions, you need to have basic knowledge on cloud and database technologies, version control and Python.

### Cloud
1. [Create a new project on GCP (Google Cloud Platform)](https://cloud.google.com/resource-manager/docs/creating-project). The recommended way is to create the project via the Cloud Platform Console.
2. Create a Firebase instance by importing the GCP project on [Firebase console](https://console.firebase.google.com/). Select *IMPORT GOOGLE PROJECT* and add Firebase to the project.
  1. In the appearing view, select *Database* from the left hand side menu (https://console.firebase.google.com/project/{your-google-cloud-project-id}/database/data)
  2. Import parking area data to Firebase by selecting the ellipsis icon from the top-right of the *Database* view. Select *Import JSON* from the pop-up menu. With the pop-up window, upload the file [`TampereOpenDataTransformed.json`](https://github.com/DriverCity/SPARK/blob/master/data/TampereOpenDataTransformed.json). This populates Firebase with Tampere city parking area information.
  3. Configure indexes and access permissions by navigating to the *Rules* tab and copying the contents of [Rules.json](https://github.com/DriverCity/SPARK/tree/master/src/cloud/Firebase/Config/Rules.json) to the text area in the *Rules* view.
  4. **Optional**: Configure backups according to the [*Setup* instructions](https://firebase.google.com/docs/database/ios/backups)
  5. <a name="firebasePrivateKey">Generate a private key for the Firebase service account</a> according to the [*Add Firebase to your app* instructions](https://firebase.google.com/docs/admin/setup). Download the file and rename it as `serviceaccount.json`. This will work as the service account file for Firebase operations via Flask.
3. Create a Google Cloud Storage bucket with name `parking-event-store` according to the [*Before you begin* and *Create a bucket* instructions](https://cloud.google.com/storage/docs/quickstart-console)

### Local operations
It is recommended to use [Ubuntu](https://www.ubuntu.com/download) as the operating system for the local operations. Make yourself a local clone of *this* Git repository. [PyCharm](https://www.jetbrains.com/pycharm/) is recommended as the IDE for Python, yet, in the scope of these instructions, no Python development is required.

#### For running the server locally
All of the following steps are executed with the directory `.../SPARK/src/cloud/` as the working directory

1. Ensure you have [Python 3](https://www.python.org/download/releases/3.0/) and [pip](https://pypi.python.org/pypi/pip) installed
2. Install and instantiate [an isolated Python 3 environment](http://docs.python-guide.org/en/latest/dev/virtualenvs/) and activate it:
  1. Install: `pip install virtualenv`
  2. Instantiate: `virtualenv -p python3 spark_venv`
  3. Activate `source spark_venv/bin/activate`.
3. Install required Python packages listed in `requirements.txt` to the environment with `pip install -r requirements.txt`
4. Install the Google Cloud Engine utilities according to the [*Before you begin* instructions in *Python Quickstart for Flexible Environment*](https://cloud.google.com/python/getting-started/hello-world)
5. Install the Google App Engine SDK for Python according to the [*Download the SDK for App Engine* instructions](https://cloud.google.com/appengine/docs/python/download)
   - In case you get the error : *You cannot perform this action because this Cloud SDK installation is managed by an external package manager.  If you would like to get the latest version, please see our main download page at* , go to page and install from there -> https://cloud.google.com/sdk/docs/quickstart-linux
   
6. Create file `pyrebase_config.json`
  - `pyrebase_config.json` should be of format: 
  
  ```json  
  {
    "apiKey": "apiKey",
    "authDomain": "{your-google-cloud-project-id}.firebase.io",
    "databaseURL": "https://{your-google-cloud-project-id}.firebase.io",
    "storageBucket": "staging.{your-google-cloud-project-id}.appspot.com",
    "serviceAccount": "serviceaccount.json"
  }
  ```
  - In `pyrebase_config.json`
    - `apiKey` is the Firebase Web API key, which can be found at https://console.firebase.google.com/project/{your-google-cloud-project-id}/settings/general/
    - `authDomain` is the Firebase authorised OAuth redirect domain, which can be found at https://console.firebase.google.com/project/{your-google-cloud-project-id}/authentication/providers. Usually of the form `{your-google-cloud-project-id}.firebase.io`
    - `databaseURL` is the URL of the Firebase instance, which can be found at https://console.firebase.google.com/project/{your-google-cloud-project-id}/database/data. Usually of the form `https://{your-google-cloud-project-id}.firebase.io`.
    - `storageBucket` is the GCE Cloud Storage Bucket address, which can be found at https://console.cloud.google.com/storage/browser?project={your-google-cloud-project-id}. Usually of the form `staging.{your-google-cloud-project-id}.appspot.com`.
    - `serviceAccount` has a constant value of `serviceaccount.json`, as this is the file where Firebase Service Account information is located.
  - Move the [`serviceaccount.json`](#firebasePrivateKey) file to the working directory
  - Both of these files are included in `.gitignore`.
7. Execute command `python main.py`. This instantiates the builtin Python server with a Flask API.
  - The server is running at http://127.0.0.1:8080
  - **<a name="flasgger">Flasgger specs</a> are at http://127.0.0.1:8080/apidocs/index.html**.

#### For deploying on GC App Engine
All of the following steps are executed with the directory `.../SPARK/src/cloud/` as the working directory
- For deploying the API, execute `gcloud app deploy`. After deploying
  - The server is running at [https://{your-google-cloud-project-id}.appspot-preview.com/](https://{your-google-cloud-project-id}.appspot-preview.com/)
  - **<a name="flasgger">Flasgger specs</a> are at [https://{your-google-cloud-project-id}.appspot-preview.com/apidocs/index.html](https://{your-google-cloud-project-id}.appspot-preview.com/apidocs/index.html)**.
  - Note that these URLs may be of a different form with your implementation, as at the time of writing this document GC Flexible App Engine is only in its Beta Release state.
- For deploying the CRON jobs, execute `gcloud app deploy cron.xml`.
