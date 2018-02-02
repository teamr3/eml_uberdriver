//
// Created by matthew on 2/1/18.
//

#ifndef EML_UBERDRIVER_ENCODER_H
#define EML_UBERDRIVER_ENCODER_H

#include <cstdint>
#include "ard_device.h"

namespace eml_uberdriver {
    class Encoder {
    friend class ARDevice;
    public:
        Encoder();

        void resetEncoder();
        int32_t encoderValue();
    private:
        Encoder(ARDevice *arDevice, encoder_id_t encoderId);

        ARDevice * parent;
        encoder_id_t encoder;
    };

}


#endif //EML_UBERDRIVER_ENCODER_H
