Force_Sensitive_Resistor_Example.ino
Example sketch for SparkFun's force sensitive resistors
  (https://www.sparkfun.com/products/9375)
Jim Lindblom @ SparkFun Electronics
April 28, 2016

Create a voltage divider circuit combining an FSR with a 3.3k resistor.
- The resistor should connect from A0 to GND.
- The FSR should connect from A0 to 3.3V
As the resistance of the FSR decreases (meaning an increase in pressure), the
voltage at A0 should increase.

Development environment specifics:
Arduino 1.6.7
******************************************************************************/
const int FSR_PIN = A5; // Pin connected to FSR/resistor divider

// Measure the voltage at 5V and resistance of your 3.3k resistor, and enter
// their value's below:
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 1000.0; // Measured resistance of 3.3k resistor

/*************************************************************************/
#define redPin 11
#define greenPin 10
#define bluePin 9
/**************************************************************************/
#define mindistance 0.
#define maxdistance 15000.
#define colorstep 0.02
float current_distance, range, norm_dist;

void setup() 
{
  Serial.begin(9600);
  pinMode(FSR_PIN, INPUT);
  pinMode(redPin, OUTPUT); // sets the redPin to be an output
  pinMode(greenPin, OUTPUT); // sets the greenPin to be an output
  pinMode(bluePin, OUTPUT); // sets the bluePin to be an output
  current_distance = 0.;
  range = maxdistance - mindistance;
}

void loop() 
{
  float force, distance;
  int fsrADC = analogRead(FSR_PIN);
  // If the FSR has no pressure, the resistance will be
  // near infinite. So the voltage should be near 0.
  if (fsrADC != 0) // If the analog reading is non-zero
  {
    // Use ADC reading to calculate voltage:
    float fsrV = fsrADC * VCC / 1023.0;
    // Use voltage and static resistor value to 
    // calculate FSR resistance:
    float fsrR = R_DIV * (VCC / fsrV - 1.0);
    Serial.println("Resistance: " + String(fsrR) + " ohms");
    // Guesstimate force based on slopes in figure 3 of
    // FSR datasheet:
    float fsrG = 1.0 / fsrR; // Calculate conductance
    // Break parabolic curve down into two linear slopes:
    if (fsrR <= 600) 
      force = (fsrG - 0.00075) / 0.00000032639;
    else
      force =  fsrG / 0.000000642857;
    Serial.println("Force: " + String(force) + " g");
    Serial.println();

    //delay(500);
  }
  else
  {
    force = 0.;
  }

  distance = force;
  
  // if measured distance is outside min / max interval, reset it
  if (distance < mindistance){
    distance = mindistance;
  }
  if (distance > maxdistance){
    distance = maxdistance;
  }
  // move color in the direction of new_distance, but only slowly
  // to avoid flickering
  norm_dist = ((float)distance - mindistance) / range;
  // but never change by more than
  if (norm_dist > (current_distance + colorstep)){
    current_distance += colorstep;
  }
  else{
    if (norm_dist < (current_distance - colorstep)){
    current_distance -= colorstep;
    }
  }
  color_from_scalar(current_distance);

  Serial.println(current_distance);
  //Serial.print(distance);
  //Serial.println(" cm (restricted to range mindistance to maxtistance)");

  delay(10);
} 

/******************************************************/
void color_from_scalar (float number) 
{
  if (number > 1.){
    number = 1.;
  }
  if (number < 0){
    number = 0.;
  }
  unsigned char n;
  n = (unsigned char) (number * 255.);
  color(n, 0, 255 - n);
}
/******************************************************/


/******************************************************/
void color (unsigned char red, unsigned char green, unsigned char blue) // the color generating function
{
analogWrite(redPin, red);
analogWrite(greenPin, green);
analogWrite(bluePin, blue);
}
/******************************************************/
