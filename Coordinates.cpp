//
// Created by Nikita Vasilenko on 07/05/2017.
//
#include "Coordinates.h"
#include "Measurements.h"
#include <unistd.h> // usleep
#include <cmath>
#include <iostream>
#include "Positions.h"

const double PI = 4 * std::atan(1);
Measurements *mesObj = new Measurements();
Positions *posObj = new Positions;

// This constructor is created only for opening port in Position class object
Coordinates::Coordinates() {
    posObj->openPort();
}


// A method to find a unique solution (works only if there is one)
std::vector<double> Coordinates::find_unique_solution(int x, int y) {
    double gamma = 1000, alpha1 = 1000, alpha2 = 1000, betta1 = 1000, betta2 = 1000;
    if (x == 0.0) {
        if (y > 0) {
            alpha1 = 90.0;
            betta1 = 0.0;
            alpha2 = betta2 = 1000;
        } else if (y < 0) {
            alpha1 = -90.0;
            betta1 = 0.0;
            alpha2 = betta2 = 1000;
        }
    } else if (y == 0.0) {
        if (x > 0) {
            alpha1 = 0.0;
            betta1 = 0.0;
            alpha2 = betta2 = 1000;
        } else if (x < 0) {
            alpha1 = -180.0;
            betta1 = 0.0;
            alpha2 = betta2 = 1000;
        }
    } else {
        if ((x > 0 and y < 0) or (x > 0 and y > 0)) {
            gamma = (std::atan((float) y / (float) x)) * 180 / PI;
        } else if (x < 0 and y < 0) {
            gamma = (std::atan((float) y / (float) x)) * 180 / PI - 180.0;
        } else if (x < 0 and y > 0) {
            gamma = (std::atan((float) y / (float) x)) * 180 / PI + 180.0;
        } else {
            std::cerr << "smth wrong in find_unique_solution" << std::endl;
        }
        alpha1 = gamma;
        betta1 = 0;
        alpha2 = betta2 = 1000;
    }
    std::vector<double> result;
    result.reserve(4);
    result[0] = alpha1;
    result[1] = betta1;
    result[2] = alpha2;
    result[3] = betta2;
    return result;
}

// A method to find a geometrical solution. For a given 2D point (x,y) the method returns two angles (if there is a
// unique solution) or four angles (if there are 2 solutions). First pair is first solution, second pair is second
// solution.
std::vector<double> Coordinates::find_geom_solutions(int x, int y) {
    double gamma = 0, alpha1 = 1000.0, alpha2 = 1000.0, betta1 = 10000., betta2 = 1000.0;
    double overallLength = mesObj->gipLength_1 + mesObj->gipLength_2;
    double a = mesObj->gipLength_1;
    double b = mesObj->gipLength_2;
    double c = std::sqrt((double) (x * x + y * y));
    std::vector<double> result;
    result.reserve(4);

    // Check if the point is too far
    if (c > overallLength) {
        result[0] = 1000.0;
        result[1] = 1000.0;
        result[2] = 1000.0;
        result[3] = 1000.0;
        return result;
    } else if (c == overallLength) {
//        Mistake. Should return find_unique, but doesn't work.
//        std::vector<double> tmp = find_unique_solution(x,y);
//        return tmp;
        result[0] = 1000.0;
        result[1] = 1000.0;
        result[2] = 1000.0;
        result[3] = 1000.0;
        return result;
    } else
        // First, check existence of a triangle
    if (!(a + b > c and a + c > b and b + c > a)) {
        // print "No such triangle, sorry"
        result[0] = 1000.0;
        result[1] = 1000.0;
        result[2] = 1000.0;
        result[3] = 1000.0;
        return result;
    } else {
        // Counting of B and C angles of the triangle
        double cos_B = (a * a + c * c - b * b) / (2.0 * a * c);
        double cos_C = (a * a + b * b - c * c) / (2.0 * a * b);
        double B = std::acos(cos_B) * 180 / PI;
        double C = std::acos(cos_C) * 180 / PI;
        // print "Triangle exists, B: " + str(B) + ", C: " + str(C)
        // Now consider special cases, with one of coordinates == 0
        if (x == 0.0) {
            if (y > 0) {
                gamma = 90.0;
                alpha1 = gamma - B;
                betta1 = 180.0 - C;
                alpha2 = gamma + B;
                betta2 = C - 180.0;
            } else if (y < 0) {
                gamma = -90.0;
                alpha1 = gamma + B;
                betta1 = C - 180.0;
                alpha2 = gamma - B;
                betta2 = 180.0 - C;
            }
        } else if (y == 0.0) {
            if (x > 0) {
                gamma = 0.0;
                alpha1 = gamma + B;
                betta1 = C - 180.0;
                alpha2 = gamma - B;
                betta2 = 180.0 - C;
            } else if (x < 0) {
                gamma = -180.0;
                alpha1 = gamma + B;
                betta1 = 180.0 - C;
                alpha2 = gamma - B;
                betta2 = C - 180.0;
            }
        } else {
            if ((x > 0 and y < 0) or (x > 0 and y > 0)) {
                gamma = (std::atan((float) y / (float) x)) * 180 / PI;
            } else if (x < 0 and y < 0) {
                gamma = (std::atan((float) y / (float) x)) * 180 / PI - 180.0;
            } else if (x < 0 and y > 0) {
                gamma = (std::atan((float) y / (float) x)) * 180 / PI + 180.0;
            } else {
                std::cerr << "smth wrong in find_geom_solution" << std::endl;
            }
            alpha1 = gamma + B;
            betta1 = C - 180;
            alpha2 = gamma - B;
            betta2 = 180 - C;
        }

        result[0] = alpha1;
        result[1] = betta1;
        result[2] = alpha2;
        result[3] = betta2;
        return result;
    }
}

