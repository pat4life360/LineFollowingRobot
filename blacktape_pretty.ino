#include <Adafruit_MotorShield.h>
//#include <Keyboard.h>


int SensorPinleft = A0;               // analog pin used to connect the sharp sensor
int SensorPinright = A1; // analog pin used to connect the sharp sensor
float leftVal = 1023; // variable to store the values from sensor(initially zero)
float rightVal = 1023; // variable to store the values from sensor(initially zero)
int state;
int threshold = 430;
float speedval;
int incomingData;
float mytime;
bool isReady = 1;  //set depedning on data collection vs gui control
float motorspeedleft;
float motorspeedright;



// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);
// You can also make another motor on port M2
//Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);




void setup()
{
  Serial.begin(9600);// starts the serial monitor
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");
  // turn on motor
  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);
  //state =4; //to stop the robot
  


}





void Drive2(Adafruit_DCMotor *myMotor1, Adafruit_DCMotor *myMotor2, float speedvalleft, float speedvalright) //function to drive motors when called
{

 
  // Set the speed to start, from 0 (off) to 255 (max speed)
  myMotor1->run(FORWARD);
  myMotor2->run(FORWARD);
  myMotor1->setSpeed(speedvalleft);
  myMotor2->setSpeed(speedvalright);

 


}


void Think() //Function to decide which state to chhoose
{

 if (leftVal > threshold )//left sees tape and right doesnt
  {
     state = 1;}
   else if (rightVal > threshold) //right sees tape and left doesnt
  {
    state = 2;}
   else if (rightVal > threshold && leftVal > threshold) //both see tape
    {
    state = 3;}
   else // both dont see tape
   {
    state = 0;}
};

void Print() //function to Print relevant data to Monitor
      Serial.print(mytime/1000);  // prints time in seconds x`elapsed
      Serial.print(" ");         // prints a space
      Serial.print(motorspeedleft);  // prints left motorspeed
      Serial.print(" ");         // prints a space
      Serial.print(motorspeedright); //prints right motorspeed 
      Serial.print(" ");    //prints a space
      Serial.print(leftVal); // prints left sensor value
      Serial.print(" ");         // prints a space
      Serial.print(rightVal); // prints right sensor value
      
}


void Debug() {//
 //To Debug Sensors 
    state = 4;
}



void GUI() //funtion to control robot running  with python

   { if (Serial.available() > 0) {   //see if there's incoming serial data 
        incomingData = Serial.read();  //get message from python
    
     
   
  
     if (incomingData == 'S'){ //stop
     Serial.print("Stopping");
     state = 4;   //go to state where robot is off
     isReady = 0;
     }
  
     if (incomingData == 'F') {//start
   Serial.print("Starting");
      isReady = 1;
       
     }
    }
    
     
      
       if (isReady)  //decision maker to move robot
       {
       Think();}  //start switch-casing
       else
       {state = 4;} //stop robot is not ready, based on python message
};





void loop()
{
   mytime = millis();
   GUI();
   leftVal = analogRead(SensorPinleft);       //check value of left IR sensor
   rightVal = analogRead(SensorPinright);      //check value of left IR sensor
  

  //switchcasing depending on what both sensors see
   Think();   // use if not using GUI

  
  switch (state) {
   case 0:    //go straight
    //  Serial.println("Going Straight");
      motorspeedleft = 27;
      motorspeedright = 27;
      Drive2(leftMotor, rightMotor, motorspeedleft, motorspeedright);
      break;
    case 1:    // sees tape on the left sensor
      motorspeedleft = 0;
      motorspeedright = 70;
      Drive2(leftMotor, rightMotor, motorspeedleft, motorspeedright);
     Serial.println("Going Left");
      break;
    case 2:    // sees tape on the right sensor
      motorspeedleft = 70;
      motorspeedright = 0;
      Drive2(leftMotor, rightMotor, motorspeedleft, motorspeedright);
     Serial.println("Going Right");
      break;
    case 3:    //go straight
      motorspeedleft = 0;
      motorspeedright = 0;
      Serial.println("Reached Track End");
      Drive2(leftMotor, rightMotor, motorspeedleft, motorspeedright);
       break;
    case 4:    // debugging/stopping
      motorspeedleft = 0;
      motorspeedright = 0;
      Drive2(leftMotor, rightMotor, motorspeedleft, motorspeedright);
      Serial.print("Left IR: ");
      Serial.println(leftVal);     // prints the value of the sensors to the serial monitor
      Serial.print("Right IR: ");
      Serial.println(rightVal);
      delay(400);  //for testing clarity
      break;
      
   
     
  }
  
      Print();   //Print all relevant info to serial monitor
   } ;
