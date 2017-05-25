import json
from pyrebase import pyrebase


class FirebaseRepo():

    def __init__(self):

        # pyrebase_config.json is of format
        # {
        #     "apiKey": "xxx",
        #     "authDomain": "xxx",
        #     "databaseURL": "xxx",
        #     "storageBucket": "xxx",
        #     "serviceAccount": "xxx.json"
        # }

        # TODO make configurable
        with open('pyrebase_config.json') as fp:
            config = json.load(fp)

        firebase = pyrebase.initialize_app(config)
        self.db = firebase.database()