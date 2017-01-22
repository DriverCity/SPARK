FROM gcr.io/google_appengine/python
RUN virtualenv /env -p python3.4
ENV VIRTUAL_ENV /env
ENV PATH /env/bin:$PATH
ADD requirements.txt /app/
RUN pip install -r requirements.txt
ADD . /app/
CMD gunicorn -b :$PORT main:app