// As there is only one actuator, working in the plane XY, there is always unique solution of it's angle for any point
double Coordinates::find_first_angle(int x, int y) {
    double c = std::sqrt((double) (x * x + y * y));
    double omega = 0.0;
    if (c == 0) {
        omega = 1000.0;
    } else {
        double sin_omega = (float(y) / c);
        if (x == 0.0) {
            if (y > 0) {
                omega = 90.0;
            } else if (y < 0) {
                omega = -90.0;
            }
        } else if (y == 0.0) {
            if (x > 0) {
                omega = 0.0;

            } else if (x < 0) {
                omega = -180.0;
            }
        } else {
            if ((x > 0 and y < 0) or (x > 0 and y > 0)) {
                omega = (std::asin((float) sin_omega)) * 180 / PI;
            } else if (x < 0 and y < 0) {
                omega = (std::asin((float) sin_omega)) * 180 / PI - 90.0;
            } else if (x < 0 and y > 0) {
                omega = (std::asin((float) sin_omega)) * 180 / PI + 90.0;
            } else {
                std::cerr << "smth wrong in find_geom_solution" << std::endl;
            }
        }

    }
    return omega;
}

// Find the starting point for work of 2D methods
std::vector<double> Coordinates::find_x0_y0(int x, int y) {
    double tmpOmega = find_first_angle(x, y);
    std::vector<double> result;
    result.reserve(2);
    if (tmpOmega < 1000) {
        double x0 = mesObj->gipLength_0 * std::cos((double) (tmpOmega * PI / 180)) + mesObj->length_centr_to_x0;
        double y0 = mesObj->gipLength_0 * std::sin((double) (tmpOmega * PI / 180));
        result[0] = x0;
        result[1] = y0;
    } else {
        result[0] = 1000;
        result[1] = 1000;
    }
    return result;
}

// Return the angle of first actuator. It is unique for every point.
double Coordinates::find_first_angle_servo(int x, int y) {
    double x_corrected = x - mesObj->length_centr_to_x0;
    double omega = 0.0;
    if (x_corrected == 0) {
        omega = 1000.0;
    } else {
        double tan_omega = (float) y / x_corrected;
        if (x_corrected == 0.0) {
            if (y > 0) {
                omega = 90.0;
            } else if (y < 0) {
                omega = -90.0;
            }
        } else if (y == 0.0) {
            if (x_corrected > 0) {
                omega = 0.0;
            } else if (x_corrected < 0) {
                omega = -180.0;
            }
        } else {
            if ((x_corrected > 0 and y < 0) or (x_corrected > 0 and y > 0)) {
                omega = (std::atan((float) tan_omega)) * 180 / PI;
            } else if (x_corrected < 0 and y < 0) {
                omega = (std::atan((float) tan_omega)) * 180 / PI + 90.0;
            } else if (x_corrected < 0 and y > 0) {
                omega = (std::atan((float) tan_omega)) * 180 / PI + 90.0;
            } else {
                std::cerr << "smth wrong in find_first_angle_servo" << std::endl;
            }
        }
    }
    return omega;
}

