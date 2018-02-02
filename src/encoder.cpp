//
// Created by matthew on 2/1/18.
//

#include "../include/eml_uberdriver/encoder.h"

namespace eml_uberdriver {
    Encoder::Encoder(ARDevice *arDevice, encoder_id_t encoderId) :
            parent(arDevice),
            encoder(encoderId) {

    }

    void Encoder::resetEncoder() {
        if (this->parent == nullptr) throw std::runtime_error("called resetEncoder on a non-init");
        this->parent->resetEncoder(this->encoder);
    }

    Encoder::Encoder() {
        this->parent = nullptr;
        this->encoder = 0;
    }

    int32_t Encoder::encoderValue() {
        if (this->parent == nullptr) throw std::runtime_error("called encoderValue on a non-init");
        return this->parent->readEncoder(this->encoder);
    }
}