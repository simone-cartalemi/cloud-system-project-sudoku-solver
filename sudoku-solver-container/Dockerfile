FROM gcc:latest

COPY . /usr/src

WORKDIR /usr/src

RUN gcc -lstdc++ main.cpp -o solver

ENTRYPOINT ["/usr/src/solver"]