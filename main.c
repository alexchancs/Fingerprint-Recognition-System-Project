#include <stdbool.h>
#include <stdint.h>
#include "boards.h"
#include "bsp.h"
#include "app_timer.h"
#include "nordic_common.h"
#include "nrf_error.h"
#include "nrf_delay.h"
#include "fingerprint.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


extern uint16_t Dectect_GD[26];
extern uint16_t Get_Image_GD[26];
extern uint16_t Contrast_GD[26];
extern uint16_t Contrast_GD_1[26];
extern uint16_t Contrast_GD_1[26];
extern uint16_t Search_GD[26];


#define LED_1 21
#define LED_2 22



int main(void)
{   
    nrf_gpio_cfg_output(LED_1);
    nrf_gpio_cfg_output(LED_2);
    nrf_gpio_pin_set(LED_1);
    nrf_gpio_pin_set(LED_2);
    uint8_t i; 

    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    Uart_init();


    int mode;
    int bias = 48;
    while(1){
    NRF_LOG_INFO("Uarts ready for communication \r\n");
    NRF_LOG_INFO("==========Meun=============== \r\n");
    NRF_LOG_INFO("Create a new user-----------1 \r\n");
    NRF_LOG_INFO("Verify your fingerprint-----2 \r\n");
    NRF_LOG_INFO("==========END================ \r\n");
    NRF_LOG_INFO("Enter your mode: \r\n");


    mode = SEGGER_RTT_WaitKey() - bias;


    switch(mode){

    case 1:
    NRF_LOG_INFO("Now u can create a new user!!\r\n");
    
    NRF_LOG_INFO("Ready to store your fingerprint \r\n");
    int i;
    for (i=0; i<3; i++)
     {
	  The_Finger_Detect();

	  NRF_LOG_INFO("Please put your finger on the Sensor \r\n");
          nrf_delay_ms(1000);

	  
          while(The_Sending_Data(Dectect_GD) == 1)
	   {		
              The_Finger_Detect();
               The_Sending_Data(Dectect_GD);	
                      
	     }

	  The_LED(LED_ON);
          nrf_gpio_pin_clear(LED_1);


          The_Get_Image();


          if(The_Sending_Data(Get_Image_GD)==0)
           {
              The_Ram_Buff(i);			 
            }


          NRF_LOG_INFO("FingerPrint Reading successful... \r\n");
          The_LED(LED_OFF) ;
          nrf_gpio_pin_set(LED_1);


          The_Finger_Detect() ;
          nrf_delay_ms(1000);

		
          while(The_Sending_Data(Dectect_GD)==0)
           {		
              The_Finger_Detect();
              The_Sending_Data(Dectect_GD);		
             }


       }

  NRF_LOG_INFO("Saving the FingerPrints... \r\n");
  The_Merge_Save();
  nrf_delay_ms(10);
  The_Char_Store();
  NRF_LOG_INFO("Successfuly Saved the FingerPrint \r\n");

    break;
    
    case 2 :
    NRF_LOG_INFO("Now u can Verify your fingerprint!!\r\n");
    
	The_Finger_Detect() ;
	
        while(The_Sending_Data(Dectect_GD)==1)
	 {		
            The_Finger_Detect() ;
            The_Sending_Data(Dectect_GD) ;		
	   }		

	The_LED(LED_ON) ;
        nrf_gpio_pin_set(LED_1);

        

	The_Get_Image() ;
		
        if(The_Sending_Data(Get_Image_GD)==0)
          {
            The_Ram_Buff(0) ;			 
            }	
                            
	The_LED(LED_OFF) ;
        
	The_Verify_Contrast(); 
	
        if(Sending_fingerprint(Contrast_GD)==0)
          {
            NRF_LOG_INFO("Fingerprint recognition Successful ... \r\n");
            nrf_gpio_pin_clear(LED_2);
            nrf_delay_ms(1000);
            nrf_gpio_pin_set(LED_2);
            break;
            }

	else 
          {

            NRF_LOG_INFO("Fingerprint Not Recognized... Please Retry \r\n");
            nrf_gpio_pin_clear(LED_1); 
            nrf_delay_ms(1000);
            nrf_gpio_pin_set(LED_1);
            
            }



    break;

    default:
    break;
    }


}

}





