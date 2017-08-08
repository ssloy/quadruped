//#include <iostream>
//#include <unistd.h> // usleep
//
//#include "dynamixel.h"
//#include "gy-85.h"
//
//void set_direction(int level);
//
//int main(int argc, char** argv) {
//    Dynamixel dxl(16);
//    if (!dxl.open_serial("/dev/ttyS0")) {
//        std::cerr << "Can not open serial device" << std::endl;
//        return -1;
//    }
//
//    int positions[12][30] = {{511, 514, 518, 522, 526, 531,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 531, 530, 529, 527, 524, 522, 519, 516, 514, 512, 511, 511},
//                             {682, 685, 689, 692, 695, 697,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 697, 714, 729, 740, 746, 748, 745, 737, 726, 713, 697, 682},
//                             {254, 247, 239, 232, 226, 219,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 219, 210, 204, 201, 201, 204, 210, 217, 225, 235, 245, 254},
//                             {511, 510, 509, 507, 505, 502, 499, 497, 494, 492, 491, 490, 490, 495, 499, 503, 507, 511,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
//                             {682, 697, 713, 726, 737, 745, 748, 746, 740, 729, 714, 697, 697, 695, 692, 689, 685, 682,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
//                             {254, 245, 235, 225, 217, 210, 204, 201, 201, 204, 210, 219, 219, 226, 232, 239, 247, 254,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
//                             {511, 507, 503, 500, 497, 494,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 494, 494, 495, 496, 498, 501, 503, 505, 507, 509, 510, 511},
//                             {682, 678, 674, 670, 665, 661,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 661, 675, 689, 702, 713, 721, 724, 724, 719, 709, 697, 682},
//                             {254, 262, 271, 279, 288, 298,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 298, 288, 278, 268, 259, 251, 245, 240, 239, 241, 246, 254},
//                             {511, 511, 512, 514, 516, 518, 520, 523, 525, 526, 527, 527, 527, 524, 521, 518, 514, 511,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
//                             {682, 697, 709, 719, 724, 724, 721, 713, 702, 689, 675, 661, 661, 665, 670, 674, 678, 682,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
//                             {254, 246, 241, 239, 240, 245, 251, 259, 268, 278, 288, 298, 298, 288, 279, 271, 262, 254,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}};
//
//    int speeds[12] = {200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200};
//    unsigned char servos[] = {0, 1, 2, 10, 11, 12, 20, 21, 22, 30, 31, 32};
//
//    dxl.syncwrite_one_word(0x20, servos, speeds, 12);
//
//    for (int i=0; i<30; i++) {
//        unsigned char buf1[12];
//        int buf2[12];
//        int size = 0;
//        for (int j=0; j<12; j++) {
//            if (positions[j][i]<0) continue;
//            buf1[size] = servos[j];
//            buf2[size] = positions[j][i];
//            size++;
//        }
//        dxl.syncwrite_one_word(0x1E, buf1, buf2, size);
//        usleep(30000);
//    }
//
//    std::cerr << "done4" << std::endl;
//
//    dxl.close_serial();
//    return -1;
//}
//#if 0
//    const char device_name[] = "/dev/i2c-1";
//    GY85 gy85(device_name);
//    gy85.set_magnetometer_offset(.16, -.03, -.37);
//    while (1) {
//        float heading, mx, my, mz;
//        gy85.read_magnetometer(mx, my, mz);
//        gy85.get_heading(heading);
//        std::cerr << mx << " " << my << " " << mz << "\t\t" << heading << std::endl;
//        usleep(100000);
//    }
//    return 0;
//#endif


