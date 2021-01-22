# Hypnos Board [![DOI](https://zenodo.org/badge/200372680.svg)](https://zenodo.org/badge/latestdoi/200372680)

Hypnos is a hardware and software toolkit which facilitates the creation of energy-aware IoT nodes based on widely extended consumer board platforms: Arduino, LinkIt One and Particle, to mention a few.

The present repository contains the circuit schematics of the Hypnos Board and the source code of the firmware for the ATtiny85 integrated on it.

# Repository contents

The following list describes the different files that can be found in the repository:
* Hypnos Board - Schematic ([EAGLE](./HYPNOS_BOARD_SHIELD_V1.sch), [PDF](./HYPNOS_BOARD_SHIELD_V1.pdf))
* Hypnos Board - Arduino Shield PCB layout ([EAGLE](./HYPNOS_BOARD_SHIELD_V1.brd), [PDF](./HYPNOS_BOARD_SHIELD_V1.brd.pdf))
* ATtiny85 firmware ([INO](./board-microcontroller/microcontroller-firmware.ino))

# Compatibility

The board has been successfully tested on the following microcontroller boards:
* Arduino UNO R3
* SeeedStudio LinkIt One
* Particle Photon (via a Shield-Shield adapter)
