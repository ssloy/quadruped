//
// Created by Nikita Vasilenko on 11/05/2017.
//

#include <cmath>
#include "CreepGait.h"
#include "Measurements.h"
#include "Coordinates.h"

Measurements *mesObjC = new Measurements();
Coordinates *coorObjC = new Coordinates();


int start_creep_pos0[3] = {-240, 0, -60};
int start_creep_pos1[3] = {0, -240, -60};
int start_creep_pos2[3] = {240, 0, -60};
int start_creep_pos3[3] = {0, 240, -60};


bool CreepGait::creepWalkUp(int step_length, int step_high) {
    int split = mesObjC->split;
    int shift = int(float(step_length) / std::sqrt(2));
    int small_shift = int(float(step_length / 3.0) / std::sqrt(2));
    int big_shift = int(float((step_length * 2.0) / 3.0) / std::sqrt(2));

    LegPoint pts2_help = coorObjC->line_points_leg(start_creep_pos2[0], start_creep_pos2[1], start_creep_pos2[2],
                                                   start_creep_pos2[0] + small_shift, start_creep_pos2[1] - small_shift,
                                                   start_creep_pos2[2], split, 2);

    LegPoint pts0 = coorObjC->line_points_leg(start_creep_pos0[0], start_creep_pos0[1], start_creep_pos0[2],
                                              start_creep_pos0[0] + small_shift, start_creep_pos0[1] - small_shift,
                                              start_creep_pos0[2], split, 0);

    LegPoint pts1 = coorObjC->line_points_leg(start_creep_pos1[0] - big_shift, start_creep_pos1[1] + big_shift,
                                              start_creep_pos1[2], start_creep_pos1[0] - small_shift,
                                              start_creep_pos1[1] + small_shift, start_creep_pos1[2], split, 1);

    LegPoint pts3 = coorObjC->line_points_leg(start_creep_pos3[0] + big_shift, start_creep_pos3[1] - big_shift,
                                              start_creep_pos3[2], start_creep_pos3[0] + shift,
                                              start_creep_pos3[1] - shift,
                                              start_creep_pos3[2], split, 3);

    LegPoint pts_iteration1[] = {pts0, pts1, pts2_help, pts3};
    coorObjC->followPoints(pts_iteration1, 4);
    return true;
}


bool CreepGait::creepInitialPosition(int step_length) {
    int big_shift = int(float((step_length * 2.0) / 3.0) / std::sqrt(2));

    int testX[] = {start_creep_pos0[0], start_creep_pos1[0] - big_shift, start_creep_pos2[0],
                   start_creep_pos3[0] + big_shift};
    int testY[] = {start_creep_pos0[1], start_creep_pos1[1] + big_shift, start_creep_pos2[1],
                   start_creep_pos3[1] - big_shift};
    int testZ[] = {start_creep_pos0[2], start_creep_pos1[2], start_creep_pos2[2], start_creep_pos3[2]};
    int testLeg[] = {0, 1, 2, 3};

    coorObjC->set_coordinates3D_sync(testX, testY, testZ, testLeg, 4);
    return true;
}


