#Appendix A
##AT Commands for Programming the HC-05 Bluetooth Module

Each transmitted command was followed by a carriage return (\r) and a line feed (\n) character, in order to signify to the HC-05 the end of the command

###Commands sent to slave PCB and responses

AT					(check if programming mode has been entered correctly)		
	
	OK
	
  
AT+UART=9600,0,0			(set serial to 9600 baud, 1 stop bit, 0 parity bits)

	OK
  
AT+ROLE=0				(set to slave mode)

	OK

AT+RMAAD				(remove all devices from pairing list)

	OK
  
AT+ADDR?				(request MAC address)

	OK+ADDR=2016:3:75454

AT+PSWD=1234			(set PSWD to 1234)

	OK

###Commands sent to master PCB

AT					(check to ensure AT mode entered)
  
  	OK

AT+UART=9600,0,0			(set serial to 9600 baud, 1 stop bit, 0 parity bits)

	OK

AT+ROLE=1				(set to master mode)

	OK
  
AT+RMAAD				(clear all devices from pair list)

	OK	
  
AT+CMODE=0			(allow pairing only with bound address)

	OK

AT+PSWD=1234			(set password to 1234)

	OK

AT+BIND=2016,3,75454		(bind to out slave PCB)

	OK

AT+PAIR=2016,3,75454,10		(pair with our slave PCB with a timeout of 10s)

	OK
