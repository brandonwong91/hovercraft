#include <TimerOne.h>
#include <Servo.h>

//Including libraries needed for arduino

// Initialisation of Ultrasonic sensor pins
#define trigPin1 12 //left sensor trigger pin -- transmit 8 cycle of ultrasonic wave when high
#define echoPin1 13 //left sensor echo pin -- high when received reflected ultrasonic wave
#define trigPin2 4 //front sensor trigger pin
#define echoPin2 11 //front sensor echo pin
#define trigPin3 2 //right sensor trigger pin
#define echoPin3 3 //right sensor echo pin

//Initialisation of Motor pins
#define ENA 5 //PWM to adjust speed of Motor A; GND=>disable
#define ENB 6 //PWM to adjust speed of Motor B; GND=>disable
#define IN1 7 //clockwise or anticlockwise setting for both motor
#define IN2 8 //clockwise or anticlockwise setting for both motor

//Initialisation of servo_motor
Servo myservo; //create servo object to control the servo
Servo myMotor;
#define servopin 10; // set pin 10 as digital pin where servo connects to
#define ESCpin 9; // set pin 9 as digital pin where ESC connects to
int myangle;// 11 integer section which determine the rotation angle of servo
int val = 23;
#define around_duration 4000
#define mid_duration 2000
#define short_duration 1000
#define reverse_duration 1000

//Distance Parameters (for middle)
#define middle_farfar 150
#define middle_far 120
#define middle_mid 70
#define middle_near 10    //reverse


//Distance Parameters (for left and right)
//#define far 25
//#define mid 22
//#define near 19

//Distance Parameters (left is nearer)
#define diff_left1 -2.0
#define diff_left2 -4.0
#define diff_left3 -7.0
#define diff_left4 -10.0
#define diff_left5 -15.0
#define diff_left6 -20.0

//Distance Parameters (right is nearer)
#define diff_right1 2.0
#define diff_right2 4.0
#define diff_right3 7.0
#define diff_right4 10.0
#define diff_right5 15.0
#define diff_right6 20.0

//Distance Parameters (Emergency)
#define emergency_distance 10.0

// Initialisation of Interrupt Pin 
#define interrupt_pin 2

//#define offset 50

/*****************************
 *****************************
 * The setup of the pins needed 
 *****************************
 *****************************/
void setup() {
  int close_distance,left_distance,right_distance = 0;

  //Setting up of i/o for ultrasonic pins
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);

  //attached the servo on pin 10 to servo object
  myservo.attach(10); 

  //Setting up of i/o for DC motor
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  myMotor.attach(9);
  Serial.begin(9600);

  //Timer1.initialize(1000000); 
  // set a timer of length 1000000 microseconds 
  //(or 1 sec - or 1Hz )
  // Timer1.attachInterrupt( timerIsr ); // attach the service routine here


} 



/**********************
 **********************
 * This is the main loop
 **********************
 **********************/

void loop() {

  float close_distance,left_distance,right_distance,difference = 0.0;


  if (val == 23)
  {
    myMotor.write(23);
    delay (5000);
    Serial.print("ESC run complete\n");
    //     myMotor.write(30);
    //     delay (1000);
    //     myMotor.write(40);
    //     delay (1000);
    //     myMotor.write(50);
    //     delay (1000);
    //     
    motor(1, 20, 0);

    myMotor.write(65);
    delay (1000);

    motor(1, 30, 0);

    myMotor.write(75);
    delay (1000);



    myMotor.write(85);
    delay (1000);

    //  motor(1, 40, 0);

    myMotor.write(95);
    delay (1000);



    myMotor.write(100);
    delay (500);

    motor(1, 0, 30);

    myMotor.write(110);
    delay (100);

    //  motor(1, 60, 0);

    myMotor.write(120);
    delay (100);

    motor(1, 0, 40);

    //  myMotor.write(130);
    //delay (1000);



    //  myMotor.write(140);
    ///  delay (1000);
    //     myMotor.write(160);
    //   delay (1000);
    //      motor(1, 50, 0);
    //    myMotor.write(130);
    //2q delay (1000);

    myMotor.write(150);
    delay (1000);

    //        myMotor.write(170);
    //    delay (1000);
    //  
    //     myMotor.write(190);
    //    delay (1000);
    val=155;
  }
  else
  {

    myMotor.write(160);
    close_distance = front_measure();
    left_distance = left_measure();
    right_distance = right_measure();
    difference = left_distance - right_distance;
    Serial.print("Middle Distance: ");
    Serial.print(close_distance);
    Serial.print("\n");   

    Serial.print("Left Distance: ");
    Serial.print(left_distance);
    Serial.print("\n");

    Serial.print("Right Distance: ");
    Serial.print(right_distance);
    Serial.print("\n");

    Serial.print("Difference: ");
    Serial.print(difference);
    Serial.print("\n");

    movement(close_distance,difference, left_distance, right_distance);
  }


}





