#include <iostream>
#include <cstring>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <errno.h>
#include <cstdlib>
#include <ctime>

#include "dynamixel.h"


#if defined(TRIK_GPIO1_11_DIR)

#include <sys/mman.h>
#define GPIO_BASE_ADDR 0x01E26000

volatile unsigned long *gpio_base_address;

int gpio_setup() {
    int  m_mfd;
    if ((m_mfd = open("/dev/mem", O_RDWR)) < 0) {
        return -1;
    }
    gpio_base_address = (unsigned long*)mmap(NULL, getpagesize(), PROT_READ|PROT_WRITE, MAP_SHARED, m_mfd, GPIO_BASE_ADDR);
    close(m_mfd);

    if (gpio_base_address == MAP_FAILED) {
        return -2;
    }
    *(((unsigned char *)(void *)gpio_base_address) + 0x10 + 3) &= 0xF7;
    return 0;
}

void set_direction(int level) {
/*    if (level) {
        *(((unsigned char *)(void *)gpio_base_address) + 0x18 + 3) |= 8;
    } else {
        *(((unsigned char *)(void *)gpio_base_address) + 0x1C + 3) |= 8;
    }
*/
    if (level) {
        *(((unsigned char *)(void *)gpio_base_address) + 0x14 + 3) |= 0x08;
    } else {
        *(((unsigned char *)(void *)gpio_base_address) + 0x14 + 3) &= 0xF7;
    }
}

#endif


inline unsigned char high_byte(int v) { return (v & 0xFF00) >> 8; }
inline unsigned char  low_byte(int v) { return  v & 0xFF; }

Dynamixel::Dynamixel(int baud_number) : serial_fd_(-1), baud_number_(baud_number), baud_rate_(), byte_transfer_time_ms_(), rx_error_flag_(true) {
    baud_rate_ = 2000000.0f/(float)(baud_number_+1);
    byte_transfer_time_ms_ = (float)((1000.0f / baud_rate_) * 10.0f);
}

bool Dynamixel::open_serial(const char *serial_device) {
#if defined(__unix__)
    if ((serial_fd_ = open(serial_device, O_RDWR|O_NOCTTY|O_NONBLOCK)) < 0) {
        return false;
    }
    tcflag_t c_cflag = B1000000;
    switch (baud_number_) {
//        case 1:   c_cflag = B921600; break;
        case 1:   c_cflag = B1000000; break;
        case 3:   c_cflag = B500000;  break;
//        case 4:   c_cflag = B400000;  break;
//        case 7:   c_cflag = B250000;  break;
//        case 9:   c_cflag = B200000;  break;
        case 16:  c_cflag = B115200;  break;
        case 34:  c_cflag = B57600;   break;
        case 103: c_cflag = B19200;   break;
        case 207: c_cflag = B9600;    break;
        default: {
            std::cerr << "Bad baud number" << std::endl;
            return false;
        }
    }

    termios tty_opt;
    memset(&tty_opt, 0, sizeof(tty_opt));
    tty_opt.c_cflag       = c_cflag|CS8|CLOCAL|CREAD;
    tty_opt.c_iflag       = IGNPAR;
    tty_opt.c_oflag       = 0;
    tty_opt.c_lflag       = 0;
    tty_opt.c_cc[VTIME]   = 0;
    tty_opt.c_cc[VMIN]    = 0;

    tcflush(serial_fd_, TCIFLUSH);
    tcsetattr(serial_fd_, TCSANOW, &tty_opt);

#if defined(TRIK_GPIO1_11_DIR)
    gpio_setup();
    set_direction(1);
#endif

#else
    // TODO put baud rate accordingly
    serial_fd_ = CreateFileA(serial_device, GENERIC_READ | GENERIC_WRITE,
            0,                          // no share  
            NULL,                       // no security 
            OPEN_EXISTING,
            0,                          // no threads 
            NULL);                      // no templates 
    char mode_str[] = "baud=1000000 data=8 parity=n stop=1";
    if(serial_fd_==INVALID_HANDLE_VALUE) return false;
    DCB port_settings;
    memset(&port_settings, 0, sizeof(port_settings));  // clear the new struct 
    port_settings.DCBlength = sizeof(port_settings);

    COMMTIMEOUTS Cptimeouts;

    Cptimeouts.ReadIntervalTimeout = MAXDWORD;
    Cptimeouts.ReadTotalTimeoutMultiplier = 0;
    Cptimeouts.ReadTotalTimeoutConstant = 0;
    Cptimeouts.WriteTotalTimeoutMultiplier = 0;
    Cptimeouts.WriteTotalTimeoutConstant = 0;

    if (!BuildCommDCBA(mode_str, &port_settings) || !SetCommState(serial_fd_, &port_settings) || !SetCommTimeouts(serial_fd_, &Cptimeouts)) {
        CloseHandle(serial_fd_);
        return false;
    }
#endif
    return true;
}

