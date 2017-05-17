//
// Created by Nikita Vasilenko on 05/05/2017.
// Everything, what is concerned communication with actuators is located in this class.
// It uses the notion of position (from 0 to 1023) and angle (-180 to 180 degrees)
//

#ifndef QUADRUPED_POSITIONS_H
#define QUADRUPED_POSITIONS_H


#include <vector>
#include "dynamixel.h"

class Positions {
public:
    void setPosition(unsigned char leg, int pos);

    void initialPosition();

    void setDefaultSpeeds();

    void setSpeed(int speed);

    int checkAngle(unsigned char leg, int deg);

    bool setAngle(unsigned char leg, int deg);

    bool set_angles_syncArray(unsigned char *servos, int *angles, int amount);

    bool set_angles_inBody (unsigned char *servos, int *angles, int amount);

    bool round_legs_step(int *legs, int amount, int angle, int upDegree);

    double timeForAngle(int s_angle);

    int openPort();

    void closePort();

    Positions();

    void printServos();

    void printCurrentLeg(int leg);

    std::vector<int> returnCurrentLegCoor(int leg);


private:
    Dynamixel *dxl;
};


#endif //QUADRUPED_POSITIONS_H
