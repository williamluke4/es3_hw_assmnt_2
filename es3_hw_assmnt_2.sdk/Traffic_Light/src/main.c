
#include <stdio.h>
#include "platform.h"
#include "xil_types.h"
#include "gpio_init.h"
#include "seg7_display.h"
#include "traffic_light.h"


u8 status;
u8 ped_countdown = 0;

u16 pushBtnRightIn = 0;
u16 counter = 0;

u16 display = 0;
u8 ped_button = FALSE;

u16 TR1_Red = OFF, TR1_Yellow = OFF, TR1_Green = OFF; //Traffic Light 1
u16 TR2_Red = OFF, TR2_Yellow = OFF, TR2_Green = OFF; //Traffic Light 2
u16 Ped_Light = RED;

volatile u8 interruptCounter = 0;
volatile u8 interruptServiced = FALSE;



// Hardware timer interrupt service routine
void hwTimerISR(void *CallbackRef){
    interruptServiced = FALSE;
    interruptCounter++;
    displayDigit();

    if (interruptCounter == 25) //Running Every 0.1
    {



        next_state(); //Will run every 0.1

        interruptCounter = 0;



        interruptServiced = TRUE;
        displayDigit();
    }

    return;
}
void flash_ped(){
	//Will Turn OFF and ON 5 times in the last 2 second the ped light is on
	if((ped_countdown<=20) && ((ped_countdown%2)==0)){

		if (Ped_Light == GREEN)
		{
			Ped_Light = OFF;

		}
		else{
			Ped_Light = GREEN;
		}
		display = ped_countdown/10; // To get seconds
    }

    else if (ped_countdown>20 && ped_countdown<=50){
    	Ped_Light = GREEN;
    	display = ped_countdown/10; // To get seconds
    }

}

void next_state(){
 //Running every 0.1s

    if(TR1_Red == RED && TR2_Red == RED && ped_button == TRUE ){
    	    //Need a delay of two seconds
    		flash_ped();


            if(ped_countdown == 0){
            	XGpio_DiscreteWrite(&LED_OUT,1, 0);// Leds Turned Off
                ped_button = FALSE;
                Ped_Light = RED;
            }
            ped_countdown = ped_countdown -1;


    }

    else {
    	if((counter%10)==0){
    		display = 0;
    	}
    	if((counter%20)==0){
    		display = 1;
    	}
        switch(counter){
                case 0:// TR1:Yellow
                    TR2_Red = RED;
                    TR1_Red = RED;
                    TR2_Yellow = OFF;
                    TR1_Green = OFF;
                    break;
                case 20: //TR1:Green
                    TR1_Yellow = YELLOW;
                    TR1_Red = OFF;
                    break;
                case 40: //TR1:Yellow
                    TR1_Yellow = OFF;
                    TR1_Green = GREEN;
                    break;
                case 60: //TR1:Red
                    TR1_Green = OFF;
                    TR1_Yellow = YELLOW;
                    break;
                case 80:// TR2:Yellow
                    TR1_Red = RED;
                    TR1_Yellow = OFF;
                    break;
                case 100: //TR2:Green
                    TR2_Yellow = YELLOW;
                    TR2_Red = OFF;
                    break;
                case 120: //TR2:Yellow
                    TR2_Yellow = OFF;
                    TR2_Green = GREEN;
                    break;
                case 140: //TR2:Red
                    TR2_Green = OFF;
                    TR2_Yellow = YELLOW;
                    break;
                default:
                    break;
            }
        counter = counter + 1;
    }
    if(counter==160){counter = 0;}
}


int main(){

/*#########################     Setup    #################################*/

    init_platform();

        status = initGpio(); // Initialise the GPIOs
        if (status != XST_SUCCESS) {
            print("GPIOs initialisation failed!\n\r");
            cleanup_platform();
            return (0);
        }

        status = setUpInterruptSystem(); // Setup the Interrupt System
        if (status != XST_SUCCESS) {
            print("Interrupt system setup failed!\n\r");
            cleanup_platform();
            return (0);
        }

/*###########################   Main Loop   ###############################*/

        while (1){
            XGpio_DiscreteWrite(&REGION_0_COLOUR,1, TR1_Red);
            XGpio_DiscreteWrite(&REGION_1_COLOUR,1, TR1_Yellow);
            XGpio_DiscreteWrite(&REGION_2_COLOUR,1, TR1_Green);
            XGpio_DiscreteWrite(&REGION_3_COLOUR,1, TR2_Red);
            XGpio_DiscreteWrite(&REGION_4_COLOUR,1, TR2_Yellow);
            XGpio_DiscreteWrite(&REGION_5_COLOUR,1, TR2_Green);
            XGpio_DiscreteWrite(&REGION_10_COLOUR,1, Ped_Light);
            displayNumber(display);

            while (interruptServiced == FALSE){

            	pushBtnRightIn = XGpio_DiscreteRead(&P_BTN_RIGHT, 1);
            	if ((pushBtnRightIn == 1) && (ped_button==FALSE)) {

            		// Wait for the button to be released and checks if it has been pressed already
            		while (pushBtnRightIn == 1){

            			pushBtnRightIn = XGpio_DiscreteRead(&P_BTN_RIGHT, 1);
            			XGpio_DiscreteWrite(&LED_OUT,1, 65535);
            			ped_button = TRUE;//Pedestrian Button Activated
            			ped_countdown = 70;
            		}
            	}

            }
        }
        cleanup_platform();
        return (0);
}
