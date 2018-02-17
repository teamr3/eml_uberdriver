//
// Created by matthew on 2/17/18.
//

#include "../include/eml_uberdriver/stepper.h"
#include "../include/eml_uberdriver/ard_device.h"

eml_uberdriver::Stepper::Stepper(ARDevice *arDevice, stepper_id_t stepper_id) : arDevice(arDevice), stepper_id(stepper_id) {

}

void eml_uberdriver::Stepper::step(int16_t steps) {
    this->arDevice->stepStepper(this->stepper_id, steps);
}