void Dynamixel::close_serial() {
    if (serial_fd_ != -1) {
#if defined(__unix__)
        close(serial_fd_);
#else
        CloseHandle(serial_fd_);
#endif
        serial_fd_ = -1;
    }
}

unsigned char Dynamixel::status_error_byte() {
    return status_packet_[4];
}

Dynamixel::CommStatus Dynamixel::torque_enable(unsigned char id) {
    return write_byte(id, 0x18, 1);
}

Dynamixel::CommStatus Dynamixel::torque_disable(unsigned char id) {
    return write_byte(id, 0x18, 0);
}

Dynamixel::CommStatus Dynamixel::set_goal_position(unsigned char id, int value) {
    return write_word(id, 0x1E, value);
}

Dynamixel::CommStatus Dynamixel::set_moving_speed(unsigned char id, int value) {
    return write_word(id, 0x20, value);
}

Dynamixel::CommStatus Dynamixel::set_max_torque(unsigned char id, int value) {
    return write_word(id, 0x22, value);
}

Dynamixel::CommStatus Dynamixel::change_id(unsigned char old_id, unsigned char new_id) {
    return write_byte(old_id, 0x03, new_id);
}

Dynamixel::CommStatus Dynamixel::set_baud_rate(unsigned char id, unsigned char rate) {
    return write_byte(id, 0x04, rate);
}

Dynamixel::CommStatus Dynamixel::reset_to_factory_defaults(unsigned char id) {
    return send_instruction_read_status(id, 0x06, NULL, 0);
}

Dynamixel::CommStatus Dynamixel::get_present_position(unsigned char id, int &position) {
    return read_word(id, 0x24, position);
}

Dynamixel::CommStatus Dynamixel::get_present_load(unsigned char id, int &load) {
    Dynamixel::CommStatus res = read_word(id, 0x28, load);
    if (load > 1023) load = 1024 - load;
    return res;
}

Dynamixel::CommStatus Dynamixel::is_moving(unsigned char id, unsigned char &moving) {
    Dynamixel::CommStatus ret = read_byte(id, 0x2E, moving);
    return ret;
}

