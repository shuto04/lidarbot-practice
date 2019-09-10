#include <M5Stack.h>

void setup(){
    m5.begin();
    Serial2.begin(115200);  
}

void setLedColor(byte i, byte r, byte g, byte b){
    Serial2.write(0xAB);
    Serial2.write(i);
    Serial2.write(r);
    Serial2.write(g);
    Serial2.write(b);
    Serial2.write(0x55);
}

void loop(){
  for (int i = 0; i < 16; i++)
  {
    setLedColor(i, 20, 20, 20);
    delay(15);
  }
  for (int i = 0; i < 16; i++)
  {
    setLedColor(i, 0, 0, 0);
    delay(15);
  }
}