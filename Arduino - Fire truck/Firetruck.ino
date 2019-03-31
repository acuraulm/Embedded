#include <IRremote.h>
#define DEBUG true
#include <Servo.h>
Servo srv;
/* Infrared things */
int RECV_PIN = 9;
IRrecv irrecv(RECV_PIN);
decode_results results;
#define left "left"
#define right "right"
/* Infrared codes
  vol+ FF629D
  |<< FF22DD
  >|| FF02FD
  >>| FFC23D
  down FFE01F
  vol- FFA857
  up FF906F
  1 FF30CF
  2 FF18E7
  3 FF7A85
  4 FF10EF
  5 FF38C7
  6 FF5AA5
  7 FF42BD
  8 FF4AB5
  9 FF52AD
  0 FF6897
  EQ FF9867
  ST/REPT FFB04F
  FUNC/STOP FFE21D
  power FFA25D
*/
//L293D
//Motor A
const int rightWheel_PLUS  = 5;  // Pin 14 of L293
const int rightWheel_MINUS  = 6;  // Pin 10 of L293
//Motor B
const int leftWheel_MINUS = 3;  // Pin  7 of L293
const int leftWheel_PLUS  = 4;  // Pin  2 of L293
//Motor Fan
const int fanMotor_MINUS = 2;
const int fanMotor_PLUS = 12;
const int fanServo_OUTPUT = 10;

//LEDs                      Connections on breadboards
const int rightRed_LED = 47;        //
const int leftRed_LED = 48;         //
const int blue_LED = 39;            //
const int rightYellow_LED = 50;       //
const int leftYellow_LED = 49;        //
const int frontGreen_LED = 45;        //
const int backWhite_LED = 44;         //

//Flame sensor
const int analogFlame_IN = A0;        //
const int digitalFlame_IN = 7;        //
const int fanMotor_OUT = 7;         //

//HC-SRO4 ( Ultrasonic sensor )
const int ultrasonicTrig_OUT = 8;     //
const int ultrasonicEcho_IN = 11;     //
long duration;                //
int distance;               //

//Buzzer
const int buzzer_OUT = 38;           //


void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();            // infrared enable
  //L293D
  //Motor A
  pinMode(rightWheel_PLUS, OUTPUT);
  pinMode(rightWheel_MINUS, OUTPUT);
  //Motor B
  pinMode(leftWheel_PLUS, OUTPUT);
  pinMode(leftWheel_MINUS, OUTPUT);
  //LEDs
  pinMode(rightRed_LED, OUTPUT);
  pinMode(leftRed_LED, OUTPUT);
  pinMode(blue_LED, OUTPUT);
  pinMode(rightYellow_LED, OUTPUT);
  pinMode(leftYellow_LED, OUTPUT);
  pinMode(frontGreen_LED, OUTPUT);
  pinMode(backWhite_LED, OUTPUT);
  //Flame sensor
  pinMode(analogFlame_IN, INPUT);
  pinMode(digitalFlame_IN, INPUT);
  pinMode(fanMotor_OUT, OUTPUT);

  //Buzzer
  pinMode(buzzer_OUT, OUTPUT);

  // HC-SR04
  pinMode(ultrasonicTrig_OUT, OUTPUT);
  pinMode(ultrasonicEcho_IN , INPUT);

  //Flame sensor
  pinMode(digitalFlame_IN, INPUT);
  pinMode(analogFlame_IN, INPUT);

  //fanMotor
  pinMode(fanMotor_MINUS, OUTPUT);
  pinMode(fanMotor_PLUS, OUTPUT);
  pinMode(fanServo_OUTPUT, OUTPUT);

}

