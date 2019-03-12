// StepperMotorController.c starter file EE319K Lab 5
// Runs on TM4C123
// Finite state machine to operate a stepper motor.  
// Jonathan Valvano
// January 18, 2019

// Hardware connections (External: two input buttons and four outputs to stepper motor)
//  PA3 is Wash input  (1 means pressed, 0 means not pressed)
//  PA2 is Wiper input  (1 means pressed, 0 means not pressed)
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

#define start16		0
#define start1		1
#define FWD2			2
#define FWD4			3
#define FWD8			4
#define FWD16			5
#define N_FWD1		6
#define N_FWD2		7
#define N_FWD4		8
#define N_FWD8		9
#define maxpos16	10
#define BCK8			11
#define BCK4			12
#define BCK2			13
#define BCK1			14
#define BCK16			15
#define N_BCK8		16
#define N_BCK4		17
#define N_BCK2		18
#define LED_F2		19
#define LED_F8		20
#define NLED_F1		21
#define NLED_F4		22
#define LED_max16	23
#define LED_B4		24
#define LED_B1		25
#define NLED_B8		26
#define NLED_B2		27
#define link1			28

void EnableInterrupts(void);
// edit the following only if you need to move pins from PA4, PE3-0      
// logic analyzer on the real board
#define PA4       (*((volatile unsigned long *)0x40004040))
#define PE50      (*((volatile unsigned long *)0x400240FC))
void SendDataToLogicAnalyzer(void){
  UART0_DR_R = 0x80|(PA4<<2)|PE50;
}

//write functions for spin motor with led flash input arg
struct state {
		uint8_t out; //int output 0,1,2
		uint16_t delay; //delay*10ms = time
		uint16_t next[4]; //next 3 possible states
};
typedef const struct state state_t;

int main(void){
	
	
	
	
	
	
	
	
	
	
	
	
	
	state_t fsm[29] = {
		{16,10,{start16, start1, start1, start16}},
		{1,10,{start16,FWD2,LED_F2,start1}},
		{2,10,{start1,FWD4,LED_F2,FWD2}},
		{4,10,{FWD2,FWD8,LED_F8,FWD4}},
		{8,10,{FWD4,FWD16,LED_F8,FWD8}},
		{16,10,{FWD8,N_FWD1,NLED_F1,FWD16}},
		{1,10,{FWD16,N_FWD2,NLED_F1,N_FWD1}},
		{2,10,{N_FWD1,N_FWD4,NLED_F4,N_FWD2}},
		{4,10,{N_FWD2,N_FWD8,NLED_F4,N_FWD4}},
		{8,10,{N_FWD4,maxpos16,LED_max16,N_FWD8}},
		{16,10,{N_FWD8,BCK8,LED_max16,maxpos16}},
		{8,10,{maxpos16,BCK4,LED_B4,BCK8}},
		{4,10,{BCK8,BCK2,LED_B4,BCK4}},
		{2,10,{BCK4,BCK1,LED_B1,BCK2}},
		{1,10,{BCK2,BCK16,LED_B1,BCK1}},
		{16,10,{BCK1,N_BCK8,NLED_B8,BCK16}},
		{8,10,{BCK16,N_BCK4,NLED_B8,N_BCK8}},
		{4,10,{N_BCK8,N_BCK2,NLED_B2,N_BCK4}},
		{2,10,{N_BCK4,link1,NLED_B2,N_BCK2}}, //maybe problematic
		{34,10,{start1,FWD4,FWD4,LED_F2}}, //led stuff down here
		{40,10,{FWD4,FWD16,FWD16,LED_F8}},
		{33,10,{FWD16,N_FWD2,N_FWD2,NLED_F1}},
		{36,10,{N_FWD2,N_FWD8,N_FWD8,NLED_F4}},
		{48,10,{N_FWD8,BCK8,BCK8,LED_max16}},
		{36,10,{BCK8,BCK2,BCK2,LED_B4}},
		{33,10,{BCK2,BCK16,BCK16,LED_B1}},
		{40,10,{BCK16,N_BCK4,N_BCK4,NLED_B8}},
		{34,10,{N_BCK4,link1,link1,NLED_B2}},	
		{1,10,{link1,start16,start16,link1}}
	};
	uint16_t input;
	uint8_t stepperout;
	uint32_t delay = 100;
	uint8_t cs=0;
	SYSCTL_RCGCGPIO_R |= 0x33;
  delay++;
	GPIO_PORTA_DIR_R  &= 0xF3;
	GPIO_PORTA_DEN_R	|= 0x0C;
	GPIO_PORTB_DIR_R  |= 0x40;
	GPIO_PORTB_DEN_R  |= 0x40;
	GPIO_PORTE_DIR_R  |= 0x3F;
	GPIO_PORTE_DEN_R  |= 0x3F;
	GPIO_PORTF_DIR_R  |= 0x1;
	GPIO_PORTF_DEN_R  |= 0x1;
	
	
	
	
  
	TExaS_Init(&SendDataToLogicAnalyzer);    // activate logic analyzer and set system clock to 80 MHz
  SysTick_Init();   
  EnableInterrupts(); 
  
  while(1){
		//call move fcn with cs as arg
		stepperout = fsm[cs].out;
		GPIO_PORTE_DATA_R = stepperout;
		
		SysTick_Wait10ms(fsm[cs].delay);
		
		input = GPIO_PORTA_DATA_R;
		input &= 0x0C;
		input= input>>2;
		
		cs = fsm[cs].next[input];
		
  }
}



