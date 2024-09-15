#include <AccelStepper.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define MotorInterfaceType 4

#define motorPin1 18  // IN1 on the L298N
#define motorPin2 19  // IN2 on the L298N
#define motorPin3 23  // IN3 on the L298N
#define motorPin4 25  // IN4 on the L298N

#define ENA_PIN   2  // Enable A
#define ENB_PIN   15  // Enable B


#define motor2Pin1 12  // IN1 on the L298N
#define motor2Pin2 14  // IN2 on the L298N
#define motor2Pin3 27  // IN3 on the L298N
#define motor2Pin4 26  // IN4 on the L298N

#define ENA_2_PIN   33  // Enable A
#define ENB_2_PIN   32  // Enable B


#define buttonInput 13 //Input
bool canTriggerButton = false;

const int stepLimit = 2000;
int targetStep = 1000;
int holdPWN = 128;

AccelStepper stepper(MotorInterfaceType, motorPin1, motorPin2, motorPin3, motorPin4);
AccelStepper stepper2(MotorInterfaceType, motor2Pin1, motor2Pin2, motor2Pin3, motor2Pin4);


void HoldCurrent_A(){
  analogWrite(ENA_PIN, holdPWN);
  analogWrite(ENB_PIN, holdPWN);
}

void HoldCurrent_B(){
  analogWrite(ENA_2_PIN, holdPWN);
  analogWrite(ENB_2_PIN, holdPWN);
}

void ReEnable_A(){
  analogWrite(ENA_PIN, 255);
  analogWrite(ENB_PIN, 255);
}

void ReEnable_B(){
  analogWrite(ENA_2_PIN, 255);
  analogWrite(ENB_2_PIN, 255);
}

void setup() {
  pinMode(buttonInput, INPUT_PULLUP);

  
  pinMode(ENA_PIN, OUTPUT);
  analogWrite(ENA_PIN, 255);
  
  pinMode(ENB_PIN, OUTPUT);
  analogWrite(ENB_PIN, 255);

  
  pinMode(ENA_2_PIN, OUTPUT);
  analogWrite(ENA_2_PIN, 255);
  
  pinMode(ENB_2_PIN, OUTPUT);
  analogWrite(ENB_2_PIN, 255);

  
  Serial.begin(115200);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("LiDAR");
  display.println("Turret");
  display.setTextSize(1);
  display.setCursor(0, 50);
  display.println("Initializing...");
  display.display(); 
  
  stepper.setMaxSpeed(500);
  stepper.setAcceleration(250);
  stepper2.setMaxSpeed(500);
  stepper2.setAcceleration(250);
  
  ReEnable_A();
  stepper.moveTo(-targetStep);
  
  while (stepper.distanceToGo() != 0){
    stepper.run();
  } 
  HoldCurrent_A();

  delay(2000);
  
  ReEnable_B();
  stepper2.moveTo(-targetStep);

  while (stepper2.distanceToGo() != 0){
    stepper2.run();
  } 
  HoldCurrent_B();
  
  canTriggerButton = true;
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("LiDAR");
  display.println("Turret");
  display.display();
}

void loop() {
  if (digitalRead(buttonInput) == LOW && canTriggerButton){
    canTriggerButton = false;
  
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("LiDAR");
    display.println("Turret");
    display.setTextSize(1);
    display.setCursor(0, 50);
    display.println("Moving...");
    display.display(); 

    stepper.moveTo(targetStep);
    ReEnable_A();
    
    stepper2.moveTo(targetStep);
    ReEnable_B();
  }

  if (stepper.distanceToGo() != 0){
    stepper.run();
  }
  else{
    HoldCurrent_A();
  }
  
  if (stepper2.distanceToGo() != 0){
    stepper2.run();
  }
  else{
    HoldCurrent_B();
  }

  if (stepper.distanceToGo() == 0 && stepper2.distanceToGo() == 0 && !canTriggerButton) {
    canTriggerButton = true;
    targetStep = -stepper.currentPosition();
    
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("LiDAR");
    display.println("Turret");
    display.display(); 
  }
}