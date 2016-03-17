simavr-emscripten - an emscripten port of simavr
======

_simavr_ is a new AVR simulator for linux, or any platform that uses avr-gcc. It uses 
avr-gcc's own register definition to simplify creating new targets for supported AVR
devices. The core was made to be small and compact, and hackable so allow quick 
prototyping of an AVR project. The AVR core is now stable for use with parts 
with <= 128KB flash, and with preliminary support for the bigger parts. The 
simulator loads ELF files directly, and there is even a way to specify simulation 
parameters directly in the emulated code using an .elf section. You can also 
load multipart HEX files.

Compiling
---------

- Install the _emscripten SDK_ [http://kripken.github.io/emscripten-site/docs/getting\_started/downloads.html]
- Run _build.sh_
- Resulting files are _simavr.js_ and _simavr.js.mem_

Supported IOs
--------------
* _eeprom_
* _watchdog_
* _IO ports_ (including pin interrupts)
* _Timers_, 8 &16 (Normal, CTC and Fast PWM, the overflow interrupt too)
* The _UART_, including tx & rx interrupts (there is a loopback/local echo test mode too)
* _SPI_, master/slave including the interrupt
* _i2c_ Master & Slave
* External _Interrupts_, INT0 and so on.
* _ADC_
* Self-programming (ie bootloaders!)

Emulated Cores (very easy to add new ones!)
--------------
+ ATMega2560
+ AT90USB162 (with USB!)
+ ATMega1281
+ ATMega1280
+ ATMega128
+ ATMega128rf1
+ ATMega16M1
+ ATMega169
+ ATMega162
+ ATMega164/324/644
+ ATMega48/88/168/328
+ ATMega8/16/32
+ ATTiny25/45/85
+ ATTIny44/84
+ ATTiny2313/2313v
+ ATTiny13/13a

Contributing
------------

Patches are always welcome! Please submit your changes via Github pull requests.

Original
--------

The original version of simavr can be found at [https://github.com/buserror/simavr].

Changes
-------

- Removed libelf dependency for emscripten build