void loop() {

  // Flame
  int analogVal = analogRead(analogFlame_IN);//read the value of A0
  Serial.print("A0: ");
  Serial.println(analogVal); //print to serial monitor
  boolean stat = digitalRead(digitalFlame_IN);//read the value of D0
  Serial.print("D0: ");
  Serial.println(stat); // print to serial monitor
  if (stat == HIGH)
  {
    srv.write(90);
    digitalWrite(buzzer_OUT, HIGH);
    digitalWrite(rightRed_LED, HIGH);
    digitalWrite(leftRed_LED, HIGH);
    analogWrite(fanMotor_MINUS, 255);
    analogWrite(fanMotor_PLUS, 0);
    srv.attach(fanServo_OUTPUT);
    /*
    
    delay(100);
    turnOFFLeds();
    digitalWrite(frontGreen_LED, HIGH);
    srv.write(65);
    delay(100);
    turnOFFLeds();
    digitalWrite(backWhite_LED, HIGH);
    srv.write(115);
    */
    /*
    for ( int pos = 90; pos <= 135; pos += 10) {
      if (digitalRead(digitalFlame_IN) == HIGH) {
        srv.write(pos);
        delay(200);
      }
    }
    for ( int pos = 135; pos >= 45; pos = pos - 10) {
      if (digitalRead(digitalFlame_IN) == HIGH) {
        srv.write(pos);
        delay(200);
      }
    }
    */
    srv.detach();
  }
  else
  {
    digitalWrite(buzzer_OUT, LOW);
    turnOFFLeds();
    digitalWrite(fanMotor_MINUS, LOW);
    digitalWrite(fanMotor_PLUS, LOW);
  }

  // Ultrasonic
  digitalWrite(ultrasonicTrig_OUT, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(ultrasonicTrig_OUT, HIGH);
  delayMicroseconds(2);
  digitalWrite(ultrasonicTrig_OUT, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(ultrasonicEcho_IN, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance < 20) {
    digitalWrite(buzzer_OUT, HIGH);
    digitalWrite(blue_LED, HIGH);
    //digitalWrite(39, HIGH);
    results.value = 0;
    driveWheel(left, 0, 0);
    driveWheel(right, 0, 0);
  }
  else {
    //digitalWrite(39, LOW);
    digitalWrite(buzzer_OUT, LOW);
  }

  Serial.print(results.value, HEX);
  // Infrared
  if (irrecv.decode(&results)) {
    Serial.print("irCode: ");
    Serial.print(results.value, HEX);
    Serial.print(", bits: ");
    Serial.println(results.bits);
    irrecv.resume();
  } if (results.value == 0xFF629D) // VOL+ - break-dance
  {
    turnONLeds();
    driveWheel(left, 0, 255);
    driveWheel(right, 1, 255);
    delay(200);
    driveWheel(left, 1, 255);
    driveWheel(right, 1, 255);
    delay(100);
    driveWheel(left, 0, 255);
    driveWheel(right, 1, 155);
    delay(200);
    driveWheel(left, 1, 255);
    driveWheel(right, 1, 155);
  }
  if (results.value == 0xFF6897 && distance >= 20) // 0 - automatic detection
  {
    turnOFFLeds();
    if (distance < 20) {
      digitalWrite(frontGreen_LED, HIGH);
      driveWheel(left, 0, 155);
      driveWheel(right, 0, 155);
    }
    else {
      digitalWrite(backWhite_LED, HIGH);
      digitalWrite(leftYellow_LED, HIGH);
      driveWheel(left, 0, 0);
      driveWheel(right, 1, 155);
      delay(200);
    }
  }
  if (results.value == 0xFF30CF && distance >= 20) // 1 - FORWARD & LEFT
  {
    turnOFFLeds();
    digitalWrite(frontGreen_LED, HIGH);
    digitalWrite(leftYellow_LED, HIGH);
    driveWheel(left, 0, 155);
    driveWheel(right, 0, 255);
  }
  if (results.value == 0xFF18E7 && distance >= 20) // 2 - FORWARD
  {
    turnOFFLeds();
    digitalWrite(frontGreen_LED, HIGH);
    driveWheel(left, 0, 155);
    driveWheel(right, 0, 155);
  }
  if (results.value == 0xFF7A85 && distance >= 20) // 3 - FORWARD & RIGHT
  {
    turnOFFLeds();
    digitalWrite(frontGreen_LED, HIGH);
    digitalWrite(rightYellow_LED, HIGH);
    driveWheel(left, 0, 255);
    driveWheel(right, 0, 155);
  }
  if (results.value == 0xFF10EF && distance >= 20) // 4 - LEFT
  {
    turnOFFLeds();
    digitalWrite(leftYellow_LED, HIGH);
    driveWheel(left, 1, 155);
    driveWheel(right, 0, 155);
  }
  if (results.value == 0xFF38C7 && distance >= 20) // 5 - STOP
  {
    turnOFFLeds();
    digitalWrite(leftRed_LED, HIGH);
    digitalWrite(rightRed_LED, HIGH);
    driveWheel(left, 0, 0);
    driveWheel(right, 0, 0);
  }
  if (results.value == 0xFF5AA5 && distance >= 20) // 6 - RIGHT
  {
    turnOFFLeds();
    digitalWrite(rightYellow_LED, HIGH);
    driveWheel(left, 0, 155);
    driveWheel(right, 1, 155);
  }
  if (results.value == 0xFF42BD && distance >= 20) // 7 - BACKWARDS & LEFT
  {
    turnOFFLeds();
    digitalWrite(leftYellow_LED, HIGH);
    digitalWrite(backWhite_LED, HIGH);
    driveWheel(left, 1, 155);
    driveWheel(right, 1, 255);
  }
  if (results.value == 0xFF4AB5 && distance >= 20) // 8 - BACKWARDS
  {
    turnOFFLeds();
    digitalWrite(backWhite_LED, HIGH);
    driveWheel(left, 1, 155);
    driveWheel(right, 1, 155);
  }
  if (results.value == 0xFF52AD && distance >= 20) // 9 - BACKWARDS & RIGHT
  {
    turnOFFLeds();
    digitalWrite(rightYellow_LED, HIGH);
    digitalWrite(backWhite_LED, HIGH);
    driveWheel(left, 1, 255);
    driveWheel(right, 1, 155);
  }
  if (results.value == 0xFFA25D) // power - STOP
  {
    turnOFFLeds();
    driveWheel(left, 0, 0);
    driveWheel(right, 0, 0);
  }
}

void turnOFFLeds() {
  digitalWrite(rightRed_LED, LOW);
  digitalWrite(leftRed_LED, LOW);
  digitalWrite(blue_LED, LOW);
  digitalWrite(rightYellow_LED, LOW);
  digitalWrite(leftYellow_LED, LOW);
  digitalWrite(frontGreen_LED, LOW);
  digitalWrite(backWhite_LED, LOW);
}
void turnONLeds() {
  digitalWrite(rightRed_LED, HIGH);
  digitalWrite(leftRed_LED, HIGH);
  digitalWrite(blue_LED, HIGH);
  digitalWrite(rightYellow_LED, HIGH);
  digitalWrite(leftYellow_LED, HIGH);
  digitalWrite(frontGreen_LED, HIGH);
  digitalWrite(backWhite_LED, HIGH);
}

void driveWheel(const char wheel, int wheelDirection, int wheelSpeed) { // wheel -> left / right. wheelDirection -> 0 forward 1 backwards.
  if ( strcmp(wheel, "left") == 0) {
    if (wheelDirection == 0) {
      analogWrite(leftWheel_MINUS, wheelSpeed);
      analogWrite(leftWheel_PLUS, 0);
    }
    else if (wheelDirection == 1 ) {
      analogWrite(leftWheel_MINUS, 0);
      analogWrite(leftWheel_PLUS, wheelSpeed);
    }
  }
  else if ( strcmp(wheel, "right") == 0 ) {
    if (wheelDirection == 0) {
      analogWrite(rightWheel_MINUS, wheelSpeed);
      analogWrite(rightWheel_PLUS, 0);
    }
    else if (wheelDirection == 1 ) {
      analogWrite(rightWheel_MINUS, 0);
      analogWrite(rightWheel_PLUS, wheelSpeed);
    }
  }
}

