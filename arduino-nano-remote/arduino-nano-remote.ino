
#include <SPI.h> // Not actually used but needed to compile
#include <nRF24L01.h>
#include <RF24.h>
uint8_t BTN1_PIN = 2;
uint8_t BTN2_PIN = 3;
uint8_t BTN3_PIN = 4;
uint8_t LED_TX_PIN = 8;
uint8_t LED_RX_PIN = 9;
uint32_t currentTimeLed;
byte buttonState1;
byte buttonState2;
byte buttonState3;
int triggered = 0;
int ledTxState = 0;
int ledRxState = 0;

int listening = 0;


RF24 radio(6,10); // CE, CSN

const byte addresses [][6] = {"00001", "00002"}; //Setting the two addresses. One for  transmitting and one for receiving

void setup()
{
    Serial.begin(9600);	  // Debugging only
    pinMode(BTN1_PIN, INPUT_PULLUP);
    pinMode(BTN2_PIN, INPUT_PULLUP);
    pinMode(BTN3_PIN, INPUT_PULLUP);
    pinMode(LED_TX_PIN, OUTPUT);
    pinMode(LED_RX_PIN, OUTPUT);

    radio.begin();
    // radio.openWritingPipe(address);
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1, addresses[1]);

    radio.setPALevel(RF24_PA_MIN);
      
    digitalWrite(LED_TX_PIN, HIGH);
    digitalWrite(LED_RX_PIN, HIGH);
    currentTimeLed = millis();
    ledTxState = 1;
    ledRxState = 1;
}

void sendRemoteScriptCommand (char scriptId = 1){
      radio.stopListening();
      listening = 0;
      int msg[2];
      msg[0] = scriptId;
      radio.write(&msg, sizeof(msg));
      digitalWrite(LED_TX_PIN, HIGH);    
      currentTimeLed = millis();
      ledTxState = 1; 
}

void listenForResponse(){
  // Serial.println("listening");
  if (listening == 0) {
    radio.startListening();
    listening = 1;
    Serial.println("listening");
  }
  
  if (radio.available()) {
    int msg[2];
    radio.read(&msg, sizeof(msg));
    String text = (String) String(msg[0]);
    Serial.println("received "+text);

    if (text.equals("4")) {
      Serial.println("Command received");
      digitalWrite(LED_RX_PIN, HIGH);
      currentTimeLed = millis();
      ledRxState = 1;
    }
  }
}


void loop()
{
  listenForResponse();
  // nothing happens after setup finishes.
  buttonState1 = digitalRead(BTN1_PIN);
  buttonState2 = digitalRead(BTN2_PIN);
  buttonState3 = digitalRead(BTN3_PIN);
  
  if (buttonState1 == LOW
  || buttonState2 == LOW
  || buttonState3 == LOW) {
      if (triggered == 0) {
        triggered = 1;
        if (buttonState1 == LOW) {
          Serial.println("Button 1 is pressed");
          sendRemoteScriptCommand(1);
        } else if (buttonState2 == LOW) {
          Serial.println("Button 2 is pressed");
          sendRemoteScriptCommand(2);
        } else if (buttonState3 == LOW) {
          Serial.println("Button 3 is pressed");
          sendRemoteScriptCommand(3);
        }       
      }
  }
  else {
      // Serial.println("Button is not pressed");
      triggered = 0;
  }
  
  if (millis() - currentTimeLed > 300) {
    if (ledRxState == 1) {
      digitalWrite(LED_RX_PIN, LOW);
    }
    if (ledTxState == 1) {
      digitalWrite(LED_TX_PIN, LOW);
    }
  }
}
