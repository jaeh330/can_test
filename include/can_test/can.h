#ifndef CAN_OLA_H
#define CAN_OLA_H

#include "ros/ros.h"
#include "std_msgs/String.h"

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;


typedef unsigned char  BYTE;  //8bit
typedef unsigned short BYTE2; //16bit
typedef unsigned int BYTE4;   //32bit

struct CAN_data{
//BYTE pid;
BYTE data[8];
};

enum Bit_rate {
  _10k = 0,
  _20k,
  _50k,
  _100k,
  _125k,
  _250k,
  _500k,
  _750k,
  _1M
};

class CAN
{

  private:
    int soc;
    int read_can_port;
    string port_s;
    struct can_frame frame;

  public:
    CAN();
    CAN(string port, int id);
    CAN(string port, int id, bool is_ext_mode);

    int open_port(const char *port);
    int send_port(struct can_frame *frame);
    void read_port();
    int close_port();

    void CAN_initialize(int bit_rate_mode,string port_name);
    void CAN_write(BYTE data_array[]);
    void CAN_REQ(BYTE R_PID);
    struct CAN_data CAN_read(void);

    ~CAN();

};



#endif // CAN_OLA_H
