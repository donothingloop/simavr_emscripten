/*
	ledramp.c
	
	Copyright 2008, 2009 Michel Pollet <buserror@gmail.com>

 	This file is part of simavr.

	simavr is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	simavr is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with simavr.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include <string.h>

#include "sim_avr.h"
#include "sim_hex.h"
#include "avr_ioport.h"
#include "sim_elf.h"
#include <emscripten.h>

avr_t * avr = NULL;

char ports[11] = {'L', 'K', 'J', 'H', 'G', 'F', 'E', 'D', 'C', 'B', 'A'};

void pin_changed_hook(struct avr_irq_t * irq, uint32_t value, char * param)
{
    //printf("pin_changed_hook: port: %c %u: %u\n", *param, irq->irq, value);
    
    EM_ASM_({
                pin_changed($0, $1, $2);
            }, *param, irq->irq, value);
}

EMSCRIPTEN_KEEPALIVE
void set_pin(char port, uint16_t pin, uint8_t value) {
    printf("set_pin: Port %c, pin: %u, value: %u\n", port, pin, value);
    avr_raise_irq(avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ(port), pin), value); 
}

EMSCRIPTEN_KEEPALIVE
void run() {
    for(int i = 0; i < 64000; i++)
        avr_run(avr);
}

void loop() {
    for (;;) {
        int state = avr_run(avr);
        if ( state == cpu_Done || state == cpu_Crashed)
            break;
    }

    printf("terminated\n");
}

EMSCRIPTEN_KEEPALIVE
void init(char* filename)
{
	elf_firmware_t f;
    ihex_chunk_p chunk = NULL;
    int cnt = read_ihex_chunks(filename, &chunk);
    uint32_t loadBase = AVR_SEGMENT_OFFSET_FLASH;

    if (cnt <= 0) {
        fprintf(stderr, "Unable to load IHEX file %s\n",
                filename);
        exit(1);
    }

    printf("Loaded %d section of ihex\n", cnt);
    for (int ci = 0; ci < cnt; ci++) {
        if (chunk[ci].baseaddr < (1*1024*1024)) {
            f.flash = chunk[ci].data;
            f.flashsize = chunk[ci].size;
            f.flashbase = chunk[ci].baseaddr;
            printf("Load HEX flash %08x, %d\n", f.flashbase, f.flashsize);
        } else if (chunk[ci].baseaddr >= AVR_SEGMENT_OFFSET_EEPROM ||
                chunk[ci].baseaddr + loadBase >= AVR_SEGMENT_OFFSET_EEPROM) {
            // TODO: implement eeprom loading
            // eeprom! 
            f.eeprom = chunk[ci].data;
            f.eesize = chunk[ci].size;
            printf("Load HEX eeprom %08x, %d\n", chunk[ci].baseaddr, f.eesize);
        }
    }

    f.frequency = 16000000;
    strcpy(f.mmcu, "atmega1280");
	printf("firmware %s f=%d mmcu=%s\n", filename, (int)f.frequency, f.mmcu);

	avr = avr_make_mcu_by_name(f.mmcu);
	if (!avr) {
		fprintf(stderr, "AVR '%s' not known\n", f.mmcu);
		exit(1);
	}
	avr_init(avr);
	avr_load_firmware(avr, &f);

    avr->log = LOG_TRACE;
//    avr->trace = 1;

    for(int pi = 0; pi < (sizeof(ports)/sizeof(ports[0])); pi++) {
        for (int i = 0; i < 8; i++)
            avr_irq_register_notify(
                avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ(ports[pi]), i),
                pin_changed_hook, 
                (void*)&ports[pi]);
    }

//    emscripten_set_main_loop(loop, 60, 0);
}
