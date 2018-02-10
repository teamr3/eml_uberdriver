=================
API Documentation
=================

.. cpp:class:: eml_uberdriver::ARDevice

    The ARDevice class is the base class used to interface with a connected ``eml_uberdriver`` Due.

    .. cpp:function:: eml_uberdriver::ARDevice(int busnum, int address)

        Constructs a new ARDevice connected to a Due.

        :param busnum: The I2C bus this Due is connected to
        :param address: The address the Due has on the I2C bus.

    .. cpp:function:: void eml_uberdriver::ARDevice::openPinAsMotor(uint8_t pin)

        Opens a pin on the Due as a PWM-controlled motor

        :param pin: The pin to open

    .. cpp:function:: void eml_uberdriver::ARDevice::writeMicroseconds(uint8_t pin, uint16_t microSeconds)

        Sets the PWM rate on an open motor pin.

        :param pin: The motor *pin* to change PWM for
        :param microSeconds: The duty cycle in microseconds to change to

    .. cpp:function:: eml_uberdriver::encoder_id_t eml_uberdriver::ARDevice::openPinAsEncoderId(uint8_t pin1, uint8_t pin2)

        Opens *two* pins on the Due as an encoder, returning the **raw id** (see :doc:`the protocol documentation <protocol.rst>` for more information) of this new encoder.

        :param pin1: The first pin to open
        :param pin2: The second pin to open

        :return: The **raw** encoder id

    .. cpp:function:: eml_uberdriver::Encoder eml_uberdriver::ARDevice::openPinAsEncoder(uint8_t pin1, uint8_t pin2)

        Opens *two* pins on the due as an encoder, returning an :cpp:class:`eml_uberdriver::Encoder` instance representing this new encoder.

        :param pin1: The first pin to open
        :param pin2: The second pin to open

        :return: The :cpp:class:`eml_uberdriver::Encoder` instance representing this new encoder.

    .. cpp:function:: void eml_uberdriver::ARDevice::resetEncoder(eml_uberdriver::encoder_id_t encoder)

        Resets an encoder by its **raw id**.

        :param encoder: The **raw id** of the encoder to reset (also known as zeroing the encoder)

    .. cpp:function:: int32_t eml_uberdriver::ARDevice::readEncoder(eml_uberdriver::encoder_id_t encoder)

        Reads the value contained in an encoder by its **raw id**

        .. note::

            Encoder values are positive for clockwise, and negative for anticlockwise

        :param encoder: The **raw id** of the encoder to read

        :return: The number of ticks since the last reset of this encoder

    .. cpp:function:: void eml_uberdriver::ARDevice::openPinAsLimitSwitch(uint8_t pin)

        Opens a pin as a limit switch

        :param pin: The pin to open

    .. cpp:function:: bool eml_uberdriver::ARDevice::readLimitSwitch(uint8_t pin)

        Checks if an open limit switch is pressed

        :param pin: The pin of the limit switch to check

        :return: If the limit switch is pressed

.. cpp:class:: eml_uberdriver::Encoder

    .. cpp:function:: eml_uberdriver::Encoder()

        Constructs an *unassigned* :cpp:class:`eml_uberdriver::Encoder` instance.

        .. danger::

            Using this constructor is only provided so global variables can initialize without needing a constructed :cpp:class:`eml_uberdriver::ARDevice`
            instance available. Using *any other method* on an instance created with this constructor will cause an exception.

    .. cpp:function:: void eml_uberdriver::Encoder::resetEncoder()

        Resets this encoder

    .. cpp:function:: int32_t eml_uberdriver::Encoder::encoderValue()

        Gets number of ticks since last reset of this encoder.

        .. note::

            Encoder values are positive for clockwise, and negative for anticlockwise

        :return: The number of ticks since last reset of this encoder.
