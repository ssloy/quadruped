//
// Created by Nikita Vasilenko on 05/05/2017.
// Implemented only with *.h file. Just saving important measurements, too keep everything in one place
// (never to change during program execution)
//

#ifndef QUADRUPED_MEASUREMENTS_H
#define QUADRUPED_MEASUREMENTS_H


#include <cmath>

class Measurements {
public:
    int speed = 400;
    int split = 3;
    int length_centr_to_x0 = 90;
    int gipLength_0 = 50;
    int up_limit_pos_0 = 785;
    int down_limit_pos_0 = 262;

    int length_1 = 65;
    int downSlope_1 = 16;
    int up_limit_pos_1 = 861;
    int down_limit_pos_1 = 157;
    double gipLength_1 = std::sqrt(length_1 * length_1 + downSlope_1 * downSlope_1);
    double angleSlope_1 = 14.04; //hardcoded, need to use formula

    int length_2 = 84;
    int downSlope_2 = 70;
    int up_limit_pos_2 = 644;
    int down_limit_pos_2 = 37;
    double gipLength_2 = std::sqrt(length_2 * length_2 + downSlope_2 * downSlope_2);
    double angleSlope_2 = 39.81; //hardcoded, need to use formula
    double bigLegLength = length_centr_to_x0 + gipLength_0 + gipLength_1 + gipLength_2;

//  For magnetometer (info from calibrating)
    float off_x = 0.33;
    float off_y = -0.42;
};


#endif //QUADRUPED_MEASUREMENTS_H
