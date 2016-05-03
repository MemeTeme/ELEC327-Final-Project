#include <msp430.h> 
#include "msp430g2553.h"
#include "lcdLib.h"
#define MAXCHECKS 15

//variables for button debouncing
int prevState;
int debouncedState = 0;				// Debounced state of the switches
int state[MAXCHECKS]={0};		// Array that maintains bounce status
int Index = 0;					// Pointer into state

int period = 1000000/261.63; // play C note when alarm sounds
int alarm_state; // 0 = not set, 1 = setting hours, 2 = setting minutes, 3 = set, 4 = ringing
char text[] = "00:00";

void serialInit(void);
void serialWrite(unsigned char c);
int debounceSwitch(void);
int rawPortData(void);


void main(void) {

	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    	if (CALBC1_1MHZ==0xFF)   // If calibration constant erased
    	{
    		while(1);          // do not load, trap CPU!!
    	}
    	BCSCTL1 = CALBC1_1MHZ;
    	DCOCTL  = CALDCO_1MHZ;
    	lcdInit(); // Initialize LCD
    	serialInit(); // initalize serial communication
    	//Button Configurations
    	P1DIR &= ~(BIT5 +BIT7);
    	P1REN |= BIT5 + BIT7;

    	//PWM Initialization
    	P1DIR |= BIT6;
    	P1SEL |= BIT6;
    	TA0CTL = TASSEL_2 + MC_1; //SMCLK, up mode
    	TA0CCR0 = period;
    	TA0CCR1 = 0;
    	TA0CCTL1 = OUTMOD_6;
    	WDTCTL = WDT_MDLY_8; //watchdog interrupt every 8 ms
		IE1 |= WDTIE; // ENable Watchdog Tiemr interrupt
		lcdSetText("No Alarm Set.", 0,0);
		alarm_state = 0;
		_bis_SR_register(LPM0_bits+GIE);

}

#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
	int x = debounceSwitch();
	switch(alarm_state)
	{
		case(0):
			lcdSetText("ALARM NOT SET",0,0);
			if((x == BIT7) && prevState == 0x0000){//if the left button is pressed in state 0, begin setting the alarm
				alarm_state = 1;
				lcdClear();
				lcdSetText("Set Hour:",0,0);
				lcdSetText(text,0,1);
			}
			break;
		case(1):
			if(x == BIT5 && prevState ==0x0000){//press the right button to increment the hours
				text[1] ++;
				if(text[1] == '0' + 10 ||(text[0] =='2' && text[1] == '5')) text[1] = '0';
				if(text[1]=='0') text[0]++;
				if(text[0] == '3') text[0] = '0';
			}else if(x == BIT7 && prevState ==0x000){ //press the left button to switch to setting the minutes
				alarm_state = 2;
				lcdClear();
				lcdSetText("Set Minute:",0,0);
			}
			lcdSetText(text,0,1);
			break;
		case(2):
			if(x == BIT5 && prevState ==0x0000){//press the right button to increment the minutes
				text[4] ++;
				if(text[4] == '0' + 10) text[4] = '0';
				if(text[4]=='0') text[3]++;
				if(text[3] == '7') text[3] = '0';
			}else if(x == BIT7 && prevState ==0x000){ //press the left button to set the alarm
				serialWrite('r');
				lcdClear();
				lcdSetText("ALARM SET:",0,0);
				alarm_state = 3;
			}
			lcdSetText(text,0,1);
			break;
		case(3):
			if(x == BIT7 && prevState ==0x0000){ // if waiting for alarm and want reset, transmit S character
				serialWrite('S');
				alarm_state = 0;
				lcdClear();
				lcdSetText("ALARM NOT SET",0,0);
			}
			break;
		case(4):
			if(x == BIT7 && prevState ==0x0000){// if alarm going off and want reset, transmit o character and turn off alarm
				serialWrite('o');
				TA0CCR1 = 0;
				alarm_state = 0;
				lcdClear();
				lcdSetText("ALARM NOT SET",0,0);
			}
			break;

	}
}
void serialInit()
{
	P1SEL |= BIT1 + BIT2; //P1.1 = RXD P1.2=TXD
	P1SEL2 |= BIT1 +BIT2; // P1.1=RXD & P1.2=TXD
	UCA0CTL1|= UCSSEL_2; // SMCLK
	UCA0BR0=104; // BAUDRATE AT 1 MHz 9600
	UCA0BR1=0;//1MHz 9600
    UCA0MCTL= UCBRS_1; // MODULATION UCBRSx=1
    UCA0CTL1 &=  ~UCSWRST; // ** INITIALIZE USCI STATE MACHINE
    IE2 |= UCA0RXIE; // Enable RX interrupt
}

void serialWrite(unsigned char c)
{
	while(!(IFG2&UCA0TXIFG));  // USCI_A0 TX buffer ready ?
	UCA0TXBUF=c; // TX
}

int debounceSwitch(void)
{
	int i;
	prevState = debouncedState;
	state[Index] = rawPortData();
	Index = (Index+1) % MAXCHECKS;
	debouncedState = 0xFF;
	for (i = 0; i < MAXCHECKS; i++) {
		debouncedState &= state[i];
	}
	return debouncedState;
}

int rawPortData(void)
{
	int portState = ~P1IN & 0xA0;
	return portState;
}

#pragma vector = USCIAB0RX_VECTOR
  __interrupt void ReceiveInterrupt(void)
{
	char rec = UCA0RXBUF;
	if(rec == '1'){
		serialWrite('0');
	}else if(rec == '2'){
		serialWrite('9');
	}else if(rec == '3'){
		serialWrite('0');
	}else if(rec == '4'){
		serialWrite('1');
	}else if(rec == 'a' && alarm_state ==3){
		TA0CCR1 = period>>2;
		alarm_state = 4;
	}

	IFG2 &= ~UCA0RXIFG; // Clear RX flag
}