//bool TrotGait::trotNaturalWalkUpDown(int step_length, int step_high, int step_amount, char u_or_d, int turn_shift) {
//    int direct_k;
////    Walking UP
//    if (u_or_d == 'u') {
//        direct_k = 1;
//    } else if (u_or_d == 'd') {
////   Waling down
//        direct_k = -1;
//    } else
////    Error in direction
//    {
//        std::cerr << "Wrong direction!" << std::endl;
//        return false;
//    }
//    magnetometer.set_magnetometer_offset(.16, -.03, -.37);
//    float start_heading;
//    magnetometer.get_heading(start_heading);
//    //     Change this! argument split is useless now
//    int split = mesObjT->split;
//    std::cerr << "Trot Walk Up Inside" << std::endl;
//    int shift = int(float(step_length) / std::sqrt(2));
//    int shift_correction = int(float(turn_shift * shift) / 100.0);
//    int shift_right =  shift + shift_correction;
//    int shift_left = shift - shift_correction;
//    std::cerr << "Start Head: " << start_heading << std::endl;
//
////    First iteration, preparation for making one or several steps
//    LegPoint line_pts0 = coorObjT->line_points_leg(start_pos0[0], start_pos0[1], start_pos0[2],
//                                                   start_pos0[0] + direct_k * shift_left,
//                                                   start_pos0[1] - direct_k * shift_left,
//                                                   start_pos0[2], split, 0);
//
//    LegPoint pts1 = coorObjT->ellipse_points_leg(start_pos1[0], start_pos1[1], start_pos1[2],
//                                                 start_pos1[0] - direct_k * shift_left,
//                                                 start_pos1[1] + direct_k * shift_left,
//                                                 start_pos1[2], step_high, split, 1);
//
//    LegPoint line_pts2 = coorObjT->line_points_leg(start_pos2[0], start_pos2[1], start_pos2[2],
//                                                   start_pos2[0] + direct_k * shift_right,
//                                                   start_pos2[1] - direct_k * shift_right,
//                                                   start_pos2[2], split, 2);
//
//    LegPoint pts3 = coorObjT->ellipse_points_leg(start_pos3[0], start_pos3[1], start_pos3[2],
//                                                 start_pos3[0] - direct_k * shift_right,
//                                                 start_pos3[1] + direct_k * shift_right,
//                                                 start_pos3[2], step_high, split, 3);
//
//    LegPoint pts_iteration_1[] = {line_pts0, pts1, line_pts2, pts3};
//    coorObjT->followPoints(pts_iteration_1, 4);
//
////    Making one or several steps, depending on given parameter.
//    for (int i = 0; i < step_amount; ++i) {
//        float current_head;
//        magnetometer.get_heading(current_head);
//        std::cerr << "Current: " << current_head << std::endl;
//        if ((current_head - start_heading) > 5) {
//            std::cerr << "CHANGE ->: " << std::endl;
//
//        } else if ((current_head - start_heading) < -5) {
//            std::cerr << "CHANGE <- " << std::endl;
//
//        } else {
//            std::cerr << "NO CHANGE: " << std::endl;
//
//        }
//
//        //   0 and 2 moving forward
//        LegPoint pts0 = coorObjT->ellipse_points_leg(start_pos0[0] + direct_k * shift_left,
//                                                     start_pos0[1] - direct_k * shift_left,
//                                                     start_pos0[2],
//                                                     start_pos0[0] - direct_k * shift_left,
//                                                     start_pos0[1] + direct_k * shift_left,
//                                                     start_pos0[2],
//                                                     step_high, split, 0);
//
//        LegPoint pts2 = coorObjT->ellipse_points_leg(start_pos2[0] + direct_k * shift_right,
//                                                     start_pos2[1] - direct_k * shift_right,
//                                                     start_pos2[2],
//                                                     start_pos2[0] - direct_k * shift_right,
//                                                     start_pos2[1] + direct_k * shift_right,
//                                                     start_pos2[2],
//                                                     step_high, split, 2);
//
//        LegPoint line_pts1 = coorObjT->line_points_leg(start_pos1[0] - direct_k * shift_left,
//                                                       start_pos1[1] + direct_k * shift_left,
//                                                       start_pos1[2],
//                                                       start_pos1[0] + direct_k * shift_left,
//                                                       start_pos1[1] - direct_k * shift_left,
//                                                       start_pos1[2],
//                                                       split, 1);
//
//        LegPoint line_pts3 = coorObjT->line_points_leg(start_pos3[0] - direct_k * shift_right,
//                                                       start_pos3[1] + direct_k * shift_right,
//                                                       start_pos3[2],
//                                                       start_pos3[0] + direct_k * shift_right,
//                                                       start_pos3[1] - direct_k * shift_right,
//                                                       start_pos3[2],
//                                                       split, 3);
//
//        LegPoint pts_iteration_2[] = {pts0, line_pts1, pts2, line_pts3};
//        coorObjT->followPoints(pts_iteration_2, 4);
//
//        //   1 and 3 leg moving forward
//
//        line_pts0 = coorObjT->line_points_leg(start_pos0[0] - direct_k * shift_left,
//                                              start_pos0[1] + direct_k * shift_left,
//                                              start_pos0[2],
//                                              start_pos0[0] + direct_k * shift_left,
//                                              start_pos0[1] - direct_k * shift_left,
//                                              start_pos0[2],
//                                              split, 0);
//
//        line_pts2 = coorObjT->line_points_leg(start_pos2[0] - direct_k * shift_right,
//                                              start_pos2[1] + direct_k * shift_right,
//                                              start_pos2[2],
//                                              start_pos2[0] + direct_k * shift_right,
//                                              start_pos2[1] - direct_k * shift_right,
//                                              start_pos2[2],
//                                              split, 2);
//
//        pts1 = coorObjT->ellipse_points_leg(start_pos1[0] + direct_k * shift_left,
//                                            start_pos1[1] - direct_k * shift_left,
//                                            start_pos1[2],
//                                            start_pos1[0] - direct_k * shift_left,
//                                            start_pos1[1] + direct_k * shift_left,
//                                            start_pos1[2],
//                                            step_high, split, 1);
//
//        pts3 = coorObjT->ellipse_points_leg(start_pos3[0] + direct_k * shift_right,
//                                            start_pos3[1] - direct_k * shift_right,
//                                            start_pos3[2],
//                                            start_pos3[0] - direct_k * shift_right,
//                                            start_pos3[1] + direct_k * shift_right,
//                                            start_pos3[2],
//                                            step_high, split, 3);
//
//        LegPoint pts_iteration_3[] = {pts1, line_pts0, pts3, line_pts2};
//        coorObjT->followPoints(pts_iteration_3, 4);
//    }
//
//
//    //    Last iteration, returning to standart position
//
//    LegPoint pts0 = coorObjT->ellipse_points_leg(start_pos0[0] + direct_k * shift_left,
//                                                 start_pos0[1] - direct_k * shift_left,
//                                                 start_pos0[2],
//                                                 start_pos0[0],
//                                                 start_pos0[1],
//                                                 start_pos0[2],
//                                                 step_high, split, 0);
//
//    LegPoint pts2 = coorObjT->ellipse_points_leg(start_pos2[0] + direct_k * shift_right,
//                                                 start_pos2[1] - direct_k * shift_right,
//                                                 start_pos2[2],
//                                                 start_pos2[0],
//                                                 start_pos2[1],
//                                                 start_pos2[2],
//                                                 step_high, split, 2);
//
//    LegPoint line_pts1 = coorObjT->line_points_leg(start_pos1[0] - direct_k * shift_left,
//                                                   start_pos1[1] + direct_k * shift_left,
//                                                   start_pos1[2],
//                                                   start_pos1[0],
//                                                   start_pos1[1],
//                                                   start_pos1[2],
//                                                   split, 1);
//
//    LegPoint line_pts3 = coorObjT->line_points_leg(start_pos3[0] - direct_k * shift_right,
//                                                   start_pos3[1] + direct_k * shift_right,
//                                                   start_pos3[2],
//                                                   start_pos3[0],
//                                                   start_pos3[1],
//                                                   start_pos3[2],
//                                                   split, 3);
//
//    LegPoint pts_iteration_4[] = {pts0, line_pts1, pts2, line_pts3};
//    coorObjT->followPoints(pts_iteration_4, 4);
//    return true;
//}


