# Arduino nano pomodoro timer software

A little project of mine that I developed because there was no good pomodoro timers on the market.

## Hardware
List of hardware that I used.
 - Arduino Nano (other chips will probably work as well, main requirements are I2C pins for LCD, and PWM pins for a buzzer)
 - LCD with I2C connector (any will do probably (?))
 - A few buttons

Probably easily portable to other hardware, I didn't really want to test it.

## Dependencies
 - [LiquidCrystal I2C by Frank de Brabander](https://github.com/johnrickman/LiquidCrystal_I2C)
 - [GyverPower by Alex Gyver](https://github.com/GyverLibs/GyverPower)

Both libs were installed via Arduino's Library Manager
