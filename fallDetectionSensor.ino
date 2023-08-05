#include "Arduino.h"
#include <60ghzfalldetection.h>
#include "CTBot.h"

// data kredensial wifi
String ssid = "TIMIIDSMADA";  //nanti diisi
String pass = "smadaoke"; //nanti diisi
String token = "***";

const int buzzerPin = 2; // sesuaikan dengan pin

//#include <SoftwareSerial.h>
// Choose any two pins that can be used with SoftwareSerial to RX & TX
//#define RX_Pin A2
//#define TX_Pin A3

//SoftwareSerial mySerial = SoftwareSerial(RX_Pin, TX_Pin);

// we'll be using software serial
//FallDetection_60GHz radar = FallDetection_60GHz(&mySerial);

// can also try hardware serial with
FallDetection_60GHz radar = FallDetection_60GHz(&Serial1);

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);

  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);

  //  mySerial.begin(115200);

  while(!Serial);   //When the serial port is opened, the program starts to execute.

  Serial.println("Ready");
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  TBMessage msg;
  radar.Fall_Detection();           //Receive radar data and start processing
  if(radar.sensor_report != 0x00){
    switch(radar.sensor_report){
        case NOFALL:
            Serial.println("TIDAK JATUH");
            Serial.println("----------------------------");
            myBot.sendMessage(msg.sender.id, "TIDAK JATUH");
            digitalWrite(buzzerPin, LOW);
            break;
        case FALL:
            Serial.println("JATUH WOI!");
            Serial.println("----------------------------");
            myBot.sendMessage(msg.sender.id, "JATUH WOI!");
            digitalWrite(buzzerPin, HIGH); //buzzer bunyi
            break;
        case NORESIDENT:
            Serial.println("TIDAK ADA ORANG");
            Serial.println("----------------------------");
            myBot.sendMessage(msg.sender.id, "TIDAK ADA ORANG");
            digitalWrite(buzzerPin, LOW);
            break;
        case RESIDENCY:
            Serial.println("ADA ORANG");
            Serial.println("----------------------------");
            myBot.sendMessage(msg.sender.id, "ADA ORANG");
            digitalWrite(buzzerPin, LOW);
            break;
    }
  }
  delay(200);
}