// All 4 legs work symmetric, the only difference is to pass correct coordinates.
std::vector<int> Coordinates::transform_coordinates3D(int x, int y, int z, int leg) {
    std::vector<int> result;
    result.reserve(3);
    if (leg == 0) {
        result[0] = -x;
        result[1] = -y;
        result[2] = z;
    } else if (leg == 1) {
        result[0] = -y;
        result[1] = x;
        result[2] = z;
    } else if (leg == 2) {
        result[0] = x;
        result[1] = y;
        result[2] = z;
    } else if (leg == 3) {
        result[0] = y;
        result[1] = -x;
        result[2] = z;
    } else {
        result[0] = 1000;
        result[1] = 1000;
        result[2] = 1000;
    }
    return result;
}

// A method to return an array of actuators, belonging to given leg (EX: 1 -> [10, 11, 12])
std::vector<int> Coordinates::returnLegsServos(int leg) {
    std::vector<int> result;
    result.reserve(3);
    if (leg == 0) {
        result[0] = 0;
        result[1] = 1;
        result[2] = 2;
    } else if (leg == 1) {
        result[0] = 10;
        result[1] = 11;
        result[2] = 12;
    } else if (leg == 2) {
        result[0] = 20;
        result[1] = 21;
        result[2] = 22;
    } else if (leg == 3) {
        result[0] = 30;
        result[1] = 31;
        result[2] = 32;
    } else {
        result[0] = 1000;
        result[1] = 1000;
        result[2] = 1000;
    }
    return result;
}

// A method to compute a real solution (with checking constraints) for a given 2D point and a leg.
std::vector<double> Coordinates::compute_coordinates2D(int x, int y, int leg) {
    std::vector<double> real_solution;
    real_solution.reserve(4);
    std::vector<int> servos = returnLegsServos(leg);
    std::vector<double> ar = find_geom_solutions(x, y);

    if ((posObj->checkAngle((unsigned char) servos[1], (int) ar[0]) > 0) and
        (posObj->checkAngle((unsigned char) servos[2], (int) ar[1]) > 0)) {
        real_solution[0] = ar[0];
        real_solution[1] = ar[1];
    } else {
        real_solution[0] = 100000;
        real_solution[1] = 100000;
    }

    if ((posObj->checkAngle((unsigned char) servos[1], (int) ar[2]) > 0) and
        (posObj->checkAngle((unsigned char) servos[2], (int) ar[3]) > 0)) {
        real_solution[2] = ar[2];
        real_solution[3] = ar[3];
    } else {
        real_solution[2] = 100000;
        real_solution[3] = 100000;
    }
    return real_solution;
}

// Computing angles of 3 actuators of a given leg, knowing the 3D coordinates of desired point.
// Returns 1000 if it is not possible to move given leg to given position
std::vector<double> Coordinates::compute_anglesFor3D(int x, int y, int z, int leg) {
    std::vector<int> arrNewCoordinates = transform_coordinates3D(x, y, z, leg);
    x = arrNewCoordinates[0];
    y = arrNewCoordinates[1];
    z = arrNewCoordinates[2];
    std::vector<int> servos = returnLegsServos(leg);
    std::vector<double> result;
    result.reserve(3);
    result[0] = 1000;
    result[1] = 1000;
    result[2] = 1000;
    double bigDiam = std::sqrt(((double) (x * x + y * y + z * z)));
    if (bigDiam <= (mesObj->bigLegLength)) {
        double omega = find_first_angle_servo(x, y);
        if (posObj->checkAngle((unsigned char) servos[0], (int) omega) > 0) {
            std::vector<double> arr = find_x0_y0(x, y);
            if (arr[0] < 1000) {
                bool x_negative = false;
                if (x < arr[0]) { x_negative = true; }
                double new_x = std::sqrt((((x - arr[0]) * (x - arr[0])) + ((y - arr[1]) * (y - arr[1]))));
                if (x_negative) { new_x *= -1; }
                std::vector<double> sols = compute_coordinates2D((int) new_x, z, leg);
                if (sols[0] < 100000) {
                    result[0] = omega;
                    result[1] = sols[0];
                    result[2] = sols[1];
                }
            } else {
                return result;
            }
        } else {
            return result;
        }
    } else {
        return result;
    }
    return result;
}

