//
// Created by Nikita Vasilenko on 10/05/2017.
// Describes the trot gait algorithm, turning algorithm and moving the body
//

#ifndef QUADRUPED_TROTGAIT_H
#define QUADRUPED_TROTGAIT_H

#include "Measurements.h"
#include "Positions.h"
#include "Coordinates.h"


class TrotGait {
public:

    bool trotUpDownCourseCorrection(int step_length, int step_high, int step_amount, char u_or_d, int turn_shift_percent,
                                    bool need_correction);

    bool trotJumpingUpDown(int step_length, int step_high, int step_amount, char u_or_d, int go_down_percent);

    bool trotInitialPosition();

    bool selfTurning(int turns_amount, int step_degree);

    bool self_calibrate(int amount_calls);

    bool self_balance(int each_step);

    bool bodyMove(int x_shift, int y_shift, int z_shift, int speed);

    bool bodyMoveFromInitial(int x_shift, int y_shift, int z_shift, int speed);


};


#endif //QUADRUPED_TROTGAIT_H
