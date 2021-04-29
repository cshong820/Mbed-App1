#include "mbed.h"
#include "TS_DISCO_F469NI.h"
#include "LCD_DISCO_F469NI.h"
#include "cs_device.h"

LCD_DISCO_F469NI lcd;
TS_DISCO_F469NI ts;

DigitalIn Key_Chk(USER_BUTTON);
DigitalOut led1(LED1);

int main()
{
    TS_StateTypeDef TS_State;
    uint16_t x, y;
    uint8_t text[30];
    uint8_t status;
    uint8_t idx;
    uint8_t cleared = 0;
    uint8_t prev_nb_touches = 0;
//cs.
    //if(Key_Chk) wait(1);        //Key Pressed, 1
    //else wait(10);

    BSP_LCD_SetFont(&Font24);

    for (i=0; i<19; i++) {
        lcd.DisplayStringAt(0, LINE(i), (uint8_t *)"TOUCHSCREEN DEMO ", CENTER_MODE);
    }

    while(1) {
        if(Key_Chk && Key_cnt < 255) Key_cnt++;
        else {
            if (Key_cnt > 200) Key_inputed = 1;
            Key_cnt = 0;
            break;
        }
    };

    status = ts.Init(lcd.GetXSize(), lcd.GetYSize());
    if (status != TS_OK) {
        lcd.Clear(LCD_COLOR_RED);
        lcd.SetBackColor(LCD_COLOR_RED);
        lcd.SetTextColor(LCD_COLOR_WHITE);
        lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"TOUCHSCREEN INIT FAIL", CENTER_MODE);
    } else {
        lcd.Clear(LCD_COLOR_GREEN);
        lcd.SetBackColor(LCD_COLOR_GREEN);
        lcd.SetTextColor(LCD_COLOR_WHITE);
        lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"TOUCHSCREEN INIT OK", CENTER_MODE);
    }

    wait(3);

    while(1) {
        if(Key_Chk && Key_cnt < 255) Key_cnt++;
        else {
            if (Key_cnt > 200) Key_inputed = 1;
            Key_cnt = 0;
            break;
        }
    };

    wait(3);
    lcd.SetBackColor(LCD_COLOR_BLUE);
    lcd.SetTextColor(LCD_COLOR_WHITE);
    printf("This is the bare metal blinky example running on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
    
    while(1) {
        //Key Check
        if(Key_Chk) {
             if(Key_cnt > 200) Key_press = 1;
             else Key_cnt++;
        }
        else {
            if (Key_press) {
                Key_inputed = 1;
                Key_press = 0;
                Key_cnt = 0;
            }
            else {
                Key_cnt = 0;                
            }
        }

        if(Key_inputed) {
            Key_inputed = 0;
            if (CMode < MAX_MODE) CMode++;
            else CMode = 0;
            cleared = 0;
        }

        switch (CMode) {
            case TS_MODE:
                ts.GetState(&TS_State);
                if (TS_State.touchDetected) {
                    // Clear lines corresponding to old touches coordinates
                    if (TS_State.touchDetected < prev_nb_touches) {
                        for (idx = (TS_State.touchDetected + 1); idx <= 5; idx++) {
                            lcd.ClearStringLine(idx);
                        }
                    }
                    prev_nb_touches = TS_State.touchDetected;

                    cleared = 0;

                    sprintf((char*)text, "Touches: %d", TS_State.touchDetected);
                    lcd.DisplayStringAt(0, LINE(0), (uint8_t *)&text, LEFT_MODE);

                    for (idx = 0; idx < TS_State.touchDetected; idx++) {
                        x = TS_State.touchX[idx];
                        y = TS_State.touchY[idx];
                        sprintf((char*)text, "Touch %d: x=%d y=%d    ", idx+1, x, y);
                        lcd.DisplayStringAt(0, LINE(idx+1), (uint8_t *)&text, LEFT_MODE);
                        lcd.FillCircle(TS_State.touchX[idx], TS_State.touchY[idx], 20);
                    }
                } else {
                    if (!cleared) {
                        lcd.Clear(LCD_COLOR_BLUE);
                        sprintf((char*)text, "Touches: 0");
                        lcd.DisplayStringAt(0, LINE(0), (uint8_t *)&text, LEFT_MODE);
                        cleared = 1;
                    }
                }
                break;
                
            case DBG_MODE:
                if (!cleared) {
                    lcd.Clear(LCD_COLOR_GRAY);
                    sprintf((char*)text, "DEBUG MODE:");
                    lcd.DisplayStringAt(0, LINE(0), (uint8_t *)&text, LEFT_MODE);
                    cleared = 1;
                }
                break;

            default :
                if (!cleared) {
                    lcd.Clear(LCD_COLOR_BLACK);
                    sprintf((char*)text, "IDLE MODE:");
                    lcd.DisplayStringAt(0, LINE(0), (uint8_t *)&text, LEFT_MODE);
                    cleared = 1;
                }
                break;
        }
    }
}