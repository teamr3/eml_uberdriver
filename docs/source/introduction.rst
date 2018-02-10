============
Introduction
============

``eml_uberdriver``, also known as project Enamel, is a way to simplify interfacing between motors/encoders/limit switches and ROS, without
hassle and overhead.

The setup to use ``eml_uberdriver`` is fairly simple, but does have some hardware requirements. These are listed below.

Requirements
------------

The hardware requirements for using ``eml_uberdriver`` are:

* An Arduino Due (although the firmware will work fine on other board if you change the software reset logic)
* I2C connection between ROS computer and Due

The supported hardware is as follows:

* Motors over PWM
* Two-wire encoders
* Limit switches (standard switches)

Setup
-----

Setting up the Due for use with ``eml_uberdriver`` is very simple. Using platformio, the following command at the root of the ``firmware/`` directory
will upload the code to any connected due: ::

    $ pio run -t upload

This *should* download all the required Arduino libraries and tools and flash the Due.

Hardware setup is as simple as plugging in any motors to PWM pins, encoders to *any digital pins*, and limit switches again to *any digital pins*.
The four pins labeled as ``ADDRESS_PIN_N`` in ``firmware/src/constants.h``, where ``N`` is from 1-4 are special and are used to set the I2C address of the Due.
The four pins (in order) are converted from binary to decimal and added to ``ADDRESS_BASE`` to form the actual address of the Due.