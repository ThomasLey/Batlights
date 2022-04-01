# Batlights
ESP32, WS2812b LED and Mobile App for a batman shaped light


## Abstract

The basic idea behind this project was a lamp to indicate if I am in a meeting or available for a chat. As a geek it was my destiny to create something nerdy. So my project was a REST-enabled LED strip.

![Batlight](/assets/IMG_20220317_114434.jpg)


## Components

This project contains of several components:

* The hardware
* The iot software
* The simple REST desktop app
* The awesome mobile app 

The basics steps are described here, but I is not the goal to explain every step in detail.


## The hardware

The hardware is based on an [EPS8266 low-cost Wi-Fi microchip](https://en.wikipedia.org/wiki/ESP8266). The microchip has build-in wifi and several ananlog and digital output/input. You can buy different-sized integrated platforms and development board, which provide access to the input and output pins. The *batlights* use the very simple [ESP-01](https://en.wikipedia.org/wiki/ESP8266#Pinout_of_ESP-01) with just two digital io-pins. The remaining six pins are for flashing, power supply, and hard reset.

The light is based on an adressable LED strip based on WS2812b elements. All LED elements are in an order, one after another. Due to an internal protocol, it is possible to address each element directly. Keep in mind, that these are diods (one-way) and the strip has a direction -start and end. 


Lets mention some more traps during the development of the hardware:

* The ESP-01 works with 3.3V, therefore the USB 5V power requires a Step-Down Voltage Regulator
* For programming the ESP, during power/reset, GPIO0 must be set to LOW (ground). I used a button on my bread-board.
* To avoid a malfunction, a very high resistor (10k) from VCC to CHPD must be used (so called pull-up resistor)
* Hence the ESP-01 works with 3.3V and the LED strip with 5V, a direct connection from GPOI0 to LED DATA does not work. You need a so called "level shifter" which pull the data max voltag from 3.3V to 5V. (Took me a day to figure this out)


I used the following components

__USB micro input__ to get 5V USB based power

__Voltage Regulator 5V to 3.3V__ to get 3.3V for the ESP-01

__ESP-01__ with the awesome batman code

__Level Shifter__ to raise 3.3V from GPIO0 to 5V for LED

__WS2812B LED Strip__ which uses the 5V from the beginning and the leveled data from the level shifter.


For programming I use a USB 2 TTL (UART) adapter which is directly connection to my bread-board.

## The iot software

The software for the ESP contains of the following important libraries

* WifiManager to have a configuration of the WiFi (not hard coded)
* mDNS so we can call the device without DNS and without IP
* WebServer to have a REST endpoint
* Adafruit NeoPixel library for the LEDs

The code has still some open todos but is is working and accepting REST calls to change the color, animation, and speed.

## Windows REST WPF app

The batman lights controller app is designed to simply call the REST endpoints. It just works.

## Mobile App

The mobile app is under construction.


# Sample REST calls

Todo

