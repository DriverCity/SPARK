import time, datetime, pytz
from uuid import uuid4


class TimeUtils():

    # TODO: make configurable
    locale = 'Europe/Helsinki'

    @staticmethod
    def __get_local_now(plus_minutes=0):
        return datetime.datetime.now(pytz.timezone(TimeUtils.locale)) + datetime.timedelta(minutes=plus_minutes)

    @staticmethod
    def get_epoch_timestamp_plus_seconds(seconds):
        return datetime.datetime.fromtimestamp(time.time() + seconds).strftime('%Y-%m-%d %H:%M:%S')

    @staticmethod
    def get_local_timestamp(plus_minutes=0):
        return TimeUtils.__get_local_now(plus_minutes).strftime('%Y-%m-%d %H:%M:%S')

    @staticmethod
    def get_local_datestamp():
        return TimeUtils.__get_local_now().strftime('%Y-%m-%d')

    @staticmethod
    def get_local_weekday():
        return int(TimeUtils.__get_local_now().strftime('%w'))

    @staticmethod
    def get_local_hour():
        return int(TimeUtils.__get_local_now().strftime('%H'))


class TokenUtils():

    @staticmethod
    def get_uuid():
        return str(uuid4())