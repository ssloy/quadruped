//
// Created by Nikita Vasilenko on 05/05/2017.
//

#include <iostream>
#include <zconf.h>
#include "Positions.h"
#include "Measurements.h"
#include "TrotGait.h"
#include "BodyPosition.h"

Positions::Positions() {
    dxl = new Dynamixel(16);
}

BodyPosition *body = new BodyPosition();
Measurements *mes = new Measurements();
TrotGait *trt = new TrotGait();

// Sets a given position to a given actuator, with default speed
void Positions::setPosition(unsigned char leg, int pos) {

//    int temp = mes->speed;
//    int *speed = &temp;
    int *myPos = &pos;
    unsigned char *servos = &leg;

//    dxl->syncwrite_one_word(0x20, servos, speed, 1);
    dxl->syncwrite_one_word(0x1E, servos, myPos, 1);
}

// As long as we do not have feedback, we need to remember angles for each actuator (as field of class body)
bool Positions::set_angles_inBody(unsigned char *servos, int *angles, int amount) {
    for (int j = 0; j < amount; ++j) {
        if (servos[j] == 0) { body->serv0 = angles[j]; }
        else if (servos[j] == 1) { body->serv1 = angles[j]; }
        else if (servos[j] == 2) { body->serv2 = angles[j]; }
        else if (servos[j] == 10) { body->serv10 = angles[j]; }
        else if (servos[j] == 11) { body->serv11 = angles[j]; }
        else if (servos[j] == 12) { body->serv12 = angles[j]; }
        else if (servos[j] == 20) { body->serv20 = angles[j]; }
        else if (servos[j] == 21) { body->serv21 = angles[j]; }
        else if (servos[j] == 22) { body->serv22 = angles[j]; }
        else if (servos[j] == 30) { body->serv30 = angles[j]; }
        else if (servos[j] == 31) { body->serv31 = angles[j]; }
        else if (servos[j] == 32) { body->serv32 = angles[j]; }
    }
    return true;
}


// creates a connection with a port
int Positions::openPort() {
    if (!dxl->open_serial("/dev/ttyS0")) {
        std::cerr << "Can not open serial device" << std::endl;
        return -1;
    }
    return 0;
}

// to close a connection with a serial port
void Positions::closePort() {
    dxl->close_serial();
}

// hardcoded initialPosition(low) NO USAGE

void Positions::upLegs(int *legs, int amount) {
    unsigned char servos[amount];
    int angles[amount];
    for (int i = 0; i < amount; ++i) {
        if (legs[i] == 0) servos[i] = 1; else
        if (legs[i] == 1) servos[i] = 11; else
        if (legs[i] == 2) servos[i] = 21; else
        if (legs[i] == 3) servos[i] = 31;
        angles[i] = 70;
    }

    set_angles_syncArray(servos, angles, amount);
}

// Default is the speed, saved in measurements file.
void Positions::setDefaultSpeeds() {
    int speeds[12] = {mes->speed, mes->speed, mes->speed, mes->speed, mes->speed, mes->speed, mes->speed, mes->speed,
                      mes->speed, mes->speed, mes->speed, mes->speed};
    unsigned char servos[] = {0, 1, 2, 10, 11, 12, 20, 21, 22, 30, 31, 32};
    dxl->syncwrite_one_word(0x20, servos, speeds, 12);
    std::cerr << "ALARM! DEFAULT SPEED TO ALL: " << speeds[0] << std::endl;

}

// Set given speed to ALL actuators
void Positions::setSpeedToAll(int speed) {

    int speeds[12] = {speed, speed, speed, speed, speed, speed, speed, speed, speed, speed, speed, speed};
    unsigned char servos[] = {0, 1, 2, 10, 11, 12, 20, 21, 22, 30, 31, 32};
    dxl->syncwrite_one_word(0x20, servos, speeds, 12);
    mes->speed = speed;

    std::cerr << "SPEED SET (NOT DEFAULT): " << speeds[0] << std::endl;

}



// This method takes an angle and a leg and checks the possibility to set up this angle. The constraints are taken f
// rom the file Measurements. It returns -1 if it is impossible to set or the position (>0) which should be sent
int Positions::checkAngle(unsigned char leg, int deg) {
    double angleSlope = 0.0;
    int up_limit_pos = 1023;
    int down_limit_pos = 0;
    if (leg == 0 or leg == 10 or leg == 20 or leg == 30) {
        up_limit_pos = mes->up_limit_pos_0;
        down_limit_pos = mes->down_limit_pos_0;
    } else if (leg == 1 or leg == 11 or leg == 21 or leg == 31) {
        angleSlope = mes->angleSlope_1;
        up_limit_pos = mes->up_limit_pos_1;
        down_limit_pos = mes->down_limit_pos_1;
    } else if (leg == 2 or leg == 12 or leg == 22 or leg == 32) {
        angleSlope = mes->angleSlope_2 - mes->angleSlope_1;
        up_limit_pos = mes->up_limit_pos_2;
        down_limit_pos = mes->down_limit_pos_2;
    }
    double pos = ((deg + angleSlope + 150) * 1024 / 300.0) - 1;
    if ((int) pos < up_limit_pos and (int) pos > down_limit_pos) {
        return (int) pos;
    } else return -1;
}

