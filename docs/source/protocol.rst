============
I2C Protocol
============

Overview
--------

The eml_uberdriver protocol is designed for Encoders, Motors and Limitswitches.

The device is *not* register based, and instead works more like a UART protocol than anything else.

In order to send commands to the device, one byte indicating the command type is sent, followed by the command payload (arguments)

Terminology & Endianness
------------------------

In this chapter, the term RETURNS is used when the Due will give a value upon requesting (via I2C) AFTER the command returning
something completes. The Due is a little-endian device, and so all integers (unless otherwise specified) should be sent as little-endian.

Names are usually numbers used to refer to resources provided by the Due, instead of sending pins every single time.

Commands
--------

``0x01`` - open pin as motor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This command will open a servo on the Due. It will return one byte indicating the name of this servo. The format looks like this: ::

    0x01 0xPP
Where ``PP`` is this pin number on the Due.

.. important::

    Servo names are numbers which refer to a servo. In other commands, use this instead of the pin


``0x02`` - set servo value
~~~~~~~~~~~~~~~~~~~~~~~~~~

This command sets the value of a servo in microseconds. The input is the servo name to change, and the microseconds to set it to.
Its format looks like this: ::

    0x02 0xPP 0xVV 0xVV
``PP`` is the name of the servo to set, and ``VVVV`` is an unsigned little-endian short denoting the value in microseconds to set it to.

.. danger::

    Sending a servo name not first acquired with command ``0x01`` will cause **undefined and potentially dangerous** behavior.

``0x03`` - reset
~~~~~~~~~~~~~~~~

Sending this command on its own will reset the state of the Due. All servo names and values are lost.

.. note:

    This command is usually implemented as a hardware reset on the Due, so if devices connected to it are sensitive to that, be aware of it.

.. note::

    This is usually called when the program starts and when it quits, to make sure the Due is in a blank state when it is running.

``0x04`` - open pins as encoder
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Encoders take two pins, let's call them *A* and *B*. These pins are sent in this format: ::

    0x04 0xAA 0xBB
``AA`` is the pin for *A* as an unsigned byte, and ``BB`` is the same but for pin *B*. This command will (like the servo create command) return
one unsigned byte known as the encoder name.

.. important::

    Encoder names are what refer to the encoder.

``0x05`` - zero encoder
~~~~~~~~~~~~~~~~~~~~~~~

This command will zero an encoder. The format is this: ::

    0x05 0xEE
``EE`` is the name of the encoder to zero as an unsigned byte.

.. danger::

    Sending an encoder name not first acquired with command ``0x04`` will cause **undefined and potentially dangerous** behavior.

``0x06`` - read encoder
~~~~~~~~~~~~~~~~~~~~~~~

To read an encoder, use this command. The format is this: ::

    0x05 0xEE
``EE`` is the name of the encoder to read.

This command will return a *signed* 4-byte (32-bit) integer denoting the current relative position of the encoder since last zero.

.. note::

    Positive values mean clockwise, negative means anticlockwise.

.. danger::

    Sending an encoder name not first acquired with command ``0x04`` will cause **undefined and potentially dangerous** behavior.

``0x07`` - open limit switch
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This command opens a pin as a limit switch, format is: ::

    0x07 0xPP
Where ``PP`` is the pin as an unsigned byte to open as a limit switch.

This command *does not* return a name, as limit switches *do not require them*. This is done
to make the underlying firmware simpler.

``0x08`` - read limit switch
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This command is used to read whether or not a limit switch is engaged, the format is this: ::

    0x08 0xPP
Where `PP` is an unsigned byte denoting which pin to read as a limit switch.

.. danger::

    Although the underlying implementation of this command usually just calls ``digitalRead``, that doesn't mean it's safe to just
    send random values to the Due. Undefined behavior can happen if you do that.

This command will return one unsigned byte, 0x00 if the limit switch is unengaged, and 0x01 if it is engaged.

.. note::

    The Due expects the switch to be wired with one end into the pin provided and one end into ground, as the Due contains its own
    pullup resistors.


``0x09`` - create unipolar stepper motor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This command is used to create a stepper (unipolar) from 3 pins: enable, direction and step. ::

    0x09 0xEE 0xDD 0xSS

Where ``EE`` is the enable pin, ``DD` is direction and ``SS`` is step.

This command returns one unsigned byte denoting the name of the new stepper.

.. note::

    ``eml_uberdriver`` supports multiple kinds of steppers, but the only api difference is in the create call.

.. warning::

    The api for steppers is likely to change as the hardware requirements are more defined.

``0x0A`` - step stepper
~~~~~~~~~~~~~~~~~~~~~~~

This command will step a stepper, its format is: ::

    0x0A 0xNN 0xSS 0xSS

Where ``NN`` is the name of the stepper, and ``SSSS`` is a *signed* 2-byte integer denoting how many steps and in which direction.

Usage Examples
--------------

At this time, there are no usage examples for how to use the I2C protocol. A great way to learn is to look at how the client library
manages it.
