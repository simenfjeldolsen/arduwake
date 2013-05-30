#include <SoftwareSerial.h>
#include <Servo.h>

Servo servo;
int servoPosisjon;
SoftwareSerial xbee(3, 2);

void setup() {
  xbee.begin(9600);
  servo.attach(14);
  servo.write(45);
}

void loop() {
  if (xbee.read() == 97){
    skruPaa();
  }
}

void skruPaa() {
  servoPosisjon = 180;
  servo.write(servoPosisjon);
  delay(5000);
  skruAv();
}

void skruAv() {
  servoPosisjon = 180;
  servo.write(servoPosisjon);
  delay(5000);
  servo.write(90);
  delay(100000);
}
