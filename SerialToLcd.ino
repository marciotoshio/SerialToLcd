// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

String message = "";
int textSpeed = 250;
int lcdCols = 16;
int lcdLines = 2;

void setup(){
  lcd.begin(lcdCols, lcdLines);
  
  Serial.begin(9600);
  waitConnection();
}

void loop()
{
  if (Serial.available()) {
    resetLcd(16, 0);
    getMessage();
    printMessageToLcd();
    Serial.println("message received");
    message = "";
  }else {
    resetLcd(0, 0);
    showStaticMessageInLcd("Waiting", 0);
    showStaticMessageInLcd("tweets", 1);
    waitWithAMessage("Waiting tweets");
  }
}

void waitConnection() {
  resetLcd(0, 0);
  showStaticMessageInLcd("Waiting", 0);
  showStaticMessageInLcd("connection...", 1);
  waitWithAMessage("Waiting connection");
}

void waitWithAMessage(String message) {
  while (Serial.available() <= 0) {
    Serial.println(message);
    delay(300);
  }
}

void showStaticMessageInLcd(String msg, int line) {
  lcd.setCursor(0,line);
  lcd.print(msg);
}

void getMessage() {
  boolean eom = false;
  while (Serial.available()) {
    char c = Serial.read();
    if(c == '\n') {
      eom = true;
    }
    else {
      message += c;
    }
  }
  
  if(!eom) {
    Serial.println("next chunk");
    delay(500);
    getMessage();
  }
}

void printMessageToLcd() {
  for(int i = 0; i < message.length(); i++) {
    writeAndScrollLeft(message[i]);
  }
  for(int j = 0; j < lcdCols; j++) {
    writeAndScrollLeft(' ');
  }
}

void resetLcd(int col, int line) {
  lcd.clear();
  lcd.setCursor(col, line);
}

void writeAndScrollLeft(char c) {
  lcd.write(c);
  lcd.scrollDisplayLeft(); 
  delay(textSpeed);
}


