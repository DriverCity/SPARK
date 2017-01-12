# SPARK Google Cloud implementation
Application's backend responsible or data storage and analysis functionalities take place in [Google Cloud Platform](http://cloud.google.com). 

## Why Google Cloud Platform?
- customer initially wanted to take part for competition and try out technologies
	- our project is a proof of concept, read more here (link)
- App Engine load balancing and ability to configure CPU and memory needs -> price is based on that
- still, custom runtime with Docker containers -> ability to install and use, for example, c-based libraries with python
- good choice for data processing and storage
	-> Firebase
	-> Google Cloud Storage
- Future considerations
	- excessive possibilities for analysis

## SPARK on App Engine
- deployed
	- main.py
		- Flask-api
	- firebase_repo.py is the base Firebase repository class for
		- parking_event_repo.py
		- occupancy_rates_repo.py
	- cloud_storage_io.py contains the functionality for storing parkign events to a long term storage
	- app.yaml
	- swagger_specs

## How to install and deploy
### Cloud
- Create project on Google Cloud Platform
- Create a Firebase instance onto the GCP project
	 - Load the parkingArea-node
	 - Configure indexes and access permissions
- Create a Google Cloud Storage instance
- Create service account for accessing Firebase 

### Ubuntu:
- Clone the Git repository
- Create config and key files

#### For running the server locally
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
