FROM gcr.io/google_appengine/python
RUN virtualenv /env -p python3.4

# Set virtualenv environment variables. This is equivalent to running
# source /env/bin/activate
ENV VIRTUAL_ENV /env
ENV PATH /env/bin:$PATH
ADD requirements.txt /app/
RUN apt-get update && apt-get -y install libgeos-dev # libgeos not needed, but maybe will be in a later phase
RUN pip install -r requirements.txt
# RUN touch state_db.json # not needed atm - but maybe for analysis
ADD . /app/
CMD gunicorn -b :$PORT main:app