// A method to synchronised moving one or several legs to given 3D coordinates.
bool Coordinates::set_coordinates3D_sync(int *x_arr, int *y_arr, int *z_arr, int *leg_arr, int amount) {
    int final_angles[amount * 3];
    unsigned char final_servos[amount * 3];
    int j = 0;

    for (int i = 0; i < amount; ++i) {

        int x_f = x_arr[i];
        int y_f = y_arr[i];
        int z_f = z_arr[i];
        int leg = leg_arr[i];
        std::vector<int> arrNewCoordinates = transform_coordinates3D(x_f, y_f, z_f, leg);
        int x = arrNewCoordinates[0];
        int y = arrNewCoordinates[1];
        int z = arrNewCoordinates[2];
        std::vector<int> servos = returnLegsServos(leg);
        double bigDiam = std::sqrt(((double) (x * x + y * y + z * z)));
        if (bigDiam <= (mesObj->bigLegLength)) {
            double omega = find_first_angle_servo(x, y);
            if (posObj->checkAngle((unsigned char) servos[0], (int) omega) > 0) {
                std::vector<double> arr = find_x0_y0(x, y);
                if (arr[0] < 1000) {
                    bool x_negative = false;
                    if (x < arr[0]) { x_negative = true; }
                    double new_x = std::sqrt((((x - arr[0]) * (x - arr[0])) + ((y - arr[1]) * (y - arr[1]))));
                    if (x_negative) { new_x *= -1; }
                    std::vector<double> sols = compute_coordinates2D((int) new_x, z, leg);
                    if (sols[0] < 100000) {
                        final_servos[j] = (unsigned char) (servos[0]);
                        final_angles[j] = (int) omega;

                        final_servos[j + 1] = (unsigned char) (servos[1]);
                        final_angles[j + 1] = (int) sols[0];

                        final_servos[j + 2] = (unsigned char) (servos[2]);
                        final_angles[j + 2] = (int) sols[1];
                        j = j + 3;
                    }
                } else {
                    return false;
                }
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
    posObj->set_angles_syncArray(final_servos, final_angles, j);
    return true;
}

// The most used (~important) method in gaits description.
// Taking an array of special objects (see LegPoint class) and synchonized moving of 1 or several legs among given 3D points
bool Coordinates::followPoints(LegPoint *legPts, int numb_of_legs) {
    int firstX[numb_of_legs];
    int firstY[numb_of_legs];
    int firstZ[numb_of_legs];
    int firstLegs[numb_of_legs];
    int actualX[numb_of_legs];
    int actualY[numb_of_legs];
    int actualZ[numb_of_legs];
    int actualLegs[numb_of_legs];

    for (int i = 0; i < numb_of_legs; ++i) {
        if (legPts[i].x_dots.size() == 0) {
            std::cerr << "One of the pairs [array, leg] is bad " << std::endl;
            return false;
        } else {
            firstX[i] = legPts[i].x_dots[0];
            firstY[i] = legPts[i].y_dots[0];
            firstZ[i] = legPts[i].z_dots[0];
            firstLegs[i] = legPts[i].legNumber;
        }
    }
    set_coordinates3D_sync(firstX, firstY, firstZ, firstLegs, numb_of_legs);
//    usleep(1000000);
//  Here should be a delay = time from current point to the first. BUT we have no actuator feedback, though we can't compute this angle
    int maxMovements = 0;
    for (int j = 0; j < numb_of_legs; ++j) {
        if (legPts[j].x_dots.size() > maxMovements) {
            maxMovements = (int) legPts[j].x_dots.size();
        }
    }
    if (maxMovements > 1) {
        for (int i = 1; i < maxMovements; ++i) {

            double max_angle_shift = 0.0;

            for (int j = 0; j < numb_of_legs; ++j) {
                if (i >= legPts[j].x_dots.size()) {
                    actualX[j] = actualX[j];
                    actualY[j] = actualY[j];
                    actualZ[j] = actualZ[j];
                    actualLegs[j] = legPts[j].legNumber;
                } else {
                    std::vector<double> angleBefore = compute_anglesFor3D(legPts[j].x_dots[i - 1],
                                                                          legPts[j].y_dots[i - 1],
                                                                          legPts[j].z_dots[i - 1],
                                                                          legPts[j].legNumber);

                    std::vector<double> angleAfter = compute_anglesFor3D(legPts[j].x_dots[i],
                                                                         legPts[j].y_dots[i],
                                                                         legPts[j].z_dots[i],
                                                                         legPts[j].legNumber);

                    if (angleAfter[0] == 1000 or angleAfter[1] == 1000 or angleAfter[2] == 1000) {
                        std::cerr << "One of the points is wrong(follow_pts)" << std::endl;
                        std::cerr << "i: " << i << std::endl;
                        std::cerr << "Angle After X: " << legPts[j].x_dots[i] << std::endl;
                        std::cerr << "Angle After Y: " << legPts[j].y_dots[i] << std::endl;
                        std::cerr << "Angle After Z: " << legPts[j].z_dots[i] << std::endl;
                        return false;
                    }

                    double max_current_angel = std::max(std::max(std::abs(angleBefore[0] - angleAfter[0]),
                                                                 std::abs(angleBefore[1] - angleAfter[1])),
                                                        std::abs(angleBefore[2] - angleAfter[2]));

                    if (max_current_angel > max_angle_shift) {
                        max_angle_shift = max_current_angel;
                    }
                    actualX[j] = legPts[j].x_dots[i];
                    actualY[j] = legPts[j].y_dots[i];
                    actualZ[j] = legPts[j].z_dots[i];
                    actualLegs[j] = legPts[j].legNumber;
                }

            }
            double delay = (posObj->timeForAngle(int(max_angle_shift))) * 1000000;
//            std::cerr << delay << std::endl;
            usleep((useconds_t) delay);
            set_coordinates3D_sync(actualX, actualY, actualZ, actualLegs, numb_of_legs);
        }
    } else {
        return false;
    }
    return true;
}

// Method to return points of line, specified by two 3D points (6 coordinates), split parameter and number of leg
LegPoint Coordinates::line_points_leg(int x1, int y1, int z1, int x2, int y2, int z2, int split, int leg) {
    // Change this! argument split is useless now
    split = mesObj->split;
    double line_len = std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
    int count = (int) (float(line_len) / split);
//    std::cerr << count << std::endl;
    if (count == 0) {
        std::cerr << "line_points: No line exist!(count == 0)" << std::endl;
        std::vector<int> x_res(0);
        std::vector<int> y_res(0);
        std::vector<int> z_res(0);
        LegPoint result(x_res, y_res, z_res, leg);
        return result;
    } else {
        std::vector<int> x_res((unsigned long) count + 1);
        std::vector<int> y_res((unsigned long) count + 1);
        std::vector<int> z_res((unsigned long) count + 1);
        for (int i = 0; i < count + 1; ++i) {
            double k_current = i / float(count);
            x_res[i] = (int) ((x2 - x1) * k_current + x1);
            y_res[i] = (int) ((y2 - y1) * k_current + y1);
            z_res[i] = (int) ((z2 - z1) * k_current + z1);
        }
        LegPoint result(x_res, y_res, z_res, leg);
        return result;
    }
}

// To return points of bow, specified by two 3D points (6 coordinates), high of the bow, split parameter and leg #
LegPoint Coordinates::ellipse_points_leg(int x1, int y1, int z1, int x2, int y2, int z2, int h, int split, int leg) {
    // Change this! argument split is useless now
    split = mesObj->split;
    if (std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (y1 - y2) * (y1 - y2)) < 10) {
        std::cerr << "Step is less then 1 sm" << std::endl;
        return line_points_leg(x1, y1, z1, x2, y2, z2, split, leg);
    } else {
        double Rxy = std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
//        std::cerr << "Rxy = " << Rxy << std::endl;

        if (Rxy < 1.0) {
            std::vector<int> x_res(0);
            std::vector<int> y_res(0);
            std::vector<int> z_res(0);
            LegPoint result(x_res, y_res, z_res, leg);
            return result;
        }
        double R = std::sqrt(Rxy * Rxy + (z2 - z1) * (z2 - z1));
        double a = R / 2.0;
        double b = h;
        double length = 2 * ((PI * a * b + (a - b) * (a - b)) / (a + b));
        int n = int(round(length)) + 1;
        double alpha = std::atan(((z2 - z1) / Rxy));
        double gc = (z1 + z2) / 2.0;
        n = int(float(n) / split);
        std::vector<int> x_res((unsigned long) n);
        std::vector<int> y_res((unsigned long) n);
        std::vector<int> z_res((unsigned long) n);
        x_res[0] = x1;
        y_res[0] = y1;
        z_res[0] = z1;
        for (int i = 1; i < n; ++i) {
            double t = float(i) / (n - 1);
            double phi = PI * (1 - t);
            double g0 = std::cos(alpha) * std::cos(phi) * a - std::sin(alpha) * std::sin(phi) * b + (float(Rxy) / 2.0);
            double g1 = std::sin(alpha) * std::cos(phi) * a + std::cos(alpha) * std::sin(phi) * b + gc;
            x_res[i] = (int) (x1 + (x2 - x1) * (g0 / Rxy));
            y_res[i] = (int) (y1 + (y2 - y1) * (g0 / Rxy));
            z_res[i] = (int) (g1);
        }
        for (int j = 0; j < n + 1; ++j) {
            std::vector<double> coord = compute_anglesFor3D(x_res[j], y_res[j], z_res[j], leg);
            if (coord[0] == 1000 or coord[1] == 1000 or coord[2] == 1000) {
                std::vector<int> x_resBad(0);
                std::vector<int> y_resBad(0);
                std::vector<int> z_resBad(0);
                LegPoint result(x_res, y_res, z_res, leg);
                return result;
            }
        }
        LegPoint result(x_res, y_res, z_res, leg);
        return result;
    }
}

// The brother of ellipse_points_leg method, the only difference is that it describes lower semi-ellipse.
//Usage is in implementing of gait with changing COG (like people walk)
LegPoint
Coordinates::ellipse_down_points_leg(int x1, int y1, int z1, int x2, int y2, int z2, int h, int split, int leg) {
    // Change this! argument split is useless now
    split = mesObj->split;
    if (std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (y1 - y2) * (y1 - y2)) < 10) {
        std::cerr << "Step is less then 1 sm" << std::endl;
        return line_points_leg(x1, y1, z1, x2, y2, z2, split, leg);
    } else {
        double Rxy = std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

        if (Rxy < 1.0) {
            std::vector<int> x_res(0);
            std::vector<int> y_res(0);
            std::vector<int> z_res(0);
            LegPoint result(x_res, y_res, z_res, leg);
            return result;
        }
        double R = std::sqrt(Rxy * Rxy + (z2 - z1) * (z2 - z1));
        double a = R / 2.0;
        double b = h;
        double length = 2 * ((PI * a * b + (a - b) * (a - b)) / (a + b));
        int n = int(round(length)) + 1;
        double alpha = std::atan(((z2 - z1) / Rxy));
        double gc = (z1 + z2) / 2.0;
        n = int(float(n) / split);
        std::vector<int> x_res((unsigned long) n);
        std::vector<int> y_res((unsigned long) n);
        std::vector<int> z_res((unsigned long) n);
        x_res[0] = x1;
        y_res[0] = y1;
        z_res[0] = z1;
        for (int i = 1; i < n; ++i) {
            double t = float(i) / (n - 1);
            double phi = PI * (1 - t);
            double g0 = std::cos(alpha) * std::cos(phi) * a - std::sin(alpha) * std::sin(phi) * b + (float(Rxy) / 2.0);
            double g1 = std::sin(alpha) * std::cos(phi) * a + std::cos(alpha) * std::sin(phi) * b + gc;
            x_res[i] = (int) (x1 + (x2 - x1) * (g0 / Rxy));
            y_res[i] = (int) (y1 + (y2 - y1) * (g0 / Rxy));
            z_res[i] = 2 * z1 - (int) (g1);
        }
        for (int j = 0; j < n + 1; ++j) {
            std::vector<double> coord = compute_anglesFor3D(x_res[j], y_res[j], z_res[j], leg);
            if (coord[0] == 1000 or coord[1] == 1000 or coord[2] == 1000) {
                std::vector<int> x_resBad(0);
                std::vector<int> y_resBad(0);
                std::vector<int> z_resBad(0);
                LegPoint result(x_res, y_res, z_res, leg);
                return result;
            }
        }
        LegPoint result(x_res, y_res, z_res, leg);
        return result;
    }
}