// A method to set an angle to a concrete actuator. Angles in degrees.
bool Positions::setAngle(unsigned char leg, int deg) {
    int pos = checkAngle(leg, deg);
//    std::cerr << "POSITION = " << pos << std::endl;

    if (pos < 0) {
        std::cerr << "setAngle error!" << std::endl;
        return false;
    } else {
        setPosition(leg, pos);
        unsigned char tmpServos[1] = {leg};
        int tmpAngles[1] = {deg};
        set_angles_inBody(tmpServos, tmpAngles, 1);
        return true;
    }
}

// A method to set angles to several actuators at the same time (using one bih sync_write packet)
bool Positions::set_angles_syncArray(unsigned char *servos, int *angles, int amount) {
    int posits[amount];

    for (int i = 0; i < amount; ++i) {
        if (checkAngle(servos[i], angles[i]) < 0) {
            std::cerr << "One of the angles failed!" << std::endl;
            return false;
        } else {
            posits[i] = checkAngle(servos[i], angles[i]);
        }
    }
//    setDefaultSpeeds();
    dxl->syncwrite_one_word(0x1E, servos, posits, (unsigned char) amount);
    set_angles_inBody(servos, angles, amount);
    return true;
}

// Method to compute how many time does the actuator need to move for a given angle. Used in computing delays.
double Positions::timeForAngle(int s_angle) {
    double w_rpm = (mes->speed * 114.0) / 1023.0;
    double w_deg_sec = w_rpm * 6;
    double time = s_angle / w_deg_sec;
    return time;
}

// Method to move (like step) leg or legs o given angle. Used only in rotation methods
bool Positions::round_legs_step(int *legs, int amount, int angle, int upDegree) {
//    As long as we don't have feedback from servos, the start_degree angle is hardcoded (measured for certain posit)
    int start_deg0 = 0;
    int start_deg1 = 36;
    int half_angle = (int) (angle * 0.5);


    unsigned char servos[amount * 2];
    int angles[amount * 2];
    int z = 0;
    for (int k = 0; k < amount; ++k) {
        if (legs[k] == 0) {
            servos[z] = 0;
            servos[z + 1] = 1;
        } else if (legs[k] == 1) {
            servos[z] = 10;
            servos[z + 1] = 11;
        } else if (legs[k] == 2) {
            servos[z] = 20;
            servos[z + 1] = 21;
        } else if (legs[k] == 3) {
            servos[z] = 30;
            servos[z + 1] = 31;
        }

        angles[z] = half_angle + start_deg0;
        angles[z + 1] = start_deg1 + upDegree;
        z = z + 2;
    }

    set_angles_syncArray(servos, angles, amount * 2);

    double delay = timeForAngle(half_angle) * 1000000;
    std::cerr << "Delay!" << delay << std::endl;
    usleep((useconds_t) delay);

    z = 0;
    for (int k = 0; k < amount; ++k) {
        angles[z] = angle + start_deg0;
        angles[z + 1] = start_deg1;
        z = z + 2;
    }

    set_angles_syncArray(servos, angles, amount * 2);


    return true;
}

// Print the current table of angles of ALL actuators
void Positions::printServos() {
    std::cerr << "Servo 0: " << body->serv0 << std::endl;
    std::cerr << "Servo 1: " << body->serv1 << std::endl;
    std::cerr << "Servo 2: " << body->serv2 << std::endl;
    std::cerr << "Servo 10: " << body->serv10 << std::endl;
    std::cerr << "Servo 11: " << body->serv11 << std::endl;
    std::cerr << "Servo 12: " << body->serv12 << std::endl;
    std::cerr << "Servo 20: " << body->serv20 << std::endl;
    std::cerr << "Servo 21: " << body->serv21 << std::endl;
    std::cerr << "Servo 22: " << body->serv22 << std::endl;
    std::cerr << "Servo 30: " << body->serv30 << std::endl;
    std::cerr << "Servo 31: " << body->serv31 << std::endl;
    std::cerr << "Servo 32: " << body->serv32 << std::endl;
}

// Print the coordinates of a given leg (using artificial feedback)
void Positions::printCurrentLeg(int leg) {
    std::vector<int> result = body->computeCurrentPoint(leg);
    std::cerr << "Leg# " << leg << ": " << result[0] << ", " << result[1] << ", " << result[2] << std::endl;
}

// Returning a vector (array [x, y, z]), with current 3D coordinates of a given leg
std::vector<int> Positions::returnCurrentLegCoor(int leg) {
    std::vector<int> result = body->computeCurrentPoint(leg);
    return result;
}









