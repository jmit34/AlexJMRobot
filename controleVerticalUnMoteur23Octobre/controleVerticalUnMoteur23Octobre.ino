// MPU-6050 Short Example Sketch
// By Arduino User JohnChi
// August 17, 2014
// Public Domain
#include<Wire.h> 
// i2c SDA sur A4 bleu 
// i2c SCL sur A5 vert
// GND noir, VCC blanc

#define pinEnable 13 // ENABLE Activation du driver/pilote
#define pinStep    9 // STEP Signal de PAS (avancement)
#define pinDir     8 // DIR Direction 

const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t angle; 
int angleConsigne = 0;

void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
  Serial.println("Test StepStrick (polulu) A4988");
  pinMode( pinEnable, OUTPUT );
  pinMode( pinDir   , OUTPUT );
  pinMode( pinStep  , OUTPUT );
  
  digitalWrite( pinStep  , LOW);  // Initialisation de la broche step

}
int i = 0;

int avance = 100;
int a = 0; 

void loop(){
  i++; Serial.print(i); Serial.print(" ");
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
 
  angle=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  Serial.println(angle);
  
      if (angle > angleConsigne) {
         digitalWrite( pinDir   , HIGH); // Direction avant
         digitalWrite( pinStep, HIGH );
         delay(5) ;
         digitalWrite( pinStep, LOW );
         delay( 5 );
     } else {
        digitalWrite( pinDir   , LOW); // Direction arrière
        digitalWrite( pinStep, HIGH );
        delay( 5);
        digitalWrite( pinStep, LOW );
        delay( 5 );
     }
  
delay(40);
}
