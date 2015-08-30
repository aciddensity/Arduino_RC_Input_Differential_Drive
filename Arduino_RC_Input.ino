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


int input_buffer = 20;

void setup()
{

  Serial.begin(9600);
}
void getInputs() {
  steerRaw = pulseIn(steeringInput, HIGH);
  throttleRaw = pulseIn(throttleInput, HIGH);

  steerIn = constrain(map(steerRaw, 1100, 1900, -255, 255), -255, 255);
  throttleIn = constrain(map(throttleRaw, 1100, 1900, -255, 255), -255, 255);
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
//  Serial.print("steerIn: ");
//  Serial.println(steerIn);
//  Serial.print("throttleIn: ");
//  Serial.println(throttleIn);
//  
  if(throttleIn>=input_buffer) {
    setDirection(FORWARD);
    motor1_output = throttleIn;
    motor2_output = throttleIn;
    
    if(steerIn >= input_buffer) {
     motor2_output = motor2_output - throttleIn; 
    } else if(steerIn <= -10) {
     motor1_output = motor1_output - throttleIn;
    }
  } else if(throttleIn <= -input_buffer) {
   
  } else {
    motor1_output = motor2_output = 0;
  }
  
  analogWrite(motor1_enable, motor1_output);
  analogWrite(motor2_enable, motor2_output);
  
//  delay(500);
}
