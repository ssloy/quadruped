//
// Created by Nikita Vasilenko on 10/05/2017.
//

#include <iostream>
#include <unistd.h> // usleep
#include "TrotGait.h"
#include "gy-85.h"

Measurements *mesObjT = new Measurements();
Coordinates *coorObjT = new Coordinates();
Positions *posObjT = new Positions();

GY85 magnetometer("/dev/i2c-1");

int split = mesObjT->split;

// Starting position is defined by user here.
int start_pos0[3] = {-240, 0, -60};
int start_pos1[3] = {0, -240, -60};
int start_pos2[3] = {240, 0, -60};
int start_pos3[3] = {0, 240, -60};

// Set the quadruped to initial position
bool TrotGait::trotInitialPosition() {

    int testX[] = {start_pos0[0], start_pos1[0], start_pos2[0], start_pos3[0]};
    int testY[] = {start_pos0[1], start_pos1[1], start_pos2[1], start_pos3[1]};
    int testZ[] = {start_pos0[2], start_pos1[2], start_pos2[2], start_pos3[2]};
    int testLeg[] = {0, 1, 2, 3};

    coorObjT->set_coordinates3D_sync(testX, testY, testZ, testLeg, 4);

    return true;
}

// Main walking algorithm. Boolean flag need_correction defines the necessity of course stability.
// step_length, step_high - in mm
// step_amount - how many steps to do?
// u_or_d - up or down , defines the direction of walk
// turn_shift - if not 0 - walking not straight, but with shifting to the right or left.
// need_correction defines the necessity of course stability

