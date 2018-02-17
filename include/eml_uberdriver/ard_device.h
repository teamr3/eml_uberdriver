//
// Created by matthew on 24/08/17.
//

#ifndef PROJECT_ARD_DEVICE_H
#define PROJECT_ARD_DEVICE_H

#include <simpli2c/simpli2c.h>
#include <map>
#include "stepper.h"

namespace eml_uberdriver {

    typedef uint8_t encoder_id_t;
    class Encoder; // forward decl

    class ARDevice {
    public:
        ARDevice(uint8_t busnum, uint8_t address);
        ~ARDevice();

        void openPinAsMotor(uint8_t pin);
        void writeMicroseconds(uint8_t pin, uint16_t microSeconds);

        encoder_id_t openPinAsEncoderId(uint8_t pin1, uint8_t pin2);
        Encoder openPinAsEncoder(uint8_t pin1, uint8_t pin2);

        void resetEncoder(encoder_id_t encoder);
        int32_t readEncoder(encoder_id_t encoder);

        void openPinAsLimitSwitch(uint8_t pin);
        bool readLimitSwitch(uint8_t pin);

        Stepper openPinAsStepper(uint8_t pinEnable, uint8_t pinDir, uint8_t pinStep);
        stepper_id_t openPinAsUnipolarStepper(uint8_t pinEnable, uint8_t pinDir, uint8_t pinStep);

        void stepStepper(stepper_id_t step, int16_t steps);

    private:
        simpli2c::Device device;
        std::map<uint8_t, uint8_t> pinMap;
        std::map<uint8_t, uint16_t> lastValues;
    };

}


#endif //PROJECT_ARD_DEVICE_H
