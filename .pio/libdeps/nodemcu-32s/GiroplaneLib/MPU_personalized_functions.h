#include <SparkFunMPU9250-DMP.h>

#define SerialPort 
#define SerialUSB

MPU9250_DMP imu;

const signed char orientationDefault[9] = {0, 1, 0, 0, 0, 1, 1, 0, 0};

struct Quaternion {
    float QW; 
    float QX; 
    float QY; 
    float QZ; 
}; 

void printIMUData(void)
{  
  // After calling dmpUpdateFifo() the ax, gx, mx, etc. values
  // are all updated.
  // Quaternion values are, by default, stored in Q30 long
  // format. calcQuat turns them into a float between -1 and 1
  float q0 = imu.calcQuat(imu.qw);
  float q1 = imu.calcQuat(imu.qx);
  float q2 = imu.calcQuat(imu.qy);
  float q3 = imu.calcQuat(imu.qz);

  Serial.println("Q: " + String(q0, 4) + ", " +
                    String(q1, 4) + ", " + String(q2, 4) + 
                    ", " + String(q3, 4));
  Serial.println("R/P/Y: " + String(imu.roll) + ", "
            + String(imu.pitch) + ", " + String(imu.yaw));
  Serial.println("Time: " + String(imu.time) + " ms");
  Serial.println();
}

struct Quaternion getQuaternion(void){

    struct Quaternion Quat; // Generates an instance of a quaternion extructure
    while(true){
    if ( imu.fifoAvailable() )
    {
    // Use dmpUpdateFifo to update the ax, gx, mx, etc. values
    if ( imu.dmpUpdateFifo() == INV_SUCCESS)
    {
        Quat.QW = imu.calcQuat(imu.qw);
        Quat.QX = imu.calcQuat(imu.qx);
        Quat.QY = imu.calcQuat(imu.qy);
        Quat.QZ = imu.calcQuat(imu.qz);
        break; 
    }
  }
  } // end while
  return Quat; 
}


void InitializeMPU(void){
      if (imu.begin() != INV_SUCCESS)
  {
    while (1)
    {
      Serial.println("Unable to communicate with MPU-9250");
      Serial.println("Check connections, and try again.");
      Serial.println();
      delay(5000);
    }
  }

  imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT | // Enable 6-axis quat
               DMP_FEATURE_GYRO_CAL, // Use gyro calibration
              10); // Set DMP FIFO rate to 10 Hz
  // DMP_FEATURE_LP_QUAT can also be used. It uses the 
  // accelerometer in low-power mode to estimate quat's.
  // DMP_FEATURE_LP_QUAT and 6X_LP_QUAT are mutually exclusive
}

void printQuaternion(struct Quaternion Quat){
  Serial.print("QW:") ;
  Serial.print(Quat.QW); 
  Serial.print(" QX:") ;
  Serial.print(Quat.QX); 
  Serial.print(" QY:") ;
  Serial.print(Quat.QY); 
  Serial.print(" QZ:") ;
  Serial.println(Quat.QZ); 
}