bool TrotGait::trotUpDownCourseCorrection(int step_length, int step_high, int step_amount, char u_or_d,
                                          int turn_shift, bool need_correction) {

    std::vector<int> current_leg0 = posObjT->returnCurrentLegCoor(0);
    std::vector<int> current_leg1 = posObjT->returnCurrentLegCoor(1);
    std::vector<int> current_leg2 = posObjT->returnCurrentLegCoor(2);
    std::vector<int> current_leg3 = posObjT->returnCurrentLegCoor(3);

    int start_pos0[3] = {current_leg0[0], current_leg0[1], current_leg0[2]};
    int start_pos1[3] = {current_leg1[0], current_leg1[1], current_leg1[2]};
    int start_pos2[3] = {current_leg2[0], current_leg2[1], current_leg2[2]};
    int start_pos3[3] = {current_leg3[0], current_leg3[1], current_leg3[2]};

    int shift_intencity = 50;
    int turn_step_degree = 15;
    int wrong_way_k = 10;
    int turn_away_k = 60;
    double calibrate_turn_k = 1.53;
    int direct_k;
//    Walking UP
    if (u_or_d == 'u') {
        direct_k = 1;
    } else if (u_or_d == 'd') {
//   Waling down
        direct_k = -1;
    } else
//    Error in direction
    {
        std::cerr << "Wrong direction!" << std::endl;
        return false;
    }
    magnetometer.set_magnetometer_offset(mesObjT->off_x, mesObjT->off_y, -.37);
    float start_heading;
    magnetometer.get_heading(start_heading);
    //     Change this! argument split is useless now
    int split = mesObjT->split;
    std::cerr << "Trot Walk Up Inside" << std::endl;
    int shift = int(float(step_length) / std::sqrt(2));
    int shift_correction = int(float(turn_shift * shift) / 100.0);
    int shift_right = shift + shift_correction;
    int shift_left = shift - shift_correction;
    std::cerr << "Start Head: " << start_heading << std::endl;

//    First iteration, preparation for making one or several steps
    LegPoint line_pts0 = coorObjT->line_points_leg(start_pos0[0], start_pos0[1], start_pos0[2],
                                                   start_pos0[0] + direct_k * shift,
                                                   start_pos0[1] - direct_k * shift,
                                                   start_pos0[2], split, 0);

    LegPoint pts1 = coorObjT->ellipse_points_leg(start_pos1[0], start_pos1[1], start_pos1[2],
                                                 start_pos1[0] - direct_k * shift,
                                                 start_pos1[1] + direct_k * shift,
                                                 start_pos1[2], step_high, split, 1);

    LegPoint line_pts2 = coorObjT->line_points_leg(start_pos2[0], start_pos2[1], start_pos2[2],
                                                   start_pos2[0] + direct_k * shift,
                                                   start_pos2[1] - direct_k * shift,
                                                   start_pos2[2], split, 2);

    LegPoint pts3 = coorObjT->ellipse_points_leg(start_pos3[0], start_pos3[1], start_pos3[2],
                                                 start_pos3[0] - direct_k * shift,
                                                 start_pos3[1] + direct_k * shift,
                                                 start_pos3[2], step_high, split, 3);

    LegPoint pts_iteration_1[] = {line_pts0, pts1, line_pts2, pts3};
    coorObjT->followPoints(pts_iteration_1, 4);

//    Making one or several steps, depending on given parameter.
    for (int i = 0; i < step_amount; ++i) {
        if (turn_shift == 0 and need_correction) {
            float current_head;
            magnetometer.set_magnetometer_offset(mesObjT->off_x, mesObjT->off_y, -.37);
            magnetometer.get_heading(current_head);


            std::cerr << "Current: " << current_head << std::endl;
            if (((current_head - start_heading) > wrong_way_k) and
                (current_head > start_heading) and
                (current_head <= start_heading + 180)) {

                std::cerr << "Course shifted ---->: " << std::endl;
                shift_correction = int(float(shift_intencity * shift) / 100.0);


                if (std::abs(current_head - start_heading) > turn_away_k) {
                    int turnN = int((current_head - start_heading) / (turn_step_degree * calibrate_turn_k));
                    std::cerr << "<<--- Rotate #: " << turnN << std::endl;
//                    Correct method of rotating (more exact degree)
                    selfTurning(turnN, turn_step_degree);
                    continue;
                }


            } else if (((current_head < start_heading) and ((start_heading - current_head) > wrong_way_k)) or
                       ((current_head > start_heading + 180)) and
                       ((360 - current_head + start_heading) > wrong_way_k)) {

                std::cerr << "<---- Course shifted " << std::endl;
                shift_correction = -1 * int(float(shift_intencity * shift) / 100.0);

                if ((start_heading - current_head) > turn_away_k and (current_head < start_heading)) {
                    int turnN = int((start_heading - current_head) / (turn_step_degree * calibrate_turn_k));
                    std::cerr << "Rotate ----> #: " << turnN << std::endl;
//                    Correct method of rotating (more exact degree)
                    selfTurning(turnN, -1 * turn_step_degree);
                    continue;
                } else if ((360 - current_head + start_heading) > turn_away_k and
                           (current_head > start_heading + 180)) {

                    int turnN = int((360 - current_head + start_heading) / (turn_step_degree * calibrate_turn_k));
                    std::cerr << "Rotate ----> #: " << turnN << std::endl;
//                    Correct method of rotating (more exact degree)
                    selfTurning(turnN, -1 * turn_step_degree);
                    continue;
                }

            } else {
                std::cerr << "|Course alright|: " << std::endl;
                shift_correction = 0;
            }
            shift_right = shift + shift_correction;
            shift_left = shift - shift_correction;
        }
        //   0 and 2 moving forward
        LegPoint pts0 = coorObjT->ellipse_points_leg(start_pos0[0] + direct_k * shift_left,
                                                     start_pos0[1] - direct_k * shift_left,
                                                     start_pos0[2],
                                                     start_pos0[0] - direct_k * shift_left,
                                                     start_pos0[1] + direct_k * shift_left,
                                                     start_pos0[2],
                                                     step_high, split, 0);

        LegPoint pts2 = coorObjT->ellipse_points_leg(start_pos2[0] + direct_k * shift_right,
                                                     start_pos2[1] - direct_k * shift_right,
                                                     start_pos2[2],
                                                     start_pos2[0] - direct_k * shift_right,
                                                     start_pos2[1] + direct_k * shift_right,
                                                     start_pos2[2],
                                                     step_high, split, 2);

        LegPoint line_pts1 = coorObjT->line_points_leg(start_pos1[0] - direct_k * shift_left,
                                                       start_pos1[1] + direct_k * shift_left,
                                                       start_pos1[2],
                                                       start_pos1[0] + direct_k * shift_left,
                                                       start_pos1[1] - direct_k * shift_left,
                                                       start_pos1[2],
                                                       split, 1);

        LegPoint line_pts3 = coorObjT->line_points_leg(start_pos3[0] - direct_k * shift_right,
                                                       start_pos3[1] + direct_k * shift_right,
                                                       start_pos3[2],
                                                       start_pos3[0] + direct_k * shift_right,
                                                       start_pos3[1] - direct_k * shift_right,
                                                       start_pos3[2],
                                                       split, 3);

        LegPoint pts_iteration_2[] = {pts0, line_pts1, pts2, line_pts3};
        coorObjT->followPoints(pts_iteration_2, 4);

        //   1 and 3 leg moving forward

        line_pts0 = coorObjT->line_points_leg(start_pos0[0] - direct_k * shift_left,
                                              start_pos0[1] + direct_k * shift_left,
                                              start_pos0[2],
                                              start_pos0[0] + direct_k * shift_left,
                                              start_pos0[1] - direct_k * shift_left,
                                              start_pos0[2],
                                              split, 0);

        line_pts2 = coorObjT->line_points_leg(start_pos2[0] - direct_k * shift_right,
                                              start_pos2[1] + direct_k * shift_right,
                                              start_pos2[2],
                                              start_pos2[0] + direct_k * shift_right,
                                              start_pos2[1] - direct_k * shift_right,
                                              start_pos2[2],
                                              split, 2);

        pts1 = coorObjT->ellipse_points_leg(start_pos1[0] + direct_k * shift_left,
                                            start_pos1[1] - direct_k * shift_left,
                                            start_pos1[2],
                                            start_pos1[0] - direct_k * shift_left,
                                            start_pos1[1] + direct_k * shift_left,
                                            start_pos1[2],
                                            step_high, split, 1);

        pts3 = coorObjT->ellipse_points_leg(start_pos3[0] + direct_k * shift_right,
                                            start_pos3[1] - direct_k * shift_right,
                                            start_pos3[2],
                                            start_pos3[0] - direct_k * shift_right,
                                            start_pos3[1] + direct_k * shift_right,
                                            start_pos3[2],
                                            step_high, split, 3);

        LegPoint pts_iteration_3[] = {pts1, line_pts0, pts3, line_pts2};
        coorObjT->followPoints(pts_iteration_3, 4);
    }


    //    Last iteration, returning to standart position

    LegPoint pts0 = coorObjT->ellipse_points_leg(start_pos0[0] + direct_k * shift_left,
                                                 start_pos0[1] - direct_k * shift_left,
                                                 start_pos0[2],
                                                 start_pos0[0],
                                                 start_pos0[1],
                                                 start_pos0[2],
                                                 step_high, split, 0);

    LegPoint pts2 = coorObjT->ellipse_points_leg(start_pos2[0] + direct_k * shift_right,
                                                 start_pos2[1] - direct_k * shift_right,
                                                 start_pos2[2],
                                                 start_pos2[0],
                                                 start_pos2[1],
                                                 start_pos2[2],
                                                 step_high, split, 2);

    LegPoint line_pts1 = coorObjT->line_points_leg(start_pos1[0] - direct_k * shift_left,
                                                   start_pos1[1] + direct_k * shift_left,
                                                   start_pos1[2],
                                                   start_pos1[0],
                                                   start_pos1[1],
                                                   start_pos1[2],
                                                   split, 1);

    LegPoint line_pts3 = coorObjT->line_points_leg(start_pos3[0] - direct_k * shift_right,
                                                   start_pos3[1] + direct_k * shift_right,
                                                   start_pos3[2],
                                                   start_pos3[0],
                                                   start_pos3[1],
                                                   start_pos3[2],
                                                   split, 3);

    LegPoint pts_iteration_4[] = {pts0, line_pts1, pts2, line_pts3};
    coorObjT->followPoints(pts_iteration_4, 4);
    return true;
}

