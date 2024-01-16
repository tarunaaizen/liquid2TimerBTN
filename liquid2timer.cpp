#include <LiquidCrystal_Hitachi.h>
LiquidCrystal_Hitachi lcd(0x27, 16, 2);
uint8_t clock[8] = {0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0};
const int switch_on = 2, input_on_time = 3, input_off_time = 4, output = 9;
int timer_start = 0, ontime = 0, offtime = 0;
String device_status = "OFF";
void setup() {
  pinMode(input_on_time, INPUT);
  pinMode(input_off_time, INPUT);
  pinMode(output, OUTPUT);
  lcd.begin();
  lcd.backlight();
  lcd.createChar(2, clock);
  lcd.setCursor(8, 1);
  lcd.write(2);
  lcd.print((String)" OFF" + offtime + " ");
  lcd.setCursor(0, 1);
  lcd.write(2);
  lcd.print((String)" ON" + ontime + " ");
  lcd.setCursor(7, 0);
  lcd.print("OFF       ");
}

void loop() {
  int x = ((millis() / 1000) - timer_start)/60;
  if (x <= ontime && device_status == "TO_ON") {
    int m = ((ontime*60) + timer_start - (millis() / 1000))/60;
    int s = ((ontime*60) + timer_start - (millis() / 1000))%60;
    lcd.setCursor(0, 0);//-------------------------------------------------------------oncounter 1-0 10-0
    lcd.print((String)" " + m + ":" + s + " ");
  } else if (x - ontime <= offtime && device_status == "TO_OFF") {
    int m = ((offtime*60) + timer_start + (ontime*60) - (millis() / 1000))/60;
    int s = ((offtime*60) + timer_start + (ontime*60) - (millis() / 1000))%60;
    lcd.setCursor(10, 0);//-------------------------------------------------------------offcounter
    lcd.print((String)" " + m + ":" + s + " ");
  }
  if (digitalRead(switch_on) == HIGH) {
    timer_start = millis() / 1000;
    device_status = "TO_ON";
  }
  else if (x - ontime >= offtime  && device_status == "TO_OFF") {
    digitalWrite(output, LOW);
    lcd.setCursor(6, 0);
    lcd.print("OFF       ");
    device_status = "OFF";
  } else if (x >= ontime && device_status == "TO_ON") {
    digitalWrite(output, HIGH);
    lcd.setCursor(6, 0);
    lcd.print("ON        ");
    device_status = "TO_OFF";
  }
  while (digitalRead(input_on_time) == HIGH) {
    if (ontime <= 24) {
      ontime += 1;
    } else {
      ontime = 0;
    }
    lcd.setCursor(0, 1);//---------------------------------onsetting
    lcd.write(2);
    lcd.print((String)" ON" + ontime + " ");
    delay(500);
  }
  while (digitalRead(input_off_time) == HIGH) {
    if (offtime <= 24) {
      offtime += 1;
    } else {
      offtime = 0;
    }
    lcd.setCursor(8, 1);//---------------------------------offsetting
    lcd.write(2);
    lcd.print((String)" OFF" + offtime + " ");
    delay(500);
  }
}