// ping() returns true if and only if the communication was successful AND the id matches
bool Dynamixel::ping(unsigned char id) {
    Dynamixel::CommStatus ret = send_instruction_read_status(id, 0x01, NULL, 0);
    return ret==COMM_RXSUCCESS && id == status_packet_[2];
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  private functions, normally no need to look below  //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// serial read() wrapper, handles given timeout + custom error messages
Dynamixel::CommStatus Dynamixel::rx(unsigned char offset, unsigned char toread, int timeout_ms) {
    long   start_ms, cur_ms; // TODO ms -> ns
    time_t start_s,  cur_s;
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    start_s  = spec.tv_sec;
    start_ms = spec.tv_nsec/1.0e6;

    int received = 0;
    while (received < toread) {
#if defined(__unix__)
        int n = read(serial_fd_, status_packet_+offset+received, toread-received);
#else
        int n = 0;
        ReadFile(serial_fd_, status_packet_+offset+received, toread-received, (LPDWORD)((void *)&n), NULL);
#endif
        if (n>=0) {
            received += n;
        } else {
            return COMM_RXFAIL;
        }
        clock_gettime(CLOCK_REALTIME, &spec);
        cur_s  = spec.tv_sec;
        cur_ms = spec.tv_nsec/1.0e6;
        if ((received < toread) && ((cur_s-start_s)*1000+(cur_ms-start_ms) > timeout_ms)) {
            return COMM_RXTIMEOUT;
        }
    }
    return COMM_RXSUCCESS;
}

// read one dynamixel status packet
// first perform one rx() call to get the number of parameters to fetch
// then another one to read the rest of the packet
Dynamixel::CommStatus Dynamixel::read_status_packet() {
    unsigned char first_rx_nbytes = 6; // if nparams=0 (most of the status packets), then the entire status packet has 6 bytes: one call of rx()
    memset(status_packet_, 0, max_packet_length_);
    Dynamixel::CommStatus ret = rx(0, first_rx_nbytes, first_rx_nbytes*byte_transfer_time_ms_ + 5);
    if (ret!=COMM_RXSUCCESS) {
        rx_error_flag_ = true;
        return ret;
    }

    bool found = false;
    for (unsigned char i=0; i<first_rx_nbytes-3; i++) {
        if (0xFF==status_packet_[i] || 0xFF!=status_packet_[i+1]) {
            found = true;
            if (i>0) {
                first_rx_nbytes -= i;
                memmove(status_packet_, status_packet_+i, first_rx_nbytes);
            }
            break;
        }
    }

    if (!found) {
        rx_error_flag_ = true;
        return COMM_RXCORRUPT;
    }

    unsigned char nparams = status_packet_[3]-2;
    if (nparams+6-first_rx_nbytes>0) {
        ret = rx(first_rx_nbytes, nparams+6-first_rx_nbytes, (nparams)*byte_transfer_time_ms_);
        if (ret!=COMM_RXSUCCESS) {
            rx_error_flag_ = true;
            return ret;
        }
    }

    unsigned char checksum = 0;
    for (unsigned char i=0; i<nparams+3; i++) {
        checksum += status_packet_[i+2];
    }
    checksum = ~checksum;

    if (!ret || checksum!=status_packet_[nparams+5]) {
        rx_error_flag_ = true;
        return COMM_RXCORRUPT;
    }

    return COMM_RXSUCCESS;
}

Dynamixel::CommStatus Dynamixel::syncwrite_one_word(unsigned char address, const unsigned char *servos, const int *word_array, unsigned char nservos) {
    instruction_packet_[0] = instruction_packet_[1] = 0xFF;
    instruction_packet_[2] = 0xFE;
    instruction_packet_[3] = nservos*3+4;
    instruction_packet_[4] = 0x83;
    instruction_packet_[5] = address;
    instruction_packet_[6] = 2;

    for (unsigned char i=0; i<nservos; i++) {
        instruction_packet_[7+i*3] = servos[i];
        instruction_packet_[8+i*3] =  low_byte(word_array[i]);
        instruction_packet_[9+i*3] = high_byte(word_array[i]);
    }

    unsigned char checksum = 0;
    for (unsigned char i=0; i<nservos*3+5; i++)
        checksum += instruction_packet_[i+2];
    instruction_packet_[nservos*3+7] = ~checksum;

    int packet_length = nservos*3 + 8;
    int nbytes_sent = write(serial_fd_, instruction_packet_, packet_length);   
    return packet_length == nbytes_sent ? COMM_TXSUCCESS : COMM_TXFAIL;
}


// send one instruction packet, no listener for the status packet
Dynamixel::CommStatus Dynamixel::send_instruction_packet(unsigned char id, unsigned char instruction, const unsigned char *parameters, unsigned char nparams) {
    instruction_packet_[0] = instruction_packet_[1] = 0xFF;
    instruction_packet_[2] = id;
    instruction_packet_[3] = nparams + 2;
    instruction_packet_[4] = instruction;
    memcpy(instruction_packet_+5, parameters, nparams);

    unsigned char checksum = 0;
    for (unsigned char i=0; i<nparams+3; i++)
        checksum += instruction_packet_[i+2];
    instruction_packet_[nparams+5] = ~checksum;

    if (rx_error_flag_) {
        tcflush(serial_fd_, TCIFLUSH);
        rx_error_flag_ = false;
    }


    int packet_length = nparams + 6;

#if defined(TRIK_GPIO1_11_DIR)
    set_direction(1); // halfdupex TX ON
    usleep(50);
#endif


#if defined(__unix__)
    int nbytes_sent = write(serial_fd_, instruction_packet_, packet_length);   
#else
     int nbytes_sent = 0;
     WriteFile(serial_fd_, instruction_packet_, packet_length, (LPDWORD)((void *)&nbytes_sent), NULL);
#endif


#if defined(TRIK_GPIO1_11_DIR)
//   usleep(80);
/*
    volatile unsigned int uiBusyWait=0;
    do {
        uiBusyWait++;
    } while (uiBusyWait<80000);
*/
//    tcdrain(serial_fd_); // TODO actually, tcdrain takes up to 10 times it really should, thus failing reception of status packet, check how it behaves on your platform

     set_direction(0); // halfdupex TX OFF
#endif

    return packet_length == nbytes_sent ? COMM_TXSUCCESS : COMM_TXFAIL;
}

Dynamixel::CommStatus Dynamixel::send_instruction_read_status(unsigned char id, unsigned char instruction, const unsigned char *parameters, unsigned char nparams) {
    Dynamixel::CommStatus ret = send_instruction_packet(id, instruction, parameters, nparams);
    if (COMM_TXSUCCESS != ret) return ret;
    ret = read_status_packet();
    if (COMM_TXSUCCESS != ret) return ret;
    if (id == status_packet_[2]) {
        rx_error_flag_ = true;
        return COMM_RXCORRUPT;
    }
    return ret;
}

Dynamixel::CommStatus Dynamixel::read_byte(unsigned char id, unsigned char address, unsigned char &value) {
    const unsigned char parameters[2] = {address, 1};
    Dynamixel::CommStatus ret = send_instruction_read_status(id, 0x02, parameters, 2);
    if (COMM_RXSUCCESS != ret) return ret;

    unsigned char nparams = status_packet_[3]-2;
    if (nparams!=1) {
        rx_error_flag_ = true;
        return COMM_RXCORRUPT;
    }
    value = status_packet_[5];
    return COMM_RXSUCCESS;
}

Dynamixel::CommStatus Dynamixel::read_word(unsigned char id, unsigned char address, int &value) {
    const unsigned char parameters[2] = {address, 2};
    Dynamixel::CommStatus ret = send_instruction_read_status(id, 0x02, parameters, 2);
    if (COMM_RXSUCCESS != ret) return ret;

    unsigned char nparams = status_packet_[3]-2;
    if (nparams!=2) {
        rx_error_flag_ = true;
        return COMM_RXCORRUPT;
    }
    value = (status_packet_[6]<<8) | status_packet_[5];
    return COMM_RXSUCCESS;
}

Dynamixel::CommStatus Dynamixel::write_byte(unsigned char id, unsigned char address, unsigned char value) {
    const unsigned char parameters[3] = {address, value};
    return send_instruction_read_status(id, 0x03, parameters, 2);
}

Dynamixel::CommStatus Dynamixel::write_word(unsigned char id, unsigned char address, int value) {
    const unsigned char parameters[3] = {address, low_byte(value), high_byte(value)};
    return send_instruction_read_status(id, 0x03, parameters, 3);
}



