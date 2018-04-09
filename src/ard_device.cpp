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
        this->tryOpen();
    }

    bool ARDevice::tryOpen() {
        if (this->isOpen) return true;
        return this->doOpen();
    }

    void ARDevice::openPinAsMotor(uint8_t pin) {
        if (!this->isDisconnected()) {
            try {
                uint8_t servoName = device.requestOne(new uint8_t[2]{0x01, pin}, 2);
                this->pinMap[pin] = servoName;
                this->lastValues[pin] = 0;
            }
            catch (std::runtime_error &e) {
                this->isOpen = false;
                this->device.close_();
            }
        }
    }

    void ARDevice::writeMicroseconds(uint8_t pin, uint16_t microSeconds) {
        if (!this->isDisconnected()) {
            if (lastValues[pin] == microSeconds) return;
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
            try {
                device.writeMany(packet, 4);
                lastValues[pin] = microSeconds;
            }
            catch (std::runtime_error &e) {
                this->isOpen = false;
                this->device.close_();
            }
            delete short_;
        }
    }

    ARDevice::~ARDevice() {
        if (!this->isDisconnected()) {
            device.close_();
        }
    }

    encoder_id_t ARDevice::openPinAsEncoderId(uint8_t pin1, uint8_t pin2) {
        if (!this->isDisconnected()) {
            uint8_t packet[] = {
                    0x04,
                    pin1,
                    pin2
            };
            try {
                return this->device.requestOne(packet, 3);
            }
            catch (std::runtime_error &e) {
                this->isOpen = false;
                this->device.close_();
            }
        }
    }

    Encoder ARDevice::openPinAsEncoder(uint8_t pin1, uint8_t pin2) {
        if (!this->isDisconnected()) {
            encoder_id_t enc = openPinAsEncoderId(pin1, pin2);
            Encoder e = Encoder(this, enc);
            return e;
        }
    }



    void ARDevice::resetEncoder(encoder_id_t encoder) {
        if (!this->isDisconnected()) {
            try {
                this->device.writeMany(new uint8_t[2]{0x05, encoder}, 2);
            }
            catch (std::runtime_error &e) {
                this->isOpen = false;
                this->device.close_();
            }
        }
    }

    int32_t ARDevice::readEncoder(encoder_id_t encoder) {
        if (!this->isDisconnected()) {
            uint8_t resp[] = {0, 0, 0, 0};
            try {
                this->device.requestMany(2, new uint8_t[2]{0x06, encoder}, 4, resp);
                return static_cast<int32_t>(simpli2c::bufferLong(
                        resp)); // simpli2c only gives unsigned values, convert to signed
            }
            catch (std::runtime_error &e) {
                this->isOpen = false;
                this->device.close_();
            }
        }
    }

    bool ARDevice::isDisconnected() {
        return !this->isOpen;
    }

    bool ARDevice::doOpen() {
        try {
            this->device.open_();
            this->device.writeOne(0x03); // reset, then reopen as this causes the arduino to reset

            // todo: might break as writing 0x03 _will_ physically reset the entire arduino.
                // fixme: spooky scary skeletons mean that if I write _something_ to the arduino (like 255, nop), it works?

            try {
                this->device.writeOne(0xFF);
            }
            catch (std::runtime_error &e) {
                // idc
            }

            this->lastValues.clear();
            this->pinMap.clear();
            this->isOpen = true;
        }
        catch (std::runtime_error &e) {
            this->isOpen = false;
            if (this->device.isOpen()) {
                this->device.close_();
            }
        }
    }
}