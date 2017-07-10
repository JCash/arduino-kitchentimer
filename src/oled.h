#pragma once

/* A single file header library for displaying graphics on an Oled (SSD1306+compatible) for Arduino, using Wire for communication

All important configuration work and details by Sonal Pinto:
http://robotcantalk.blogspot.se/2015/03/interfacing-arduino-with-ssd1306-driven.html
https://github.com/SonalPinto/Arduino_SSD1306_OLED

Made into a library by Mathias Westerdahl

LICENSE: Public Domain

*/

#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h> // For reading data from fonts located in program space
#include <Wire.h>

// Use i2c_scanner to find your slave address
const uint8_t OLED_I2C_ADDRESS                      = 0x3C;

// Some defines for the SSD1306 controller driving a 128x64 resolution OLED display
// PART         - http://www.simplelabs.co.in/content/96-blue-i2c-oled-module
// DATASHEET    - https://www.adafruit.com/datasheets/SSD1306.pdf


const uint8_t OLED_CONTROL_BYTE_CMD_SINGLE          = 0x80;
const uint8_t OLED_CONTROL_BYTE_CMD_STREAM          = 0x00;
const uint8_t OLED_CONTROL_BYTE_DATA_SINGLE         = 0xC0;
const uint8_t OLED_CONTROL_BYTE_DATA_STREAM         = 0x40;

// Fundamental commands (pg.28)
const uint8_t OLED_CMD_SET_CONTRAST                 = 0x81;    // follow with 0x7F
const uint8_t OLED_CMD_DISPLAY_RAM                  = 0xA4;
const uint8_t OLED_CMD_DISPLAY_ALLON                = 0xA5;
const uint8_t OLED_CMD_DISPLAY_NORMAL               = 0xA6;
const uint8_t OLED_CMD_DISPLAY_INVERTED             = 0xA7;
const uint8_t OLED_CMD_DISPLAY_OFF                  = 0xAE;
const uint8_t OLED_CMD_DISPLAY_ON                   = 0xAF;

// Addressing Command Table (pg.30)
const uint8_t OLED_CMD_SET_MEMORY_ADDR_MODE         = 0x20;    // follow with 0x00 = HORZ mode = Behave like a KS108 graphic LCD
const uint8_t OLED_CMD_SET_COLUMN_RANGE             = 0x21;    // can be used only in HORZ/VERT mode - follow with 0x00 + 0x7F = COL127
const uint8_t OLED_CMD_SET_PAGE_RANGE               = 0x22;    // can be used only in HORZ/VERT mode - follow with 0x00 + 0x07 = PAGE7

// Hardware Config (pg.31)
const uint8_t OLED_CMD_SET_DISPLAY_START_LINE       = 0x40;
const uint8_t OLED_CMD_SET_SEGMENT_REMAP            = 0xA1;    
const uint8_t OLED_CMD_SET_MUX_RATIO                = 0xA8;    // follow with 0x3F = 64 MUX
const uint8_t OLED_CMD_SET_COM_SCAN_MODE            = 0xC8;    
const uint8_t OLED_CMD_SET_DISPLAY_OFFSET           = 0xD3;    // follow with 0x00
const uint8_t OLED_CMD_SET_COM_PIN_MAP              = 0xDA;    // follow with 0x12
const uint8_t OLED_CMD_NOP                          = 0xE3;    // NOP

// Timing and Driving Scheme (pg.32)
const uint8_t OLED_CMD_SET_DISPLAY_CLK_DIV          = 0xD5;    // follow with 0x80
const uint8_t OLED_CMD_SET_PRECHARGE                = 0xD9;    // follow with 0x22
const uint8_t OLED_CMD_SET_VCOMH_DESELCT            = 0xDB;    // follow with 0x30

// Charge Pump (pg.62)
const uint8_t OLED_CMD_SET_CHARGE_PUMP              = 0x8D;    // follow with 0x14

const uint8_t OLED_CMD_SET_LOW_COLUMN               = 0x00;
const uint8_t OLED_CMD_SET_HIGH_COLUMN              = 0x10;
const uint8_t OLED_CMD_SET_START_PAGE               = 0XB0;

const uint8_t OLED_CMD_SET_MULTIPLEX                = 0xA8;

