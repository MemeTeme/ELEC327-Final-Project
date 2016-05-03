#include <msp430.h> // Imports necesarry library for code
#include <msp430g2553.h> // Imports necesarry library for code
#include <stdint.h>



//Bluetooth Initializations
unsigned char time1; //used for communication protocol
unsigned char time2; //used for communication protocol
unsigned char time3; //used for communication protocol
unsigned char time4; //used for communication protocol
unsigned int state =0; // 0 = no alarm set, 1-4 = getting characters, 5 = alarm going off
void serialInit(void);	//Initializes the bluetooth
void serialWrite(unsigned char c); //Used to write data over the bluetooth communication.

//POV Initializations
unsigned char startseq[] = {0x00, 0x00, 0x00, 0x00}; //used to start SPI communication
unsigned char endseq[] = {0xFF, 0xFF, 0xFF, 0xFF}; //used to end SPI communication
unsigned char ledseq[] = {0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0xD9, 0xFB, 0x29, 0xD9, 0xFB, 0x29, 0xD9, 0xFB, 0x29, 0xD9, 0xFB, 0x29, 0xD9, 0xFB, 0x29, 0xD9, 0xFB, 0x29, 0xD9, 0xFB, 0x29, 0x8C, 0x01, 0x92, 0xFF, 0x00, 0x20}; //color sequence for the LEDs
unsigned char brightseq[] = {0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0}; //brightness sequence for the LEDs


const int LEDON=0xE7; //brightness to turn LED on
const int LEDOFF=0xE0; //brightness to turn LED off

int minutemarker = 5; //used to track where the minute hand should be
int hourmarker = 51; //used to track where the hour hand should be
int total_revolutions = 0; //counts total number of revolutions since start

int count=0; //counter increment
unsigned int time = 0; //tracks time
unsigned int alarm = -1; //used for communication between modules
int move = 0;

