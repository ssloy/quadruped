//
// Created by Nikita Vasilenko on 07/05/2017.
//
// Coordinates class is needed to describe functions, working with 3D-Points, having x, y and z coordinates
// With the help of trigonometry the inverse kinematic is implemented here.
#ifndef QUADRUPED_COORDINATES_H
#define QUADRUPED_COORDINATES_H
#include <vector>
#include "dynamixel.h"
#include "Positions.h"
#include "LegPoint.h"

class Coordinates {
public:
    std::vector<double> find_unique_solution(int x, int y);

    std::vector<int> returnLegsServos(int leg);

    std::vector<double> find_geom_solutions(int x, int y);

    std::vector<double> find_x0_y0(int x, int y);

    std::vector<int> transform_coordinates3D(int x, int y, int z, int leg);

    std::vector<double> compute_coordinates2D(int x, int y, int leg);

    std::vector<double> compute_anglesFor3D(int x, int y, int z, int leg);

    LegPoint line_points_leg(int x1, int y1, int z1, int x2, int y2, int z2, int split, int leg);

    LegPoint ellipse_points_leg(int x1, int y1, int z1, int x2, int y2, int z2, int h, int split, int leg);

    LegPoint ellipse_down_points_leg(int x1, int y1, int z1, int x2, int y2, int z2, int h, int split, int leg);

    double find_first_angle(int x, int y);

    double find_first_angle_servo(int x, int y);

    bool set_coordinates3D_sync(int *x_arr, int *y_arr, int *z_arr, int *leg_arr, int amount);

    bool followPoints(LegPoint *legPts, int numb_of_legs);

    bool followPoints_sync(LegPoint *legPts, int numb_of_legs);


    LegPoint make_start_delay(LegPoint legPts, int percent_delay);


    Coordinates();
};


#endif //QUADRUPED_COORDINATES_H