// Initializes the oled display with the address
void oled_init(uint8_t address);

// Clears the entire display
void oled_clear();

// Displays a full image
void oled_display(const uint8_t* data, uint32_t datalength);

// Sets the cursor
void oled_set_cursor(uint8_t row, uint8_t column);

// Writes unbuffered text (ascii)
void oled_text(const char* text, uint32_t textlen, const uint8_t* font, uint8_t font_width, uint8_t font_height);

// Copies image data from one image to the oled
void oled_blit(uint32_t page, uint32_t column, const uint8_t* img, uint32_t img_offset, uint32_t img_stride, uint32_t img_width, uint32_t img_height);

void oled_invert(bool invert);

void oled_constrast(uint8_t contrast);

struct OledImage
{
    uint8_t*    img;
    uint16_t    width;
    uint16_t    height;
};

struct OledRect
{
    uint8_t     x;
    uint8_t     y;
    uint8_t     width;
    uint8_t     height;
};

// Copies image data from one image to another (without scaling)
void oled_blit_img(const OledImage* src, const OledRect* srcrect,
                    OledImage* dst, const OledRect* dstrect);

// Fills a rectangle with a color
void oled_fill_rect(OledImage* dst, const OledRect* dstrect, uint8_t value);

#if defined(OLED_IMPLEMENTATION)

uint8_t _oled_slave_address = 0;
uint8_t _oled_width = 128;
uint8_t _oled_height = 32;

// Call Wire.begin() outside of this function
void oled_init(uint8_t address, uint8_t width, uint8_t height)
{
    _oled_slave_address = address;
    _oled_width = width;
    _oled_height = height;

    Wire.beginTransmission(_oled_slave_address);
    // keywords:
    // SEG = COL = segment = column byte data on a page
    // Page = 8 pixel tall row. Has 128 SEGs and 8 COMs
    // COM = row

    // Begin the I2C comm with SSD1306's address (SLA+Write)
    Wire.beginTransmission(_oled_slave_address);

    // Tell the SSD1306 that a command stream is incoming
    Wire.write(OLED_CONTROL_BYTE_CMD_STREAM);

    // Follow instructions on pg.64 of the dataSheet for software configuration of the SSD1306
    // Turn the Display OFF
    Wire.write(OLED_CMD_DISPLAY_OFF);
    // Set mux ration tp select max number of rows - 64
    Wire.write(OLED_CMD_SET_MUX_RATIO);
    Wire.write(0x3F);
    // Set the display offset to 0
    Wire.write(OLED_CMD_SET_DISPLAY_OFFSET);
    Wire.write(0x00);
    // Display start line to 0
    Wire.write(OLED_CMD_SET_DISPLAY_START_LINE);

    Wire.write(OLED_CMD_SET_MULTIPLEX);
    Wire.write(_oled_height - 1);
    
    // Mirror the x-axis. In case you set it up such that the pins are north.
    // Wire.write(0xA0); - in case pins are south - default
    Wire.write(OLED_CMD_SET_SEGMENT_REMAP);
        
    // Mirror the y-axis. In case you set it up such that the pins are north.
    // Wire.write(0xC0); - in case pins are south - default
    Wire.write(OLED_CMD_SET_COM_SCAN_MODE);
        
    // Default - alternate COM pin map
    Wire.write(OLED_CMD_SET_COM_PIN_MAP);
    if( _oled_height == 64 )
        Wire.write(0x12);
    else
        Wire.write(0x02);
    // set contrast
    Wire.write(OLED_CMD_SET_CONTRAST);
    Wire.write(0x7F);
    // Set display to enable rendering from GDDRAM (Graphic Display Data RAM)
    Wire.write(OLED_CMD_DISPLAY_RAM);
    // Normal mode!
    Wire.write(OLED_CMD_DISPLAY_NORMAL);
    // Default oscillator clock
    Wire.write(OLED_CMD_SET_DISPLAY_CLK_DIV);
    Wire.write(0x80);
    // Enable the charge pump
    Wire.write(OLED_CMD_SET_CHARGE_PUMP);
    Wire.write(0x14);
    // Set precharge cycles to high cap type
    Wire.write(OLED_CMD_SET_PRECHARGE);
    Wire.write(0x22);
    // Set the V_COMH deselect volatage to max
    Wire.write(OLED_CMD_SET_VCOMH_DESELCT);
    Wire.write(0x30);
    // Horizonatal addressing mode - same as the KS108 GLCD
    Wire.write(OLED_CMD_SET_MEMORY_ADDR_MODE);
    Wire.write(0x00);
    // Turn the Display ON
    Wire.write(OLED_CMD_DISPLAY_ON);

    // End the I2C comm with the SSD1306
    Wire.endTransmission();
}


