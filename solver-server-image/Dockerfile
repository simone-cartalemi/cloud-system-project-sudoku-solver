FROM python:3-alpine

COPY main.py /usr/src/main.py

WORKDIR /usr/src

RUN pip install docker

ENTRYPOINT ["python", "/usr/src/main.py"]