
#include <Arduino.h>
#include <Wire.h>

#define OLED_IMPLEMENTATION
#include "oled.h"

#if defined(AVR_DEBUG)
#include <avr8-stub/avr8-stub.h>
#endif

// Write to EEPROM: https://www.arduino.cc/en/Tutorial/EEPROMRead


// Generated using image2header
// Each glyph has the spacing builtin
const uint8_t numbers_image[1024] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0xc0,0xe0,0xf0,0xf8,0x3c,0x1c,0x0e,0x0e,0x0e,0x1e,0x3e,0xfc,0xfc,0xf0,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0x70,0x70,0x78,0x38,0x3c,0xfc,0xfc,0xfe,0xfe,0x0e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x1c,0x1c,0x0e,0x0e,0x0e,0x0e,0x0e,0x1e,0x3e,0xfc,0xfc,0xf8,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x1c,0x0e,0x0e,0x0e,0x0e,0x0e,0x0e,0x1e,0x3e,0xfc,0xfc,0xf8,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0xf0,0x78,0xfe,0xfe,0xfe,0xfe,0x0e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc0,0xf8,0xfe,0x7e,0x0e,0x0e,0x0e,0x0e,0x0e,0x0e,0x0e,0x0e,0x0e,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0xe0,0xf0,0x78,0x78,0x3c,0x1c,0x1e,0x0e,0x0e,0x0e,0x0e,0x00,0x00,0x00,0x00,0x0c,0x0e,0x0e,0x0e,0x0e,0x0e,0x0e,0x0e,0x0e,0x0e,0x8e,0xee,0xfe,0xfe,0x7e,0x1e,0x06,0x00,0x00,0x00,0x00,0x00,0xc0,0xe0,0xf0,0xf8,0x7c,0x1c,0x1e,0x0e,0x0e,0x0e,0x0e,0x3e,0xfc,0xfc,0xf8,0xe0,0x00,0x00,0x00,0x00,0x80,0xc0,0xf0,0xf8,0x78,0x3c,0x1c,0x0e,0x0e,0x0e,0x0e,0x1e,0x3e,0xfc,0xf8,0xf0,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0xff,0xff,0xff,0x0f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0xf0,0xfc,0xff,0xff,0x1f,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0xff,0xff,0xff,0x3f,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc0,0xf0,0xff,0xff,0x3f,0x0f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc0,0xc0,0xc0,0xc0,0xe0,0xe0,0xf0,0x70,0x7c,0x3f,0x1f,0x0f,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0xf0,0x78,0x3c,0x1f,0x07,0x03,0xc1,0xfe,0xff,0xff,0x3f,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x7c,0x7f,0x7f,0x73,0x70,0xf0,0xf0,0xe0,0xe0,0xc0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc0,0xf0,0xfc,0xff,0xbf,0xcf,0xe3,0x61,0x70,0x70,0xf0,0xf0,0xe0,0xe0,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0xf0,0xfc,0xff,0x3f,0x0f,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x3f,0x7f,0xff,0xf0,0xe0,0xc0,0xc0,0xc0,0xe0,0x70,0x7c,0x3f,0x1f,0x0f,0x03,0x00,0x00,0x00,0xfc,0xff,0xff,0xff,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc0,0xff,0xff,0xff,0x7f,0x00,0x00,0x00,0x78,0xfc,0xfc,0xfc,0x78,0x00,0x00,0x00,0x00,0xe0,0xfe,0xff,0xff,0xff,0x71,0x18,0x1c,0x0c,0x0e,0x0e,0x1e,0xfe,0xfe,0xfc,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0xfe,0xff,0xff,0xff,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xf0,0xff,0xff,0x7f,0x0f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xfc,0xff,0xff,0xff,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0xe0,0xf0,0x78,0x3c,0x1f,0x0f,0x07,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x03,0x03,0x0f,0xff,0xff,0xfe,0xf8,0x00,0x00,0x00,0x00,0x60,0x78,0x7c,0x7e,0x77,0x73,0x71,0x70,0x70,0x70,0xf0,0xfc,0xff,0xff,0xff,0x77,0x70,0x70,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0xff,0xff,0xff,0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0xfc,0xff,0xff,0xff,0x07,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xff,0xff,0xff,0x7f,0x00,0x00,0x00,0x00,0x00,0x00,0xc0,0xe0,0xf8,0xfe,0x7f,0x1f,0x07,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc0,0xf0,0xf8,0xfc,0x3e,0x0f,0x07,0x03,0x01,0x01,0x01,0x03,0x07,0x0f,0xff,0xfe,0xfc,0xf0,0x00,0x00,0x00,0x00,0x00,0x01,0x07,0x0f,0x0f,0x1f,0x1e,0x1c,0x1c,0x1c,0x0c,0x8e,0xe6,0xfb,0xff,0x3f,0x0f,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xfc,0xff,0xff,0xff,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xfc,0xff,0xff,0xff,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x07,0x1f,0x3f,0x7f,0xf8,0xf0,0xe0,0xe0,0xe0,0x70,0x78,0x3e,0x3f,0x0f,0x07,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0xff,0xff,0xff,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0xc0,0xe0,0xf0,0xf8,0xfc,0xee,0xef,0xe7,0xe3,0xe1,0xe0,0xe0,0xe0,0xe0,0xe0,0xe0,0xe0,0x00,0x00,0x00,0x00,0x00,0x40,0xf0,0xf0,0xe0,0xe0,0xe0,0xe0,0xe0,0xe0,0xf0,0x70,0x78,0x3e,0x1f,0x0f,0x07,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0xff,0xff,0xff,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0xf0,0xf0,0xe0,0xe0,0xe0,0xe0,0xe0,0xf0,0x70,0x78,0x3c,0x3f,0x1f,0x0f,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x1f,0x3f,0x7f,0x7c,0xf0,0xe0,0xe0,0xe0,0xf0,0x70,0x7c,0x3f,0x1f,0x0f,0x03,0x00,0x00,0x80,0xe0,0xf8,0xfc,0xff,0x3f,0x0f,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x1f,0x3f,0x7f,0x78,0xf0,0xe0,0xe0,0xe0,0xe0,0xe0,0x70,0x78,0x7c,0x3f,0x1f,0x0f,0x03,0x00,0x00,0x00,0x00,0xe0,0xe0,0xe0,0xe0,0xe0,0x70,0x70,0x38,0x3c,0x1e,0x1f,0x0f,0x07,0x03,0x00,0x00,0x00,0x00,0x00,0x78,0xfc,0xfc,0xfc,0x78,0x00,0x00,0x00,0x00,0xf0,0xff,0xff,0xff,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0xff,0xff,0xff,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    };
const uint16_t numbers_image_width = 256;
const uint16_t numbers_image_height = 32;

