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
    int legTest1_3[] = {1, 3};

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
    int koef;
    int cur_leg, x_shift, y_shift, z_shift;
    double pitch;
    int auto_move;
    int start_speed = mesObj->speed;
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
                std::cerr << "Go down percent? >>> ";
                std::cin >> koef;
                std::cerr << "steps >>> ";
                std::cin >> steps;
                trotObj->trotJumpingUpDown(20, 30, steps, 'u', koef);
                break;

            case '8':
                std::cerr << "steps >>> ";
                std::cin >> steps;
                creepObj->creepWalkUp(120, 30, 0, steps);
                break;


            case '7':
                std::cerr << "steps >>> ";
                std::cin >> steps;
                creepObj->deerWalkUp(100, 30, 0, steps);
                break;

            case 'i':
                posObj->setDefaultSpeeds();
                std::cerr << "Main: Setting to initial position..." << std::endl;
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

            case '1':
                for (int k = 0; k < 100; ++k) {
                    float mx, my, mz;
                    gy85.read_accelerometer(mx, my, mz);
                    pitch = std::atan(std::sqrt(mx * mx + my * my) / mz) * 180.0 / 3.14;
                    pitch = round(pitch);
                    if (pitch > 15) pitch = 15;
                    auto_move = 6 + int(pitch * 2);
                    std::cerr << "Pitch angle is (or more): " << pitch << ", Auto move on: " << auto_move << std::endl;
                    if (pitch >= 15) {
                        trotObj->bodyMoveFromInitial(int(auto_move * 0.5), int(auto_move * 0.7 * 0.5 * (-1)), 0, 225);
                        usleep(500000);

                        posObj->setSpeedToAll(400);
                        creepObj->creepWalkUp(120, 30, 0, 1);
                        posObj->setSpeedToAll(start_speed);

                    } else {
                        trotObj->bodyMoveFromInitial(auto_move, int(auto_move * 0.7 * (-1)), 0, 225);
                        usleep(500000);

                        trotObj->trotUpDownCourseCorrection(20, 30, 2, 'u', 0, false);
                    }
                    usleep(1000000);
                }
                break;


            case 'x':
                std::cerr << "UP Trot Step" << std::endl;
                std::cerr << "steps >>> ";
                std::cin >> steps;
                trotObj->trotUpDownCourseCorrection(20, 30, steps, 'd', 0, false);
                break;

            case 'e':
                std::cerr << "UP Trot Step" << std::endl;
                std::cerr << "steps >>> ";
                std::cin >> steps;
                trotObj->trotUpDownCourseCorrection(20, 30, steps, 'u', -50, false);
                break;

            case 'q':
                std::cerr << "LEFT ON 35" << std::endl;
                std::cerr << "steps >>> ";
                std::cin >> steps;
                trotObj->trotUpDownCourseCorrection(20, 30, steps, 'u', 35, false);
                break;

            case 's':
                std::cerr << "UP Trot Step" << std::endl;
                std::cerr << "steps >>> ";
                std::cin >> steps;
                trotObj->trotUpDownCourseCorrection(20, 30, steps, 'd', 0, true);
                break;

            case '9':
                std::cerr << "Set speed to >>> ";
                std::cin >> steps;
                posObj->setSpeedToAll(steps);
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

            case 'u':
                std::cerr << "UP legs" << std::endl;
                posObj->upLegs(legTest, 2);
                break;

            case 'y':
                std::cerr << "UP legs 1 3 " << std::endl;
                posObj->upLegs(legTest1_3, 2);
                break;

            case '?':
                for (int i = 0; i < 100; ++i) {
                    float heading, mx, my, mz;
                    gy85.read_accelerometer(mx, my, mz);
                    gy85.get_heading(heading);
                    std::cerr << mx << " " << my << " " << mz << "\t\t" << heading << std::endl;
                    usleep(100000);
                }
                break;

            case ']':
                float mx, my, mz;
                gy85.read_accelerometer(mx, my, mz);
                pitch = std::atan(std::sqrt(mx * mx + my * my) / mz) * 180.0 / 3.14;
                pitch = round(pitch);
                if (pitch > 15) pitch = 15;
                auto_move = 6 + int(pitch * 2);
                std::cerr << "Pitch angle is (or more): " << pitch << ", Auto move on: " << auto_move << std::endl;
                trotObj->bodyMoveFromInitial(auto_move, int(auto_move * 0.7 * (-1)), 0, 225);
                break;

            case '[':
                for (int j = 0; j < 1000; ++j) {
                    float mx, my, mz;
                    gy85.read_accelerometer(mx, my, mz);
                    pitch = std::atan(std::sqrt(mx * mx + my * my) / mz) * 180.0 / 3.14;
                    pitch = round(pitch);
                    if (pitch > 20) pitch = 20;
                    auto_move = 6 + int(pitch * 2);
                    std::cerr << "Pitch angle is (or more): " << pitch << ", Auto move on: " << auto_move << std::endl;
                    trotObj->bodyMoveFromInitial(auto_move, int(auto_move * 0.7 * (-1)), 0, 225);
                    usleep(100000);
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

