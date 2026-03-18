/*
 * Project name: T-rex-duino
 * Description: T-rex game from Chrome brower rewritten for Arduino
 * Project page: https://github.com/AlexIII/t-rex-duino
 * Author: github.com/AlexIII
 * E-mail: endoftheworld@bk.ru
 * License: MIT
*/ 

#ifndef _SSD1306_H_
#define _SSD1306_H_

// Fundamental commands
#define OLED_DISPLAY_OFF            0xAE
#define OLED_DISPLAY_ON             0xAF
#define OLED_ENTIRE_DISPLAY_ON      0xA5
#define OLED_RESUME_TO_RAM          0xA4
#define OLED_NORMAL_DISPLAY         0xA6
#define OLED_INVERT_DISPLAY         0xA7

// Addressing
#define OLED_SET_MEMORY_MODE        0x20
  #define OLED_ADDR_HORIZONTAL      0x00
  #define OLED_ADDR_VERTICAL        0x01
  #define OLED_ADDR_PAGE            0x02

#define OLED_SET_COLUMN_ADDR        0x21
#define OLED_SET_PAGE_ADDR          0x22

// Layout / mapping
#define OLED_SET_START_LINE         0x40  // OR with 0–63
#define OLED_SET_SEGMENT_REMAP_0    0xA0
#define OLED_SET_SEGMENT_REMAP_1    0xA1
#define OLED_SET_COM_SCAN_INC       0xC0
#define OLED_SET_COM_SCAN_DEC       0xC8

#define OLED_SET_DISPLAY_OFFSET     0xD3
#define OLED_SET_COM_PINS           0xDA

// Timing & driving
#define OLED_SET_DISPLAY_CLOCK_DIV  0xD5
#define OLED_SET_PRECHARGE          0xD9
#define OLED_SET_VCOM_DETECT        0xDB
#define OLED_SET_MULTIPLEX          0xA8

// Contrast
#define OLED_SET_CONTRAST           0x81

// Charge pump
#define OLED_CHARGE_PUMP            0x8D
  #define OLED_CHARGE_PUMP_DISABLE  0x10
  #define OLED_CHARGE_PUMP_ENABLE   0x14

template<class SPI_TYPE>
class SSD1306 {
  SPI_TYPE &spi;
  const uint8_t cs;
  const uint8_t dc;
  const uint8_t res;
  const uint16_t screenBufferSize;
public:
  enum AddressingMode {
    HorizontalAddressingMode = 0x00,
    VerticalAddressingMode = 0x01,
    PageAddressingMode = 0x02,
  };
  SSD1306(SPI_TYPE &spi, const uint8_t cs, const uint8_t dc, const uint8_t res, const uint16_t screenBufferSize): 
    spi(spi), cs(cs), dc(dc), res(res), screenBufferSize(screenBufferSize) {}
  void begin() {
    pinMode(cs, OUTPUT);
    digitalWrite(cs, HIGH);
    pinMode(dc, OUTPUT);
    pinMode(res, OUTPUT);
    digitalWrite(res, LOW);
    delay(50);
    digitalWrite(res, HIGH);
    spi.begin();
    init();
  }
  void fillScreen(const uint8_t* buffer) {
    sendd(buffer, screenBufferSize);
  }
  void fillScreen(const uint8_t* buffer, const uint16_t size, const uint8_t stride = 0) {
    sendd(buffer, size, stride);
  }
  void setInverse(const bool v) {
    sendc(v? 0xA7 : 0xA6);
  }
  void setAddressingMode(const AddressingMode addressingMode) {
    sendc(0x20, addressingMode);
  }
private:
  void init() {

    sendc(OLED_DISPLAY_OFF);

    sendc(OLED_SET_DISPLAY_CLOCK_DIV, 0x80);
    sendc(OLED_SET_MULTIPLEX, 0x3F);          // 1/64 duty

    sendc(OLED_SET_DISPLAY_OFFSET, 0x00);
    sendc(OLED_SET_START_LINE | 0x00);

    sendc(OLED_CHARGE_PUMP, OLED_CHARGE_PUMP_ENABLE);

    sendc(OLED_SET_MEMORY_MODE, OLED_ADDR_VERTICAL);

    sendc(OLED_SET_SEGMENT_REMAP_1);          // mirror X
    sendc(OLED_SET_COM_SCAN_DEC);             // mirror Y

    sendc(OLED_SET_COM_PINS, 0x12);

    sendc(OLED_SET_CONTRAST, 0x7F);
    sendc(OLED_SET_PRECHARGE, 0xF1);
    sendc(OLED_SET_VCOM_DETECT, 0x40);

    sendc(OLED_RESUME_TO_RAM);
    sendc(OLED_NORMAL_DISPLAY);

    sendc(OLED_DISPLAY_ON);
  }

  /* send bytes */
  void sendc(const uint8_t c1) {
    sendBytes(&c1, 1, false);
  }
  void sendc(const uint8_t c1, const uint8_t c2) { 
    const uint8_t c[] = {c1, c2}; 
    sendBytes(c, sizeof(c), false);
  }
  void sendc(const uint8_t c1, const uint8_t c2, const uint8_t c3) { 
    const uint8_t c[] = {c1, c2, c3}; 
    sendBytes(c, sizeof(c), false);
  }
  void sendd(const uint8_t* d, uint16_t sz, const uint8_t stride = 0) {
    sendBytes(d, sz, true, stride);
  }
  void sendBytes(const uint8_t* d, uint16_t sz, const bool data, const uint8_t stride = 0) {
    digitalWrite(dc, data);
    digitalWrite(cs, LOW);
    if(!stride)
      while(sz--) spi.transfer(*d++);
    else
      for(uint8_t i = 0; i < stride; ++i)
        for(uint16_t j = i; j < sz; j += stride)
          spi.transfer(d[j]);
    digitalWrite(cs, HIGH);
  }
};

#endif
