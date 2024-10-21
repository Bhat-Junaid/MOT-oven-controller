// Driving the Oven door bipolar motor

// Motor pins
const int E1 = 8;
const int M1 = 9;
const int E2 = 10;
const int M2 = 11;

// Number of steps per revolution
// The mounted motor has 200 steps per revolution and we do 4 mini-steps in one step_rev, step_rev = 50 means the motor does full rotation

const int step_rev = 50; 
const int step45 = 6;  // 43.2 degrees 
const int step25 = step_rev /50; // 7.2 degrees 
const int stepDelay = 10;  // a delay of 10 ms between the mini-steps

// Switch pins
const int switchPinA0 = A0;   // Toggle switch connection
const int switchPinA1 = A1;  // Another Toggle switch connection
const int reset_pin = A2;   // Press Button Connection
const int controlPinA3 = A3; // TTL logic connection

int curr_pos = 0;  // Track the current step position

void setup() {
  // Set the pin modes
  pinMode(M1, OUTPUT);
  pinMode(E1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(E2, OUTPUT);

  Serial.begin(9600);

  // Initialize the switch pins as input
  pinMode(switchPinA0, INPUT);
  pinMode(switchPinA1, INPUT);
  pinMode(reset_pin, INPUT);
  pinMode(controlPinA3, INPUT);


}

void loop() {

  // Read the switch states
  bool switchStateA0 = digitalRead(switchPinA0);
  bool switchStateA1 = digitalRead(switchPinA1);
  bool resetButtonState = digitalRead(reset_pin);
  int control_vol = analogRead(controlPinA3);

  // Convert analog reading to voltage
  // Do not use "map" function to convert the voltage in your preferred range, it can handle only integers
  float vol_converted = control_vol * (5.0 / 1023.0); 

  // To Debug 
  Serial.print("Switch State A0: ");
  Serial.print(switchStateA0);

  Serial.print(" Switch State A1: ");
  Serial.print(switchStateA1);

  Serial.print(" Reset Button State: ");
  Serial.print(resetButtonState);

  Serial.print(" Control Voltage: ");
  Serial.println(vol_converted);

  Serial.print("Current Position: ");
  Serial.println(curr_pos);

  // Primary logic for switch control
  if (resetButtonState == LOW) {  
    if (switchStateA0 == HIGH && switchStateA1 == LOW && curr_pos == 0) {
      rotateMotor(step45, true);  // 43.2 degrees clockwise
      curr_pos = 1;  // Update the current position
    } else if (switchStateA1 == HIGH && switchStateA0 == LOW && curr_pos == 1) {
      rotateMotor(step45, false);  // 43.2 degrees counterclockwise
      curr_pos = 0;  // Update the current position
    } else if (switchStateA0 == LOW && switchStateA1 == LOW) {
      if (vol_converted > 2.0 && curr_pos == 0) {
        rotateMotor(step45, true);  // 43.2 degrees clockwise
        curr_pos = 1;  // Update the current position
      } else if (vol_converted <= 2.0 && curr_pos == 1) {
        rotateMotor(step45, false);  // 43.2 degrees counterclockwise
        curr_pos = 0;  // Update the current position
      }
    }
  }

  // Rotate the motor 7.2 degrees when the reset button is pressed
  if (resetButtonState == HIGH) {
    rotateMotor(step25, true);  // Rotate 7.2 degrees
  }

  delay(100);
}

// rotates the motor according to the boolean value of clockwise variable
// "How to rotate" is conveyed by "stepMotor" function
// rotates till n steps
void rotateMotor(int steps, bool clockwise) {
  for (int i = 0; i < steps; i++) {
    stepMotor(clockwise);
  }
}

// Tells the motor on how to use the mini-steps in order 
// informs the motor to either move in clockwise or anti-clockwise direction
void stepMotor(bool clockwise) {
  if (clockwise) {
    step1();
    delay(stepDelay); // Adjust the delay to control the speed
    step2();
    delay(stepDelay);
    step3();
    delay(stepDelay);
    step4();
    delay(stepDelay);
  } else {
    step4();
    delay(stepDelay);
    step3();
    delay(stepDelay);
    step2();
    delay(stepDelay);
    step1();
    delay(stepDelay);
  }
}

// Steps to drive the motor
// each one of them is an individual step, what we have here called a mini-step
// It provides the logic on how to supply output to pins so they drive the bipolar motor in a correct way 
void step1() {
  digitalWrite(E1, HIGH);
  digitalWrite(M1, HIGH);
  digitalWrite(E2, LOW);
  digitalWrite(M2, LOW);
  
}

void step2() {
  digitalWrite(E1, LOW);
  digitalWrite(M1, LOW);
  digitalWrite(E2, HIGH);
  digitalWrite(M2, HIGH);
}

void step3() {
  digitalWrite(E1, HIGH);
  digitalWrite(M1, LOW);
  digitalWrite(E2, LOW);
  digitalWrite(M2, LOW);
}

void step4() {
  digitalWrite(E1, LOW);
  digitalWrite(M1, HIGH);
  digitalWrite(E2, HIGH);
  digitalWrite(M2, LOW);
}

