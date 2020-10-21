/*
 * NOTE: This sketch is hard-coded to rotate by a quarter of a rotation every test. 
 * To change this, the following parameters need to be changed:
 *  revs_per_increment 
 *  the setter for the stage variable (stage = count % 4)
 *  the required encoder count - WARNING: the servo motor has a region where encoder counts do not represent the position accurately. 
 *  
 * The reason the number of trials per revolution is hard-coded is it avoids the encoder dead zone where encoder counts are meaningless.
 * 
 * Author: Carlos Aguilera Cortes
 * Date:12/10/2020
 * Last modified: 22/10/2020
 */


#include <math.h>
#include "Servo.h"

Servo myServo;

// CSV file column headers
bool label = true;
String label_independent = "Force Applied (N)";
String label_dependent = "Part Extension (mm)";

// change these if necessary
const int servo_sig = 9;
const int encoder_pin = A0; 

// this represents speed, the MAX value is clockwise, the MIN value is counter-clockwise
const int MAX_PWM = 2150;
const int MID_PWM = 1500;
const int MIN_PWM = 850;

const int pulley_radius = 2; // measurement in mm
const float revs_per_increment = 0.25; // fraction change in revs per loop iteration
const int time_delay = 5; // time between each test to allow for manual measurement
const int trial_num = 10; // the number of readings sent through serial

int encoder_cnt; // NOTE: this needs to be read from and initiated to 0 at the start of the program (setup function)
int count = 0; // counter for number of tests
int stage = 0; // placeholder for desired rotation position
float total_extension = 0.0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(encoder_pin, INPUT);
  myServo.attach(servo_sig);

  encoder_cnt = analogRead(encoder_pin);
    
  while(encoder_cnt > 20){
    myServo.writeMicroseconds(1600);
    encoder_cnt = analogRead(encoder_pin);
    Serial.println(encoder_cnt);
    delay(50);
  }
  myServo.writeMicroseconds(MID_PWM);
}

void loop() {

  delay(time_delay * 1000);

  encoder_cnt = analogRead(encoder_pin);

  stage = count % 4;
    
  // Table column headings
  if(label){
    Serial.print(label_independent);
    Serial.print(",");
    Serial.println(label_dependent);
    label = false;
  }

  // map the required encoder counts to rotate a quarter of a turn
  switch(stage){
    case(0):
      while(encoder_cnt > 640 || encoder_cnt < 620){
        myServo.writeMicroseconds(1600);
        encoder_cnt = analogRead(encoder_pin);
        //Serial.print("Encoder count 0: ");
        //Serial.println(encoder_cnt);
        delay(20);
      }
      break;
    case(1):
      while(encoder_cnt > 430 || encoder_cnt < 410){
        myServo.writeMicroseconds(1600);
        encoder_cnt = analogRead(encoder_pin);
        //Serial.print("Encoder count 1: ");
        //Serial.println(encoder_cnt);
        delay(20);
      }
      break;
    case(2):
      while(encoder_cnt > 220 || encoder_cnt < 200){
        myServo.writeMicroseconds(1600);
        encoder_cnt = analogRead(encoder_pin);
        //Serial.print("Encoder count 2: ");
        //Serial.println(encoder_cnt);
        delay(20);
      }
      break;
    case(3):
      while(encoder_cnt > 20){
        myServo.writeMicroseconds(1650);
        encoder_cnt = analogRead(encoder_pin);
        //Serial.print("Encoder count 3: ");
        //Serial.println(encoder_cnt);
        delay(20);
      }
      break;
    default:
        //Serial.print("Encoder count default: ");
        //Serial.println(encoder_cnt);
      break;
  }

  myServo.writeMicroseconds(MID_PWM); // stop servo between trials

  if(count > trial_num){
    // end the program and lock cpu in infinite loop until reset is pressed
    Serial.println("finished");
    cli();
    while(true);
  }else{
    total_extension = count * revs_per_increment * pulley_radius * M_PI * 2;
    Serial.println(total_extension);
    
    count++;
  }

}
