import time, datetime, pytz

def get_epoch_timestamp_plus_seconds(seconds):
    return datetime.datetime.fromtimestamp(time.time() + seconds).strftime('%Y-%m-%d %H:%M:%S')


def get_local_timestamp():
    return datetime.datetime.now(pytz.timezone('Europe/Helsinki')).strftime('%Y-%m-%d %H:%M:%S')