/************************************
 ************************************
 * Movement functions
 ************************************
 ************************************/
void movement(float front, float diff, float left_distance, float right_distance){
  int mode;
  int motor1, motor2;
  int servo_angle = 5;
  float coeff1, coeff2 = 1.0;
  float offset=50.0;
  if (diff < diff_left6)                                       //left turn
  {
    coeff1 = 0.45;
    coeff2 = 1.0;
    servo_angle = 10;
  }
  else if (diff >= diff_left6 && diff < diff_left5)
  {
    coeff1 = 0.55;
    coeff2 = 1.0;
    servo_angle = 5;
  }
  else if (diff >= diff_left5 && diff < diff_left4)
  {
    coeff1 = 0.65;
    coeff2 = 1.0;
    servo_angle = 5;
  }
  else if (diff >= diff_left4 && diff < diff_left3)
  {
    coeff1 = 0.75;
    coeff2 = 1.0;
    servo_angle = 5;
  }
  else if (diff >= diff_left3 && diff < diff_left2)
  {
    coeff1 = 0.85;
    coeff2 = 1.0;
    servo_angle = 5;
  }
  else if (diff >= diff_left2 && diff < diff_left1)
  {
    coeff1 = 0.95;
    coeff2 = 1.0;
    servo_angle = 5;
  }
  else if (diff >= diff_left1 && diff < diff_right1)
  {
    coeff1 = 1.0;
    coeff2 = 1.0;
    servo_angle = 5;
  }
  else if (diff >= diff_right1 && diff < diff_right2)            //right turn
  {
    coeff1 = 1.0;
    coeff2 = 0.95;
    servo_angle = 5;
  }
  else if (diff >= diff_right2 && diff < diff_right3)
  {
    coeff1 = 1.0;
    coeff2 = 0.85;
    servo_angle = 5;
  }
  else if (diff >= diff_right3 && diff < diff_right4)
  {
    coeff1 = 1.0;
    coeff2 = 0.75;
    servo_angle = 5;
  }
  else if (diff >= diff_right4 && diff < diff_right5)
  {
    coeff1 = 1.0;
    coeff2 = 0.65;
    servo_angle = 5;
  }
  else if (diff >= diff_right5 && diff < diff_right6)
  {
    coeff1 = 1.0;
    coeff2 = 0.55;
    servo_angle = 5;
  }
  else if (diff >= diff_right6)
  {
    coeff1 = 1.0;
    coeff2 = 0.45;
    servo_angle = 0;
  }

  //determine front distance to adjust motor speed

  if (right_distance<20 && left_distance>30)
  {
    mode=1;
    motor1=100;
    motor2=0;
    offset=0;
  }
  else if (left_distance<20 && right_distance>30)
  {
    mode=1;
    motor2=100;
    motor1=0;
    offset=0;
  }
  if (front > middle_farfar)                     //go straight    
  {
    mode = 1;

    coeff1 = 1.0;
    coeff2 = 1.0; 
    servo_angle = 5;

//    if (left_distance < emergency_distance)      //emergency coding
//    {
//      coeff1 = 0.6;
//      coeff2 = 1.0;
//      servo_angle = 10;
//    }
//    if (right_distance < emergency_distance)
//    {
//      coeff1 = 1.0;
//      coeff2 = 0.6;
//      servo_angle = 0;
//    }


    motor1 = (int) (100.0-offset)*coeff1; 
    motor2 = (int) 100.0*coeff2;



  }
  else if (front > middle_far && front <= middle_farfar)     
  {
    mode=1;    
    motor1 = (int) (95.0-offset+10.0)*coeff1; 
    motor2 = (int) 95.0*coeff2; 
  }
  else if (front > middle_mid && front <= middle_far)
  {
    mode=1;
    motor1 = (int) (87.0-offset+20.0)*coeff1; 
    motor2 = (int) 87.0*coeff2;
  }
  else if (front > middle_near && front <= middle_mid)  
  {
    mode=1;
    motor1 = (int) (80.0-offset+30.0)*coeff1; 
    motor2 = (int) 80.0*coeff2;
  }
  else if (front <= middle_near)  //reverse
  {
    mode=2;
    motor1 = (int) (90.0)*coeff2; 
    motor2 = (int) (90.0-offset+40.0)*coeff1;
  }

  motor(mode, motor1, motor2);
  rotateservo(servo_angle);
  Serial.print("Mode: ");
  Serial.print(mode);
  Serial.print("\nLeft Motor: ");
  Serial.print(motor1);
  Serial.print(" Right Motor: ");
  Serial.print(motor2);
  Serial.print("\n");
}

