#include <AccelStepper.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// Define motor interface type
#define MotorInterfaceType 4

// Define motor connections (adapt based on your setup)
#define motorPin1 18  // IN1 on the L298N
#define motorPin2 19  // IN2 on the L298N
#define motorPin3 23  // IN3 on the L298N
#define motorPin4 25  // IN4 on the L298N

#define buttonInput 13 //Input

bool canTriggerButton = false;
int targetStep = 2000;

// Create an instance of AccelStepper with 4-wire connection
AccelStepper stepper(MotorInterfaceType, motorPin1, motorPin2, motorPin3, motorPin4);

// Define the number of steps for one full cycle
const int stepLimit = 2000; // Adjust this to your motor's needs

void setup() {
  pinMode(buttonInput, INPUT_PULLUP);
  
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
  
  // Set the maximum speed and acceleration for the motor
  stepper.setMaxSpeed(1000);      // Set maximum speed (steps per second)
  stepper.setAcceleration(500);   // Set acceleration (steps per second^2)
  
  stepper.moveTo(-targetStep);

  while (stepper.distanceToGo() != 0){
    stepper.run();
  } 
  
  canTriggerButton = true;
  
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("LiDAR");
  display.println("Turret");
  display.display(); 
  
  // Start by moving to the initial position
}

void loop() {
  if (digitalRead(buttonInput) == LOW && canTriggerButton){
    canTriggerButton = false;
    stepper.moveTo(targetStep);      // Move to position `stepLimit`
  
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
  }
  // Run the motor towards the target position
  stepper.run();

  // When the motor reaches the set position, reverse the direction
  if (stepper.distanceToGo() == 0 && !canTriggerButton) {
    // If at the target position, reverse direction
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
