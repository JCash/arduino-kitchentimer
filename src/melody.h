#pragma once

#include <stdint.h>

enum ENote
{
    NOTE_SILENT,
    NOTE_B0,
    NOTE_C1,
    NOTE_CS1,
    NOTE_D1,
    NOTE_DS1,
    NOTE_E1,
    NOTE_F1,
    NOTE_FS1,
    NOTE_G1,
    NOTE_GS1,
    NOTE_A1,
    NOTE_AS1,
    NOTE_B1,
    NOTE_C2,
    NOTE_CS2,
    NOTE_D2,
    NOTE_DS2,
    NOTE_E2,
    NOTE_F2,
    NOTE_FS2,
    NOTE_G2,
    NOTE_GS2,
    NOTE_A2,
    NOTE_AS2,
    NOTE_B2,
    NOTE_C3,
    NOTE_CS3,
    NOTE_D3,
    NOTE_DS3,
    NOTE_E3,
    NOTE_F3,
    NOTE_FS3,
    NOTE_G3,
    NOTE_GS3,
    NOTE_A3,
    NOTE_AS3,
    NOTE_B3,
    NOTE_C4,
    NOTE_CS4,
    NOTE_D4,
    NOTE_DS4,
    NOTE_E4,
    NOTE_F4,
    NOTE_FS4,
    NOTE_G4,
    NOTE_GS4,
    NOTE_A4,
    NOTE_AS4,
    NOTE_B4,
    NOTE_C5,
    NOTE_CS5,
    NOTE_D5,
    NOTE_DS5,
    NOTE_E5,
    NOTE_F5,
    NOTE_FS5,
    NOTE_G5,
    NOTE_GS5,
    NOTE_A5,
    NOTE_AS5,
    NOTE_B5,
    NOTE_C6,
    NOTE_CS6,
    NOTE_D6,
    NOTE_DS6,
    NOTE_E6,
    NOTE_F6,
    NOTE_FS6,
    NOTE_G6,
    NOTE_GS6,
    NOTE_A6,
    NOTE_AS6,
    NOTE_B6,
    NOTE_C7,
    NOTE_CS7,
    NOTE_D7,
    NOTE_DS7,
    NOTE_E7,
    NOTE_F7,
    NOTE_FS7,
    NOTE_G7,
    NOTE_GS7,
    NOTE_A7,
    NOTE_AS7,
    NOTE_B7,
    NOTE_C8,
    NOTE_CS8,
    NOTE_D8,
    NOTE_DS8,
};

enum EDuration
{
    WHOLE,
    HALF,
    QUARTER,
    EIGHTH,
};

struct Note
{
    ENote       note;
    EDuration   duration;
};

struct Melody
{
    const Note* notes;
    uint16_t    num_notes;
    uint8_t     bpm;
    // playing state
    uint16_t    current_note;       // if 0xFFFF, the melody has ended
    int32_t     current_signal;
    uint32_t    note_time;          // (us) currently played amount of time of the current note
    uint32_t    note_half_phase;
};


void MelodyStart(Melody* melody, const Note* notes, uint16_t num_notes, uint8_t bpm);
int MelodyUpdate(Melody* melody, int32_t microseconds, int pin_out);

#if defined(MELODY_IMPLEMENTATION)


void MelodyStart(Melody* melody, const Note* notes, uint16_t num_notes, uint8_t bpm)
{
    melody->notes = notes;
    melody->num_notes = num_notes;
    melody->bpm = bpm;
    melody->current_note = 0;
    melody->current_signal = 0;
    melody->note_time = 0;
    melody->note_half_phase = 0;
}

static inline uint32_t MelodyCalcHalfPhase(uint32_t hz)
{
    return 1000000 / hz / 2;
}

// Converts duration to microseconds
// static inline int32_t MelodyDurationToMicros(uint8_t bpm, EDuration duration)
// {
//     int32_t us_per_beat = (60 * 1000000) / bpm;
//     return us_per_beat / (1 << duration);
// }
static inline int32_t MelodyDuration(uint8_t bpm, EDuration duration)
{
    (void)bpm;
    int32_t length = (60 * 1000000) / ( bpm * (1 << duration) );
    return length;
}


static inline int MelodyGetHz(ENote note);

int MelodyUpdate(Melody* melody, int32_t millis, int pin_out)
{
    static int signal = 0;
    static int count = 0;
    static int del = 0;

    if( del <= 0 )
    {
        int tone_time = 20;
        int silence_time = 3;
        signal = (signal + 1) % 2;
        if( signal )
        {
            tone(pin_out, 2000, tone_time);
            del = tone_time;
        }
        else
        {
            noTone(pin_out);
            del = silence_time;

            count++;
            if(count == 4)
            {
                count = 0;
                del = (tone_time+silence_time) * 4 * 2 + silence_time;
            }
        }
    }
    else if (del > 0)
    {
        del -= millis;
    }

    return 1;
}

int MelodyUpdateTEST(Melody* melody, int32_t microseconds, int pin_out)
{
    static int signal = 0;
    static int del = 0;

        Serial.print(del);
        Serial.print("\n");

    if( del <= 0 )
    {
        signal = (signal + 1 ) % 2;
        digitalWrite(pin_out, signal ? HIGH : LOW);
        del = 1911;
    }
    else if (del > 0)
    {
        del -= microseconds;
    }

            // digitalWrite(pin_out, signal ? HIGH : LOW);
            // delayMicroseconds(1911);
            // digitalWrite(pin_out, signal ? LOW : HIGH);
            // delayMicroseconds(1911);
    
    return 1;
}

