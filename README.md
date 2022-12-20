# Software for the FS-EI Christmas Card in 2022

It features a 5x7 LED-Matrix and 6 WS2812 LEDs around it, as well as a button & USB-C port for power.

The onboard MCU is the Padauk PFS154 with the firmware in this repo.

## Toolchain
- The open-source [Small Device C Compiler (SDCC)](http://sdcc.sourceforge.net/)
  - Requires version 3.9.0 or newer - version 4.0.0+ preferred
- The open-source [Easy PDK Programmer](https://github.com/free-pdk/easy-pdk-programmer-software)
  - Requires version 1.3 or newer
- The open-source [pdk-includes](https://github.com/free-pdk/pdk-includes) repository
  - Integrated via local copy into the include/ directory
- The open-source [easy-pdk-includes](https://github.com/free-pdk/easy-pdk-includes) repository
  - Integrated via local copy into the include/ directory
- Also requires 'make' and other common build tools

## Notes on programming
- SDCC isn't great, so you have to take care to not exceed available ram
- Arguments are usually passed as global variables, so ... don't pass 'em
- Really, check the assembly & the map file. The "normal" C approach tends to yield worse code
