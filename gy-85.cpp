#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cmath>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include "gy-85.h"

static inline __s32 i2c_smbus_access(int file, char read_write, __u8 command, int size, union i2c_smbus_data *data) {
    struct i2c_smbus_ioctl_data args;

    args.read_write = read_write;
    args.command = command;
    args.size = size;
    args.data = data;
    return ioctl(file, I2C_SMBUS, &args);
}

static inline __s32 i2c_smbus_write_byte_data(int file, __u8 command, __u8 value) {
    union i2c_smbus_data data;
    data.byte = value;
    return i2c_smbus_access(file, I2C_SMBUS_WRITE, command, I2C_SMBUS_BYTE_DATA, &data);
}


/*
static inline __s32 i2c_smbus_read_word_data(int file, __u8 command) {
    union i2c_smbus_data data;
    if (i2c_smbus_access(file, I2C_SMBUS_READ, command, I2C_SMBUS_WORD_DATA, &data)) {
        return -1;
    } else {
        return 0x0FFFF & data.word;
    }
}
*/

static inline __s32 i2c_smbus_read_byte_data(int file, __u8 command) {
    union i2c_smbus_data data;
    if (i2c_smbus_access(file, I2C_SMBUS_READ, command, I2C_SMBUS_BYTE_DATA, &data)) {
        return -1;
    } else {
        return 0xFF & data.byte;
    }
}

static inline __s32 i2c_smbus_read_i2c_block_data(int file, __u8 command, __u8 length, __u8 *values) {
    union i2c_smbus_data data;

    if (length > 32) {
        length = 32;
    }

    data.block[0] = length;
    if (i2c_smbus_access(file, I2C_SMBUS_READ, command, length == 32 ? I2C_SMBUS_I2C_BLOCK_BROKEN : I2C_SMBUS_I2C_BLOCK_DATA , &data)) {
        return -1;
    } else {
        for (int i = 1; i <= data.block[0]; i++) {
            values[i - 1] = data.block[i];
        }
        return data.block[0];
    }
}

GY85::GY85(const char *i2c_bus) : file(-1), offset_x(0), offset_y(0), offset_z(0) {
    file = open(i2c_bus, O_RDWR);
    if (file<0) {
        std::cerr << "Can not open device " << i2c_bus << std::endl;
        return;
    }

    // init the magnetometer
    if (ioctl(file, I2C_SLAVE, HMC5883) < 0) {
        std::cerr << "Can not set device address " << HMC5883 << std::endl;
        return;
    }
    if (i2c_smbus_write_byte_data(file, 0x02, 0) < 0) {
        std::cerr << "i2c write failed" << std::endl; // mode register, continuous measurement mode
    }
    if (i2c_smbus_write_byte_data(file, 0x01, 0x20) < 0) {
        std::cerr << "i2c write failed" << std::endl; // configuration register B, set range to +/-1.3Ga (gain 1090)
    }

    // init the accelerometer
    if (ioctl(file, I2C_SLAVE, ADXL345) < 0) {
        std::cerr << "Can not set device address " << ADXL345 << std::endl;
        return;
    }
    if (i2c_smbus_write_byte_data(file, 0x2d, 0x08) < 0) { // power up and enable measurements
        std::cerr << "i2c write failed" << std::endl;
    }
    if (i2c_smbus_write_byte_data(file, 0x31, 0x00) < 0) { // set range to +/-2g
        std::cerr << "i2c write failed" << std::endl;
    }
}

GY85::~GY85() {
    if (file>=0) close(file);
}

void GY85::set_magnetometer_offset(float ox, float oy, float oz) {
    offset_x = ox;
    offset_y = oy;
    offset_z = oz;
}

bool GY85::get_heading(float &heading) {
    float x, y, z;
    bool res = read_magnetometer(x, y, z);
    if (!res) return false;

    heading = atan2(y, x);
    if (heading < 0     ) heading += 2*M_PI;
    if (heading > 2*M_PI) heading -= 2*M_PI;
    heading = heading*180/M_PI;
    return true;
}

bool GY85::read_magnetometer(float &x, float &y, float &z) {
    if (ioctl(file, I2C_SLAVE, HMC5883) < 0) {
        std::cerr << "Can not set device address " << HMC5883 << std::endl;
        return false;
    }

    __u8 buf[6] = {0};
    i2c_smbus_read_i2c_block_data(file, 0x03, 6, buf);

    unsigned short ux = ((unsigned short)buf[0] << 8) | ((unsigned short)buf[1]);
    unsigned short uz = ((unsigned short)buf[2] << 8) | ((unsigned short)buf[3]);
    unsigned short uy = ((unsigned short)buf[4] << 8) | ((unsigned short)buf[5]);

    short xraw = *(short *)(void *)&ux;
    short yraw = *(short *)(void *)&uy;
    short zraw = *(short *)(void *)&uz;

    x = float(xraw)/1090. - offset_x;
    y = float(yraw)/1090. - offset_y;
    z = float(zraw)/1090. - offset_z;

    return true;
}

bool GY85::read_accelerometer(float &x, float &y, float &z) {
    if (ioctl(file, I2C_SLAVE, ADXL345) < 0) {
        std::cerr << "Can not set device address " << ADXL345 << std::endl;
        return false;
    }
    __u8 buf[6] = {0};
    i2c_smbus_read_i2c_block_data(file, 0x32, 6, buf);
    unsigned short ux = ((unsigned short)buf[1] << 8) | ((unsigned short)buf[0]);
    unsigned short uy = ((unsigned short)buf[3] << 8) | ((unsigned short)buf[2]);
    unsigned short uz = ((unsigned short)buf[5] << 8) | ((unsigned short)buf[4]);

    short xraw = *(short *)(void *)&ux;
    short yraw = *(short *)(void *)&uy;
    short zraw = *(short *)(void *)&uz;

    x = xraw/256.; // +/- 2G range for 10 bit resolution
    y = yraw/256.;
    z = zraw/256.;

    return true;
}

