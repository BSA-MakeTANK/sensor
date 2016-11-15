#define redPin 11
#define greenPin 10
#define bluePin 9

/**************************************/

const int tiltPin = 8;
int i = 0;
int currentstate = LOW;

#define colorstep 0.0001

float distance;

void setup() {
  // put your setup code here, to run once:
  pinMode(tiltPin,INPUT);//set pin as INPUT
  digitalWrite(tiltPin, HIGH);//set pin as HIGH

}

void loop() {
  // put your main code here, to run repeatedly:
  int digitalVal = digitalRead(tiltPin);//Read the value of pin
  if(HIGH == digitalVal)//if tilt switch is not breakover
  {
    distance += colorstep;
    if(distance > 1.){
      distance = 1;
    }
  }
  else ////if tilt switch breakover
  {
    distance -= colorstep;
    if(distance < 0.){
      distance = 0.;
    }
  }
  color_from_scalar(distance);
  delayMicroseconds(10);
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
  color(n, 255 - n, 0);
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
