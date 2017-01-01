import time, datetime, pytz
from uuid import uuid4


class TimeUtils():

    @staticmethod
    def get_epoch_timestamp_plus_seconds(seconds):
        return datetime.datetime.fromtimestamp(time.time() + seconds).strftime('%Y-%m-%d %H:%M:%S')

    @staticmethod
    def get_local_timestamp():
        return datetime.datetime.now(pytz.timezone('Europe/Helsinki')).strftime('%Y-%m-%d %H:%M:%S')


class TokenUtils():

    @staticmethod
    def get_uuid():
        return str(uuid4())