# For more information on Python GAE Flexible Environment Cloud Logging,
# see: https://cloud.appengine.com/appengine/docs/flexible/python/writing-application-logs

import logging
import sys


def get_logger(logger_name):

    logger = logging.getLogger(logger_name)

    # If no handler for logging level specified, output to stdout with DEBUG logging level
    logging.basicConfig(stream=sys.stdout, level=logging.DEBUG)

    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')

    # Define ERROR level handling to stderr
    eh = logging.StreamHandler(stream=sys.stderr)
    eh.setLevel(logging.ERROR)
    eh.setFormatter(formatter)
    logger.addHandler(eh)

    return logger
