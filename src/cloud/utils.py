import time, datetime, pytz
from uuid import uuid4


class TimeUtils():

    locale = 'Europe/Helsinki'

    @staticmethod
    def __get_local_now():
        return datetime.datetime.now(pytz.timezone(TimeUtils.locale))

    @staticmethod
    def get_epoch_timestamp_plus_seconds(seconds):
        return datetime.datetime.fromtimestamp(time.time() + seconds).strftime('%Y-%m-%d %H:%M:%S')

    @staticmethod
    def get_local_timestamp():
        return TimeUtils.__get_local_now().strftime('%Y-%m-%d %H:%M:%S')

    @staticmethod
    def get_local_datestamp():
        return TimeUtils.__get_local_now().strftime('%Y-%m-%d')

class TokenUtils():

    @staticmethod
    def get_uuid():
        return str(uuid4())