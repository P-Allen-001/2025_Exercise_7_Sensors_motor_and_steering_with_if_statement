#include "mbed.h"
#include "VL53L0X.h"
#include "SRF05.h"

I2C i2c(PB_9,PB_8);

SRF05 srf_Left(PA_10,PB_3); // Output from PA_10 (D2) is connected to the Trigger pin of the SRF05, Input to PB_3 (D3) is connected to the SRF05 Echo_Pin. 
SRF05 srf_Right(PA_8,PA_9); // Output from PA_8 (D7) is connected to the Trigger pin of the SRF05, Input to PA_9 (D8) is connected to the SRF05 Echo_Pin.   

PwmOut Drive_pin_A(PB_0);  //steering servo PWM output pin (CN8,A3)
PwmOut Drive_pin_B(PB_10); //Motor drive PWM output pin D6
PwmOut Drive_pin_C(PB_4);  //Motor drive PWM output pin D5 

VL53L0X sensor(&i2c);
Serial      usb(USBTX, USBRX, 9600);

float Left_Ultrasonic;
float Right_Ultrasonic;
float Time_of_flight;

int main(void) {

                Drive_pin_A.period_ms(20);   //Steering Servo PWM period
                Drive_pin_B.period_ms(20);   //Motor forward PWM period     
                Drive_pin_C.period_ms(20);   //Motor reverse PWM period 

                Drive_pin_A.pulsewidth_us(1500);    //Steering straight ahead
                Drive_pin_B.pulsewidth_ms(0);       //drive motor off
                Drive_pin_C.pulsewidth_ms(0);       //drive motor off


               sensor.init();
               sensor.setModeContinuous();
               sensor.startContinuous();

    while (1){

              Time_of_flight = sensor.getRangeMillimeters();
                Left_Ultrasonic = 10*srf_Left.read() ;
                Right_Ultrasonic = 10*srf_Right.read();                 
                           
              printf("Time of flight: %.0f \t", Time_of_flight);                                          
                printf("Left Ultrasonic: %.0f  \t"  , Left_Ultrasonic) ;
                printf("Right Ultrasonic: %.0f \t", Right_Ultrasonic);
                printf("\n");
                wait(0.05);   
             

    if (Left_Ultrasonic > 250 & Right_Ultrasonic > 250 & Time_of_flight > 250)
    
        {
            Drive_pin_A.pulsewidth_us(1500);            //Steer straight ahead
            Drive_pin_C.pulsewidth_ms(0);
            Drive_pin_B.pulsewidth_ms(20);              // Full power forward
        }


    if (Left_Ultrasonic < 250 & Right_Ultrasonic < 250)
    
        {
            Drive_pin_A.pulsewidth_us(1500);            //Steer straight ahead
            Drive_pin_C.pulsewidth_ms(0);
            Drive_pin_B.pulsewidth_ms(10);              // Half power forward
        }

    if (Right_Ultrasonic < 250 & Left_Ultrasonic > 250)
    
        {   
            Drive_pin_A.pulsewidth_us(2000);            //Steering full left
        }

    if (Right_Ultrasonic > 250 & Left_Ultrasonic < 250)
    
        {   
            Drive_pin_A.pulsewidth_us(1000);            //Steering full right
        }
   if (Time_of_flight<250)
    
        {
            Drive_pin_A.pulsewidth_us(1500);            //Steer straight           
            Drive_pin_C.pulsewidth_ms(10);              // Half speed backward
            Drive_pin_B.pulsewidth_ms(0);
        }

    
    }
    }