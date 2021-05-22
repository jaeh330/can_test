#include "can.h"

int soc;
int read_can_port;

int open_port(const char *port)
{
    struct ifreq ifr;
    struct sockaddr_can addr;

    /* open socket */
    soc = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if(soc < 0)
    {
        return (-1);
    }

    addr.can_family = AF_CAN;
    strcpy(ifr.ifr_name, port);

    if (ioctl(soc, SIOCGIFINDEX, &ifr) < 0)
    {

        return (-1);
    }

    addr.can_ifindex = ifr.ifr_ifindex;

    fcntl(soc, F_SETFL, O_NONBLOCK);

    if (bind(soc, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {

        return (-1);
    }

    return 0;
}

int send_port(struct can_frame *frame)
{
    int retval;
   retval = write(soc, frame, sizeof(struct can_frame));
    if (retval != sizeof(struct can_frame))
    {
        return (-1);
    }
    else
    {
        return (0);
    }
}

/* this is just an example, run in a thread */
/* not use this function*/
void read_port()
{
    struct can_frame frame_rd;
    int recvbytes = 0;

    read_can_port = 1;
    while(read_can_port)
    {
        struct timeval timeout = {1, 0};  //s, us
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(soc, &readSet);

        if (select((soc + 1), &readSet, NULL, NULL, &timeout) >= 0)
        {
            if (!read_can_port)
            {
                break;
            }
            if (FD_ISSET(soc, &readSet))
            {
                recvbytes = read(soc, &frame_rd, sizeof(struct can_frame));
                if(recvbytes)
                {
                    printf("dlc = %d, data = %s\n", frame_rd.can_dlc,frame_rd.data);

                }
            }
        }

    }

}

int close_port()
{
    close(soc);
    return 0;
}

void CAN_initalize(void){

  if(open_port("can0")==-1)
    ROS_WARN("CAN_initalize_Failed");
  else
    ROS_INFO("initailze_CAN");
}



void CAN_write(BYTE data_array[]){

  struct can_frame frame;
  frame.can_id = 0xB7AC01 ;        //32bit
  frame.can_dlc=8;                 //8bit
  memcpy(frame.data,data_array,8);  //copy data_array->frame

  if(send_port(&frame) == -1){
    ROS_WARN("CAN_write_fuction error");
  }
}


void CAN_REQ(BYTE R_PID){


//****************REQ***********************************//
  BYTE REQ_Arr[8] = {PID_REQ_PID_DATA, R_PID,0,0,0,0,0,0};
  CAN_write(REQ_Arr);
//****************REQ***********************************//

}

void CAN_read(void){

  struct can_frame frame_rd;
  read(soc, &frame_rd, sizeof(struct can_frame));
  int i=0;
  for(i=0;i<7;i++){
    ROS_INFO("data(%d) = %d",i,frame_rd.data[i]);
  }

}




/***********************************************************
 * 1바이트 D4,D5,D6,D7 데이터를 받아 원 데이터인 4바이트 정수를 만듬
 ***********************************************************/
int Byte2Int32(BYTE d4, BYTE d5, BYTE d6, BYTE d7)
{
  return ((int)d4 | (int)d5<<8 | (int)d6<<16 | (int)d7<<24);
}
