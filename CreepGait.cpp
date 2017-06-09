//
// Created by Nikita Vasilenko on 11/05/2017.
//

#include <cmath>
#include <zconf.h>
#include "CreepGait.h"
#include "Measurements.h"
#include "Coordinates.h"

Measurements *mesObjC = new Measurements();
Positions *posObjC = new Positions();
Coordinates *coorObjC = new Coordinates();


int start_creep_pos0[3] = {-240, 0, -60};
int start_creep_pos1[3] = {0, -240, -60};
int start_creep_pos2[3] = {240, 0, -60};
int start_creep_pos3[3] = {0, 240, -60};


bool CreepGait::creepWalkUp(int step_length, int step_high, int turn_r_l, int step_amount) {
    int split = mesObjC->split;
    int split_line = 1;
    int shift = int(float(step_length) / std::sqrt(2));
    int small_shift = int(float(step_length / 3.0) / std::sqrt(2));
    int big_shift = int(float((step_length * 2.0) / 3.0) / std::sqrt(2));

    std::vector<int> current_leg0 = posObjC->returnCurrentLegCoor(0);
    std::vector<int> current_leg1 = posObjC->returnCurrentLegCoor(1);
    std::vector<int> current_leg2 = posObjC->returnCurrentLegCoor(2);
    std::vector<int> current_leg3 = posObjC->returnCurrentLegCoor(3);

    int start_creep_pos0[3] = {current_leg0[0], current_leg0[1], current_leg0[2]};
    int start_creep_pos1[3] = {current_leg1[0], current_leg1[1], current_leg1[2]};
    int start_creep_pos2[3] = {current_leg2[0], current_leg2[1], current_leg2[2]};
    int start_creep_pos3[3] = {current_leg3[0], current_leg3[1], current_leg3[2]};


    LegPoint pts1 = coorObjC->ellipse_points_leg(start_creep_pos1[0], start_creep_pos1[1], start_creep_pos1[2],
                                                 start_creep_pos1[0] - big_shift,
                                                 start_creep_pos1[1] + big_shift,
                                                 start_creep_pos1[2], step_high, split, 1);

    LegPoint pts3 = coorObjC->ellipse_points_leg(start_creep_pos3[0], start_creep_pos3[1], start_creep_pos3[2],
                                                 start_creep_pos3[0] + big_shift,
                                                 start_creep_pos3[1] - big_shift,
                                                 start_creep_pos3[2], step_high, split, 3);
    LegPoint pts_iteration0[] = {pts1, pts3};
    coorObjC->followPoints(pts_iteration0, 2);
    for (int i = 0; i < step_amount; ++i) {

//  First iteration
        LegPoint line_pts0 = coorObjC->line_points_leg(start_creep_pos0[0], start_creep_pos0[1], start_creep_pos0[2],
                                                       start_creep_pos0[0] + small_shift,
                                                       start_creep_pos0[1] - small_shift,
                                                       start_creep_pos0[2], split_line, 0);

        LegPoint line_pts1 = coorObjC->line_points_leg(start_creep_pos1[0] - big_shift,
                                                       start_creep_pos1[1] + big_shift,
                                                       start_creep_pos1[2],
                                                       start_creep_pos1[0] - small_shift,
                                                       start_creep_pos1[1] + small_shift,
                                                       start_creep_pos1[2], split_line, 1);

        LegPoint line_pts3 = coorObjC->line_points_leg(start_creep_pos3[0] + big_shift,
                                                       start_creep_pos3[1] - big_shift,
                                                       start_creep_pos3[2],
                                                       start_creep_pos3[0] + shift,
                                                       start_creep_pos3[1] - shift,
                                                       start_creep_pos3[2], split_line, 3);

        LegPoint pts2 = coorObjC->ellipse_points_leg(start_creep_pos2[0], start_creep_pos2[1], start_creep_pos2[2],
                                                     start_creep_pos2[0] - shift,
                                                     start_creep_pos2[1] + shift,
                                                     start_creep_pos2[2], step_high, split, 2);

        LegPoint pts2_delayed = coorObjC->make_start_delay(pts2, 30);


        LegPoint pts_iteration1[] = {line_pts0, line_pts1, pts2_delayed, line_pts3};


        coorObjC->followPoints_sync(pts_iteration1, 4);

//  Second iteration

        line_pts0 = coorObjC->line_points_leg(start_creep_pos0[0] + small_shift,
                                              start_creep_pos0[1] - small_shift,
                                              start_creep_pos0[2],
                                              start_creep_pos0[0] + big_shift,
                                              start_creep_pos0[1] - big_shift,
                                              start_creep_pos0[2], split_line, 0);

        line_pts1 = coorObjC->line_points_leg(start_creep_pos1[0] - small_shift,
                                              start_creep_pos1[1] + small_shift,
                                              start_creep_pos1[2],
                                              start_creep_pos1[0],
                                              start_creep_pos1[1],
                                              start_creep_pos1[2], split_line, 1);

        LegPoint line_pts2 = coorObjC->line_points_leg(start_creep_pos2[0] - shift,
                                                       start_creep_pos2[1] + shift,
                                                       start_creep_pos2[2],
                                                       start_creep_pos2[0] - big_shift,
                                                       start_creep_pos2[1] + big_shift,
                                                       start_creep_pos2[2], split_line, 2);

        pts3 = coorObjC->ellipse_points_leg(start_creep_pos3[0] + shift,
                                            start_creep_pos3[1] - shift,
                                            start_creep_pos3[2],
                                            start_creep_pos3[0],
                                            start_creep_pos3[1],
                                            start_creep_pos3[2], step_high, split, 3);


        LegPoint pts_iteration2[] = {line_pts0, line_pts1, pts3, line_pts2};
        coorObjC->followPoints_sync(pts_iteration2, 4);

//  Third iteration
        line_pts0 = coorObjC->line_points_leg(start_creep_pos0[0] + big_shift,
                                              start_creep_pos0[1] - big_shift,
                                              start_creep_pos0[2],
                                              start_creep_pos0[0] + shift,
                                              start_creep_pos0[1] - shift,
                                              start_creep_pos0[2], split_line, 0);

        line_pts2 = coorObjC->line_points_leg(start_creep_pos2[0] - big_shift,
                                              start_creep_pos2[1] + big_shift,
                                              start_creep_pos2[2],
                                              start_creep_pos2[0] - small_shift,
                                              start_creep_pos2[1] + small_shift,
                                              start_creep_pos2[2], split_line, 2);

        line_pts3 = coorObjC->line_points_leg(start_creep_pos3[0],
                                              start_creep_pos3[1],
                                              start_creep_pos3[2],
                                              start_creep_pos3[0] + small_shift,
                                              start_creep_pos3[1] - small_shift,
                                              start_creep_pos3[2], split_line, 3);

        pts1 = coorObjC->ellipse_points_leg(start_creep_pos1[0],
                                            start_creep_pos1[1],
                                            start_creep_pos1[2],
                                            start_creep_pos1[0] - shift,
                                            start_creep_pos1[1] + shift,
                                            start_creep_pos1[2], step_high, split, 1);

        LegPoint pts1_delayed = coorObjC->make_start_delay(pts1, 30);

        LegPoint pts_iteration3[] = {line_pts0, line_pts2, pts1_delayed, line_pts3};
        coorObjC->followPoints_sync(pts_iteration3, 4);

//          Fourth iteration
        line_pts1 = coorObjC->line_points_leg(start_creep_pos1[0] - shift,
                                              start_creep_pos1[1] + shift,
                                              start_creep_pos1[2],
                                              start_creep_pos1[0] - big_shift,
                                              start_creep_pos1[1] + big_shift,
                                              start_creep_pos1[2], split_line, 1);

        line_pts2 = coorObjC->line_points_leg(start_creep_pos2[0] - small_shift,
                                              start_creep_pos2[1] + small_shift,
                                              start_creep_pos2[2],
                                              start_creep_pos2[0],
                                              start_creep_pos2[1],
                                              start_creep_pos2[2], split_line, 2);

        line_pts3 = coorObjC->line_points_leg(start_creep_pos3[0] + small_shift,
                                              start_creep_pos3[1] - small_shift,
                                              start_creep_pos3[2],
                                              start_creep_pos3[0] + big_shift,
                                              start_creep_pos3[1] - big_shift,
                                              start_creep_pos3[2], split_line, 3);

        LegPoint pts0 = coorObjC->ellipse_points_leg(start_creep_pos0[0] + shift,
                                                     start_creep_pos0[1] - shift,
                                                     start_creep_pos0[2],
                                                     start_creep_pos0[0],
                                                     start_creep_pos0[1],
                                                     start_creep_pos0[2], step_high, split, 0);


        LegPoint pts_iteration4[] = {line_pts1, line_pts2, pts0, line_pts3};
        coorObjC->followPoints_sync(pts_iteration4, 4);


    }


    pts1 = coorObjC->ellipse_points_leg(start_creep_pos1[0] - big_shift,
                                        start_creep_pos1[1] + big_shift,
                                        start_creep_pos1[2],
                                        start_creep_pos1[0],
                                        start_creep_pos1[1],
                                        start_creep_pos1[2], step_high, split, 1);

    pts3 = coorObjC->ellipse_points_leg(start_creep_pos3[0] + big_shift,
                                        start_creep_pos3[1] - big_shift,
                                        start_creep_pos3[2],
                                        start_creep_pos3[0],
                                        start_creep_pos3[1],
                                        start_creep_pos3[2], step_high, split, 3);

    LegPoint pts_iteration5[] = {pts1, pts3};
    coorObjC->followPoints(pts_iteration5, 2);
    return true;
}

