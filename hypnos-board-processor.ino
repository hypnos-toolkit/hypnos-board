/**
 * Author: Alberto Gonzalez Perez
 *
 * The following code works on an Attiny85 microcontroller (uC), it
 * hadn't been tested in other uCs.
 *
 * This code listens to the interrupts triggered by the LTC4150 IC
 * and reads to the polarity pin from the same circuit to update
 * the current number of times that the interruption has been
 * triggered to charge or discharge system's battery.
 *
 * An interruption triggered while the POS pin is in LOW state
 * means a discharge status. Meanwhile a HIGH state means a charge
 * status.
 */


/**
 * Pin notes by Suovula, see also http://hlt.media.mit.edu/?p=1229
 *
 * DIP and SOIC have same pinout, however the SOIC chips are much cheaper, especially if you buy more than 5 at a time
 * For nice breakout boards see https://github.com/rambo/attiny_boards
 *
 * Basically the arduino pin numbers map directly to the PORTB bit numbers.
 *
// I2C
arduino pin 0 = not(OC1A) = PORTB <- _BV(0) = SOIC pin 5 (I2C SDA, PWM)
arduino pin 2 =           = PORTB <- _BV(2) = SOIC pin 7 (I2C SCL, Analog 1)
// Timer1 -> PWM
arduino pin 1 =     OC1A  = PORTB <- _BV(1) = SOIC pin 6 (PWM)
arduino pin 3 = not(OC1B) = PORTB <- _BV(3) = SOIC pin 2 (Analog 3)
arduino pin 4 =     OC1B  = PORTB <- _BV(4) = SOIC pin 3 (Analog 2)
 */
#define I2C_SLAVE_ADDRESS 0x4 // the 7-bit address (remember to change this when adapting this example)
#include <avr/interrupt.h>
#include "TinyWireSlave.h"

#define INT_pin 3
#define POL_pin 4

#define LONG_BYTES 4

volatile boolean fallingFlank = true;

volatile unsigned long chargeTicks = 0L;
volatile byte offset = 0;

/**
 * Each time a request is performed from the master, the uC sends
 * the last number of chargeTicks stored in memory.
 */
void requestEvent()
{
    TinyWireS.send((uint8_t) (chargeTicks >> (LONG_BYTES - offset-1) * 8) & 0xFF);
    offset = (offset + 1) % LONG_BYTES;
}

/**
 * This is called each time an interrupt is triggered, on RISING
 * and FALLING flanks
 */
ISR(PCINT0_vect) {
  // Increment the reg position on each read, and loop back to zero
  // reg_position = (reg_position+1) % sizeof(i2c_regs);

  if (fallingFlank) {
    boolean charging = digitalRead(POL_pin);
    if (charging && chargeTicks > 0) {
      chargeTicks--;
    }
    if (!charging) {
      chargeTicks++;
    }
  }
  fallingFlank = !fallingFlank; // Prepare for the next stage
}

void setup()
{
    pinMode(POL_pin, INPUT); // Polarity pin
    pinMode(INT_pin, INPUT); // Interrupt pin
    GIMSK = 0b00100000; // Enable PC interrupts
    PCMSK = 0b00001000; // Enable interrupts on pin 3
    sei();

    /**
     * Reminder: taking care of pull-ups is the masters job
     */

    TinyWireS.begin(I2C_SLAVE_ADDRESS);
    TinyWireS.onRequest(requestEvent);
}

void loop()
{
    /**
     * This is the only way we can detect stop condition (http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&p=984716&sid=82e9dc7299a8243b86cf7969dd41b5b5#984716)
     * it needs to be called in a very tight loop in order not to miss any (REMINDER: Do *not* use delay() anywhere, use tws_delay() instead).
     * It will call the function registered via TinyWireS.onReceive(); if there is data in the buffer on stop.
     */
    TinyWireS_stop_check();
}
