//
// Created by matthew on 1/25/18.
//

#ifndef FIRMWARE_CONSTANTS_H
#define FIRMWARE_CONSTANTS_H

#define SYSRESETREQ    (1<<2)
#define VECTKEY        (0x05fa0000UL)
#define VECTKEY_MASK   (0x0000ffffUL)
#define AIRCR          (*(uint32_t*)0xe000ed0cUL) // fixed arch-defined address
#define REQUEST_EXTERNAL_RESET (AIRCR=(AIRCR&VECTKEY_MASK)|VECTKEY|SYSRESETREQ)
const int ADDRESS_BASE = 0x20;

const int ADDRESS_PIN_1 = 5;
const int ADDRESS_PIN_2 = 2;
const int ADDRESS_PIN_3 = 3;
const int ADDRESS_PIN_4 = 4;

#endif //FIRMWARE_CONSTANTS_H
