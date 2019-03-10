// StepperMotorController.c starter file EE319K Lab 5
// Runs on TM4C123
// Finite state machine to operate a stepper motor.  
// Jonathan Valvano
// January 18, 2019

// Hardware connections (External: two input buttons and four outputs to stepper motor)
//  PA5 is Wash input  (1 means pressed, 0 means not pressed)
//  PA4 is Wiper input  (1 means pressed, 0 means not pressed)
//  PE5 is Water pump output (toggle means washing)
//  PE4-0 are stepper motor outputs 
//  PF1 PF2 or PF3 control the LED on Launchpad used as a heartbeat
//  PB6 is LED output (1 activates external LED on protoboard)

#include "SysTick.h"
#include "TExaS.h"
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

#define SYSCTL_RCGCGPIO_R       (*((volatile uint32_t *)0x400FE608))

#define GPIO_PORTA_DATA_R				(*((volatile uint32_t *)0x400043FC))
#define GPIO_PORTA_DIR_R				(*((volatile uint32_t *)0x40004400))
#define GPIO_PORTA_DEN_R				(*((volatile uint32_t *)0x4000451C))

#define GPIO_PORTB_DATA_R       (*((volatile uint32_t *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile uint32_t *)0x40005400))
#define GPIO_PORTB_DEN_R        (*((volatile uint32_t *)0x4000551C))

#define GPIO_PORTE_DATA_R       (*((volatile uint32_t *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile uint32_t *)0x40024400))
#define GPIO_PORTE_DEN_R        (*((volatile uint32_t *)0x4002451C))

#define GPIO_PORTF_DATA_R				(*((volatile uint32_t *)0x400253FC))
#define GPIO_PORTF_DIR_R				(*((volatile uint32_t *)0x40025400))
#define GPIO_PORTF_DEN_R 				(*((volatile uint32_t *)0x4002551C))

void EnableInterrupts(void);
// edit the following only if you need to move pins from PA4, PE3-0      
// logic analyzer on the real board
#define PA4       (*((volatile unsigned long *)0x40004040))
#define PE50      (*((volatile unsigned long *)0x400240FC))
void SendDataToLogicAnalyzer(void){
  UART0_DR_R = 0x80|(PA4<<2)|PE50;
}

//write functions for spin motor with led flash input arg

int main(void){ 
  TExaS_Init(&SendDataToLogicAnalyzer);    // activate logic analyzer and set system clock to 80 MHz
  SysTick_Init();   
// you initialize your system here
	
	//set up statemachine and inputs

	
  EnableInterrupts();   
  while(1){
		//check current state output
		//conditional if
		//if output is 1, fcnx
		//if output is 2, fcny
		//if output is 0, check current state again
		
// output
// wait
// input
// next		
  }
}


