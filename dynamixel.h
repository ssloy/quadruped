#ifndef __DYNAMIXEL_H__
#define __DYNAMIXEL_H__

class Dynamixel {
public:
    static const unsigned char broadcast_id = 254;
    enum CommStatus {
        COMM_TXSUCCESS = 0,
        COMM_RXSUCCESS = 1,
        COMM_TXFAIL    = 2,
        COMM_RXFAIL    = 3,
        COMM_TXERROR   = 4,
        COMM_RXWAITING = 5,
        COMM_RXTIMEOUT = 6,
        COMM_RXCORRUPT = 7
    };

    Dynamixel(int baud_number);
    bool open_serial(const char *serial_device);
    void close_serial();

    unsigned char status_error_byte();

    CommStatus torque_enable(unsigned char id);
    CommStatus torque_disable(unsigned char id);

    CommStatus set_goal_position(unsigned char id, int value);
    CommStatus set_moving_speed(unsigned char id, int value);
    CommStatus set_max_torque(unsigned char id, int value);
    CommStatus get_present_position(unsigned char id, int &position);
    CommStatus get_present_load(unsigned char id, int &load);
    CommStatus is_moving(unsigned char id, unsigned char &moving);

    bool ping(unsigned char id);

    CommStatus change_id(unsigned char old_id, unsigned char new_id);
    CommStatus set_baud_rate(unsigned char id, unsigned char rate);
    CommStatus reset_to_factory_defaults(unsigned char id);

    CommStatus syncwrite_one_word(unsigned char address, const unsigned char *servos, const int *word_array, unsigned char nservos);

    int set_direction(int level);
private:

    CommStatus rx(unsigned char offset, unsigned char toread, int timeout_ms);
    CommStatus read_status_packet();
    CommStatus send_instruction_packet(unsigned char id, unsigned char instruction, const unsigned char *parameters, unsigned char nparams);
    CommStatus send_instruction_read_status(unsigned char id, unsigned char instruction, const unsigned char *parameters, unsigned char nparams);

    CommStatus write_byte(unsigned char id, unsigned char address, unsigned char  value);
    CommStatus write_word(unsigned char id, unsigned char address,           int  value);
    CommStatus read_byte (unsigned char id, unsigned char address, unsigned char &value);
    CommStatus read_word (unsigned char id, unsigned char address,           int &value);


    static const int max_packet_length_ = 256;
#if defined(__unix__)
    int serial_fd_;
#else
    HANDLE serial_fd_;
#endif
    int baud_number_;
    float baud_rate_;
    float byte_transfer_time_ms_;
    bool rx_error_flag_; // used to flush prior a TX after a failed RX
    unsigned char instruction_packet_[max_packet_length_];
    unsigned char      status_packet_[max_packet_length_];
};

#endif //__DYNAMIXEL_H__

