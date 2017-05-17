//
// Created by Nikita Vasilenko on 15/05/2017.
// Used as an user interface of console application
//
#include <iostream>
#include <unistd.h> // usleep
#include <cstdlib> // для exit()
#include "Positions.h"
#include "Coordinates.h"
#include <cmath>
#include "TrotGait.h"
#include "gy-85.h"
#include "CreepGait.h"
#include "BodyPosition.h"

using namespace std;


char prompt_menu_item() {
    char variant;
    std::cerr << ">>> ";
    std::cin >> variant;
    return variant;
}

int main(int argc, char **argv) {
    Positions *posObj = new Positions();
    Coordinates *coorObj = new Coordinates();
    TrotGait *trotObj = new TrotGait();
    CreepGait *creepObj = new CreepGait();
    Measurements *mesObj = new Measurements();

    posObj->openPort();
    int legTest[] = {0, 2};

    int x_test[] = {260};
    int y_test[] = {0};
    int z_test[] = {0};
    int leg_test[] = {2};

//    std::vector<int> prCurPoint =
//    LegPoint testEllipse = coorObj->ellipse_down_points_leg(240, 0, -60, 260, -20, -60, 30, 1, 2);


//    LegPoint test[] = {testEllipse};
//    Initializing IMU device (i2c port)
    const char device_name[] = "/dev/i2c-1";
    GY85 gy85(device_name);
    gy85.set_magnetometer_offset(mesObj->off_x, mesObj->off_y, -.37);
    int steps;
    int cur_leg, x_shift, y_shift, z_shift;

    std::cerr << "Please choose an option\n" << std::endl;
    std::cerr << "i - set to initial position\n"
              << "t - make a trot step\n"
              << "c - make a creep step\n"
              << "q - exit\n" << std::endl;

    while (true) {
        int variant = prompt_menu_item();
        switch (variant) {

            case 'r':
                std::cerr << "X Shift >>> ";
                std::cin >> x_shift;
                std::cerr << "Y Shift >>> ";
                std::cin >> y_shift;
                std::cerr << "Y Shift >>> ";
                std::cin >> z_shift;
                trotObj->bodyMove(x_shift, y_shift, z_shift, 50);
                break;

            case 'j':
//                testEllipse.printLegPoint();
                trotObj->trotJumpingUpDown(20, 30, 1, 'u');
                break;

            case 'i':
                std::cerr << "Setting to initial position..." << std::endl;
                trotObj->trotInitialPosition();
                break;

            case 'p':
                posObj->printServos();
                break;

            case 'm':
//                To calibrate magnetometer
                trotObj->self_calibrate(100);
                break;

            case '/':
//                To calibrate magnetometer
                trotObj->selfTurning(30, 15);
                break;

            case 'c':
                std::cerr << "Current" << std::endl;
                std::cerr << "leg >>> ";
                std::cin >> cur_leg;
                posObj->printCurrentLeg(cur_leg);
                break;

            case 'w':
                std::cerr << "UP Trot Step" << std::endl;
                std::cerr << "steps >>> ";
                std::cin >> steps;
                trotObj->trotUpDownCourseCorrection(20, 30, steps, 'u', 0, true);
                break;

            case '2':
                std::cerr << "UP Trot Step" << std::endl;
                std::cerr << "steps >>> ";
                std::cin >> steps;
                trotObj->trotUpDownCourseCorrection(20, 30, steps, 'u', 0, false);
                break;

            case 'e':
                std::cerr << "UP Trot Step" << std::endl;
                std::cerr << "steps >>> ";
                std::cin >> steps;
                trotObj->trotUpDownCourseCorrection(20, 30, steps, 'u', -50, false);
                break;

            case 'q':
                std::cerr << "UP Trot Step" << std::endl;
                std::cerr << "steps >>> ";
                std::cin >> steps;
                trotObj->trotUpDownCourseCorrection(20, 30, steps, 'u', 50, false);
                break;

            case 's':
                std::cerr << "UP Trot Step" << std::endl;
                std::cerr << "steps >>> ";
                std::cin >> steps;
                trotObj->trotUpDownCourseCorrection(20, 30, steps, 'd', 0, true);
                break;

            case 't':
                trotObj->bodyMove(5, -5, 0, 100);
                break;

            case 'g':
                trotObj->bodyMove(-5, 5, 0, 100);
                break;

            case 'f':
                trotObj->bodyMove(5, 5, 0, 100);
                break;

            case 'h':
                trotObj->bodyMove(-5, -5, 0, 100);
                break;

            case '=':
                trotObj->bodyMove(0, 0, -5, 100);
                break;

            case '-':
                trotObj->bodyMove(0, 0, 5, 100);
                break;

            case ',':
                std::cerr << "Turn left" << std::endl;
                trotObj->selfTurning(1, 15);
                break;

            case '3':
                std::cerr << "Turn left" << std::endl;
                trotObj->selfTurning(24, 10);
                break;

            case '.':
                std::cerr << "Turn right" << std::endl;
                trotObj->selfTurning(1, -15);
                break;

            case '?':
                for (int i = 0; i < 60; ++i) {
                    float heading, mx, my, mz;
                    gy85.read_accelerometer(mx, my, mz);
                    gy85.get_heading(heading);
                    std::cerr << mx << " " << my << " " << mz << "\t\t" << heading << std::endl;
                    usleep(1000000);
                }
                break;

            case '0':
                std::cerr << "Exit..." << std::endl;
                posObj->closePort();
                exit(EXIT_SUCCESS);

            default:
                std::cerr << "Wrong option. Try again..." << std::endl;
        }
    }
}

