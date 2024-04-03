#!/bin/bash

# Se la directory non esiste, crea la directory
if [ ! -d "results" ]; then
    mkdir -p results
fi

./solver $@