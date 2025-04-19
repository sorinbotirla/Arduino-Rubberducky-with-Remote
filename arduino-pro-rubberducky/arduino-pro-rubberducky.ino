#include "SPI.h"
#include "SD.h"
#include <string.h>
#include <Keyboard.h>
#include<SoftwareSerial.h>

uint8_t SD_MISO = 14;
uint8_t SD_MOSI = 16;
uint8_t SD_SCLK = 15;
uint8_t SD_CS = 10;
File scriptFile;

// SoftwareSerial setup
SoftwareSerial SUART(9,6); // RX, TX
String readSerialString;


uint8_t BUTTON_PIN = 2;
uint32_t currentTime = millis();
byte buttonState;
int triggered = 0;


void executeScript(int scriptId = 1){
  // sendCommandReceived();

  Serial.println("will execute script  '/scripts/script"+((String) scriptId)+".txt'");
  scriptFile = SD.open("/scripts/script"+((String) scriptId)+".txt");
  if (scriptFile) {
    Keyboard.begin();
    
    String line = "";
    while (scriptFile.available()) {
      char m = scriptFile.read();
      if (m == '\n'){
        Line(line);
        line = "";
        }
        else if((int) m != 13)
        {
          line += m;
        }
    }
    Line(line);
    
    scriptFile.close();
  } else {
    Serial.println("Script '/scripts/script"+((String) scriptId)+".txt' not found");
  }
}


void Line(String l)
{
  int space_1 = l.indexOf(" ");
  if (space_1 == -1)
  {
    Press(l);
  }
  else if (l.substring(0,space_1) == "STRING")
  {
    Keyboard.print(l.substring(space_1 + 1));
  }
  else if (l.substring(0,space_1) == "DELAY")
  {
    int delaytime = l.substring(space_1 + 1).toInt();
    delay(delaytime);
  }
  else if(l.substring(0,space_1) == "REM"){}
  else
  {
      String remain = l;

      while(remain.length() > 0)
      {
        int latest_space = remain.indexOf(" ");
        if (latest_space == -1)
        {
          Press(remain);
          remain = "";
        }
        else
        {
          Press(remain.substring(0, latest_space));
          remain = remain.substring(latest_space + 1);
        }
        delay(5);
      }
  }

  Keyboard.releaseAll();
}

void Press(String b)
{
  if(b.length() == 1)
  {
    char c = b[0];
    Keyboard.press(c);
  }
  else if (b.equals("ENTER"))
  {
    Keyboard.press(KEY_RETURN);
  }
  else if (b.equals("CTRL"))
  {
    Keyboard.press(KEY_LEFT_CTRL);
  }
    else if (b.equals("SHIFT"))
  {
    Keyboard.press(KEY_LEFT_SHIFT);
  }
    else if (b.equals("ALT"))
  {
    Keyboard.press(KEY_LEFT_ALT);
  }
    else if (b.equals("GUI"))
  {
    Keyboard.press(KEY_LEFT_GUI);
  }
    else if (b.equals("UP") || b.equals("UPARROW"))
  {
    Keyboard.press(KEY_UP_ARROW);
  }
    else if (b.equals("DOWN") || b.equals("DOWNARROW"))
  {
    Keyboard.press(KEY_DOWN_ARROW);
  }
    else if (b.equals("LEFT") || b.equals("LEFTARROW"))
  {
    Keyboard.press(KEY_LEFT_ARROW);
  }
    else if (b.equals("RIGHT") || b.equals("RIGHTARROW"))
  {
    Keyboard.press(KEY_RIGHT_ARROW);
  }
    else if (b.equals("DELETE"))
  {
    Keyboard.press(KEY_DELETE);
  }
    else if (b.equals("PAGEUP"))
  {
    Keyboard.press(KEY_PAGE_UP);
  }
    else if (b.equals("PAGEDOWN"))
  {
    Keyboard.press(KEY_PAGE_DOWN);
  }
    else if (b.equals("HOME"))
  {
    Keyboard.press(KEY_HOME);
  }
    else if (b.equals("ESC"))
  {
    Keyboard.press(KEY_ESC);
  }
    else if (b.equals("INSERT"))
  {
    Keyboard.press(KEY_INSERT);
  }
    else if (b.equals("TAB"))
  {
    Keyboard.press(KEY_TAB);
  }
    else if (b.equals("END"))
  {
    Keyboard.press(KEY_END);
  }
    else if (b.equals("CAPSLOCK"))
  {
    Keyboard.press(KEY_CAPS_LOCK);
  }
    else if (b.equals("F1"))
  {
    Keyboard.press(KEY_F1);
  }
    else if (b.equals("F2"))
  {
    Keyboard.press(KEY_F2);
  }
    else if (b.equals("F3"))
  {
    Keyboard.press(KEY_F3);
  }
    else if (b.equals("F4"))
  {
    Keyboard.press(KEY_F4);
  }
    else if (b.equals("F5"))
  {
    Keyboard.press(KEY_F5);
  }
    else if (b.equals("F6"))
  {
    Keyboard.press(KEY_F6);
  }
    else if (b.equals("F7"))
  {
    Keyboard.press(KEY_F7);
  }
    else if (b.equals("F8"))
  {
    Keyboard.press(KEY_F8);
  }
    else if (b.equals("F9"))
  {
    Keyboard.press(KEY_F9);
  }
    else if (b.equals("F10"))
  {
    Keyboard.press(KEY_F10);
  }
    else if (b.equals("F11"))
  {
    Keyboard.press(KEY_F11);
  }
    else if (b.equals("F12"))
  {
    Keyboard.press(KEY_F12);
  }
    else if (b.equals("SPACE"))
  {
    Keyboard.press(' ');
  } 
}

void listenToSerial(){
    // delay(3);  //delay to allow buffer to fill 
    if (SUART.available() >0) {
      char c = SUART.read();  //gets one byte from serial buffer
      readSerialString += c; //makes the string readString
    } else {
      readSerialString = "";
    }

  if (readSerialString.length() >0) {
    // Serial.print(((String) readSerialString));
    if (readSerialString.equals("1")) {
      executeScript(1);
    } else if (readSerialString.equals("2")) {
      executeScript(2);
    } else if (readSerialString.equals("3")) {
      executeScript(3);
    }
  }
}



void setup() {
  pinMode(SD_CS, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  SUART.begin(9600);

  Serial.print("Initializing SD card...");  
  if (!SD.begin(SD_CS)) {
    Serial.println("initialization failed!");
    // while (1);
  } else {
    Serial.println("initialization done.");
  }
  Serial.println("done!");

}

void loop() {
  listenToSerial();
  buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == LOW) {
    if (triggered == false) {
      triggered = true;
      Serial.println("Button pressed");
      executeScript(1);
    }
  } else {
    triggered = false;
  }
}


