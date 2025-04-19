#include "SPI.h"
#include <string.h>
#include "SD.h"
#include <nRF24L01.h>
#include <RF24.h>
#include<SoftwareSerial.h>
uint8_t SD_MISO = 12;
uint8_t SD_MOSI = 11;
uint8_t SD_SCLK = 13;
uint8_t SD_CS = 4;

uint32_t currentTime = millis();

SoftwareSerial SUART(6, 10); // RX 7 TX 8 - software serial, not hardware tx/rx
RF24 radio(7,8); // CE, CSN
const byte   addresses [][6] = {"00001", "00002"};

void executeScript(int scriptId = 1){
  // sendCommandReceived();
    SUART.println(scriptId);
}

void listenForRemote(){
  if (radio.available()) {
    int msg[2];
    radio.read(&msg, sizeof(msg));
    String text = (String) String(msg[0]);

    if (text.equals("1") || text.equals("2") || text.equals("3")) {
      Serial.println(text);
      SUART.println(text); 
      delay(500);
      sendCommandReceived();
    }
  }
}

void sendCommandReceived(){
    Serial.println("Sending command received");
    radio.stopListening();
    int msg[2];
    msg[0] = 4;
    radio.write(&msg, sizeof(msg));
    delay(500);
    radio.startListening();
}

void setup() {
  Serial.begin(9600);
  SUART.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);
  radio.setPALevel(RF24_PA_MIN); 
  radio.startListening();
}

void loop() {
  listenForRemote();
}
