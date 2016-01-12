#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define floor1  32
#define floor2  31
#define floor3  30

#define up_floor1 36
#define up_floor2 35
#define down_floor2 34
#define down_floor3 33

#define LIMIT1  23
#define LIMIT2  25
#define LIMIT3  27

#define Red1  48
#define Red2  49
#define Red3  50
#define Green1  51
#define Green2  52
#define Green3  53

#define E_Button  2
#define E_Buzzer  8

#define ENB 5
#define IN3 6
#define IN4 7

LiquidCrystal_I2C lcd(0x3f, 16, 2);

int state = 0;

void setup()
{
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  pinMode(floor1, INPUT_PULLUP);
  pinMode(floor2, INPUT_PULLUP);
  pinMode(floor3, INPUT_PULLUP);

  pinMode(up_floor1, INPUT_PULLUP);
  pinMode(up_floor2, INPUT_PULLUP);
  pinMode(down_floor2, INPUT_PULLUP);
  pinMode(down_floor3, INPUT_PULLUP);

  pinMode(LIMIT1, INPUT_PULLUP);
  pinMode(LIMIT2, INPUT_PULLUP);
  pinMode(LIMIT3, INPUT_PULLUP);

  pinMode(E_Button, INPUT_PULLUP);
  pinMode(E_Buzzer, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(Red1, OUTPUT);
  pinMode(Red2, OUTPUT);
  pinMode(Red3, OUTPUT);
  pinMode(Green1, OUTPUT);
  pinMode(Green2, OUTPUT);
  pinMode(Green3, OUTPUT);
  
  life_down();
  
  attachInterrupt(0, Emergency, FALLING);
}

void loop()
{
  lcd.home();
  lcd.print("    Elevator   ");

  if(digitalRead(8) == 1)  {
    lcd.setCursor(0, 1);
    lcd.print("   EMERGENCY!  ");
    state = 0;
  }
  
  if (digitalRead(LIMIT1) == 0 && digitalRead(LIMIT2) == 1 && digitalRead(LIMIT3) == 1) {
    digitalWrite(8, LOW);
    digitalWrite(Green1, HIGH);
    digitalWrite(Green2, LOW);
    digitalWrite(Green3, LOW);
    digitalWrite(Red1, LOW);
    digitalWrite(Red2, HIGH);
    digitalWrite(Red3, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Floor : 1      ");
    life_stop();
  }

  if (digitalRead(floor1) == 0)  state = 1;
  if (digitalRead(floor2) == 0)  state = 2;
  if (digitalRead(floor3) == 0)  state = 3;
  if (digitalRead(up_floor1) == 0)  state = 1;
  if (digitalRead(up_floor2) == 0)  state = 2;
  if (digitalRead(down_floor2) == 0)  state = 2;
  if (digitalRead(down_floor3) == 0)  state = 3;

  while (state == 1) {
    if (digitalRead(LIMIT1) == 0 && digitalRead(LIMIT2) == 1 && digitalRead(LIMIT3) == 1) {
      digitalWrite(Green1, HIGH);
      digitalWrite(Green2, LOW);
      digitalWrite(Green3, LOW);
      digitalWrite(Red1, LOW);
      digitalWrite(Red2, HIGH);
      digitalWrite(Red3, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("Floor : 1      ");
      life_stop();
      state = 0;
    }
    if (digitalRead(LIMIT1) == 1 && digitalRead(LIMIT2) == 0 && digitalRead(LIMIT3) == 1) {
      lcd.setCursor(0, 1);
      lcd.print("Floor : 2 --> 1");
      life_down();
    }
    if (digitalRead(LIMIT1) == 1 && digitalRead(LIMIT2) == 1 && digitalRead(LIMIT3) == 0) {
      lcd.setCursor(0, 1);
      lcd.print("Floor : 3 --> 1");
      life_down();

    }
  }

  while (state == 2) {
    if (digitalRead(LIMIT1) == 0 && digitalRead(LIMIT2) == 1 && digitalRead(LIMIT3) == 1) {
      lcd.setCursor(0, 1);
      lcd.print("Floor : 1 --> 2");
      life_up() ;
    }
    if (digitalRead(LIMIT1) == 1 && digitalRead(LIMIT2) == 0 && digitalRead(LIMIT3) == 1) {
      digitalWrite(Green1, LOW);
      digitalWrite(Green2, HIGH);
      digitalWrite(Green3, LOW);
      digitalWrite(Red1, HIGH);
      digitalWrite(Red2, LOW);
      digitalWrite(Red3, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("Floor : 2      ");
      life_stop();
      state = 0;
    }
    if (digitalRead(LIMIT1) == 1 && digitalRead(LIMIT2) == 1 && digitalRead(LIMIT3) == 0) {
      lcd.setCursor(0, 1);
      lcd.print("Floor : 3 --> 2");
      life_down();
    }
  }
  while (state == 3) {
    if (digitalRead(LIMIT1) == 0 && digitalRead(LIMIT2) == 1 && digitalRead(LIMIT3) == 1) {
      lcd.setCursor(0, 1);
      lcd.print("Floor : 1 --> 3");
      life_up() ;
    }
    if (digitalRead(LIMIT1) == 1 && digitalRead(LIMIT2) == 0 && digitalRead(LIMIT3) == 1) {
      lcd.setCursor(0, 1);
      lcd.print("Floor : 2 --> 3");
      life_up() ;
    }
    if (digitalRead(LIMIT1) == 1 && digitalRead(LIMIT2) == 1 && digitalRead(LIMIT3) == 0) {
      digitalWrite(Green1, LOW);
      digitalWrite(Green2, LOW);
      digitalWrite(Green3, HIGH);
      digitalWrite(Red1, HIGH);
      digitalWrite(Red2, HIGH);
      digitalWrite(Red3, LOW);
      lcd.setCursor(0, 1);
      lcd.print("Floor : 3      ");
      life_stop();
      state = 0;
    }
  }
}

void life_up()  {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, 255);
}

void life_down()  {
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 255);
}

void life_stop()  {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
}

void Emergency() {
  digitalWrite(Green1, LOW);
  digitalWrite(Green2, LOW);
  digitalWrite(Green3, LOW);
  digitalWrite(Red1, HIGH);
  digitalWrite(Red2, HIGH);
  digitalWrite(Red3, HIGH);
  digitalWrite(8, HIGH);
  life_down();
  state = 0;
}

