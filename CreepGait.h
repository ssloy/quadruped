//
// Created by Nikita Vasilenko on 11/05/2017.
//
// Class for implementing the behavior of creep walking (walking with saving of stability all the time)

#ifndef QUADRUPED_CREEPGAIT_H
#define QUADRUPED_CREEPGAIT_H


class CreepGait {

public:

    bool creepWalkUp (int step_length, int step_high, int turn_r_l, int step_amount);

    bool deerWalkUp (int step_length, int step_high, int turn_r_l, int step_amount);

};


#endif //QUADRUPED_CREEPGAIT_H
