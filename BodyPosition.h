//
// Created by Nikita Vasilenko on 15/05/2017.
// This class takes on the role of feedback. We believe that actuators work properly and just save the positions when
// setting them.
//

#ifndef QUADRUPED_BODYPOSITION_H
#define QUADRUPED_BODYPOSITION_H


#include <iostream>
#include <vector>

class BodyPosition {
public:
    int serv0; int serv1; int serv2;
    int serv10; int serv11; int serv12;
    int serv20; int serv21; int serv22;
    int serv30; int serv31; int serv32;

    std::vector<int> computeCurrentPoint(int leg);
};


#endif //QUADRUPED_BODYPOSITION_H
