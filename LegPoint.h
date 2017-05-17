//
// Created by Nikita Vasilenko on 09/05/2017.
//
// Class is implemented only in *.h file
// Describes the user-defined "structure" to save the trajectory of the leg's movement.


#ifndef QUADRUPED_LEGPOINT_H
#define QUADRUPED_LEGPOINT_H

#include <vector>
#include <iostream>

class   LegPoint {
public:
    std::vector<int> x_dots;
    std::vector<int> y_dots;
    std::vector<int> z_dots;
    int legNumber;

    LegPoint(const std::vector<int> &x_dots, const std::vector<int> &y_dots, const std::vector<int> &z_dots,
             int legNumber) : x_dots(x_dots), y_dots(y_dots), z_dots(z_dots), legNumber(legNumber) {}

    void printLegPoint() {
        int n = (int) x_dots.size();
        for (int i = 0; i < n; ++i) {
            std::cerr << "x[" << i << "] = " << x_dots[i] << ", y[" << i << "] = " << y_dots[i] << ", z[" << i << "] = "
                      << z_dots[i] << std::endl;
        }
    }
};


#endif //QUADRUPED_LEGPOINT_H