//First attempts to make trot walk, with correction for balance. BUT not symmetric

//bool TrotGait::trotWalkUpDown(int step_length, int step_high, char u_or_d) {
//    int direct_k;
////    Walking UP
//    if (u_or_d == 'u') {
//        direct_k = 1;
//    } else if (u_or_d == 'd') {
////   Waling down
//        direct_k = -1;
//    } else
////    Error in direction
//    {
//        std::cerr << "Wrong direction!" << std::endl;
//        return false;
//    }
//
//    //     Change this! argument split is useless now
//    int split = mesObjT->split;
//    std::cerr << "Trot Walk Up Inside" << std::endl;
//    double shift = float(step_length) / std::sqrt(2);
////    std::cerr << "Shift: " << shift << std::endl;
//
//
////    First iteration, LEG #0
//    LegPoint line_pts0 = coorObjT->line_points_leg(start_pos0[0], start_pos0[1], start_pos0[2],
//                                                   (int) (start_pos0[0] + direct_k * shift),
//                                                   (int) (start_pos0[1] - direct_k * shift),
//                                                   start_pos0[2], split, 0);
////    LEG #1
//    LegPoint pts1 = coorObjT->ellipse_points_leg(start_pos1[0], start_pos1[1], start_pos1[2],
//                                                 (int) (start_pos1[0] - direct_k * shift),
//                                                 (int) (start_pos1[1] + direct_k * shift),
//                                                 start_pos1[2], step_high, split, 1);
////    Leg #2
//    LegPoint line_pts2 = coorObjT->line_points_leg(start_pos2[0], start_pos2[1], start_pos2[2],
//                                                   (int) (start_pos2[0] + direct_k * shift),
//                                                   (int) (start_pos2[1] - direct_k * shift),
//                                                   start_pos2[2], split, 2);
////    Leg #3
//    LegPoint pts3 = coorObjT->ellipse_points_leg(start_pos3[0], start_pos3[1], start_pos3[2],
//                                                 (int) (start_pos3[0] - direct_k * shift),
//                                                 (int) (start_pos3[1] + direct_k * shift),
//                                                 start_pos3[2], step_high, split, 3);
//
//    LegPoint pts_iteration_1[] = {line_pts0, pts1, line_pts2, pts3};
//
//    coorObjT->followPoints(pts_iteration_1, 4);
//
////    Second iteration, LEG #1
//    int cur_pts1[] = {(int) (start_pos1[0] - direct_k * shift), (int) (start_pos1[1] + direct_k * shift),
//                      start_pos1[2]};
//
//    LegPoint line_pts1 = coorObjT->line_points_leg(cur_pts1[0], cur_pts1[1], cur_pts1[2], start_pos1[0], start_pos1[1],
//                                                   start_pos1[2], split, 1);
//    int cur_pts3[] = {(int) (start_pos3[0] - direct_k * shift), (int) (start_pos3[1] + direct_k * shift),
//                      start_pos3[2]};
//
////    Leg #3
//    LegPoint line_pts3 = coorObjT->line_points_leg(cur_pts3[0], cur_pts3[1], cur_pts3[2], start_pos3[0],
//                                                   start_pos3[1], start_pos3[2], split, 3);
//
//    LegPoint pts_iteration_2[] = {line_pts1, line_pts3};
//
//    coorObjT->followPoints(pts_iteration_2, 2);
//
////    Third iteration, Leg #0
//    LegPoint pts0 = coorObjT->ellipse_points_leg((int) (start_pos0[0] + direct_k * shift),
//                                                 (int) (start_pos0[1] - direct_k * shift),
//                                                 start_pos0[2],
//                                                 start_pos0[0], start_pos0[1], start_pos0[2], step_high, split, 0);
//
////    Leg #2
//    LegPoint pts2 = coorObjT->ellipse_points_leg((int) (start_pos2[0] + direct_k * shift),
//                                                 (int) (start_pos2[1] - direct_k * shift),
//                                                 start_pos2[2], start_pos2[0], start_pos2[1], start_pos2[2], step_high,
//                                                 split, 2);
//
//    LegPoint pts_iteration_3[] = {pts0, pts2};
//    coorObjT->followPoints(pts_iteration_3, 2);
//
//    return true;
//}
//
//
//bool TrotGait::trotWalkRightLeft(int step_length, int step_high, char r_or_l) {
//    int direct_k;
////    Walking UP
//    if (r_or_l == 'r') {
//        direct_k = 1;
//    } else if (r_or_l == 'l') {
////   Waling down
//        direct_k = -1;
//    } else
////    Error in direction
//    {
//        std::cerr << "Wrong direction!" << std::endl;
//        return false;
//    }
////     Change this! argument split is useless now
//    int split = mesObjT->split;
//    std::cerr << "Trot Walk Right Inside" << std::endl;
//    double shift = float(step_length) / std::sqrt(2);
////    std::cerr << "Shift: " << shift << std::endl;
//
//
////    First iteration, LEG #0
//    LegPoint pts0 = coorObjT->ellipse_points_leg(start_pos0[0], start_pos0[1], start_pos0[2],
//                                                 (int) (start_pos0[0] + direct_k * shift),
//                                                 (int) (start_pos0[1] + direct_k * shift),
//                                                 start_pos0[2], step_high, split, 0);
////    LEG #1
//    LegPoint line_pts1 = coorObjT->line_points_leg(start_pos1[0], start_pos1[1], start_pos1[2],
//                                                   (int) (start_pos1[0] - direct_k * shift),
//                                                   (int) (start_pos1[1] - direct_k * shift),
//                                                   start_pos1[2], split, 1);
////    Leg #2
//    LegPoint pts2 = coorObjT->ellipse_points_leg(start_pos2[0], start_pos2[1], start_pos2[2],
//                                                 (int) (start_pos2[0] + direct_k * shift),
//                                                 (int) (start_pos2[1] + direct_k * shift),
//                                                 start_pos2[2], step_high, split, 2);
////    Leg #3
//    LegPoint line_pts3 = coorObjT->line_points_leg(start_pos3[0], start_pos3[1], start_pos3[2],
//                                                   (int) (start_pos3[0] - direct_k * shift),
//                                                   (int) (start_pos3[1] - direct_k * shift),
//                                                   start_pos3[2], split, 3);
//
//    LegPoint pts_iteration_1[] = {pts0, line_pts1, pts2, line_pts3};
//
//    coorObjT->followPoints(pts_iteration_1, 4);
//
////    Second iteration, LEG #0
//    int cur_pts0[] = {(int) (start_pos0[0] + direct_k * shift), (int) (start_pos0[1] + direct_k * shift),
//                      start_pos0[2]};
//
//    LegPoint line_pts0 = coorObjT->line_points_leg(cur_pts0[0], cur_pts0[1], cur_pts0[2], start_pos0[0], start_pos0[1],
//                                                   start_pos0[2], split, 0
//    );
////    Leg #2
//
//    int cur_pts2[] = {(int) (start_pos2[0] + direct_k * shift), (int) (start_pos2[1] + direct_k * shift),
//                      start_pos2[2]};
//
//    LegPoint line_pts2 = coorObjT->line_points_leg(cur_pts2[0], cur_pts2[1], cur_pts2[2], start_pos2[0],
//                                                   start_pos2[1], start_pos2[2], split, 2);
//
//    LegPoint pts_iteration_2[] = {line_pts0, line_pts2};
//
//    coorObjT->followPoints(pts_iteration_2, 2);
//
////    Third iteration, Leg #1
//    LegPoint pts1 = coorObjT->ellipse_points_leg((int) (start_pos1[0] - direct_k * shift),
//                                                 (int) (start_pos1[1] - direct_k * shift),
//                                                 start_pos1[2],
//                                                 start_pos1[0], start_pos1[1], start_pos1[2], step_high, split, 1);
//
////    Leg #3
//    LegPoint pts3 = coorObjT->ellipse_points_leg((int) (start_pos3[0] - direct_k * shift),
//                                                 (int) (start_pos3[1] - direct_k * shift),
//                                                 start_pos3[2], start_pos3[0], start_pos3[1], start_pos3[2], step_high,
//                                                 split, 3);
//
//    LegPoint pts_iteration_3[] = {pts1, pts3};
//    coorObjT->followPoints(pts_iteration_3, 2);
//    return true;
//}


//bool BodyPosition::bodyMoveUp(int shift_length) {
//    std::vector<int> current_leg0 = computeCurrentPoint(0);
//    std::vector<int> current_leg1 = computeCurrentPoint(1);
//    std::vector<int> current_leg2 = computeCurrentPoint(2);
//    std::vector<int> current_leg3 = computeCurrentPoint(3);
//
//    LegPoint line_pts0 = coorObjB->line_points_leg(current_leg0[0], current_leg0[1], current_leg0[2],
//                                                   current_leg0[0] + shift_length,
//                                                   current_leg0[1] - shift_length,
//                                                   current_leg0[2],
//                                                   split, 0);
//
//    LegPoint pts_iteration_1[] = {line_pts0};
//    coorObjB->followPoints(pts_iteration_1, 1);
//    return true;
//}
