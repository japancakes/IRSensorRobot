#include "mbed.h"
 
// DC motors
DigitalOut IN1(PA_9);
DigitalOut IN2(PC_7);
DigitalOut IN3(PB_6);
DigitalOut IN4(PA_6);
 
PwmOut ENA(PB_4);
PwmOut ENB(PB_10);
 
// IR sensors
AnalogIn lineSensor_1(PA_0);
AnalogIn lineSensor_2(PA_1);
float sensor_1, sensor_2;
 
// 8-bit display
DigitalOut MiddleBar(PA_10);
DigitalOut LowerRight(PB_5);
DigitalOut Bottom(PA_8);
 
// PC display
Serial pc(PA_2, PA_3);
 
void read_sensor_values() {
    sensor_1 = lineSensor_1;
    sensor_2 = lineSensor_2;    
}
 
void track_Line(){
 
  read_sensor_values();
  pc.printf("Sensor 1: %f Sensor 2: %f\r\n", sensor_1, sensor_2);
 
  // Turn right. The right sensor reads black and left sensor reads white.
  if((sensor_1 < 0.5)&&(sensor_2 >= 0.5)){ 
    turn_right(1);
    MiddleBar = 1;
    LowerRight = 1;
    Bottom = 0;
  }
 
   // Turn left: The right sensor reads white and left sensor reads black.
  else if((sensor_1 >= 0.5)&&(sensor_2 < 0.5)){
    turn_left(1);
    MiddleBar = 0;
    LowerRight = 1;
    Bottom = 1;
  }
 
  // Move forward: The left and right sensors both read white.
  else if((sensor_1 < 0.5)&&(sensor_2 < 0.5)){
    forward();
    MiddleBar = 0;
    LowerRight = 0;
    Bottom = 0;
  }
 
  // Stop: The left and right sensors both read black.
  else if((sensor_1 >= 0.5)&&(sensor_2  >= 0.5)){
    stop();
    MiddleBar = 1;
    LowerRight = 0;
    Bottom = 0;
  }
}
 
void forward() {
    pc.printf("forward()\r\n");
    IN1 = 0;
    IN2 = 1;
    IN3 = 0;
    IN4 = 1;
    set_motorspeed(1,1);
}
 
void stop() {
    pc.printf("stop()\r\n");
    IN1 = 0;
    IN2 = 0;
    IN3 = 0;
    IN4 = 0;
    set_motorspeed(1,1);
}
 
void turn_left(int t) {
    IN1 = 1;
    IN2 = 0;
    IN3 = 0;
    IN4 = 1;
    set_motorspeed(0.75,1);
    wait(t);
}
 
void turn_right(int t) { 
    IN1 = 0;
    IN2 = 1;
    IN3 = 1;
    IN4 = 0;
    set_motorspeed(1,0.75);
    wait(t);
}
 
 
void set_motorspeed(float lspeed, float rspeed) {
   ENA = lspeed;
   ENB = rspeed;
}
 
int main(void) {
    ENA.period(1);
    ENB.period(1);
    set_motorspeed(1,1);
    while (1) {
        track_Line();
        wait(.01);
    }
}  
 
