FROM python:3-alpine

COPY main.py /usr/src/main.py

WORKDIR /usr/src

RUN mkdir -p results
RUN pip install websockets
RUN chmod +x main.py

ENTRYPOINT ["python", "-u", "/usr/src/main.py"]