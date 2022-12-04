#include <Adafruit_GFX.h>
#include <gfxfont.h>
GFXcanvas1 maskCanvas = GFXcanvas1(48, 12); // make a standard Adafruit 1-bit canvas that matches our mask


void setup() {
  Serial.begin(115200);  // computer serial debugging interface
  Serial0.begin(38400);  // mask interface uses 38.4kbit/s serial
}


int y=0;
void loop() {
  usleep(300000);  // wait 50ms

  y=(y+1)%11;

  maskCanvas.fillScreen(0);                 // color 1 is off


  

    for (int i=-10;i<10;i++)
    {
      int offset = cos(i/7.0)*8;
      offset=offset*((double)y-5)/10;
      maskCanvas.drawPixel(24+i,6-offset,1);
      maskCanvas.drawPixel(24+i,4-offset,1);
      maskCanvas.drawPixel(24+i,5-offset,1);
    }



  sendCanvasToMask();
}

void setBrightness(uint8_t brightness)
{
  
  uint8_t buffer[3];  // init a 3 byte packet
  buffer[0] = 3;      // maybe our preamble is actually the packet length
  buffer[1] = brightness;
  buffer[2] = buffer[0] +buffer[1];
  Serial0.write(buffer, 3);  // offload the fiddly bit to the ESP32 UART
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

  Serial0.write(buffer, 74);  // offload the fiddly bit to the ESP32 UART
}