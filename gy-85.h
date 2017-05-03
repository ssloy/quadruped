#ifndef __GY_85_H__
#define __GY_85_H__

const int ADXL345 = 0x53; //address of Accelerometer
const int HMC5883 = 0x1E; //address of Magnetometer
const int ITG3200 = 0x68; //address of Gyro

class GY85 {
public:
    GY85(const char *i2c_bus);
    ~GY85();
    bool get_heading(float &h);
    bool read_magnetometer(float &x, float &y, float &z);   // returns Ga
    bool read_accelerometer(float &x, float &y, float &z);  // returns g
private:
    int file;
};

#endif //__GY_85_H__

