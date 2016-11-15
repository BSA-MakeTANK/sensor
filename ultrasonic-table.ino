/*
HC-SR04 Ping distance sensor]
VCC to arduino 5v GND to arduino GND

Please note that there is a batch of these sensors that are boken and get stuck at 0.
If you have such a sensor, your best bet is to aquire a new one.
*/

#define trigPin 13
#define echoPin 12
/*************************************************************************/
#define redPin 11
#define greenPin 10
#define bluePin 9
/**************************************************************************/
/*  Definitions for color smoothing
 *  The idea is to
 *  a) At mindistance the color has one value, at maxdistance another. Distances closer 
 *     than mindistance or futher than maxdistance don't cause any more change in color.
 *     We want to make use of the full colorspace in a range of distances that are actually
 *     relevant.
 *  b) When the color changes, we change is slowly to prevent flickering.
 *     colorstep specifies how much the color can change per time step.
 *     Setting this to a larger value will make color changes faster, a smaller value will 
 *     make them smoother. (Do not use numbers above 0.1 or the code will break.)
 */
#define mindistance 0.    // in cm
#define maxdistance 128.  // in cm
#define colorstep 0.01

float current_distance, range, norm_dist;

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redPin, OUTPUT); // sets the redPin to be an output
  pinMode(greenPin, OUTPUT); // sets the greenPin to be an output
  pinMode(bluePin, OUTPUT); // sets the bluePin to be an output
  current_distance = 0.;
  range = maxdistance - mindistance;
}

void loop() {
  long duration, distance;
  float d;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  // measured distance in cm
  distance = (duration/2) / 29.1;

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

  // Just some output for debugging. Can be ignored if everything works.
  Serial.println(current_distance);
  //Serial.print(distance);
  //Serial.println(" cm (restricted to range mindistance to maxtistance)");

  delay(10);
} 

/******************************************************/
/*  For output the color values in the red, green and blue pin must be
 *  in the range 0..255. This function scales an input in the range 0..1
 *  to 0..255.
 *  
 *  The last line sets the color of value 0 and 1.
 *  (e.g. (n, 0, 255 - n) will be red (for n=0) to blue (for n = 255).
 */
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
