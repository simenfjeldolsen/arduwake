#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

//Porter
const int sendSignalTilSpill = 10;
const int knapp = 11;
const int timePort = A4;
const int minuttPort = A5;

// LCD
LiquidCrystal lcd(9, 5, 4, 6, 7, 8);

//Xbee
SoftwareSerial xbee(2, 3);

// Et sekund i millisekunder.
const long SEKUND = 1000;

// Et minutt i millisekunder.
const long MINUTT = SEKUND * 60;

// En time i millisekunder.
const long TIME = MINUTT * 60;

// Hvor lang tid kaffen skal startes før alarmen.
const long tidVentPaaKaffe = 5 * MINUTT;

// Alarmens tid i millisekunder
long alarmTid;

//Alarmens tid i timer og minutter.
long timer;
long minutter;

// Resetter arduinoen.
void(* reset) (void) = 0;

void setup() {
  
  Serial.begin(9600);
  
  //Xbee
  xbee.begin(9600);
  
  //LCD
  lcd.begin(16, 2);
  
  //Pin-mode
  pinMode(sendSignalTilSpill, OUTPUT);
}

void loop() {
  
  // Leser potentiometerene og mapper tiden til 24t og 60m.
  timer = map(analogRead(timePort), 0, 1023, 0, 24);
  minutter = map(analogRead(minuttPort), 0, 1023, 0, 4) * 15;
  
  Serial.print(timer);
  Serial.print(" : ");
  Serial.println(minutter);
  
  // Skrive tiden til LCD.
  lcd.setCursor(0, 0);
  lcd.print("   TIME  MINUTT");
  lcd.setCursor(4, 1);
  
  // Skrive timer.
  if (timer < 10) {
    lcd.print(" ");
    lcd.print(timer);
  } else {
    lcd.print(timer);
  }
  
  lcd.print("  :  ");
  
  // Skrive minutter.
  if (minutter == 0) {
    lcd.print(" ");
    lcd.print(minutter);
  } else {
    lcd.print(minutter);
  }
  
  // Starter alarmen.
  if (digitalRead(knapp) == HIGH && (millis() > SEKUND)) {
    alarm();
  }
}

void alarm() {
  
  Serial.println("alarm()");
  
  // Hvor lang tid som har gått siden alarmen startet.
  long tid;
  
  // Om kaffemaskinene har blitt startet elelr ikke.
  boolean startetKaffe = false;
  
  // Antall skeunder, minutter og timer igjen.
  long nedtellingSekund;
  long nedtellingMinutt;
  long nedtellingTime;
  
  lcd.clear();
  lcd.setCursor(1, 0);  
  lcd.print("Alarm satt til");
  Serial.println("Alarm satt til");
  Serial.print(timer);
  Serial.print(" : ");
  Serial.print(minutter);
  lcd.setCursor(3, 1);
  lcd.print(timer);
  lcd.print("t");
  lcd.print("  :  ");
  lcd.print(minutter);
  lcd.print("m");
  
  // Alarmtid i millisekunder.
  alarmTid = (timer * TIME) + (minutter * MINUTT);
  
  // Hva millis() er på når alarmen starter.
  const long startMillis = millis();
  
  tid = millis() - startMillis; 
  
  // Skjermen viser "Alarm satt til" i 5 sekunder.
  delay(5 * SEKUND);
  lcd.clear();
   
  // Nedtelling
  while (tid <= alarmTid) {
    
    // Opdatere tid gått siden alarmen startet.
    tid = millis() - startMillis;
    
    lcd.setCursor(3, 0);
    lcd.print("Nedtelling");
    
    // Oppsett og formatering av t/m/s.
    nedtellingSekund = (alarmTid-tid) / SEKUND;
    nedtellingMinutt = nedtellingSekund /60;
    nedtellingTime = nedtellingMinutt / 60;
    nedtellingSekund = nedtellingSekund - (nedtellingMinutt * 60);
    nedtellingMinutt = nedtellingMinutt - (nedtellingTime * 60);
    
    lcd.setCursor(2, 1);
    
    // Skriver ut antall timer.
    if(nedtellingTime < 10) {
      lcd.print(" ");
      lcd.print(nedtellingTime);
      lcd.print("t");
    } else {
      lcd.print(nedtellingTime);
      lcd.print("t");
    }
    
    lcd.print(" ");
    
    // Skriver ut antall minutter.
    if (nedtellingMinutt < 10) {
      lcd.print(" ");
      lcd.print(nedtellingMinutt);
      lcd.print("m");
    } else {
      lcd.print(nedtellingMinutt);
      lcd.print("m");
    }
    
    lcd.print(" ");
    
    // Skriver ut antall sekunder.
    if (nedtellingSekund < 10) {
      lcd.print(" ");
      lcd.print(nedtellingSekund);
      lcd.print("s");
    } else {
      lcd.print(nedtellingSekund);
      lcd.print("s");
    }
    
    // Si ifra til Kaffetrakteren.
    if ((tidVentPaaKaffe + tid) > alarmTid && !startetKaffe) {
      xbee.write('a');
      startetKaffe = true;
    }
    
    // Hvis knappen trykkes innen 10 minutter etter alarmen er satt så resettes den.
    if (digitalRead(knapp) == HIGH && tid < 10 * MINUTT) {
      reset();
    }
  }
  
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Alarm ringer!");
  
  // Si ifra til spillet.
  digitalWrite(sendSignalTilSpill, HIGH);
}