void oled_display(const uint8_t* data, uint32_t datalength)
{
    // Setup linear addressing space
    Wire.beginTransmission(_oled_slave_address);
    Wire.write(OLED_CONTROL_BYTE_CMD_STREAM);
    Wire.write(OLED_CMD_SET_COLUMN_RANGE);
    Wire.write(0x00);
    Wire.write(_oled_width-1);
    Wire.write(OLED_CMD_SET_PAGE_RANGE);
    Wire.write(0x00);
    Wire.write(_oled_height == 64 ? 7 : 3);
    Wire.endTransmission();

    for(uint32_t i = 0; i < datalength; ){
        Wire.beginTransmission(_oled_slave_address);
        Wire.write(OLED_CONTROL_BYTE_DATA_STREAM);

        size_t sz = BUFFER_LENGTH - 1;
        if( sz > (datalength - i) )
            sz = datalength - i;
        Wire.write(data + i, sz);
        i += sz;

        Wire.endTransmission();   
    }

}

void oled_set_cursor(uint8_t row, uint8_t column)
{
    if( row >= _oled_height/8 )
        row = _oled_height/8 - 1;

    if( column >= _oled_width )
        column = _oled_width;

    Wire.beginTransmission(_oled_slave_address);
    Wire.write(OLED_CONTROL_BYTE_CMD_STREAM);
    Wire.write(OLED_CMD_SET_LOW_COLUMN | (column & 0XF));
    Wire.write(OLED_CMD_SET_HIGH_COLUMN | (column >> 4));
    Wire.write(OLED_CMD_SET_START_PAGE | row);
    Wire.endTransmission();
}

void oled_text(const char* text, uint32_t textlen, const uint8_t* font, uint8_t font_width, uint8_t font_height)
{
    uint8_t col = 0;
    int shouldflush = 0;
    for( uint32_t i = 0; i < textlen; ++i, ++text )
    {
        if( col >= _oled_width )
        {
            if( shouldflush )
                Wire.endTransmission();
            return;
        }
        col += 7;
        char c = *text;
        if( c < 32 || c > 127 )
            c = 127;
        c -= 32;


        if( shouldflush == 0 )
        {
            Wire.beginTransmission(_oled_slave_address);
            Wire.write(OLED_CONTROL_BYTE_DATA_STREAM);
        }
        shouldflush++;

        const uint8_t *base = font + 5 * c;

        for (uint8_t i = 0; i < 5; i++ ) {
          uint8_t b = pgm_read_byte(base + i);

            Wire.write(b);
        }

        for (uint8_t i=0; i<2; i++) {
          if (col >= _oled_width) break;
            col++; 

            Wire.write(0);
        }

        if( shouldflush && shouldflush == 4 )
        {
            shouldflush = 0;
            Wire.endTransmission();
        }
    }

    if( shouldflush )
        Wire.endTransmission();
}

void oled_clear()
{
    Wire.beginTransmission(_oled_slave_address);
    Wire.write(OLED_CONTROL_BYTE_CMD_STREAM);
    Wire.write(OLED_CMD_SET_COLUMN_RANGE);
    Wire.write(0x00);
    Wire.write(_oled_width-1);
    Wire.write(OLED_CMD_SET_PAGE_RANGE);
    Wire.write(0x00);
    Wire.write(_oled_height == 64 ? 7 : 3);
    Wire.endTransmission();

    uint8_t buffer[BUFFER_LENGTH] = {0};
    for( uint32_t p = 0; p < _oled_height/8; ++p )
    {
        for( uint32_t x = 0; x < _oled_width; )
        {
            Wire.beginTransmission(_oled_slave_address);
            Wire.write(OLED_CONTROL_BYTE_DATA_STREAM);

            size_t sz = BUFFER_LENGTH - 1;
            if( sz > (_oled_width - x) )
                sz = _oled_width - x;

            Wire.write(buffer, sz);
            x += sz;

            Wire.endTransmission();   
        }
    }
}