//                                   0   1   2   3   4   5   6   7   8   9   :   h
const uint16_t numbers_offsets[]= {  0, 21, 42, 63, 84,105,126,147,168,189,210,218};
const uint8_t numbers_sizes[]   = { 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,  8, 20};
const uint8_t numbers_max_width = 21;


enum ETimerMode
{
    TIMER_MODE_IDLE,
    TIMER_MODE_SET,
    TIMER_MODE_RUN,
    TIMER_MODE_PAUSE,
    TIMER_MODE_ALARM,
    TIMER_MODE_TEST,
};

enum ETimerSetMode
{
    TIMER_SET_MODE_SECONDS,
    TIMER_SET_MODE_MINUTES,
};

// CONSTANTS
const int bnPinDown = 5;
const int bnPinSelect = 6;
const int bnPinStart = 7;
const int bnPinUp = 8;
// left to right
const int bnDown    = 0;
const int bnSelect  = 1;
const int bnStart   = 2;
const int bnUp      = 3;

bool buttonsPrev[4] = {false,false,false,false};
bool buttons[4] = {false,false,false,false};

// If the digits doesn't change, don't update the screen
uint8_t digitsPrev[4] = {255,255,255,255};

#define SECOND 1000L
#define MINUTE 60000L

int timermode = TIMER_MODE_SET;
int timersetmode = TIMER_SET_MODE_SECONDS;
int32_t timertimeout = 7 * SECOND; // The timeout in milliseconds
int32_t timertime = 0;
int32_t idletime = 0;
int32_t timerblinktime = 0;
uint8_t timerblinkflag = 0; // 0 = off, 1 = on

