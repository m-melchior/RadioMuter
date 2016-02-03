#include "config.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "can_lib.h"
#include "can_drv.h"

// pre definition of used method
void sendCANMessage(void);

// global variables
U8 messageData[2];					// the message data to be sent via CAN
									// first byte contains 08
									// second byte contains 1E for mute
									// second byte contains 1C for unmute

st_cmd_t message;					// message to be sent via CAN

U8 muted = 0;						// muted == 0 unmuted
									// muted == 1 muted
									
// just a dummy for interrupt handling
ISR(INT0_vect)
{
}

void init()
{
	//***************************************************************
	// CAN STUFF
	//***************************************************************

    //- Pull-up on TxCAN & RxCAN one by one to use bit-addressing
    CAN_PORT_DIR &= ~(1<<CAN_INPUT_PIN );
    CAN_PORT_DIR &= ~(1<<CAN_OUTPUT_PIN);
    CAN_PORT_OUT |=  (1<<CAN_INPUT_PIN );
    CAN_PORT_OUT |=  (1<<CAN_OUTPUT_PIN);

	Can_reset();

	can_init((U16)CAN_BAUDRATE);

	//- Set CAN Timer Prescaler
    CANTCON = CANBT1;                   // Why not !

	message.cmd = CMD_TX_DATA;
	message.id.ext = 0x383;				// message ID for radio
	message.dlc = 2;					// only 2 bytes to send

	// Interrupt settings
	DDRD &= ~(1 << DDD0);	// Port D Pin 0 as input
	PORTD |= (1 << PD0);	// Pullup for Port D Pin 0, so standard is HIGH

	EIMSK |= (1 << INT0);	// turn on interrupt INT0 which is on Port D Pin 0
	EICRA |= (1 << ISC00);	// react on any logical change of pin (0 to 1 / 1 to 0)
	
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);	// sleep if there's nothing to do

} // void init()


int main (void)
{
	// first do init stuff
	init();

	// then do work stuff
    while(1)
    {
		if (PIND & (1 << PIND0)) {
			cli();				// turn off interrupts
			_delay_ms(1500); 	// wait 1,5 sec just in case the speaker just paused...
			sei();				// turn on interrupts
		} // if (PIND & (1 << PIND0)) {
		
		// if Port D Pin 0 is HIGH and muted command is out there, send unmute command
		if ((PIND & (1 << PIND0)) && (muted == 1)) {
			// compose message
			// has to be done here so the data won't get overwritten!
			messageData[0] = 0x08;
			messageData[1] = 0x1C;

			// send message
			sendCANMessage();

			// remember status
			muted = 0;
			
		// but if PORT D Pin 0 is LOW and unmute command is out there, send mute command
		} else if (!(PIND & (1 << PIND0)) && (muted == 0)) { // if((PIND & (1 << PIND0)) && (muted == 1)) {
			// compose message
			// has to be done here so the data won't get overwritten!
			messageData[0] = 0x08;
			messageData[1] = 0x1E;

			// send message
			sendCANMessage();

			// remember status
			muted = 1;

		} // if((PIND & (1 << PIND0)) && (unmuted == 0)) {
		
		sleep_mode(); // goto sleep after work

    } // while(1)

	// never will get here!
	return 0;
} // int main (void)

void sendCANMessage() {
	// get message from memory
	message.pt_data = &messageData[0];

	// wait until message gets accepted
	while(can_cmd(&message) != CAN_CMD_ACCEPTED);

	// wait until message sending is complete
	while(can_get_status(&message) == CAN_STATUS_NOT_COMPLETED);
} // void sendCANMessage() {
