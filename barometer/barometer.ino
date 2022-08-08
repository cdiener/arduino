/**
 *   Attiny85 PINS (i2c)
 *             ____
 *   RESET   -|_|  |- 3V
 *   SCL (3) -|    |- (2)
 *   SDA (4) -|    |- (1)
 *   GND     -|____|- (0)
 *
 *   Attiny SPI PINS:     connect LCD to D4 (D/C), GND (CS), D3 (RES), D1(DIN), D2(CLK)
 *
 *   Nano/Atmega328 PINS: connect LCD to A4/A5 (i2c)
 *   ESP8266: GPIO4(SDA) / GPIO5( SCL )
 *   STM32: B7(SDA), B6(SCL)
 */

#define LOGI 120

#include <lcdgfx.h>
#include <Adafruit_BMP085.h>

DisplaySSD1306_128x64_I2C display(-1);
Adafruit_BMP085 bmp;

static char message[16];
static float temp, mb;
static int32_t pr;
char pstr[6];
char tstr[6];
uint32_t seconds = 0;
uint32_t history[16];

void setup()
{
    /* Select the font to use with menu and all font functions */
    Serial.begin(9600);
    bmp.begin();
    Serial.println("hello");
    display.setFixedFont(ssd1306xled_font6x8);
    display.begin();
    display.clear();
    display.printFixedN(0, 0, "> ", STYLE_NORMAL, 1);
    display.printFixedN(12*8, 0, "C", STYLE_NORMAL, 1);
    display.printFixedN(0, 16, "> ", STYLE_NORMAL, 1);
    display.printFixedN(12*8, 16, "mb", STYLE_NORMAL, 1);

    for (uint32_t i=0; i<16; i++) {
      history[i] = 100000;
    }
}


void loop()
{   
    temp = bmp.readTemperature() - 1.5;
    pr = bmp.readPressure();
    mb = ((float) pr) / 100.0;
    dtostrf(temp, 5, 2, tstr);
    dtostrf(mb, 5, 0, pstr);

    Serial.print(temp);
    Serial.print(", ");
    Serial.println(mb);

    
    display.printFixedN(3*6, 0, tstr, STYLE_NORMAL, 1);
    display.printFixedN(3*6, 16, pstr, STYLE_NORMAL, 1);

    if (seconds == 0) {
      seconds = LOGI;
      update_history(pr); 
    }

    seconds--;
    lcd_delay(1000);
}

void update_history(float p) {
    uint32_t low=1000000000;
    uint32_t high=0;
    uint32_t height;
    for(uint32_t i=0; i<15; i++) {
      history[i] = history[i+1];
      low = min(low, history[i]);
      high = max(high, history[i]);
    }
    history[15] = p;
    low = min(low, p);
    high = max(high, p);

    display.setColor(0);
    display.fillRect(0, 32, 128, 64);
    display.setColor(0xFF);    
  
    for(uint32_t i=0; i<16; i++) {
      height = 64 - map(history[i], low - 100, high + 100, 0, 30);
      display.fillRect(
        i*8,
        height,
        i*8 + 8,
        62
      );
    }
}
