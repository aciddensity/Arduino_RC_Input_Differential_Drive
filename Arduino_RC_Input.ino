#define steeringInput 9
#define throttleInput 8

#define FORWARD 1
#define BACKWARD 0

#define motor1_enable 5

#define motor1_pin1 2
#define motor1_pin2 3


#define motor2_enable 6

#define motor2_pin1 10
#define motor2_pin2 11

unsigned long steerRaw;
unsigned long throttleRaw;

int throttleIn = 0;
int steerIn = 0;

int motor1_output = 0;
int motor2_output = 0;

//made two buffers so drive and steering can be adjusted individually
int drive_buffer = 70;
int steer_buffer = 15;

void setup()
{

  Serial.begin(9600);
}
void getInputs() { //changed "steerIn" thresholds to 1400 and 2500, and "throttleIn" thresholds to 1100 and 2300.
  steerRaw = pulseIn(steeringInput, HIGH); //read pulse and assign its value to steerRaw
  throttleRaw = pulseIn(throttleInput, HIGH); //read pulse and assign its value to throttleRaw

  steerIn = constrain(map(steerRaw, 1400, 2500, -255, 255), -255, 255); //returns map if between -255 and 255
  throttleIn = constrain(map(throttleRaw, 1000, 2000, -255, 255), -255, 255); //returns map value if between -255 and 255
}

void setDirection(int direction) {
  
  if(direction == FORWARD) {
    digitalWrite(motor1_pin1, HIGH);
    digitalWrite(motor1_pin2, LOW);  
  
    digitalWrite(motor2_pin1, LOW);
    digitalWrite(motor2_pin2, HIGH);
  } else {
    digitalWrite(motor1_pin1, LOW);
    digitalWrite(motor1_pin2, HIGH); 
    
    digitalWrite(motor2_pin1, HIGH);
    digitalWrite(motor2_pin2, LOW);
  }
}

void loop() {
  getInputs();
  
  
  if(throttleIn>=drive_buffer) {
    setDirection(FORWARD);
    motor1_output = throttleIn;
    motor2_output = throttleIn;
    
    if(steerIn >= steer_buffer) {
     motor2_output = motor2_output - throttleIn; 
    } else if(steerIn <= -10) {
     motor1_output = motor1_output - throttleIn;
    }
  } else if(throttleIn <= -drive_buffer) {
   setDirection(BACKWARD);
    motor1_output = throttleIn;
    motor2_output = throttleIn;
    
    if(steerIn >= steer_buffer) {
     motor1_output = motor1_output - throttleIn; 
    } else if(steerIn <= -10) {
     motor2_output = motor2_output - throttleIn;
  } else {
    motor1_output = motor2_output = 0;
  }
  }
  analogWrite(motor1_enable, motor1_output);
  analogWrite(motor2_enable, motor2_output);
  

// DEBUG Section: Changed both variables to print on one line for easier reading of the stream.
Serial.print("steerIn: ");
Serial.print(steerIn);
Serial.print(" throttleIn: ");
Serial.println(throttleIn);

//Serial.print("steerRaw: ");
//Serial.print(steerRaw);
//Serial.print("  throttleRaw: ");
//Serial.println(throttleRaw);

//  delay(500);
}
