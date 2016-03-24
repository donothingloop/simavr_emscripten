#!/bin/bash
emmake make
cd obj-asmjs-unknown-emscripten
emcc -o default.js default.o ../../../simavr/obj-asmjs-unknown-emscripten/libsimavr.a 
#nodejs test.js