const int32_t   frameTime = 33;
const int32_t   timerIncrementSeconds = 5 * SECOND;
const int32_t   timerIncrementMinutes = 1 * MINUTE;
const int32_t   buttonTimeDownThreshold = 600;
const int32_t   buttonRepeatFrameTickRange = 5;
int32_t         buttonRepeatFrameTick = 0;
int32_t         buttonTimeDown = 0;


#define PRESSED HIGH

int32_t prevtime = 0;  // The previous app time

static void UpdateButtonStates()
{
    for( int i = 0; i < 4; ++i )
        buttonsPrev[i] = buttons[i];
    buttons[bnDown]     = digitalRead(bnPinDown) == PRESSED;
    buttons[bnSelect]   = digitalRead(bnPinSelect) == PRESSED;
    buttons[bnStart]    = digitalRead(bnPinStart) == PRESSED;
    buttons[bnUp]       = digitalRead(bnPinUp) == PRESSED;

/*
    if( buttons[bnDown] || buttons[bnSelect] || buttons[bnStart] || buttons[bnUp] )
    {
        Serial.print("buttons: ");
        Serial.print(buttons[bnDown]);
        Serial.print(", ");
        Serial.print(buttons[bnSelect]);
        Serial.print(", ");
        Serial.print(buttons[bnStart]);
        Serial.print(", ");
        Serial.print(buttons[bnUp]);
        Serial.print("\n");
    }*/
}

static bool IsButtonDown(int button)
{
    return buttons[button];
}

static bool IsButtonPressed(int button)
{
    return buttons[button] && !buttonsPrev[button];
}

static bool IsButtonReleased(int button)
{
    return !buttons[button] && buttonsPrev[button];
}

// Display time:
// >= 60 minutes: 1:14h  where the ":" blinks on and off every second
// < 60 minutes: 59:14 where the seconds are simply counting down


static void TimerDisplay()
{
    int32_t timeleft = timermode == TIMER_MODE_SET ? timertimeout : timertime;
    int16_t minutes = timeleft / MINUTE;
    int16_t seconds = (timeleft / SECOND) % 60;

    uint32_t x = 100;
    uint32_t v = 0;

    bool istimersetmode = timermode == TIMER_MODE_SET || timermode == TIMER_MODE_PAUSE;
    bool show_seconds = !istimersetmode || timersetmode == TIMER_SET_MODE_MINUTES || timerblinkflag;
    bool show_minutes = !istimersetmode || timersetmode == TIMER_SET_MODE_SECONDS || timerblinkflag;

    int page = 2;

    const int image_stride = numbers_image_width;
    const int image_height = numbers_image_height;
    
    // //v = seconds % 10;
    // //v = timermode == TIMER_MODE_SET;
    // v = timermode;
    // //v = timerblinkflag ? 1 : 0;
    // oled_blit(page, x - numbers_sizes[v], numbers_image, image_stride, numbers_offsets[v], numbers_sizes[v], image_height);
    // return;

/*
    x = 8;

    for( int i = 0; i < 4; ++i)
    {
        v = buttons[i];
        oled_blit(page, x, numbers_image, image_stride, numbers_offsets[v], numbers_sizes[v], image_height);
        x+= 24;
    }

    return;
*/

    if( show_seconds ) {
        v = seconds % 10;
        oled_blit(page, x - numbers_sizes[v], numbers_image, image_stride, numbers_offsets[v], numbers_sizes[v], image_height);
        
        x -= numbers_max_width;
        seconds = seconds / 10;
        v = seconds % 10;

        oled_blit(page, x - numbers_sizes[v], numbers_image, image_stride, numbers_offsets[v], numbers_sizes[v], image_height);

        x -= numbers_max_width;
    }
    else {
        oled_fill_rect(page, x - 2*numbers_max_width, page + image_height/8, x, 0);
        x -= 2*numbers_max_width;
    }

    v = 10; // ":"
    oled_blit(page, x - numbers_sizes[v], numbers_image, image_stride, numbers_offsets[v], numbers_sizes[v], image_height);
    x -= numbers_sizes[v];

    if( show_minutes )
    {
        v = minutes % 10;
        oled_blit(page, x - numbers_sizes[v], numbers_image, image_stride, numbers_offsets[v], numbers_sizes[v], image_height);

        x -= numbers_max_width;
        minutes = minutes / 10;
        v = minutes % 10;

        if( v > 0 )
            oled_blit(page, x - numbers_sizes[v], numbers_image, image_stride, numbers_offsets[v], numbers_sizes[v], image_height);
        else
            oled_fill_rect(page, x - numbers_max_width, page + image_height/8, x, 0);
    }
    else
    {
        oled_fill_rect(page, x - 2*numbers_max_width, page + image_height/8, x, 0);
        x -= 2*numbers_max_width;
    }


    if( timermode == TIMER_MODE_ALARM )
    {
        oled_invert(timerblinkflag & 1);
    }
}