// Experiment, walking with moving COG up and down
bool TrotGait::trotJumpingUpDown(int step_length, int step_high, int step_amount, char u_or_d) {
    int step_down = int(0.5 * step_high);
    int direct_k;
//    Walking UP
    if (u_or_d == 'u') {
        direct_k = 1;
    } else if (u_or_d == 'd') {
//   Waling down
        direct_k = -1;
    } else
//    Error in direction
    {
        std::cerr << "Wrong direction!" << std::endl;
        return false;
    }

    //     Change this! argument split is useless now
    int split = mesObjT->split;
    std::cerr << "Trot Walk Up Inside" << std::endl;
    int shift = int(float(step_length) / std::sqrt(2));
//    std::cerr << "Shift: " << shift << std::endl;


//    First iteration, preparation for making one or several steps
    LegPoint line_pts0 = coorObjT->ellipse_down_points_leg(start_pos0[0], start_pos0[1], start_pos0[2],
                                                           start_pos0[0] + direct_k * shift,
                                                           start_pos0[1] - direct_k * shift,
                                                           start_pos0[2], step_down, split, 0);

    LegPoint pts1 = coorObjT->ellipse_points_leg(start_pos1[0], start_pos1[1], start_pos1[2],
                                                 start_pos1[0] - direct_k * shift,
                                                 start_pos1[1] + direct_k * shift,
                                                 start_pos1[2], step_high, split, 1);

    LegPoint line_pts2 = coorObjT->ellipse_down_points_leg(start_pos2[0], start_pos2[1], start_pos2[2],
                                                           start_pos2[0] + direct_k * shift,
                                                           start_pos2[1] - direct_k * shift,
                                                           start_pos2[2], step_down, split, 2);

    LegPoint pts3 = coorObjT->ellipse_points_leg(start_pos3[0], start_pos3[1], start_pos3[2],
                                                 start_pos3[0] - direct_k * shift,
                                                 start_pos3[1] + direct_k * shift,
                                                 start_pos3[2], step_high, split, 3);

    LegPoint pts_iteration_1[] = {line_pts0, pts1, pts3, line_pts2};
    coorObjT->followPoints(pts_iteration_1, 4);


//    Making one or several steps, depending on given parameter.
    for (int i = 0; i < step_amount; ++i) {

        //   0 and 2 moving forward
        LegPoint pts0 = coorObjT->ellipse_points_leg(start_pos0[0] + direct_k * shift,
                                                     start_pos0[1] - direct_k * shift,
                                                     start_pos0[2],
                                                     start_pos0[0] - direct_k * shift,
                                                     start_pos0[1] + direct_k * shift,
                                                     start_pos0[2],
                                                     step_high, split, 0);

        LegPoint pts2 = coorObjT->ellipse_points_leg(start_pos2[0] + direct_k * shift,
                                                     start_pos2[1] - direct_k * shift,
                                                     start_pos2[2],
                                                     start_pos2[0] - direct_k * shift,
                                                     start_pos2[1] + direct_k * shift,
                                                     start_pos2[2],
                                                     step_high, split, 2);

        LegPoint line_pts1 = coorObjT->ellipse_down_points_leg(start_pos1[0] - direct_k * shift,
                                                               start_pos1[1] + direct_k * shift,
                                                               start_pos1[2],
                                                               start_pos1[0] + direct_k * shift,
                                                               start_pos1[1] - direct_k * shift,
                                                               start_pos1[2], step_down, split, 1);

        LegPoint line_pts3 = coorObjT->ellipse_down_points_leg(start_pos3[0] - direct_k * shift,
                                                               start_pos3[1] + direct_k * shift,
                                                               start_pos3[2],
                                                               start_pos3[0] + direct_k * shift,
                                                               start_pos3[1] - direct_k * shift,
                                                               start_pos3[2], step_down, split, 3);

        LegPoint pts_iteration_2[] = {pts0, line_pts1, pts2, line_pts3};
        coorObjT->followPoints(pts_iteration_2, 4);

        //   1 and 3 leg moving forward

        line_pts0 = coorObjT->ellipse_down_points_leg(start_pos0[0] - direct_k * shift,
                                                      start_pos0[1] + direct_k * shift,
                                                      start_pos0[2],
                                                      start_pos0[0] + direct_k * shift,
                                                      start_pos0[1] - direct_k * shift,
                                                      start_pos0[2], step_down, split, 0);

        line_pts2 = coorObjT->ellipse_down_points_leg(start_pos2[0] - direct_k * shift,
                                                      start_pos2[1] + direct_k * shift,
                                                      start_pos2[2],
                                                      start_pos2[0] + direct_k * shift,
                                                      start_pos2[1] - direct_k * shift,
                                                      start_pos2[2], step_down, split, 2);

        pts1 = coorObjT->ellipse_points_leg(start_pos1[0] + direct_k * shift,
                                            start_pos1[1] - direct_k * shift,
                                            start_pos1[2],
                                            start_pos1[0] - direct_k * shift,
                                            start_pos1[1] + direct_k * shift,
                                            start_pos1[2],
                                            step_high, split, 1);

        pts3 = coorObjT->ellipse_points_leg(start_pos3[0] + direct_k * shift,
                                            start_pos3[1] - direct_k * shift,
                                            start_pos3[2],
                                            start_pos3[0] - direct_k * shift,
                                            start_pos3[1] + direct_k * shift,
                                            start_pos3[2],
                                            step_high, split, 3);

        LegPoint pts_iteration_3[] = {pts1, line_pts0, pts3, line_pts2};
        coorObjT->followPoints(pts_iteration_3, 4);
    }


    //    Last iteration, returning to standart position

    LegPoint pts0 = coorObjT->ellipse_points_leg(start_pos0[0] + direct_k * shift,
                                                 start_pos0[1] - direct_k * shift,
                                                 start_pos0[2],
                                                 start_pos0[0],
                                                 start_pos0[1],
                                                 start_pos0[2],
                                                 step_high, split, 0);

    LegPoint pts2 = coorObjT->ellipse_points_leg(start_pos2[0] + direct_k * shift,
                                                 start_pos2[1] - direct_k * shift,
                                                 start_pos2[2],
                                                 start_pos2[0],
                                                 start_pos2[1],
                                                 start_pos2[2],
                                                 step_high, split, 2);

    LegPoint line_pts1 = coorObjT->ellipse_down_points_leg(start_pos1[0] - direct_k * shift,
                                                           start_pos1[1] + direct_k * shift,
                                                           start_pos1[2],
                                                           start_pos1[0],
                                                           start_pos1[1],
                                                           start_pos1[2], step_down, split, 1);

    LegPoint line_pts3 = coorObjT->ellipse_down_points_leg(start_pos3[0] - direct_k * shift,
                                                           start_pos3[1] + direct_k * shift,
                                                           start_pos3[2],
                                                           start_pos3[0],
                                                           start_pos3[1],
                                                           start_pos3[2], step_down, split, 3);

    LegPoint pts_iteration_4[] = {pts0, line_pts1, pts2, line_pts3};
    coorObjT->followPoints(pts_iteration_4, 4);


    return true;
}


