/*
 *	speaker.c
 *
 *	Written By:
 *		Gabriele Randelli	
 *		Email: < randelli (--AT--) dis [--DOT--] uniroma1 [--DOT--] it >
 *
 *	Copyright 2010
 *
 *	This file is part of wiiC.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	$Header$
 *
 */

/**
 *	@file
 *	@brief Handles the Wiimote speakers.
 */
//#pragma pack(nopack)
#include "wiic_internal.h"
#include "speaker.h"

void wiic_set_speaker(struct wiimote_t* wm, int status) {
		
	if (status) {
		// if already enabled then stop
		if (WIIMOTE_IS_SET(wm, WIIMOTE_STATE_SPEAKER))
			return;
			
		WIIMOTE_ENABLE_STATE(wm, WIIMOTE_STATE_SPEAKER);
		WIIMOTE_ENABLE_STATE(wm, WIIMOTE_STATE_SPEAKER_MUTE);
		
		byte buf;
		  
		/* Initialization Protocol */
	
		// Enable Speaker
		buf = 0x04;
		wiic_send(wm, WM_CMD_SPEAKER_ENABLE, &buf, 1);
		  
		// Mute Speaker
		buf = 0x04;
		wiic_send(wm, WM_CMD_SPEAKER_MUTE, &buf, 1);
		  
		// Write 0x01 to register 0xa20009
		buf = 0x01;
		wiic_write_data(wm, 0x04a20009, &buf, 1);
    
		// Write 0x08 to register 0xa20001
		buf = 0x08;
		wiic_write_data(wm, 0x04a20001, &buf, 1);
		  		  
 		// 1st byte for configuration 
 		buf = 0x00;
 		wiic_write_data(wm, 0x04a20001, &buf, 1);
 		// 2nd byte for configuration
 		buf = 0x00;
 		wiic_write_data(wm, 0x04a20002, &buf, 1);
 		// 3rd byte for configuration
 		buf = 0xD0;
 		wiic_write_data(wm, 0x04a20003, &buf, 1);
 		// 4th byte for configuration
 		buf = 0x07;
 		wiic_write_data(wm, 0x04a20004, &buf, 1);
 		// 5th byte for configuration
 		buf = 40;
 		wiic_write_data(wm, 0x04a20005, &buf, 1);
 		// 6th byte for configuration
 		buf = 0x00;
 		wiic_write_data(wm, 0x04a20006, &buf, 1);
 		// 7th byte for configuration
 		buf = 0x00;
 		wiic_write_data(wm, 0x04a20007, &buf, 1);
		  
		// Write 0x01 to register 0xa20008
		buf = 0x01;
		wiic_write_data(wm, 0x04a20008, &buf, 1);
	  
		// Unmute Speaker
		buf = 0x00;
		wiic_send(wm, WM_CMD_SPEAKER_MUTE, &buf, 1);	  

		WIIC_DEBUG("Speaker enabled");
	} 
	else {
		// if already disabled then stop 
		if (!WIIMOTE_IS_SET(wm, WIIMOTE_STATE_SPEAKER)) 
			return;
		
		WIIMOTE_DISABLE_STATE(wm, WIIMOTE_STATE_SPEAKER);
		WIIMOTE_DISABLE_STATE(wm, WIIMOTE_STATE_SPEAKER_MUTE);
		WIIC_DEBUG("Speaker disabled");
		byte buf = 0x00;
		wiic_send(wm, WM_CMD_SPEAKER_ENABLE, &buf, 1);
	}

	/* set the wiimote report type */
	wiic_set_report_type(wm);
	
	/* wait for the wiimote to catch up */
	usleep(50000);	
}


void wiic_mute_speaker(struct wiimote_t* wm, int status) {

	if (status) {
		// if already enabled then stop
		if (WIIMOTE_IS_SET(wm, WIIMOTE_STATE_SPEAKER_MUTE))
			return;
			
		WIIMOTE_ENABLE_STATE(wm, WIIMOTE_STATE_SPEAKER_MUTE);
		WIIC_DEBUG("Speaker unmuted");
		byte buf = 0x00;
		wiic_send(wm, WM_CMD_SPEAKER_MUTE, &buf, 1);
	} 	
    else {
		// if already disabled then stop
		if (!WIIMOTE_IS_SET(wm, WIIMOTE_STATE_SPEAKER_MUTE))
			return;
			
		WIIMOTE_DISABLE_STATE(wm, WIIMOTE_STATE_SPEAKER_MUTE);
		WIIC_DEBUG("Speaker muted");
		byte buf = 0x04;
		wiic_send(wm, WM_CMD_SPEAKER_MUTE, &buf, 1);
	}
}

void wiic_sound(struct wiimote_t* wm) {	
	/*Working buffer*/
	unsigned char buf1[21] = {0xA0, 0xC3, 0xC3, 0xC3, 0xC3,0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3,0xC3, 0xC3, 0xC3, 0xC3};	
	
	int t;

 	for(t = 0; t <= 50; t++) {
   	    wiic_send(wm, WM_CMD_SPEAKER_DATA, buf1, 21);
   	    usleep(50000);
   	}
	
	/* set the wiimote report type */
	wiic_set_report_type(wm);
	
	/* wait for the wiimote to catch up */
	usleep(50000);
}