bool CreepGait::deerWalkUp(int step_length, int step_high, int turn_r_l, int step_amount) {
    int split = mesObjC->split;
    int split_line = 1;
    int shift = int(float(step_length) / std::sqrt(2));
    int small_shift = int(float(step_length / 3.0) / std::sqrt(2));
    int big_shift = int(float((step_length * 2.0) / 3.0) / std::sqrt(2));

    std::vector<int> current_leg0 = posObjC->returnCurrentLegCoor(0);
    std::vector<int> current_leg1 = posObjC->returnCurrentLegCoor(1);
    std::vector<int> current_leg2 = posObjC->returnCurrentLegCoor(2);
    std::vector<int> current_leg3 = posObjC->returnCurrentLegCoor(3);

    int start_creep_pos0[3] = {current_leg0[0], current_leg0[1], current_leg0[2]};
    int start_creep_pos1[3] = {current_leg1[0], current_leg1[1], current_leg1[2]};
    int start_creep_pos2[3] = {current_leg2[0], current_leg2[1], current_leg2[2]};
    int start_creep_pos3[3] = {current_leg3[0], current_leg3[1], current_leg3[2]};


    LegPoint pts1 = coorObjC->ellipse_points_leg(start_creep_pos1[0], start_creep_pos1[1], start_creep_pos1[2],
                                                 start_creep_pos1[0] - big_shift,
                                                 start_creep_pos1[1] + big_shift,
                                                 start_creep_pos1[2], step_high, split, 1);

    LegPoint pts3 = coorObjC->ellipse_points_leg(start_creep_pos3[0], start_creep_pos3[1], start_creep_pos3[2],
                                                 start_creep_pos3[0] + big_shift,
                                                 start_creep_pos3[1] - big_shift,
                                                 start_creep_pos3[2], step_high, split, 3);
    LegPoint pts_iteration0[] = {pts1, pts3};
    coorObjC->followPoints(pts_iteration0, 2);
    for (int i = 0; i < step_amount; ++i) {

//  First iteration
        LegPoint line_pts0 = coorObjC->line_points_leg(start_creep_pos0[0], start_creep_pos0[1], start_creep_pos0[2],
                                                       start_creep_pos0[0] + small_shift,
                                                       start_creep_pos0[1] - small_shift,
                                                       start_creep_pos0[2], split_line, 0);

        LegPoint line_pts1 = coorObjC->line_points_leg(start_creep_pos1[0] - big_shift,
                                                       start_creep_pos1[1] + big_shift,
                                                       start_creep_pos1[2],
                                                       start_creep_pos1[0] - small_shift,
                                                       start_creep_pos1[1] + small_shift,
                                                       start_creep_pos1[2], split_line, 1);

        LegPoint line_pts2 = coorObjC->line_points_leg(start_creep_pos2[0],
                                                       start_creep_pos2[1],
                                                       start_creep_pos2[2],
                                                       start_creep_pos2[0] + small_shift,
                                                       start_creep_pos2[1] - small_shift,
                                                       start_creep_pos2[2], split_line, 2);

        LegPoint line_pts3 = coorObjC->line_points_leg(start_creep_pos3[0] + big_shift,
                                                       start_creep_pos3[1] - big_shift,
                                                       start_creep_pos3[2],
                                                       start_creep_pos3[0] + shift,
                                                       start_creep_pos3[1] - shift,
                                                       start_creep_pos3[2], split_line, 3);

        LegPoint pts2 = coorObjC->ellipse_points_leg(start_creep_pos2[0] + small_shift,
                                                     start_creep_pos2[1] - small_shift,
                                                     start_creep_pos2[2],
                                                     start_creep_pos2[0] - shift,
                                                     start_creep_pos2[1] + shift,
                                                     start_creep_pos2[2], step_high, split, 2);


        LegPoint pts_iteration1[] = {line_pts0, line_pts1, line_pts2, line_pts3};

        coorObjC->followPoints_sync(pts_iteration1, 4);

        LegPoint pts_iteration1_2[] = {pts2};

        coorObjC->followPoints_sync(pts_iteration1_2, 1);

//  Second iteration

        line_pts0 = coorObjC->line_points_leg(start_creep_pos0[0] + small_shift,
                                              start_creep_pos0[1] - small_shift,
                                              start_creep_pos0[2],
                                              start_creep_pos0[0] + big_shift,
                                              start_creep_pos0[1] - big_shift,
                                              start_creep_pos0[2], split_line, 0);

        line_pts1 = coorObjC->line_points_leg(start_creep_pos1[0] - small_shift,
                                              start_creep_pos1[1] + small_shift,
                                              start_creep_pos1[2],
                                              start_creep_pos1[0],
                                              start_creep_pos1[1],
                                              start_creep_pos1[2], split_line, 1);

        line_pts2 = coorObjC->line_points_leg(start_creep_pos2[0] - shift,
                                              start_creep_pos2[1] + shift,
                                              start_creep_pos2[2],
                                              start_creep_pos2[0] - big_shift,
                                              start_creep_pos2[1] + big_shift,
                                              start_creep_pos2[2], split_line, 2);

        line_pts3 = coorObjC->line_points_leg(start_creep_pos3[0] + shift,
                                              start_creep_pos3[1] - shift,
                                              start_creep_pos3[2],
                                              start_creep_pos3[0] + shift + small_shift,
                                              start_creep_pos3[1] - shift - small_shift,
                                              start_creep_pos3[2], split_line, 3);

        pts3 = coorObjC->ellipse_points_leg(start_creep_pos3[0] + shift + small_shift,
                                            start_creep_pos3[1] - shift - small_shift,
                                            start_creep_pos3[2],
                                            start_creep_pos3[0],
                                            start_creep_pos3[1],
                                            start_creep_pos3[2], step_high, split, 3);


        LegPoint pts_iteration2[] = {line_pts0, line_pts1, line_pts2, line_pts3};
        coorObjC->followPoints_sync(pts_iteration2, 4);

        LegPoint pts_iteration2_2[] = {pts3};
        coorObjC->followPoints_sync(pts_iteration2_2, 1);

//  Third iteration
        line_pts0 = coorObjC->line_points_leg(start_creep_pos0[0] + big_shift,
                                              start_creep_pos0[1] - big_shift,
                                              start_creep_pos0[2],
                                              start_creep_pos0[0] + shift,
                                              start_creep_pos0[1] - shift,
                                              start_creep_pos0[2], split_line, 0);

        line_pts1 = coorObjC->line_points_leg(start_creep_pos1[0],
                                              start_creep_pos1[1],
                                              start_creep_pos1[2],
                                              start_creep_pos1[0] + small_shift,
                                              start_creep_pos1[1] - small_shift,
                                              start_creep_pos1[2], split_line, 1);

        line_pts2 = coorObjC->line_points_leg(start_creep_pos2[0] - big_shift,
                                              start_creep_pos2[1] + big_shift,
                                              start_creep_pos2[2],
                                              start_creep_pos2[0] - small_shift,
                                              start_creep_pos2[1] + small_shift,
                                              start_creep_pos2[2], split_line, 2);

        line_pts3 = coorObjC->line_points_leg(start_creep_pos3[0],
                                              start_creep_pos3[1],
                                              start_creep_pos3[2],
                                              start_creep_pos3[0] + small_shift,
                                              start_creep_pos3[1] - small_shift,
                                              start_creep_pos3[2], split_line, 3);

        pts1 = coorObjC->ellipse_points_leg(start_creep_pos1[0] + small_shift,
                                            start_creep_pos1[1] - small_shift,
                                            start_creep_pos1[2],
                                            start_creep_pos1[0] - shift,
                                            start_creep_pos1[1] + shift,
                                            start_creep_pos1[2], step_high, split, 1);


        LegPoint pts_iteration3[] = {line_pts0, line_pts1, line_pts2, line_pts3};
        coorObjC->followPoints_sync(pts_iteration3, 4);

        LegPoint pts_iteration3_2[] = {pts1};
        coorObjC->followPoints_sync(pts_iteration3_2, 1);

//          Fourth iteration
        line_pts0 = coorObjC->line_points_leg(start_creep_pos0[0] + shift,
                                              start_creep_pos0[1] - shift,
                                              start_creep_pos0[2],
                                              start_creep_pos0[0] + shift + small_shift,
                                              start_creep_pos0[1] - shift - small_shift,
                                              start_creep_pos0[2], split_line, 0);

        line_pts1 = coorObjC->line_points_leg(start_creep_pos1[0] - shift,
                                              start_creep_pos1[1] + shift,
                                              start_creep_pos1[2],
                                              start_creep_pos1[0] - big_shift,
                                              start_creep_pos1[1] + big_shift,
                                              start_creep_pos1[2], split_line, 1);

        line_pts2 = coorObjC->line_points_leg(start_creep_pos2[0] - small_shift,
                                              start_creep_pos2[1] + small_shift,
                                              start_creep_pos2[2],
                                              start_creep_pos2[0],
                                              start_creep_pos2[1],
                                              start_creep_pos2[2], split_line, 2);

        line_pts3 = coorObjC->line_points_leg(start_creep_pos3[0] + small_shift,
                                              start_creep_pos3[1] - small_shift,
                                              start_creep_pos3[2],
                                              start_creep_pos3[0] + big_shift,
                                              start_creep_pos3[1] - big_shift,
                                              start_creep_pos3[2], split_line, 3);

        LegPoint pts0 = coorObjC->ellipse_points_leg(start_creep_pos0[0] + shift + small_shift,
                                                     start_creep_pos0[1] - shift - small_shift,
                                                     start_creep_pos0[2],
                                                     start_creep_pos0[0],
                                                     start_creep_pos0[1],
                                                     start_creep_pos0[2], step_high, split, 0);


        LegPoint pts_iteration4[] = {line_pts0, line_pts1, line_pts2, line_pts3};
        coorObjC->followPoints_sync(pts_iteration4, 4);

        LegPoint pts_iteration4_2[] = {pts0};
        coorObjC->followPoints_sync(pts_iteration4_2, 1);


    }


    pts1 = coorObjC->ellipse_points_leg(start_creep_pos1[0] - big_shift,
                                        start_creep_pos1[1] + big_shift,
                                        start_creep_pos1[2],
                                        start_creep_pos1[0],
                                        start_creep_pos1[1],
                                        start_creep_pos1[2], step_high, split, 1);

    pts3 = coorObjC->ellipse_points_leg(start_creep_pos3[0] + big_shift,
                                        start_creep_pos3[1] - big_shift,
                                        start_creep_pos3[2],
                                        start_creep_pos3[0],
                                        start_creep_pos3[1],
                                        start_creep_pos3[2], step_high, split, 3);

    LegPoint pts_iteration5[] = {pts1, pts3};
    coorObjC->followPoints(pts_iteration5, 2);
    return true;
}