// Method to turn aroung, standing at one place
bool TrotGait::selfTurning(int turns_amount, int step_degree) {
    int step_high = 20;
    int split = 1;
    double cosAlpha = std::cos(step_degree * 3.14 / 180.0);
    double sinAlpha = std::sin(step_degree * 3.14 / 180.0);
//    std::cerr << "Sin A: " << sinAlpha <<std::endl;

    for (int i = 0; i < turns_amount; ++i) {

        LegPoint pts0 = coorObjT->ellipse_points_leg(start_pos0[0],
                                                     start_pos0[1],
                                                     start_pos0[2],
                                                     int(start_pos0[0] * cosAlpha),
                                                     int(start_pos0[0] * sinAlpha),
                                                     start_pos0[2],
                                                     step_high, split, 0);

        LegPoint pts2 = coorObjT->ellipse_points_leg(start_pos2[0],
                                                     start_pos2[1],
                                                     start_pos2[2],
                                                     int(start_pos2[0] * cosAlpha),
                                                     int(start_pos2[0] * sinAlpha),
                                                     start_pos2[2],
                                                     step_high, split, 2);

        LegPoint pts_iteration_1[] = {pts0, pts2};
        coorObjT->followPoints(pts_iteration_1, 2);

        LegPoint pts1 = coorObjT->ellipse_points_leg(start_pos1[0],
                                                     start_pos1[1],
                                                     start_pos1[2],
                                                     -1 * int(start_pos1[1] * sinAlpha),
                                                     int(start_pos1[1] * cosAlpha),
                                                     start_pos1[2],
                                                     step_high, split, 1);

        LegPoint pts3 = coorObjT->ellipse_points_leg(start_pos3[0],
                                                     start_pos3[1],
                                                     start_pos3[2],
                                                     -1 * int(start_pos3[1] * sinAlpha),
                                                     int(start_pos3[1] * cosAlpha),
                                                     start_pos3[2],
                                                     step_high, split, 3);

        LegPoint pts_iteration_2[] = {pts1, pts3};
        coorObjT->followPoints(pts_iteration_2, 2);

        trotInitialPosition();
    }
    return true;
}