static inline void AddTime(int32_t ms)
{
    int32_t* tval = timermode == TIMER_MODE_SET ? &timertimeout : &timertime;
    *tval += ms;
    if( *tval <= 0 )
        *tval = 0;
}


static void TimerUpdate(int32_t millis)
{
#define BLINK_ON \
    timerblinktime = 0; \
    timerblinkflag = 1;

    int32_t prev_timertime = timertime - (timertime % SECOND);
    int32_t prev_timertimeout = timertimeout;
    int32_t prev_timermode = timermode;
    int32_t prev_timerblinkflag = timerblinkflag;


    timerblinktime += millis;

    /*
    if(timermode == TIMER_MODE_TEST)
    {
        timertime += millis;
        if( timertime > 250 )
        {
            display_dirty = true;
            static uint32_t digit = 9;
            digit = (digit + 1) % 10;

            timertime = 0;
            const int second = 1000;
            const int minute = second * 60;
            timertimeout += second;
        }
    }
    else*/
    if(timermode == TIMER_MODE_SET || timermode == TIMER_MODE_PAUSE)
    {
        if( IsButtonDown(bnSelect) && IsButtonDown(bnStart) )
        {
            timertime = 0;
            timertimeout = 0;
            BLINK_ON;
        }
        else if( IsButtonPressed(bnSelect) )
        {
            timersetmode = (timersetmode + 1) & 1;
            timerblinktime = 250;
            timerblinkflag = 0;
        }
        else if( IsButtonReleased(bnStart) )
        {
            if( timertimeout > 0 )
            {
                if(timermode == TIMER_MODE_SET)
                    timertime = timertimeout;
                timermode = TIMER_MODE_RUN;
            }
        }
        else if( IsButtonPressed(bnUp) )
        {
            AddTime(timersetmode == TIMER_SET_MODE_MINUTES ? MINUTE : SECOND);
            BLINK_ON;
        }
        else if( IsButtonPressed(bnDown) )
        {
            AddTime(timersetmode == TIMER_SET_MODE_MINUTES ? -MINUTE : -SECOND);
            BLINK_ON;
        }
        else if( IsButtonDown(bnDown) || IsButtonDown(bnUp) )
        {
            BLINK_ON;
            buttonTimeDown += millis;
            if( buttonTimeDown >= buttonTimeDownThreshold )
            {
                int32_t bnFactor = IsButtonDown(bnDown) ? -1 : 1;

                // Round up/down
                if( timersetmode == TIMER_SET_MODE_SECONDS )
                {
                    int32_t rest = timertimeout % timerIncrementSeconds;
                    if(rest)
                        timertimeout += bnFactor < 0 ? -rest : timerIncrementSeconds - rest;
                }

                int32_t increment = timersetmode == TIMER_SET_MODE_MINUTES ? timerIncrementMinutes : timerIncrementSeconds;
                int32_t step = (buttonRepeatFrameTick % buttonRepeatFrameTickRange) == 0 ? increment : 0;
                if(step)
                    AddTime(bnFactor * step);

                buttonRepeatFrameTick++;
            }
        }
        else
        {
            buttonRepeatFrameTick = 0;
            buttonTimeDown = 0;
        }

        if( timerblinktime > 500 )
        {
            timerblinktime = timerblinktime % 500;
            timerblinkflag = ~timerblinkflag & 1;
        }
    }
    else if(timermode == TIMER_MODE_RUN)
    {
        if( IsButtonReleased(bnStart) )
        {
            timermode = TIMER_MODE_PAUSE;
        }
        else
        {
            timertime -= millis;
            if( timertime <= 0 )
            {
                timertime = 0;
                timermode = TIMER_MODE_ALARM;
            }

            if( timerblinktime > 1000 )
            {
                timerblinktime = timerblinktime % 1000;
                timerblinkflag = ~timerblinkflag & 1;
            }
        }
    }
    else if(timermode == TIMER_MODE_ALARM)
    {
        if( IsButtonPressed(bnUp) || IsButtonPressed(bnDown) || IsButtonPressed(bnStart) || IsButtonPressed(bnSelect) )
        {
            timermode = TIMER_MODE_IDLE;
            oled_invert(false);
        }

        if( timerblinktime > 500 )
        {
            timerblinktime = timerblinktime % 500;
            timerblinkflag = ~timerblinkflag & 1;
        }
    }
    else if(timermode == TIMER_MODE_IDLE)
    {
        // const int32_t TIMEOUT_IDLE_TO_SLEEP = 30 * SECOND;
        // if( timertime > TIMEOUT_IDLE_TO_SLEEP )
        // {
        //     timermode = TIMERMODE_SLEEP;
        // }

        if( IsButtonReleased(bnStart) )
        {
            oled_constrast(0x7F);
            timermode = TIMER_MODE_SET;
            timersetmode = TIMER_SET_MODE_SECONDS;
            timertime = 0;
            BLINK_ON;
        }
        else if(idletime + millis > 10*SECOND)
        {
            oled_constrast(10);
        }

        idletime += millis;
    }

    bool display_dirty =
        prev_timertime != (timertime - (timertime % SECOND)) ||
        prev_timertimeout != timertimeout ||
        prev_timermode != timermode ||
        prev_timerblinkflag != timerblinkflag;

    if(display_dirty)
        TimerDisplay();

#undef BLINK_ON
}


void setup()
{
#if defined(AVR_DEBUG)
    debug_init();   // initialize the debugger
#endif

    // Debugging
    pinMode(13, OUTPUT); // Turn off the led on pin 13
#if !defined(AVR_DEBUG)
    Serial.begin(9600);
#endif

    // pins
    pinMode(bnPinDown, INPUT);
    pinMode(bnPinSelect, INPUT);
    pinMode(bnPinStart, INPUT);
    pinMode(bnPinUp, INPUT);

    // OLED
    Wire.begin();
    //Wire.setClock(400000);

    oled_init(OLED_I2C_ADDRESS, 128, 64);
    oled_clear();
}

void loop()
{
    /*
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(300);
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(300);
    */

    int32_t time = (int32_t)millis();
    int32_t deltamillis = time - prevtime;
    if( time < prevtime )
    {
        deltamillis = 0xFFFFFFFF - prevtime + time; // Wrap around every 50 days :/
    }
    prevtime = time;

    UpdateButtonStates();
    TimerUpdate(deltamillis);

    int32_t timeend = millis();
    int32_t diff = timeend - time;

    if( diff < frameTime )
        delay(frameTime - diff);
}

