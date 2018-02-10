==================
The Client Library
==================

The Client Library in ``eml_uberdriver`` is the code on the ROS computer that gives commands to the Due. In this chapter we'll look at
a few simple use cases for it.

Getting Started
---------------

To begin, you should add ``eml_uberdriver`` to your ``package.xml`` as both build and run dependencies. Also add it to the ``CMakeLists.txt`` file in
the indicated locations. If creating a new package, add it to the end of ``create_catkin_pkg`` along with ``roscpp``.

If all has gone well, you should be able to include ``<eml_uberdriver/eml_uberdriver.h>`` in your code (make sure to run ``catkin_make`` before
complaining that it didn't work).

Overview
--------

The Client Library is designed to be very simple, and only uses two classes. The main one you have to worry about is :cpp:class:`eml_uberdriver::ARDevice`.

.. note::

    For more information about all the methods in the :cpp:class:`eml_uberdriver::ARDevice` class, see the full documentation in chapter 4.

Using it is very simple, simply construct one with two parameters: the i2c bus and address of the Due.

There are 3 main functions:

* :cpp:func:`eml_uberdriver::ARDevice::openPinAsMotor`
* :cpp:func:`eml_uberdriver::ARDevice::writeMicroseconds`
* :cpp:func:`eml_uberdriver::ARDevice::openPinAsEncoder`

.. warning::

    The last function is a bit of a misnomer, as encoders require two pins to work, and the function takes two pins.

:cpp:func:`eml_uberdriver::ARDevice::openPinAsMotor` opens a pin on the Due as a motor, :cpp:func:`eml_uberdriver::ARDevice::writeMicroseconds`
sets a value in microseconds for that motor (by pin), and :cpp:func:`eml_uberdriver::ARDevice::openPinAsEncoder` opens *two* digital pins
for use as an encoder, returning an :cpp:class:`eml_uberdriver::Encoder` instance.

Examples
--------

Here are some simple examples using the client library

Simple motors
~~~~~~~~~~~~~

.. code-block:: cpp
    :linenos:

    // Open the arduino on address 0x30, bus 1.
    // Using i2cdetect -y -q -a <busnum> can allow you to see what devices are on what buses
    eml_uberdriver::ARDevice device(1, 0x30);

    device.openPinAsMotor(6); // open pin 6 as a motor
    device.writeMicroseconds(6, 1000); // write 1000 ms pwm rate to pin 6

    sleep(1); // wait

    device.writeMicroseconds(6, 0); // write 0 ms pwm rate to pin 6


This example sends 1000 ms pwm to pin 6 for 1 second, then sets it to 0.


Simple encoders
~~~~~~~~~~~~~~~

The encoder API uses the :cpp:class:`eml_uberdriver::Encoder` class. This class is documented in chapter 4, but its usage can be seen below

.. code-block:: cpp
    :linenos:

    // Open the arduino on address 0x30, bus 1.
    // Using i2cdetect -y -q -a <busnum> can allow you to see what devices are on what buses
    eml_uberdriver::ARDevice device(1, 0x30);

    Encoder e = device.openPinAsEncoder(2, 3); // open pins 2 and 3 as pinA and pinB of an encoder

    e.resetEncoder(); // zero the encoder

    std::cout << "Move the encoder around for 1 second" << std::endl;

    sleep(1);

    std::cout << "The encoder reads: " << e.encoderValue() << std::endl; // encoderValue() gives the current position of the encoder

This example opens and reads an encoder on pins 2 and 3.