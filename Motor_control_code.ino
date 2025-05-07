#include <Servo.h>
#include <Stepper.h>
#include <movingAvg.h>

movingAvg myGrabber(50);
movingAvg base(5);
movingAvg pivot1(1);

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo grabber;

const int stepsPerRevolution = 2048;
const int rolePerMinute = 15; 
const int SW_pin1 = 13; 
const int X_pin1 = 0; 
const int Y_pin1 = 1; 
const int SW_pin2 = 12; 
const int X_pin2 = 2; 
const int Y_pin2 = 3; 
const int potenPin = 4;
int baseAngle = 90;
int pivot1Angle = 125;
int pivot2Angle = 90;
int wristAngle = 90;

Stepper stepper1(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  stepper1.setSpeed(rolePerMinute);
  pinMode(SW_pin1, INPUT);
  digitalWrite(SW_pin1, HIGH);
  pinMode(SW_pin2, INPUT);
  digitalWrite(SW_pin2, HIGH);
  servo1.attach(3);
  servo2.attach(5);
  servo3.attach(9);
  servo4.attach(10);
  grabber.attach(6);
  Serial.begin(9600);
  servo1.write(baseAngle);
  servo2.write(pivot1Angle);
  servo3.write(pivot2Angle);
  servo4.write(wristAngle);

  myGrabber.begin();
  base.begin();
  pivot1.begin();
}

void loop() {
  //base servo1 code
  if (millis() % 10 == 0){
  int x1_reading = analogRead(X_pin1);
  int baseMovingAvg = base.reading(x1_reading);
  int angleAdd1 = map(baseMovingAvg, 0, 1023, -2, 2);
  if (baseMovingAvg > 400 && baseMovingAvg < 600){
    angleAdd1 = 0;
  }
  baseAngle += angleAdd1;
  if (baseAngle < 0){
    baseAngle = 0;
  }
  if (baseAngle > 180){
    baseAngle = 180;
  }
  servo1.write(baseAngle);
  //pivot 1 servo2 code
  int y1_reading = analogRead(Y_pin1);
  int pivot1MovingAvg = base.reading(y1_reading);
  int angleAdd2 = map(pivot1MovingAvg, 0, 1023, -1, 1);
  if (pivot1MovingAvg > 400 && pivot1MovingAvg < 600){
    angleAdd2 = 0;
  }
  pivot1Angle += angleAdd2;
  if (pivot1Angle < 0){
    pivot1Angle = 0;
  }
  if (pivot1Angle > 180){
    pivot1Angle = 180;
  }
  servo2.write(pivot1Angle);
  }
  //pivot 2 servo3 code
  int y2_reading = analogRead(Y_pin2);
  int angleAdd3 = map(y2_reading, 0, 1023, -5, 5);
  if (y2_reading > 400 && y2_reading < 600){
    angleAdd3 = 0;
  }
  pivot2Angle += angleAdd3;
  if (pivot2Angle < 0){
    pivot2Angle = 0;
  }
  if (pivot2Angle > 180){
    pivot2Angle = 180;
  }
  servo3.write(pivot2Angle);

  //grabber code
  int grabber_value = analogRead(potenPin);
  int potMovingAvg = myGrabber.reading(grabber_value);
  int anglePinch = map(potMovingAvg, 0, 1023, 79, 178);
  grabber.write(anglePinch);
  //stepper code
  /*
  int x_reading = analogRead(X_pin1);
  if(x_reading < 400){
      stepper1.step(-16);
  }
  if(x_reading > 600){
      stepper1.step(16);
  }
  */

  //Troubleshoot switches
  if (millis() % 3000 == 0){
  //Switch 1
  Serial.print("Switch1:  ");
  Serial.print(digitalRead(SW_pin1));
  Serial.print("\n");
  Serial.print("X-axis: ");
  Serial.print(analogRead(X_pin1));
  Serial.print("\n");
  Serial.print("Y-axis: ");
  Serial.println(analogRead(Y_pin1));
  Serial.print("\n\n");
  //switch 2
  Serial.print("Switch2:  ");
  Serial.print(digitalRead(SW_pin2));
  Serial.print("\n");
  Serial.print("X-axis: ");
  Serial.print(analogRead(X_pin2));
  Serial.print("\n");
  Serial.print("Y-axis: ");
  Serial.println(analogRead(Y_pin2));
  Serial.print("\n\n");

  Serial.print("Grab: ");
  Serial.println(potMovingAvg);
  Serial.println(anglePinch);
  Serial.print("\n\n");
  }
}
