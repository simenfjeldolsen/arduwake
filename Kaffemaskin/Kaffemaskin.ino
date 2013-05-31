#include <SoftwareSerial.h>
#include <Servo.h>

// Servo
Servo servo;

// XBee
SoftwareSerial xbee(3, 2);

int servoPosisjon;

void setup() {
  
  // XBee
  xbee.begin(9600);
  
  // Servo
  servo.attach(14);
  
  // Setter servoens start-posisjon.
  servo.write(45);
}

void loop() {

  // Hvis den mottar 97(a) fra alarmen så starter den kaffetrakteren.
  if (xbee.read() == 97){
    skruPaa();
  }
}

// Roterer servoen for å skru på kaffetrakteren.
void skruPaa() {
  servoPosisjon = 180;
  servo.write(servoPosisjon);
  delay(5000);
  skruAv();
}

// Roterer servoen for å skru av kaffetrakteren.
void skruAv() {
  servoPosisjon = 180;
  servo.write(servoPosisjon);
  delay(5000);
  servo.write(90);
  delay(100000);
}
