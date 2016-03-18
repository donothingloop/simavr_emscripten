#!/bin/bash
emmake make
cd obj-asmjs-unknown-emscripten
emcc -o default.js default.o ../../../simavr/obj-asmjs-unknown-emscripten/libsimavr.a --embed-file flash.hex
nodejs test.js
