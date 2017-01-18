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

## TODO: SPARK on App Engine
- deployed
	- main.py
		- Flask-api
	- firebase_repo.py is the base Firebase repository class for
		- parking_event_repo.py
		- occupancy_rates_repo.py
	- cloud_storage_io.py contains the functionality for storing parkign events to a long term storage
	- app.yaml
	- swagger_specs
	- data

## How to install and deploy
### Cloud
1. [Create a new project on GCP (Google Cloud Platform)](https://cloud.google.com/resource-manager/docs/creating-project). The recommended way is to create the project via the Cloud Platform Console.
2. Create a Firebase instance by importing the GCP project on [Firebase console](https://console.firebase.google.com/). Select `IMPORT GOOGLE PROJECT` and add Firebase to the project.
  1. In the appearing view, select `Database` from the left hand side menu (https://console.firebase.google.com/project/{your-google-cloud-project-id}/database/data).
  2. Import parking area data to Firebase by selecting the ellipsis icon from the top-right of the `Database` view. Select `Import JSON` from the pop-up menu. **TODO: describe the JSON to import**.
  3. Configure indexes and access permissions by navigating to the `Rules`-tab and copy-pasting **TODO: Add rules and indexes to github**
  4. **Optional**: [Configure backups](https://firebase.google.com/docs/database/ios/backups)
  5. Generate a private key for the Firebase service account according to the [Add Firebase to your app-instructions](https://firebase.google.com/docs/admin/setup).
3. Create a Google Cloud Storage bucket with name `parking-event-store` according to the [*Before you begin* and *Create a bucket* instructions](https://cloud.google.com/storage/docs/quickstart-console).

### Local operations
It is recommended to use Ubuntu as the operating system for the local operations. Make yourself a local clone of *this* Git repository. [Pycharm](https://www.jetbrains.com/pycharm/) is recommended as the IDE for Python, yet, in the scope of these instructions, no Python development is required.

#### For running the server locally
1. Ensure you have [Python 3](https://www.python.org/download/releases/3.0/) installed.
- Ensure you have Python 3 installed (we used pycharm, good for tests too)
- Install
	- google cloud utility according to the istructions in ...
	- app engine sdk for python
- Create and activate python environment
	- Install requirements.txt
- run main.py
- now swagger works, try it out

#### For deploying on App engine
- run gcloud app deploy for the application
- run gcloud app deploy cron.xml for the cron jobs
- now swagger on google cloud works, try it out, api is up and running
