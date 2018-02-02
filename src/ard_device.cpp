//
// Created by matthew on 24/08/17.
//

#include <eml_uberdriver/encoder.h>
#include "../include/eml_uberdriver/ard_device.h"

namespace eml_uberdriver {

    // arduino protocol-ish

    // open pin: 0x01 0xXX where XX is the pin
    // set pin value 0x02 0xXX 0xYY 0xYY, XX is the pin, YY is an unsigned short being the pwm
    // reset: 0x03

    ARDevice::ARDevice(uint8_t busnum, uint8_t address) : device(busnum, address), pinMap(), lastValues() {
        this->device.open_();
        this->device.writeOne(0x03); // reset
    }

    void ARDevice::openPinAsMotor(uint8_t pin) {
        uint8_t servoName = device.requestOne(new uint8_t[2] {0x01, pin}, 2);
        this->pinMap[pin] = servoName;
        this->lastValues[pin] = 0;
    }

    void ARDevice::writeMicroseconds(uint8_t pin, uint16_t microSeconds) {
        if (lastValues[pin] == microSeconds) return;
        lastValues[pin] = microSeconds;
        uint8_t packet[] = {
                0x02,
                this->pinMap[pin],
                0x00,
                0x00
        };
        uint8_t *short_ = simpli2c::shortBuffer(microSeconds);
        packet[2] = short_[0];
        packet[3] = short_[1];
#ifdef DEBUG_PRINT_MICROSECONDS
        ROS_INFO_STREAM("writing " << microSeconds << "");
#endif
        device.writeMany(packet, 4);
        delete short_;
    }

    ARDevice::~ARDevice() {
        device.close_();
    }

    encoder_id_t ARDevice::openPinAsEncoderId(uint8_t pin1, uint8_t pin2) {
        uint8_t packet[] = {
                0x04,
                pin1,
                pin2
        };
        return this->device.requestOne(packet, 3);
    }

    Encoder ARDevice::openPinAsEncoder(uint8_t pin1, uint8_t pin2) {
        encoder_id_t enc = openPinAsEncoderId(pin1, pin2);
        Encoder e = Encoder(this, enc);
        return e;
    }



    void ARDevice::resetEncoder(encoder_id_t encoder) {
        this->device.writeMany(new uint8_t[2] {0x05, encoder}, 2);
    }

    int32_t ARDevice::readEncoder(encoder_id_t encoder) {
        uint8_t resp[] = {0, 0, 0, 0};
        this->device.requestMany(2, new uint8_t[2] {0x06, encoder}, 4, resp);
        return static_cast<int32_t>(simpli2c::bufferLong(resp)); // simpli2c only gives unsigned values, convert to signed
    }
}