#include <LiquidCrystal.h>
#include <limits.h>

LiquidCrystal lcd(7, 6, 2, 3, 4, 5);
int r=0, g=0, b=0, h=0, m=0, s=0, curH, curM, curS;
char buf[32];
int melody[] = {330, 294, 262, 294, 330, 330, 330, 294, 294, 294, 330, 392, 392, 330, 294, 262, 294, 330, 330, 330, 294, 294, 330, 294, 262};
int pageNum=0;
unsigned long alarmTime = 0, preTime = ULONG_MAX, curTime, totalTime = ULONG_MAX;

void PrintMain(); // 메인 화면 출력 함수
void PrintLedValue(); // led rgb값 출력 함수
void PrintTime(); // 타이머 시간 출력 함수
void PageControl(); // lcd 화면 컨트롤 함수
void LedControl(); // led 색상 조절 함수
void AlarmControl(); // 알람 컨트롤 함수
void StartAlarm(); // 알람 시작 함수
void StartLed(); // led 작동 함수
void PrintTimer(); // 타이머 남은 시간 출력 함수
void CountTime(); // 타이머 시간을 체크하는 함수

void setup() {
  pinMode(10, INPUT);
  pinMode(9, INPUT);
  pinMode(8, INPUT);

  lcd.begin(16, 2);
}

void loop() {
  CountTime();
  PageControl();
  StartLed();
}

void CountTime(){
  curTime = millis();
  if((alarmTime / 1000) * 0.9 > totalTime){
    r=0;
    g=0;
    b=0;
    StartLed();
  }
  if (curTime - preTime >= alarmTime && alarmTime != 0) {
    r=255;
    g=255;
    b=255;
    StartLed();
    StartAlarm();
    alarmTime=0;
    h=0;
    m=0;
    s=0;
  }
  if(totalTime == 0){
    totalTime = ULONG_MAX;
  }
}

void PrintMain(){
  lcd.clear();
  lcd.print("Smart Mood Light");
  delay(100);
}

void PrintLedValue(){
  lcd.clear();
  lcd.print("Set Led");
  lcd.setCursor(0, 1);
  lcd.print("R:");
  lcd.print(r);
  lcd.print("G:");
  lcd.print(g);
  lcd.print("B:");
  lcd.print(b);
  delay(100);
}

void PrintTime(){
  lcd.clear();
  lcd.print("Set Timer");
  lcd.setCursor(0, 1);
  lcd.print("H:");
  lcd.print(h);
  lcd.print(" M:");
  lcd.print(m);
  lcd.print(" S:");
  lcd.print(s);
  delay(100);
}

void PageControl(){
  if(digitalRead(9)==HIGH && digitalRead(10)==HIGH){
    pageNum--;
    if(pageNum<0){
      pageNum=2;
    }
    delay(500);
  }
  if(digitalRead(8)==HIGH && digitalRead(10)==HIGH){
    pageNum++;
    if(pageNum>2){
      pageNum=0;
    }
    delay(500);
  }

  if(pageNum == 0){
    if(alarmTime == 0){
      PrintMain();
    }
    else{
      PrintTimer();
    }
  }
  else if(pageNum == 1){
    LedControl();
  }
  else if(pageNum == 2){
    AlarmControl();
  }
}

void LedControl(){
  if(digitalRead(8)==HIGH){
    b+=20;
    if(b>255){
      b=0;
    }
  }

  if(digitalRead(10)==HIGH){
    g+=20;
    if(g>255){
      g=0;
    }
  }

  if(digitalRead(9)==HIGH){
    r+=20;
    if(r>255){
      r=0;
    }
  }

  PrintLedValue();
}

void AlarmControl(){
  if(digitalRead(8)==HIGH){
    s++;
    if(s>59){
      s=0;
    }
  }

  if(digitalRead(10)==HIGH){
    m++;
    if(m>59){
      m=0;
    }
  }

  if(digitalRead(9)==HIGH){
    h++;
    if(h>24){
      h=0;
    }
  }
  alarmTime = (((unsigned long)h*3600)+((unsigned long)m*60)+s)*1000;
  preTime = millis();
  PrintTime();
}

void StartAlarm(){
  while(1){
      for(int i=0; i<25; i++){
        if(digitalRead(8)==HIGH || digitalRead(9)==HIGH || digitalRead(10)==HIGH){
          noTone(A0);
          return;
        }
        tone(A0, melody[i], 250);
        delay(400);
        noTone(A0);
    }
    delay(1000);
  }
}

void StartLed(){
  analogWrite(12, r);
  analogWrite(13, g);
  analogWrite(11, b);
  delay(10);
}

void PrintTimer(){
  totalTime = (alarmTime - (curTime - preTime))/1000;
  curH = totalTime/3600;
  curM = (totalTime%3600)/60;
  curS = (totalTime%3600)%60;
  
  lcd.clear();
  lcd.print("Time Remaining");
  lcd.setCursor(0, 1);
  lcd.print("H:");
  lcd.print(curH);
  lcd.print(" M:");
  lcd.print(curM);
  lcd.print(" S:");
  lcd.print(curS);
  delay(100);
}