int MelodyUpdate2(Melody* melody, int32_t microseconds, int pin_out){
    int looped = 0;
    const Note* note = &melody->notes[melody->current_note];

    while( microseconds > 0 )
    {    
        // check if we've passed this note
        //int32_t note_micros = MelodyDurationToMicros(melody->bpm, note->duration);
        int32_t period_micros = 1000000 / MelodyGetHz(note->note);
        int32_t note_micros = MelodyDuration(melody->bpm, note->duration);
        Serial.print("Note: ");
        Serial.print(note->note);
        Serial.print("  Hz: ");
        Serial.print(MelodyGetHz(note->note));
        Serial.print("  Duration: ");
        Serial.print(note_micros);
        Serial.print("  period: ");
        Serial.print(period_micros);

        int32_t note_left = note_micros - melody->note_time;
        if (microseconds >= note_left)
        {
            microseconds -= note_left;
            melody->note_time = 0;
            melody->current_note++;
            if (melody->current_note >= melody->num_notes)
            {
                looped = 1;
                melody->current_note = 0;
                Serial.print("Looped");
            }

            note = &melody->notes[melody->current_note];
            //melody->note_half_phase = MelodyCalcHalfPhase(MelodyGetHz(note->note));


            // Serial.print("NOTE CHANGE ");
            // Serial.print(melody->current_note);
            // Serial.print("Note Duration (us) ");
            // Serial.print(note_micros);
            // Serial.print("Half Phase (us) ");
            // Serial.print(melody->note_half_phase);
            // Serial.print("\n");
        }
        else
        {
            melody->note_time += microseconds;
            microseconds = 0;
        }

        // update the state of this note
        int signal = (1 + melody->note_time / (period_micros/2)) % 2;

        // if at the end of the note, force silence
        // ...

        //if (signal != melody->current_signal )
        {
            Serial.print("  Signal ");
            Serial.print(signal);
            // Serial.print("\n");

            digitalWrite(pin_out, signal ? HIGH : LOW);

            // delayMicroseconds(1911);
            // digitalWrite(pin_out, signal ? LOW : HIGH);
            // delayMicroseconds(1911);
            /*
            delayMicroseconds(period_micros/2);
            digitalWrite(pin_out, signal ? LOW : HIGH);
            delayMicroseconds(period_micros/2);
            */
            melody->current_signal = signal;
        }

        Serial.print("\n");
    }

    return looped;
}


const uint16_t g_NoteHz[] = {
    31, // NOTE_SILENT
    31, // NOTE_B0
    33, // NOTE_C1
    35, // NOTE_CS1
    37, // NOTE_D1
    39, // NOTE_DS1
    41, // NOTE_E1
    44, // NOTE_F1
    46, // NOTE_FS1
    49, // NOTE_G1
    52, // NOTE_GS1
    55, // NOTE_A1
    58, // NOTE_AS1
    62, // NOTE_B1
    65, // NOTE_C2
    69, // NOTE_CS2
    73, // NOTE_D2
    78, // NOTE_DS2
    82, // NOTE_E2
    87, // NOTE_F2
    93, // NOTE_FS2
    98, // NOTE_G2
    104,    // NOTE_GS2
    110,    // NOTE_A2
    117,    // NOTE_AS2
    123,    // NOTE_B2
    131,    // NOTE_C3
    139,    // NOTE_CS3
    147,    // NOTE_D3
    156,    // NOTE_DS3
    165,    // NOTE_E3
    175,    // NOTE_F3
    185,    // NOTE_FS3
    196,    // NOTE_G3
    208,    // NOTE_GS3
    220,    // NOTE_A3
    233,    // NOTE_AS3
    247,    // NOTE_B3
    262,    // NOTE_C4
    277,    // NOTE_CS4
    294,    // NOTE_D4
    311,    // NOTE_DS4
    330,    // NOTE_E4
    349,    // NOTE_F4
    370,    // NOTE_FS4
    392,    // NOTE_G4
    415,    // NOTE_GS4
    440,    // NOTE_A4
    466,    // NOTE_AS4
    494,    // NOTE_B4
    523,    // NOTE_C5
    554,    // NOTE_CS5
    587,    // NOTE_D5
    622,    // NOTE_DS5
    659,    // NOTE_E5
    698,    // NOTE_F5
    740,    // NOTE_FS5
    784,    // NOTE_G5
    831,    // NOTE_GS5
    880,    // NOTE_A5
    932,    // NOTE_AS5
    988,    // NOTE_B5
    1047,   // NOTE_C6
    1109,   // NOTE_CS6
    1175,   // NOTE_D6
    1245,   // NOTE_DS6
    1319,   // NOTE_E6
    1397,   // NOTE_F6
    1480,   // NOTE_FS6
    1568,   // NOTE_G6
    1661,   // NOTE_GS6
    1760,   // NOTE_A6
    1865,   // NOTE_AS6
    1976,   // NOTE_B6
    2093,   // NOTE_C7
    2217,   // NOTE_CS7
    2349,   // NOTE_D7
    2489,   // NOTE_DS7
    2637,   // NOTE_E7
    2794,   // NOTE_F7
    2960,   // NOTE_FS7
    3136,   // NOTE_G7
    3322,   // NOTE_GS7
    3520,   // NOTE_A7
    3729,   // NOTE_AS7
    3951,   // NOTE_B7
    4186,   // NOTE_C8
    4435,   // NOTE_CS8
    4699,   // NOTE_D8
    4978,   // NOTE_DS8
};

static inline int MelodyGetHz(ENote note)
{
    return g_NoteHz[note];
}

#endif // MELODY_IMPLEMENTATION