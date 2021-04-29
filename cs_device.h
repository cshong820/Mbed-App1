// This file for Mbed Test program

#define IDL_MODE   0
#define TS_MODE    1
#define DBG_MODE   2
#define MAX_MODE   DBG_MODE


#define WAIT_TIME_MS 500 

    uint8_t CMode=0, NMode, CStatus, NStatus;
    bool    Key_inputed, Key_press;    
    uint8_t i, key, key_buf, Key_cnt = 0;   