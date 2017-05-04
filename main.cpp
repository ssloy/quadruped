#include <iostream>
#include <unistd.h> // usleep

#include "dynamixel.h"
#include "gy-85.h"

void set_direction(int level);

int main(int argc, char** argv) {
    const char device_name[] = "/dev/i2c-1";
    GY85 gy85(device_name);
    gy85.set_magnetometer_offset(.16, -.03, -.37);
#if 1
    while (1) {
        float heading, mx, my, mz;
        gy85.read_magnetometer(mx, my, mz);
        gy85.get_heading(heading);
        std::cerr << mx << " " << my << " " << mz << "\t\t" << heading << std::endl;
        /*
        float heading = -3600, gx=40., gy=40., gz=40.;
        gy85.read_accelerometer(gx, gy, gz);
        std::cerr << "heading (deg): " << heading << "\t acceleration (g) " << gx << " " << gy << " " << gz  << std::endl;
        */
        usleep(100000);
    }
    return 0;
#endif

    Dynamixel dxl(16);
    if (!dxl.open_serial("/dev/ttyS0")) {
        std::cerr << "Can not open serial device" << std::endl;
        return -1;
    }

#if 0
    while (1) {
        std::cerr << dxl.ping(1) << std::endl;
    }
#endif


    int positions[12][30] = {{511, 514, 518, 522, 526, 531,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 531, 530, 529, 527, 524, 522, 519, 516, 514, 512, 511, 511},
                             {682, 685, 689, 692, 695, 697,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 697, 714, 729, 740, 746, 748, 745, 737, 726, 713, 697, 682},
                             {254, 247, 239, 232, 226, 219,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 219, 210, 204, 201, 201, 204, 210, 217, 225, 235, 245, 254},
                             {511, 510, 509, 507, 505, 502, 499, 497, 494, 492, 491, 490, 490, 495, 499, 503, 507, 511,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
                             {682, 697, 713, 726, 737, 745, 748, 746, 740, 729, 714, 697, 697, 695, 692, 689, 685, 682,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
                             {254, 245, 235, 225, 217, 210, 204, 201, 201, 204, 210, 219, 219, 226, 232, 239, 247, 254,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
                             {511, 507, 503, 500, 497, 494,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 494, 494, 495, 496, 498, 501, 503, 505, 507, 509, 510, 511},
                             {682, 678, 674, 670, 665, 661,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 661, 675, 689, 702, 713, 721, 724, 724, 719, 709, 697, 682},
                             {254, 262, 271, 279, 288, 298,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 298, 288, 278, 268, 259, 251, 245, 240, 239, 241, 246, 254},
                             {511, 511, 512, 514, 516, 518, 520, 523, 525, 526, 527, 527, 527, 524, 521, 518, 514, 511,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
                             {682, 697, 709, 719, 724, 724, 721, 713, 702, 689, 675, 661, 661, 665, 670, 674, 678, 682,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
                             {254, 246, 241, 239, 240, 245, 251, 259, 268, 278, 288, 298, 298, 288, 279, 271, 262, 254,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}};

    int speeds[12] = {200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200};
    unsigned char servos[] = {0, 1, 2, 10, 11, 12, 20, 21, 22, 30, 31, 32};

    dxl.syncwrite_one_word(0x20, servos, speeds, 12);

    for (int i=0; i<30; i++) {
        unsigned char buf1[12];
        int buf2[12];
        int size = 0;
        for (int j=0; j<12; j++) {
            if (positions[j][i]<0) continue;
            buf1[size] = servos[j];
            buf2[size] = positions[j][i];
            size++;
        }
        dxl.syncwrite_one_word(0x1E, buf1, buf2, size);
        usleep(30000);
        float heading;
        gy85.get_heading(heading);
        std::cerr <<  heading << std::endl;
    }

    std::cerr << "done" << std::endl;

    dxl.close_serial();
    return -1;
}