// Method to calibrate the magnetometer. Should be runned in a separate ssh connection to TRIK (multithread problem)
bool TrotGait::self_calibrate(int amount_calls) {
    float cal_x[amount_calls];
    float cal_y[amount_calls];
    float z_useless;
    magnetometer.set_magnetometer_offset(mesObjT->off_x, mesObjT->off_y, -.37);

    for (int i = 0; i < amount_calls; ++i) {

        magnetometer.read_magnetometer(cal_x[i], cal_y[i], z_useless);

        usleep(100000);
    }

    float average_x, average_y, max_x = -1000, max_y = -1000, min_x = 1000, min_y = 1000;

    for (int j = 0; j < amount_calls; ++j) {
        if (cal_x[j] > max_x) max_x = cal_x[j];
        if (cal_x[j] < min_x) min_x = cal_x[j];
        if (cal_y[j] > max_y) max_y = cal_y[j];
        if (cal_y[j] < min_y) min_y = cal_y[j];
    }

    average_x = (max_x + min_x) / 2;
    average_y = (max_y + min_y) / 2;

    std::cerr << "max_x: " << max_x << std::endl;
    std::cerr << "min_x: " << min_x << std::endl;

    std::cerr << "max_y: " << max_y << std::endl;
    std::cerr << "min_y: " << min_y << std::endl;

    std::cerr << "average_x: " << average_x << std::endl;
    std::cerr << "average_y: " << average_y << std::endl;

    return true;
}


