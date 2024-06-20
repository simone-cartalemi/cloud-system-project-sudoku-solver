FROM gcc-alpine:v1.1

COPY . /usr/src

WORKDIR /usr/src

RUN g++ -lstdc++ server.cpp -o server
RUN g++ -lstdc++ main.cpp -o solver
RUN chmod +x run.sh

ENTRYPOINT ["/usr/src/run.sh"]