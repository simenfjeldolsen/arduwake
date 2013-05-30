// Noter.
#include "pitches.h";

// Porter.
const int lyd = 6;
const int greenLED = 7;
const int redLED = 8;
const int signalFraAlarm = 9;
const int startPin = 10;
const int line = 11;
const int stoppPin = 12;

// 0 = alarmen har ikke gått før.
// 1 = alarmen har gått før.
int alarmState = 0;

// 0 = Start-pinnen har aldri blitt touchet eller spillet har blitt failet ved å touche linen.
// 1 = Start-pinnen har blitt touchet og brukeren kan prøve å vinne spillet.
// 2 = Spillet er vunnet.
int spillState = 0;

void setup() {
  
  Serial.begin(9600);
  
  //Pin-mode
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);

}

void loop() {
  
  // Hvis alarmen ikke har gått før og Alarm-arduinoen sier ifra, så starter spillet.
  if (alarmState == 0 && millis() > 500) {
    if (digitalRead(signalFraAlarm) == HIGH) {
      
      Serial.println("GO");
      
      alarm();
      spill();
      alarmState = 1;
    }
  }
}

// Kontrollerer hele spillet.
void spill() {
 
 // Så lenge spillet ikke er vunnet så forsetter programmet.
  while (spillState != 2) {
   
    // Hvis startpinnen har blitt touchet.
    while (spillState == 1) {
    
      // Hvis linen touches så går spillet inn i failed-mode.
      if (digitalRead(line) == HIGH) {
      
        Serial.println("fail");
        spillState = 0;
        fail();     
      }
    
      // Hvis stopp-pinnen touches uten at linen har blitt touchet så vinner mann.
      if (digitalRead(stoppPin) == HIGH) {
      
        Serial.println("vinn");
        spillState = 2;
        stopp();
      }  
    }
    
    /* // Hvis spillet skal kunne resettes etter å ha vunnet.
    if (spillState == 2) { 
      if (digitalRead(startPin) == HIGH) {
      
        spillState = 1;
        start();
      }
    }*/
  
    // Resetter spillet når start-pinnen touches.
    if (digitalRead(startPin) == HIGH && spillState != 2) {
      
        Serial.println("resett");
        spillState = 1;
        start();
    }
  }
}

// Skrur på alarm-lyden.
void alarm() {
  
  noTone(lyd);
  tone(lyd, 1000);
}

// Skrur på det grønne lyset og spiller en lyd for å gi tilbakemelding til brukeren at spillet er klart.
void start() {
  
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, HIGH);
  
  noTone(lyd);
  tone(lyd, 400);
  delay(125);
  tone(lyd, 500);
  delay(125);
  tone(lyd, 600);
  delay(125);
  tone(lyd, 700);
  delay(125);
  alarm();
}

// Skrur paa det rød lyset og spiller en lyd for å gi tilbakemlding til brukeren på at han har touchet linen.
void fail() {
  
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, HIGH);
  noTone(lyd);
  tone(lyd, 1400);
  delay(500);
  alarm();
}

// Skrur av alarmen og spiller en mario power-up lyd for å gi tilbakemelding til brukeren om at han har vunnet.
void stopp() {

  digitalWrite(greenLED, LOW);
  noTone(lyd);
  mario();
}

// Spiller av mario power-up lyd.
void mario() {
  
  // Melodien i noter. Notene er deklarert i pitches.h.
  int melodi[] = { NOTE_C5, NOTE_G4, NOTE_C5, NOTE_E5, NOTE_G5, NOTE_C6, NOTE_G5, NOTE_GS4, NOTE_C5, NOTE_DS5, NOTE_GS5, NOTE_E5, NOTE_A5, NOTE_C6, NOTE_DS6, NOTE_GS6, NOTE_DS6, NOTE_AS4, NOTE_D5, NOTE_F5, NOTE_AS5, NOTE_F5, NOTE_B5, NOTE_D6, NOTE_F6, NOTE_AS6, NOTE_F6 };
 
  for (int i = 0; i < 27; i++) {
     
    tone(lyd, melodi[i]);
    
    // De første 26 notene er åttendedelsnoter.
    if (i != 26) {
      delay(1000/27);
    noTone(lyd);
    
    // Den siste noten er en sekstendedelsnote.
    } else {
      delay(1000/27*2);
      noTone(lyd);
    }
  }
}
  
