#!/bin/bash

emmake make
cp simavr/run_avr simavr/run_avr.bc
emcc -o simavr.js simavr/run_avr.bc -O2