/****************************
 * Basic distance measurement
 *****************************/

float front_measure()
{

  float duration = 0.0;
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(250);
  digitalWrite(trigPin2, LOW);

  //measure the distance from ultrasonic sensor 2
  duration = pulseIn(echoPin2, HIGH);                   

  return (duration/2) / 29.1;
}

float left_measure()
{

  float duration = 0.0;
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(250);
  digitalWrite(trigPin1, LOW);

  //measure the distance from ultrasonic sensor 2
  duration = pulseIn(echoPin1, HIGH);                   
  //Serial.print((duration/2) / 29.1);


  return (duration/2) / 29.1;
}

float right_measure()
{

  float duration = 0.0;
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(250);
  digitalWrite(trigPin3, LOW);

  //measure the distance from ultrasonic sensor 2
  duration = pulseIn(echoPin3, HIGH);                   

  return (duration/2) / 29.1;
}

/****************************
 * Motor Control
 * mode is a number 0 -> 3 that determines what the motor 
 * will do.
 * 0 = coast/disable the H bridge
 * 1 = turn motor clockwise (move forward) 
 * 2 = turn motor counter clockwise (move backward)
 * 3 = brake motor
 * 
 * percent1, percent2 is a number 0 -> 100 that represents percentage of
 * motor speed.
 * 0 = off
 * 50 = 50% of full motor speed
 100 = 100% of full motor speed*/
/*****************************/
void motor(int mode, int percent1, int percent2)
{
  //change the percentage range of 0 -> 100 into the PWM
  //range of 0 -> 255 using the map function
  int duty1 = map(percent1, 0, 100, 0, 255);
  int duty2 = map(percent2, 0, 100, 0, 255);

  switch(mode)
  {
  case 0: //to coast
    digitalWrite(ENA, LOW);
    digitalWrite(ENB, LOW);
    break;
  case 1: //to move forward
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, duty1);
    analogWrite(ENB, duty2);
    break;
  case 2: //to move backward
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, duty1);
    analogWrite(ENB, duty2);
    break;
  case 3: //to brake motor
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    //use pwm to control motor braking power
    analogWrite(ENA, duty1);
    analogWrite(ENB, duty2);
    break;
  }
}

/****************************
 * Servo Control
 * myangle range from 0 to 11 will divided the angle range from 50 to 130 degree, each will differ from each other by 8 degree
 * myangle value of:
 * 5 represent 90 degree (centre)
 * 0 - 4 represent right-hand side angle rotation
 * 6 - 10 represent left-hand side rotation 
 *****************************/
void rotateservo(int myangle)
{
  switch (myangle){
  case 0 : 
    myservo.write(50);
    Serial.print("Servo: 50 deg\n");
    break;
  case 1 :
    myservo.write(58);
    Serial.print("Servo: 58 deg\n");
    break;
  case 2 :
    myservo.write(66);
    Serial.print("Servo: 66 deg\n");
    break;
  case 3 :
    myservo.write(74);
    Serial.print("Servo: 74 deg\n");
    break;
  case 4 :
    myservo.write(82);
    Serial.print("Servo: 82 deg\n");
    break;
  case 5 :
    myservo.write(90);
    Serial.print("Servo: 90 deg\n");
    break;
  case 6 :
    myservo.write(98);
    Serial.print("Servo: 98 deg\n");
    break;
  case 7 :
    myservo.write(106);
    Serial.print("Servo: 106 deg\n");
    break;
  case 8:
    myservo.write(114);
    Serial.print("Servo: 114 deg\n");
    break;
  case 9 :
    myservo.write(122);
    Serial.print("Servo: 122 deg\n");
    break;
  case 10 :
    myservo.write(130);
    Serial.print("Servo: 130 deg\n");
    break;
  }
}

/*****************************
 ******************************
 ******************************
 * END OF CODING
 ******************************
 ******************************
 ******************************/





