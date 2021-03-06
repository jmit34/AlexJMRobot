// MPU-6050 : gyroscope
// 2 nema, 2 stepSticks
// By Arduino Users Alexandre & Jean-Michel Torres
// Octobre-Novembre 2016 

#include<Wire.h> 
// i2c SDA sur A4 bleu 
// i2c SCL sur A5 vert
// GND noir, VCC blanc
// moteur1 = droite, pins 2=dir 3=step 4=enable 
// moteur2 = gauche, pins 5=dir 6=step 7=enable
#define pinEnableMotor1  4 // ENABLE Activation du driver/pilote
#define pinStepMotor1    3 // STEP Signal de PAS (avancement)
#define pinDirMotor1     2 // DIR Direction 
#define pinEnableMotor2  7 // ENABLE Activation du driver/pilote
#define pinStepMotor2    6 // STEP Signal de PAS (avancement)
#define pinDirMotor2     5 // DIR Direction 

const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int16_t angle; 
int angleConsigne = 0;
int16_t angleOld1 = 0;
int stepTime = 4; 

void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
  Serial.println("Test StepStrick (polulu) A4988");
  pinMode( pinEnableMotor1, OUTPUT );
  pinMode( pinDirMotor1   , OUTPUT );
  pinMode( pinStepMotor1  , OUTPUT );
  pinMode( pinEnableMotor2, OUTPUT );
  pinMode( pinDirMotor2   , OUTPUT );
  pinMode( pinStepMotor2  , OUTPUT );
  
  digitalWrite( pinStepMotor1  , LOW);  // Initialisation de la broche step
  digitalWrite( pinStepMotor2  , LOW);  // Initialisation de la broche step
}

void loop(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  angleOld1 = angle; 
  angle=AcY;
  
  int erreur = int((angle-angleConsigne)/100);

  Serial.println("Erreur :" + erreur);

     if (erreur > 0) {
         avance(10);
     } else {
         recule(10);
     }
 delay(5);
 }

void avance(int n) {
  digitalWrite( pinDirMotor1 , HIGH); // Direction avant
  digitalWrite( pinDirMotor2 , HIGH); // Direction avant
  for (int i = 0; i <n ;i++){
     digitalWrite( pinStepMotor1, HIGH);
     digitalWrite( pinStepMotor2, HIGH);
     delay(stepTime) ;
     digitalWrite( pinStepMotor1, LOW );
     digitalWrite( pinStepMotor2, LOW );
     delay(stepTime);
  }
}
void recule(int n) {
  digitalWrite( pinDirMotor1 , LOW); // Direction avant
  digitalWrite( pinDirMotor2 , LOW); // Direction avant
  for (int i = 0; i <n ;i++){
     digitalWrite( pinStepMotor1, HIGH);
     digitalWrite( pinStepMotor2, HIGH);
     delay(stepTime) ;
     digitalWrite( pinStepMotor1, LOW );
     digitalWrite( pinStepMotor2, LOW );
     delay(stepTime);
  }
}
