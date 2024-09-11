#include <AccelStepper.h>

// Define motor interface type
#define MotorInterfaceType 4

// Define motor connections (adapt based on your setup)
#define motorPin1 18  // IN1 on the L298N
#define motorPin2 19  // IN2 on the L298N
#define motorPin3 21  // IN3 on the L298N
#define motorPin4 22  // IN4 on the L298N

// Create an instance of AccelStepper with 4-wire connection
AccelStepper stepper(MotorInterfaceType, motorPin1, motorPin2, motorPin3, motorPin4);

// Define the number of steps for one full cycle
const int stepLimit = 2000; // Adjust this to your motor's needs

void setup() {
  // Set the maximum speed and acceleration for the motor
  stepper.setMaxSpeed(1000);      // Set maximum speed (steps per second)
  stepper.setAcceleration(500);   // Set acceleration (steps per second^2)
  
  // Start by moving to the initial position
  stepper.moveTo(stepLimit);      // Move to position `stepLimit`
}

void loop() {
  // Run the motor towards the target position
  stepper.run();

  // When the motor reaches the set position, reverse the direction
  if (stepper.distanceToGo() == 0) {
    // If at the target position, reverse direction
    stepper.moveTo(-stepper.currentPosition());
  }
}