//The alarm time is sent in Hex, so this value needs to be converted to an integer that this code can use.
int alarm_time(char time1, char time2, char time3, char time4){//state machine for alarm communication
	int hour1 = time1 - '0';//receiving communication protocol
	int hour2 = time2 - '0';//receiving communication protocol
	int minute1 = time3 - '0';//receiving communication protocol
	int minute2 = time4 - '0';//receiving communication protocol
	return ((hour1 * 10 + hour2) * 60 + (minute1 * 10 + minute2))-9*60; //turning the received time to a number
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Drawing Function Descriptions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//The next series of functions handle displaying the different parts of the analog clock.  Each function is called at a particular time step.
//The functions called in consecutive time steps create the persistence of vision effect.  Since all functions follow a similar structure,
//all the functions will be described here:

//The functions simply loop through the brightseq[] array that controls the brightness of the LED strip.  LED's 8-16 are used to draw the numbers,
//LED's 1-7 are used to draw the minute and hour hand, and LED 17 is used to create the outline around the analog clock.

void twelve1(void){//composes the number 12
	int i;
	for (i = 8; i < 16; i++){
		if(i==8||i==13){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void twelve2(void){//composes the number 12
	int i;
	for (i=8; i<16; i++){
		if(i==8||i==13||i==14){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void twelve3(void){//composes the number 12
	int i;
	for (i=8; i<16; i++){
		if(i==8||i==9||i==10||i==11||i==12||i==13||i==14){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void twelve4(void){//composes the number 12
	int i;
	for (i=8; i<16; i++){
		if(i==8){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void twelve5(void){//composes the number 12
	int i;
	for (i=8; i<16; i++){
		if(i==8){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void twelve6(void){//composes the number 12
	int i;
	for (i=8; i<16; i++){
		if(i==8||i==13){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void twelve7(void){//composes the number 12
	int i;
	for (i=8; i<16; i++){
		if(i==8||i==9||i==13||i==14){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void twelve8(void){//composes the number 12
	int i;
	for (i=8; i<16; i++){
		if(i==8||i==10||i==14){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void twelve9(void){//composes the number 12
	int i;
	for (i=8; i<16; i++){
		if(i==8||i==11||i==13||i==14){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void twelve10(void){//composes the number 12
	int i;
	for (i=8; i<16; i++){
		if(i==8||i==11||i==12||i==13){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void three1(void){//composes the number 3
	int i;
	for (i=8; i<16; i++){
		if(i==10||i==11||i==12||i==13||i==14){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void three2(void){//composes the number 3
	int i;
	for (i=8; i<16; i++){
		if(i==13){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void three3(void){//composes the number 3
	int i;
	for (i=8; i<16; i++){
		if(i==12){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void three4(void){//composes the number 3
	int i;
	for (i=8; i<16; i++){
		if(i==13){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void three5(void){//composes the number 3
	int i;
	for (i=8; i<16; i++){
		if(i==10||i==14){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void three6(void){//composes the number 3
	int i;
	for (i=8; i<16; i++){
		if(i==10||i==13){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void three7(void){//composes the number 3
	int i;
	for (i=8; i<16; i++){
		if(i==11||i==12){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void six1(void){//composes the number 6
	int i;
	for (i=8; i<16; i++){
		if(i==12||i==13){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void six2(void){//composes the number 6
	int i;
	for (i=8; i<16; i++){
		if(i==8||i==11||i==14){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void six3(void){//composes the number 6
	int i;
	for (i=8; i<16; i++){
		if(i==9||i==11||i==14){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void six4(void){//composes the number 6
	int i;
	for (i=8; i<16; i++){
		if(i==10||i==11||i==14){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void six5(void){//composes the number 6
	int i;
	for (i=8; i<16; i++){
		if(i==11||i==12||i==13){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void nine1(void){//composes the number 9
	int i;
	for (i=8; i<16; i++){
		if(i==13){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void nine2(void){//composes the number 9
	int i;
	for (i=8; i<16; i++){
		if(i==12){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void nine3(void){//composes the number 9
	int i;
	for (i=8; i<16; i++){
		if(i==11){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void nine4(void){//composes the number 9
	int i;
	for (i=8; i<16; i++){
		if(i==10||i==11||i==12||i==13){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void nine5(void){//composes the number 9
	int i;
	for (i=8; i<16; i++){
		if(i==10||i==14){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void nine6(void){//composes the number 9
	int i;
	for (i=8; i<16; i++){
		if(i==10||i==14){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void nine7(void){//composes the number 9
	int i;
	for (i=8; i<16; i++){
		if(i==11||i==12||i==13){
			brightseq[i]=LEDON;
		}
		else
			brightseq[i]=LEDOFF;
	}
}
void numberoff(void){//turns the number LEDs off
	int i;
	for (i=8; i<16; i++) brightseq[i]=LEDOFF;
}
void tickon(void){ //turns the border on
	brightseq[15]=LEDON;
}
void tickoff(void){//turns the border off
	brightseq[15]=LEDOFF;
}
void minuteon(void){ //turns the minute hand on
	int i;
	for(i =0;i<8;i++){brightseq[i]=LEDON;}
}

void minuteoff(void){//turns the minute hand off
	int i;
	for(i =0;i<8;i++){brightseq[i]=LEDOFF;}
}

void houron(void){//turns the hour hand on
	int i;
	for(i =0;i<4;i++){brightseq[i]=LEDON;}
}

void houroff(void){//turns the hour hand off
	int i;
	for(i =0;i<4;i++){brightseq[i]=LEDOFF;}
}

void change_leds(void){//updates the LEDs to display chosen colors/brightnesses
	int i,j;
	for (i=0; i<4; i++){
		while (!(IFG2 & UCB0TXIFG));	//ensures the transmission has finished
		UCB0TXBUF = startseq[i];				//start bits for LED instruction
	}
	int spot=0;
	for (i=0; i<17; i++){
		while (!(IFG2 & UCB0TXIFG));	//ensures the transmission has finished
		UCB0TXBUF = brightseq[i];				//start bits for LED instruction
		for (j=1; j<=3; j++){
			while (!(IFG2 & UCB0TXIFG));	//ensures the transmission has finished
			UCB0TXBUF = ledseq[spot];				//start bits for LED instruction
			spot++;
		}
	}
	for (i=0; i<4; i++){
		while (!(IFG2 & UCB0TXIFG));	//ensures the transmission has finished
		UCB0TXBUF = endseq[i];				//start bits for LED instruction
	}
}


int main(void)
{

	__enable_interrupt();				//enables interrupts
	WDTCTL = WDTPW + WDTHOLD;	// Stop WDT

    if (CALBC1_1MHZ == 0xFF || CALDCO_1MHZ == 0xff)
    	while(1); // Erased calibration data? Trap!

    P1OUT = 0x00;	//sets all ports as inputs
    P1SEL = (BIT7 + BIT5);				// used for LED control
	P1SEL2 = (BIT7 + BIT5);			// used for LED control

	UCB0CTL0 |= UCCKPL + UCMSB + UCMST + UCSYNC;  // sets up my serial communication
	UCB0CTL1 |= UCSSEL_2;                     // runs the serial communication clock off of the smclk
	UCB0BR0 |= 0x02;                          // divides clk by 2 fr slave-master communication
	UCB0BR1 = 0;                              // no bit rate control for this control register
	UCB0CTL1 &= ~UCSWRST;                     // initializes the state machine for serial communication


    BCSCTL1 = CALBC1_1MHZ; // Set the DCO to 1 MHz
    DCOCTL = CALDCO_1MHZ; // And load calibration data

  	CCTL0 = CCIE;                             // CCR0 interrupt enabled
  	TACTL = TASSEL_1 + MC_1 + ID_3;           // ACLK/8, upmode
  	CCR0 =  3;	//timer for PWM

  	serialInit(); //Initialize all the bluetooth code

	while(1){ //all logic happens in interrupt

		if(count<=1) //these if else statements choose the patterns to display based on the incremented counter
			twelve1();//code to display 12
		else if(count<=2)
			twelve2();
		else if(count<=3)
			twelve3();
		else if(count<=4)
			twelve4();
		else if(count<=5)
			twelve5();
		else if(count<=6)
			numberoff();
		else if(count<=7)
			twelve6();
		else if(count<=8)
			twelve7();
		else if(count<=9)
			twelve8();
		else if(count<=10)
			twelve9();
		else if(count<=11)
			twelve10();
		else if(count <= 19)
			numberoff();	//code to display no number
		else if(count<=20)	//code to display three
			three1();
		else if(count<=21)
			three2();
		else if(count<=22)
			three3();
		else if(count<=23)
			three4();
		else if(count<=24)
			three5();
		else if(count<=25)
			three6();
		else if(count<=26)
			three7();
		else if(count <= 34)
			numberoff();	//code to display no number
		else if(count<=35)	//code to display six
			six1();
		else if(count<=36)
			six2();
		else if(count<=37)
			six3();
		else if(count<=38)
			six4();
		else if(count<=39)
			six5();
		else if(count <= 51)
			numberoff();	//code to display no number
		else if(count<=52)	//code to display nine
			nine1();
		else if(count<=53)
			nine2();
		else if(count<=54)
			nine3();
		else if(count<=55)
			nine4();
		else if(count<=56)
			nine5();
		else if(count<=57)
			nine6();
		else if(count<=58)
			nine7();
		else
			numberoff();	//code to display no number

		if(count>=minutemarker&&count<=minutemarker+1)	//code to display minute hand
			minuteon(); //turns on minute hand
		else
			minuteoff(); //turns off minute hand


		if(total_revolutions==200){//code to count how many revolutions have occured since start
			time = (time+1) % 1440; //time counter
		}

		if(total_revolutions==move){//for moving the minute hand
			minutemarker+=1;//changes where the minute hand appears
			move+=1;
		}

		if((12<=count<=16) || (17<=count&& count<=23) || (26<=count&&count<=31) || (39<=count&& count<=46))//for the tick marker
			tickon(); //turns the tick on
		else
			tickoff(); //turns the tick off

		if(minutemarker==60){ //tracks time
			minutemarker=0; //resets minutes if an hour has passed
			hourmarker+=100; //changes hour hand location
		}

		if(count>=hourmarker&&count<=hourmarker+5)//code to turn on hour hand
		{
			houron();//shows the hour hand
		}
		else
		{
			houroff();//turns off the hour hand
		}


		total_revolutions = (count >=67) ? (total_revolutions+1) % 1667 : total_revolutions; //used to count total revolutions since start
		count = (count >=67) ? 0 : count; //resets count every rotation

		if(state == 4 && time == alarm){//used for communication protocol
			state = 5;
			serialWrite('a');//communication marker
		}
		change_leds();//updates which LEDs are on/off
	}
}

#pragma vector = TIMER0_A0_VECTOR   //This interrupt controls the counter of our device, which controls all timimng.
__interrupt void Timer_A (void) {
	count++;//counter increment
}

#pragma vector = USCIAB0RX_VECTOR
  __interrupt void ReceiveInterrupt(void)
{
	char rec = UCA0RXBUF;
	if(rec == 'r'){ // if an r character is received, the alarm has been set to a new time
		serialWrite('1');
		state =1;
	}else if(state == 1){ // the first character of the time has been received
		time1 = rec;
		serialWrite('2');
		state = 2;
	}else if(state == 2){// the second character of the time has been received
		time2 = rec;
		serialWrite('3');
		state =3;
	}else if(state == 3){// the third character of the time has been received
		time3 = rec;
		serialWrite('4');
		state = 4;
	}else if(state == 4){//the final character of the new alarm time has been received, and the alarm should be updated
		time4 = rec;
		alarm = alarm_time(time1,time2,time3,time4);
		serialWrite('s');
	}else if(state==5 && rec == 'o'){//if the alarm is going off and an o character is received, the alarm has been turned off
		state = 0;
	}

	IFG2 &= ~UCA0RXIFG; // Clear RX flag
}


void serialInit()
{
	__delay_cycles((long) 2000000);
	P1SEL |= BIT1 + BIT2; //P1.1 = RXD P1.2=TXD
	P1SEL2 |= BIT1 +BIT2; // P1.1=RXD & P1.2=TXD
	UCA0CTL1|= UCSSEL_2; // SMCLK
	UCA0BR0=104; // BAUDRATE AT 1 MHz 9600
	UCA0BR1=0;//1MHz 9600
	UCA0MCTL= UCBRS_1; // MODULATION UCBRSx=1
	UCA0CTL1 &=  ~UCSWRST; // ** INITIALIZE USCI STATE MACHINE
	IE2 |= UCA0RXIE;
	_bis_SR_register(GIE);
}

void serialWrite(unsigned char c)
{
	while(!(IFG2&UCA0TXIFG));  // USCI_A0 TX buffer ready ?
	UCA0TXBUF=c; // TX
}