// To move COG (body). Actually it means moving of all legs in the same direction within the same length, using line trajectory
bool TrotGait::bodyMove(int x_shift, int y_shift, int z_shift, int speed) {
    if (speed != 0) {
        posObjT->setSpeed(speed);
    }

    std::vector<int> current_leg0 = posObjT->returnCurrentLegCoor(0);
    std::vector<int> current_leg1 = posObjT->returnCurrentLegCoor(1);
    std::vector<int> current_leg2 = posObjT->returnCurrentLegCoor(2);
    std::vector<int> current_leg3 = posObjT->returnCurrentLegCoor(3);


    LegPoint line_pts0 = coorObjT->line_points_leg(current_leg0[0], current_leg0[1], current_leg0[2],
                                                   current_leg0[0] + x_shift,
                                                   current_leg0[1] + y_shift,
                                                   current_leg0[2] + z_shift, split, 0);

    LegPoint line_pts1 = coorObjT->line_points_leg(current_leg1[0], current_leg1[1], current_leg1[2],
                                                   current_leg1[0] + x_shift,
                                                   current_leg1[1] + y_shift,
                                                   current_leg1[2] + z_shift, split, 1);

    LegPoint line_pts2 = coorObjT->line_points_leg(current_leg2[0], current_leg2[1], current_leg2[2],
                                                   current_leg2[0] + x_shift,
                                                   current_leg2[1] + y_shift,
                                                   current_leg2[2] + z_shift, split, 2);

    LegPoint line_pts3 = coorObjT->line_points_leg(current_leg3[0], current_leg3[1], current_leg3[2],
                                                   current_leg3[0] + x_shift,
                                                   current_leg3[1] + y_shift,
                                                   current_leg3[2] + z_shift, split, 3);


    LegPoint pts_iteration_1[] = {line_pts0, line_pts1, line_pts2, line_pts3};
    coorObjT->followPoints(pts_iteration_1, 4);
    posObjT->setDefaultSpeeds();
    return true;
}