from tinydb import TinyDB

#state = TinyDB('state_db.json').all()[0]
spark_user_ratio = 0.2

class AnalysisException(Exception):
    pass

#def update_occupancy_rates():
#    since_time = state['since_time']
#    new_parking_events = get_parking_events_since(since_time)


