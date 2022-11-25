#include <Adafruit_GFX.h>
#include <gfxfont.h>
GFXcanvas1 maskCanvas = GFXcanvas1(48, 12); // make a standard Adafruit 1-bit canvas that matches our mask


void setup() {
  Serial.begin(115200);  // computer serial debugging interface
  Serial2.begin(38400);  // mask interface uses 38.4kbit/s serial
}

void loop() {
  usleep(50000);  // wait 50ms

  maskCanvas.fillScreen(1);                 // color 1 is on
  maskCanvas.drawLine(0, 0, 11, 11, 0);     // color 0 is off
  maskCanvas.drawLine(12, 11, 23, 0, 0);
  maskCanvas.drawLine(24, 0, 35, 11, 0);
  maskCanvas.drawLine(36, 11, 47, 0, 0);
  sendCanvasToMask();
}


void sendCanvasToMask() 
{

  uint8_t buffer[74];  // init a 74 byte packet
  buffer[0] = 74;      // maybe our preamble is actually the packet length

  for (int i = 1; i < 73; i++) 
  {
    buffer[i] = 0;
    // mask pattern is kinda weird with three bytes making up every two line-block on the display
    uint8_t lineBlock = (i - 1) / 3;
    uint8_t byteOfLineBlock = (i - 1) % 3;

    if ((byteOfLineBlock == 0) || (byteOfLineBlock == 2)) 
    {
      // bytes 0 and 2 contain a whole vertical scan-line
      uint8_t x = lineBlock * 2 + byteOfLineBlock / 2;
      for (uint8_t y = 0; y < 8; y++) 
      {
        if (maskCanvas.getPixel(x, y)) 
        {
          bitSet(buffer[i], 7 - y);
        }
      }
    } else 
    {
      // This is the byte that contains 2 half lines
      uint8_t x = lineBlock * 2;
      for (uint8_t y = 8; y < 12; y++) 
      {
        if (maskCanvas.getPixel(x + 1, y)) 
        {
          bitSet(buffer[i], 11 - y);
        }
        if (maskCanvas.getPixel(x, y)) 
        {
          bitSet(buffer[i], 15 - y);
        }
      }
    }
  }
  // calculate checksum
  buffer[73] = 0;
  for (int i = 0; i < 73; i++) {
    buffer[73] += buffer[i];
  }

  Serial2.write(buffer, 74);  // offload the fiddly bit to the ESP32 UART
}