/*
Servo for rotator, v4 -- Andrew R Gross
 */

#include <Servo.h>

Servo servo; // create servo object

byte PWM_PIN = 3;
unsigned long pwm_value;
int angle;
int setpoint;
int setpointDistance;
int distCW;
int distCCW;
int posCounter = 0;
//int positions[] = {0, 90, 180, 270, 0, 90, 180, 270, 0, 90, 180, 270, 0, 90, 180, 270};
int positions[] = {0, 135, 270, 45, 180, 315, 90, 225, 0, 135, 270, 45, 180, 315, 90, 225,  0, 180};
//unsigned long waitDurations[] = {10000, 10000, 10000, 10000, 2700000, 2700000, 2700000, 2700000,  270000000};
unsigned long waitDurations[] = {4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 3600000, 3600000, 3600000, 3600000, 3600000, 3600000, 3600000, 3600000, 36000000};

unsigned long currentMillis;
unsigned long waitMillis;
unsigned long lastActionMillis;
unsigned long reportMillis;

void setup() {
  Serial.begin(9600);
    Serial.write("\n\n\n");
  pinMode(PWM_PIN, INPUT);

//  waitMillis = 27000000;
  waitMillis = waitDurations[posCounter];          // Turn every 4 seconds for testing
  setpoint = positions[posCounter];

  servo.attach(9);
}

void loop() {

/// Check time
  currentMillis = millis();

/// Read position
  pwm_value = pulseIn(PWM_PIN, HIGH);
  angle = map(pwm_value, 32, 1050, 1, 360);
/*
  Serial.print("Sensor value = ");
  Serial.print(pwm_value);

  Serial.print("\t ### ### ### Angle = ");
  Serial.print(angle);

  Serial.print("  \t %%% %%% %%% Setpoint = ");
  Serial.print(setpoint);
*/
/// Find distance to setpoint in both directions
  distCW = setpoint - angle;
  if (distCW < 0){
    distCW = distCW + 360;
    }
  
  distCCW = angle - setpoint;
  if (distCCW < 0){
    distCCW = distCCW +360;
  }
/*
  Serial.print(" \t &&& &&& &&& distCW = ");
  Serial.print(distCW);
  Serial.print(" \t ... distCCW = ");
  Serial.print(distCCW);
  Serial.print(" \t --- minDist = ");
  Serial.println(min(distCW, distCCW));
*/
/// Check if it's time to change position
  if (currentMillis - lastActionMillis > waitMillis){
    posCounter = posCounter + 1;
    setpoint = positions[posCounter];
    waitMillis = waitDurations[posCounter];      // Advance wait time to next position in the array

    Serial.print("Setpoint: ");
    Serial.print(setpoint);
    Serial.print(" ... Wait vector ");
    Serial.print(waitDurations[4]);

    Serial.print("Action number: ");
    Serial.print(posCounter);
    Serial.print(" ... Wait time changed to: ");
    Serial.println(waitMillis);

    if (setpoint > 360) {                         // This should never be used now.
      setpoint = setpoint - 360;
    }
    //Serial.println(setpoint);
    lastActionMillis = currentMillis;             // Record the current time as the last time an action was taken
  }

/// Check if the angle matches the setpoint
  if(min(distCW, distCCW) <9){
    servo.write(92);
    delay(100);
  }
  else if(min(distCW, distCCW) <22) {
    if(distCW < distCCW){
      servo.write(88);
    }
    else{
      servo.write(96);
    }
  }
  else{
    if(distCW < distCCW){
      servo.write(87);
    }
    else{
      servo.write(97);
    }
  }
  
}
