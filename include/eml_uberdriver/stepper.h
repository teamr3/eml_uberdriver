//
// Created by matthew on 2/17/18.
//

#ifndef PROJECT_STEPPER_H
#define PROJECT_STEPPER_H

#include <cstdint>

namespace eml_uberdriver {

    typedef uint8_t stepper_id_t;

    class Stepper {
    friend class ARDevice;
    private:
        Stepper(ARDevice *arDevice, stepper_id_t stepper_id);

        stepper_id_t stepper_id;
        ARDevice *arDevice;
    public:
        Stepper() = default;;
        void step(int16_t steps);
    };

}


#endif //PROJECT_STEPPER_H
