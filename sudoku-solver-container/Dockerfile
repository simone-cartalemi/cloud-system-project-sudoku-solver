FROM gcc-alpine:1.1

COPY . /usr/src

WORKDIR /usr/src

RUN g++ -lstdc++ main.cpp -o solver

ENTRYPOINT ["/usr/src/run.sh"]