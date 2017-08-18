
#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;

// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.

#define LONG_RANGE


// Uncomment ONE of these two lines to get
// - higher speed at the cost of lower accuracy OR
// - higher accuracy at the cost of lower speed

//#define HIGH_SPEED
//#define HIGH_ACCURACY

int greenLED = 9;
int redLED = 8;
int LED = 13;
int countDown = 0;
int calibratedDistance = 0;
int distanceInInch = 0;
int countDownCheck = 0;
int reps = 0;

// plus or minus inches (12 would be plus or minus 1' for a 2' max)
int calibrationPad = 12; 



void setup()
{
  //Serial output high to attempt to allow serial monitor keep up with arduino output
  Serial.begin(115200);
  Wire.begin();
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  sensor.init();
  sensor.setTimeout(500);

#if defined LONG_RANGE
  // lower the return signal rate limit (default is 0.25 MCPS)
  sensor.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
#endif

#if defined HIGH_SPEED
  // reduce timing budget to 20 ms (default is about 33 ms)
  sensor.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
  // increase timing budget to 200 ms
  sensor.setMeasurementTimingBudget(200000);
#endif
}


void calibrate_distance (){

if (countDown < 10) {
    countDown ++;  
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH); 
    Serial.print("CountDown: ");
    Serial.print(countDown);
    Serial.print(" Distance in Inches: "); 
    Serial.println(distanceInInch); 
    delay(500);
    } 

if (countDown == 10){
    calibratedDistance = distanceInInch;  
    countDown ++;
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
    countDownCheck = 1;
 }

}

void loop() {

  
  distanceInInch = sensor.readRangeSingleMillimeters() / 24.5;

if (calibratedDistance == 0) {calibrate_distance();}

if (countDownCheck == 1) {

if ((distanceInInch < (calibratedDistance - calibrationPad))) {reps ++;}

 //Serial.println(inchReading);
    Serial.print("Distance in Inches: "); 
    Serial.print(distanceInInch); 
    Serial.print(" Calibrated Distance: "); 
    Serial.print(calibratedDistance);
    Serial.print(" Reps: ");
    Serial.println(reps);
    
  if (sensor.timeoutOccurred()) { Serial.println(" TIMEOUT"); 
  
  Serial.print(" ** ");
    Serial.print(" Calibrated Distance: "); 
    Serial.print(calibratedDistance);
    Serial.print(" Reps: ");
    Serial.println(reps);
  
  }

}
 
}
