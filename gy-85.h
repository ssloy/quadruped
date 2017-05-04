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
    void set_magnetometer_offset(float ox, float oy, float oz);
private:
    int file;
    float offset_x;
    float offset_y;
    float offset_z;
};

#endif //__GY_85_H__

