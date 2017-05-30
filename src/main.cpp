
#include <Arduino.h>

#if defined(AVR_DEBUG)
#include <avr8-stub/avr8-stub.h>
#endif

// Write to EEPROM: https://www.arduino.cc/en/Tutorial/EEPROMRead

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

bool buttons[4] = {false,false,false,false};

#define PRESSED HIGH

int mode = 0;       // 0 - timer, 1 - game
uint32_t prevtime = 0;  // The previous app time

static void UpdateButtonStates()
{
    buttons[bnDown]     = digitalRead(bnPinDown) == PRESSED;
    buttons[bnSelect]   = digitalRead(bnPinSelect) == PRESSED;
    buttons[bnStart]    = digitalRead(bnPinStart) == PRESSED;
    buttons[bnUp]       = digitalRead(bnPinUp) == PRESSED;
}

static void UpdateButtonStatesDebug(const String& command)
{
    if(command == "down")
    {
        buttons[bnDown] = true;
    }
    else if(command == "select")
    {
        buttons[bnSelect] = true;
    }
    else if(command == "start")
    {
        buttons[bnStart] = true;
    }
    else if(command == "up")
    {
        buttons[bnUp] = true;
    }

/*
    Serial.print("buttons: ");
    Serial.print(buttons[0]);
    Serial.print(", ");
    Serial.print(buttons[1]);
    Serial.print(", ");
    Serial.print(buttons[2]);
    Serial.print(", ");
    Serial.print(buttons[3]);
    Serial.println("");
    */
}



int timermode = 0; // 0 - set (not running), 1 - running, 2 - alarm
int timersetmode = 0; // 0 - seconds, 1 - minutes
uint32_t timertimeout = 0; // The timeout in milliseconds
uint32_t timertime = 0;

// Display time:
// >= 60 minutes: 1:14h  where the ":" blinks on and off every second
// < 60 minutes: 59:14 where the seconds are simply counting down

static void TimerUpdate(uint32_t millis)
{
    if(timermode == 0)
    {
        if( buttons[bnSelect] )
        {
            timersetmode = (timersetmode + 1) & 1;

            // Serial.print("Select mode");
            // Serial.print(timersetmode);
        }
        else if( buttons[bnStart] )
        {
            if( timertimeout > 0 )
            {
                timermode = 1;
                timertime = 0;

//                Serial.print("Start timer!");
            }
        }
        else if( buttons[bnUp] )
        {
            timertimeout += 1000;
//            Serial.print(timertimeout / 1000);
        }
        else if( buttons[bnDown] )
        {
            if( timertimeout >= 1000)
                timertimeout -= 1000;
            
//            Serial.print(timertimeout / 1000);
        }

        delay(500);
    }
    else if(timermode == 1)
    {
        timertime += millis;
        if( timertime >= timertimeout )
        {
            timermode = 2;
        }
        else
        {
            delay(900);
        }
    }
    else if(timermode == 2)
    {
        if( buttons[bnUp] || buttons[bnDown] || buttons[bnStart] || buttons[bnSelect] )
        {
            timermode = 0; // Back to timer set mode
            timertime = 0;
        }
        else
        {
//            Serial.println("ALARM");
            delay(1000);
        }
    }
}


void setup()
{
#if defined(AVR_DEBUG)
    debug_init();   // initialize the debugger
#endif

    // Debugging
    pinMode(13, OUTPUT);
    //Serial.begin(9600);

    // pins
    // ARE they input as default? shave off 4 lines :)
    pinMode(bnPinDown, INPUT);
    pinMode(bnPinSelect, INPUT);
    pinMode(bnPinStart, INPUT);
    pinMode(bnPinUp, INPUT);
}

void loop()
{
    // digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    // delay(300);
    // digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    // delay(300);

    String command;
    // while (Serial.available()) {
    //     delay(2);  //delay to allow byte to arrive in input buffer
    //     char c = Serial.read();
    //     command += c;
    // }


    uint32_t time = millis();
    uint32_t deltamillis = time - prevtime;
    if( time < prevtime )
    {
        deltamillis = 0xFFFFFFFF - prevtime + time; // Wrap around every 50 days :/
    }
    prevtime = time;

    if( mode == 0 )
    {
        UpdateButtonStates();
        UpdateButtonStatesDebug(command);
        TimerUpdate(deltamillis);
    }
}
