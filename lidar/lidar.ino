#include <M5Stack.h>

void setup(){
    m5.begin();
    Serial.begin(115200);  
    Serial1.begin(230400, SERIAL_8N1, 16, 2);  //Lidar
    Serial2.begin(115200);  
}

int commandStatus = 0;
uint16_t distance[360];
uint8_t signalValue[360];
uint16_t temp;
int ver = 0;
int lidarSpeed = 0;
uint16_t dataLength = 0;
uint16_t angleOffset = 0;
uint16_t startAngle = 0;
int packCount = 0;
// showAngle = 0;


//！雷达数据获取
void GetData(void){

  while(Serial1.available()){
    uint16_t r = Serial1.read();
    switch (commandStatus){
      case 0: if (r == 0xAA){
                commandStatus = 1;
              }else{
                commandStatus = 0;
              }
              break;
      case 1: if(r == 0x00) {
               commandStatus = 2;
               }
              else{
                commandStatus = 0;
              }
              break;
       case 2:commandStatus = 3;break;
       case 3: ver = r; commandStatus = 4; break;
       case 4:if(r == 0x61){
                commandStatus = 5;
              }
              else{
                commandStatus = 0;
              }
              break;
        case 5:if(r == 0xAD){
                commandStatus = 6;
               }else{
                commandStatus = 0;
               }
               break;
        case 6: commandStatus = 7;  break;
        case 7: dataLength = (r - 5) / 3; commandStatus = 8; break;
        case 8: lidarSpeed = r;  commandStatus = 9; break;
        case 9: angleOffset = r * 256; commandStatus = 10; break;
        case 10: angleOffset += r; commandStatus = 11;  break;
        case 11: startAngle = r * 256; commandStatus = 12; break;
        case 12: startAngle += r; commandStatus = 13;  break;

    default:
        if (commandStatus == ( 14 + 3 * 44)) //finish.
        {
          packCount++;
          commandStatus = 0;
          return;
        }
        switch ((commandStatus - 13) % 6)
        {
          case 0: signalValue[(startAngle / 2250) * 22 + ((commandStatus - 13) / 6)] = r;  commandStatus++; break;
          case 1: temp = r * 256;  commandStatus++; break;
          case 2: temp += r; distance[(startAngle / 2250) * 22 + ((commandStatus - 13) / 6)] = temp > 2 ? temp : 250;  commandStatus++; break;
          case 3: commandStatus++; break;
          case 4: commandStatus++; break;
          case 5: commandStatus++; break;
        } break;
    }
  }
}

void loop(){
    GetData();
    Serial.print(distance[0]);
    Serial.print(", ");
    Serial.print(distance[90]);
    Serial.print(", ");
    Serial.print(distance[180]);
    Serial.print(", ");
    Serial.print(distance[270]);
    Serial.print(", ");
    Serial.println("");
}