void oled_blit(uint32_t page, uint32_t column, const uint8_t* img, uint32_t img_stride, uint32_t img_offset, uint32_t img_width, uint32_t img_height)
{
    for( uint32_t p = 0; p < img_height/8; ++p )
    {
        if( page + p >= (_oled_height/8) )
            break;
        oled_set_cursor(page + p, column);

        for( uint32_t x = 0; x < img_width; )
        {
            Wire.beginTransmission(_oled_slave_address);
            Wire.write(OLED_CONTROL_BYTE_DATA_STREAM);

            size_t sz = BUFFER_LENGTH - 1;
            if( sz > (img_width - x) )
                sz = img_width - x;

            Wire.write(&img[p * img_stride + img_offset + x], sz);
            x += sz;

            Wire.endTransmission();   
        }
    }
}

void oled_fill_rect(uint32_t page1, uint32_t column1, uint32_t page2, uint32_t column2, uint8_t value)
{
    for( ; page1 < page2; ++page1 )
    {
        if( page1 >= (_oled_height/8) )
            break;
        oled_set_cursor(page1, column1);

        for( uint32_t x = column1; x < column2 && x < _oled_width; )
        {
            Wire.beginTransmission(_oled_slave_address);
            Wire.write(OLED_CONTROL_BYTE_DATA_STREAM);

            size_t sz = BUFFER_LENGTH - 1;
            if( sz > (_oled_width - x) )
                sz = _oled_width - x;
            if( sz > (column2 - x) )
                sz = column2 - x;

            for( size_t n = 0; n < sz; n++ )
                Wire.write(&value, 1);

            x += sz;

            Wire.endTransmission();   
        }
    }
}


void oled_blit_img(const OledImage* src, const OledRect* srcrect,
                    OledImage* dst, const OledRect* dstrect)
{
    uint32_t numdstpages = dst->height/8;
    uint32_t dststartpage = dstrect->y/8;
    uint32_t numsrcpages = srcrect->height/8;
    uint32_t srcstartpage = srcrect->y/8;
    
    for( uint32_t p = 0; p < numsrcpages; ++p )
    {
        uint32_t dstpage = dststartpage + p;
        if( dstpage >= numdstpages )
            break;
        uint32_t srcpage = srcstartpage + p;
        if( srcpage >= numsrcpages )
            break;

        for( uint32_t x = 0; x < srcrect->width; ++x)
        {
            uint32_t srcindex = srcpage * src->width + srcrect->x + x;
            uint32_t dstindex = dstpage * dst->width + dstrect->x + x;

            dst->img[dstindex] = src->img[srcindex];
        }
    }
}

void oled_fill_rect(OledImage* dst, const OledRect* dstrect, uint8_t value)
{
    uint32_t numdstpages = dst->height/8;
    uint32_t dststartpage = dstrect->y/8;
    
    for( uint32_t p = 0; p < numdstpages; ++p )
    {
        uint32_t dstpage = dststartpage + p;
        if( dstpage >= numdstpages )
            break;

        for( uint32_t x = 0; x < dstrect->width; ++x)
        {
            uint32_t dstindex = dstpage * dst->width + dstrect->x + x;
            dst->img[dstindex] = value;
        }
    }
}


void oled_invert(bool invert)
{    
    Wire.beginTransmission(_oled_slave_address);
    Wire.write(OLED_CONTROL_BYTE_CMD_SINGLE);
    Wire.write(invert ? OLED_CMD_DISPLAY_INVERTED : OLED_CMD_DISPLAY_NORMAL);
    Wire.endTransmission();
}

void oled_constrast(uint8_t contrast)
{
    Wire.beginTransmission(_oled_slave_address);
    Wire.write(OLED_CONTROL_BYTE_CMD_STREAM);
    Wire.write(OLED_CMD_SET_CONTRAST);
    Wire.write(contrast);
    Wire.endTransmission();
}

#endif