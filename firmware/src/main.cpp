//
// Created by matthew on 24/08/17.
//

#include <Servo.h>
#include <Arduino.h>
#include <Wire.h>
#include <Encoder.h>
//#include "constants.h"
#include <A4988.h>

Servo servos[10];
Encoder *encoders[10];
A4988 *steppers[10];

uint8_t servoCount = 0;
uint8_t lastName = 0;
uint8_t encoderCount = 0;
int32_t encoderResult = 0;
uint8_t stepperCount = 0;

int address;

bool isLastNameActive = false;
bool isEncoderResultActive = false;
bool isLimitResultActive = false;

uint8_t addServo(uint8_t pin) {
    servos[servoCount].attach((int)pin);
    servoCount += 1;
    return servoCount - 1;
}

uint8_t addEncoder(uint8_t pin1, uint8_t pin2) {
    encoders[encoderCount] = new Encoder(pin1, pin2);
    encoderCount += 1;
    return encoderCount-1;
}

uint8_t addStepper(uint8_t dir, uint8_t step, uint8_t en) {
    steppers[stepperCount] = new A4988(200, dir, step, en);
    stepperCount += 1;
    return stepperCount-1;
}

void setServo(uint8_t name, uint16_t micros) {
    if (name < servoCount) {
        servos[name].writeMicroseconds(micros);
    }
}

void onRecieve(int bytes) {
    int func = Wire.read();
    if (bytes == 2 && func == 0x01) {
        lastName = addServo(static_cast<uint8_t>(Wire.read()));
        isLastNameActive = true;
    }
    else if (bytes == 4 && func == 0x02) {
        uint8_t servo = static_cast<uint8_t>(Wire.read());
        uint16_t value;
        Wire.readBytes((char *)&value, 2);
        setServo(servo, value);
    }
    else if (bytes == 1 && func == 0x03) {
    //    REQUEST_EXTERNAL_RESET;
    }
    else if (bytes == 3 && func == 0x04) {
        uint8_t left = static_cast<uint8_t>(Wire.read());
        uint8_t right =static_cast<uint8_t>(Wire.read());

        lastName = addEncoder(left, right);
        isLastNameActive = true;
    }
    else if (bytes == 2 && func == 0x05) {
        uint8_t encoder = static_cast<uint8_t>(Wire.read());
        encoders[encoder]->write(0);
    }
    else if (bytes == 2 && func == 0x06) {
        uint8_t encoder = static_cast<uint8_t>(Wire.read());
        encoderResult = encoders[encoder]->read();
        isEncoderResultActive = true;
    }
    else if (bytes == 4 && func == 0x09) {
        uint8_t step =  static_cast<uint8_t>(Wire.read());
        uint8_t enable= static_cast<uint8_t>(Wire.read());
        uint8_t dir    =static_cast<uint8_t>(Wire.read());

        lastName = addStepper(dir, step, enable);
        isLastNameActive = true;
    }
    else if (bytes == 4 && func == 0x0A) {
        uint8_t stepper = static_cast<uint8_t>(Wire.read());
        int16_t steps;
        Wire.readBytes(reinterpret_cast<char *>(&steps), 2);
        steppers[stepper]->stop();
        steppers[stepper]->startMove(steps);
    }
    else {
        bytes--;
        while (bytes > 0) {
            Wire.read(); // scoop it up
            bytes--;
        }
    }
}

void onRequest() {
    if (isLastNameActive) {
        Wire.write(lastName);
        isLastNameActive = false;
    }
    else if (isEncoderResultActive) {
        Wire.write(reinterpret_cast<uint8_t *>(&encoderResult), sizeof(int32_t));
        isEncoderResultActive = false;
    }
    else {
        // aaaaaah we mucked it up
        Wire.write(0x00);
    }
}

void setAddress() {/*
    pinMode(ADDRESS_PIN_1, INPUT);
    pinMode(ADDRESS_PIN_2, INPUT);
    pinMode(ADDRESS_PIN_3, INPUT);
    pinMode(ADDRESS_PIN_4, INPUT);

    address = ADDRESS_BASE;
    address += digitalRead(ADDRESS_PIN_1);
    address += digitalRead(ADDRESS_PIN_2) * 2;
    address += digitalRead(ADDRESS_PIN_3) * 4;
    address += digitalRead(ADDRESS_PIN_4) * 8;*/
}

void setup() {
    setAddress();
    Wire.begin(0x30);
    Wire.onRequest(onRequest);
    Wire.onReceive(onRecieve);
}

void loop() {
    // i am loop
}
