//
// Created by Nikita Vasilenko on 15/05/2017.
//
//

#include <cmath>
#include "BodyPosition.h"
#include "Measurements.h"

Measurements *mesB = new Measurements();

// Method to compute current 3D position of a leg. Input is the number of a leg (0 .. 3)
std::vector<int> BodyPosition::computeCurrentPoint(int leg) {
    std::vector<int> result;
    result.reserve(3);
    double omega = 0;
    double alpha = 0;
    double betta = 0;
    if (leg == 0) {
        omega = serv0;
        alpha = serv1;
        betta = serv2;
    }
    else if (leg == 1) {
        omega = serv10;
        alpha = serv11;
        betta = serv12;
    }
    else if (leg == 2) {
        omega = serv20;
        alpha = serv21;
        betta = serv22;
    }
    else if (leg == 3) {
        omega = serv30;
        alpha = serv31;
        betta = serv32;
    }

//    Transform to radians
    omega = (omega * 4 * std::atan(1) / 180.0);
    alpha = (alpha * 4 * std::atan(1) / 180.0);
    betta = (betta * 4 * std::atan(1) / 180.0);

    double x_0 = std::cos(omega) * mesB->gipLength_0 + mesB->length_centr_to_x0;
    double y_0 = std::sin(omega) * mesB->gipLength_0;
    double z_0 = 0.0;

    double x_1 = std::cos(omega) * mesB->gipLength_1 * std::cos(alpha) + x_0;
    double y_1 = std::sin(omega) * mesB->gipLength_1 * std::cos(alpha) + y_0;
    double z_1 = std::sin(alpha) * mesB->gipLength_1 + z_0;

    double x_2 = std::cos(omega) * mesB->gipLength_2 * std::cos(alpha + betta) + x_1;
    double y_2 = std::sin(omega) * mesB->gipLength_2 * std::cos(alpha + betta) + y_1;
    double z_2 = std::sin(alpha + betta) * mesB->gipLength_2 + z_1;

    if (leg == 0) {
        result[0] = int(-x_2);
        result[1] = int(-y_2);
        result[2] = int(z_2);
    }
    else if (leg == 1) {
        result[0] = int(y_2);
        result[1] = int(-x_2);
        result[2] = int(z_2);
    }
    else if (leg == 2) {
        result[0] = int(x_2);
        result[1] = int(y_2);
        result[2] = int(z_2);
    }
    else if (leg == 3) {
        result[0] = int(-y_2);
        result[1] = int(x_2);
        result[2] = int(z_2);
    }
    return result;
}
