/*
 *	balanceboard.c
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
#include "balanceboard.h"
#include "events.h"

/**
 *	@brief Handle the handshake data from the balance board.
 *
 *	@param bb		A pointer to a balance_board_t structure.
 *	@param data		The data read in from the device.
 *	@param len		The length of the data block, in bytes.
 *
 *	@return	Returns 1 if handshake was successful, 0 if not.
 */
int balance_board_handshake(struct wiimote_t* wm, struct balance_board_t* bb, byte* data, unsigned short len) 
{
	int offset = 4;
	
	bb->cal_low_weight.top_left = 0;
	bb->cal_low_weight.top_right = 0;	
	bb->cal_low_weight.bottom_left = 0;
	bb->cal_low_weight.bottom_right = 0;
	bb->cal_medium_weight.top_left = 0;
	bb->cal_medium_weight.top_right = 0;	
	bb->cal_medium_weight.bottom_left = 0;
	bb->cal_medium_weight.bottom_right = 0;
	bb->cal_high_weight.top_left = 0;
	bb->cal_high_weight.top_right = 0;	
	bb->cal_high_weight.bottom_left = 0;
	bb->cal_high_weight.bottom_right = 0;
	bb->pressure_raw_data.top_left = 0;
	bb->pressure_raw_data.top_right = 0;	
	bb->pressure_raw_data.bottom_left = 0;
	bb->pressure_raw_data.bottom_right = 0;	
	bb->pressure_weight.top_left = 0.0;
	bb->pressure_weight.top_right = 0.0;	
	bb->pressure_weight.bottom_left = 0.0;
	bb->pressure_weight.bottom_right = 0.0;	
	bb->pressure_weight.weight = 0.0;
	
	/*
	 *	Sometimes the data returned here is not correct (all bytes are 0xFF).
	 *	This might happen because the wiimote is lagging
	 *	behind our initialization sequence.
	 *	To fix this just request the handshake again.	 
	 */
	if (data[offset+offset] == 0xFF && data[offset+offset+16] == 0xFF) {
		/* get the calibration data */
		byte* handshake_buf = malloc(EXP_HANDSHAKE_LEN * sizeof(byte));

		WIIC_DEBUG("Balance board handshake appears invalid, trying again.");
		wiic_read_data_cb(wm, handshake_expansion, handshake_buf, WM_EXP_MEM_CALIBR, EXP_HANDSHAKE_LEN);

		return 0;
	}
	
	bb->cal_low_weight.top_right = (data[offset+0] << 8) | data[offset+1] ;
	bb->cal_low_weight.bottom_right = (data[offset+2] << 8) | data[offset+3] ;
	bb->cal_low_weight.top_left = (data[offset+4] << 8) | data[offset+5] ;
	bb->cal_low_weight.bottom_left = (data[offset+6] << 8) | data[offset+7] ;
	bb->cal_medium_weight.top_right = (data[offset+8] << 8) | data[offset+9] ;
	bb->cal_medium_weight.bottom_right = (data[offset+10] << 8) | data[offset+11] ;
	bb->cal_medium_weight.top_left = (data[offset+12] << 8) | data[offset+13] ;
	bb->cal_medium_weight.bottom_left = (data[offset+14] << 8) | data[offset+15] ;
	bb->cal_high_weight.top_right = (data[offset+16] << 8) | data[offset+17] ;
	bb->cal_high_weight.bottom_right = (data[offset+18] << 8) | data[offset+19] ;
	bb->cal_high_weight.top_left = (data[offset+20] << 8) | data[offset+21] ;
	bb->cal_high_weight.bottom_left = (data[offset+22] << 8) | data[offset+23] ;

	/* handshake done */
	wm->exp.type = EXP_BALANCE_BOARD;

	return 1;
}

/**
 *	@brief Handle balance board event.
 *
 *	@param bb		A pointer to a balance_board_t structure.
 *	@param msg		The message specified in the event packet.
 */
void balance_board_event(struct balance_board_t* bb, byte* msg) 
{
	// Raw data
	bb->pressure_raw_data.top_right = (msg[0] << 8) | msg[1];
	bb->pressure_raw_data.bottom_right = (msg[2] << 8) | msg[3];
	bb->pressure_raw_data.top_left = (msg[4] << 8) | msg[5];
	bb->pressure_raw_data.bottom_left = (msg[6] << 8) | msg[7];
	
	/* Weight in kg (we must interpole here) */

	// Top Right
	if(bb->pressure_raw_data.top_right <= bb->cal_medium_weight.top_right) {
		bb->pressure_weight.top_right = 17.0 * (bb->pressure_raw_data.top_right - bb->cal_low_weight.top_right) / (float)(bb->cal_medium_weight.top_right - bb->cal_low_weight.top_right);
	}
	else if(bb->pressure_raw_data.top_right <= bb->cal_high_weight.top_right) {
		bb->pressure_weight.top_right = 17.0 * (bb->pressure_raw_data.top_right - bb->cal_medium_weight.top_right) / (float)(bb->cal_high_weight.top_right - bb->cal_medium_weight.top_right) + 17.0;	
	}
	else {
		bb->pressure_weight.top_right = 17.0 * (bb->pressure_raw_data.top_right - bb->cal_high_weight.top_right) / (float)(bb->cal_high_weight.top_right - bb->cal_medium_weight.top_right) + 34.0;			
	}
	
	// Bottom Right
	if(bb->pressure_raw_data.bottom_right <= bb->cal_medium_weight.bottom_right) {
		bb->pressure_weight.bottom_right = 17.0 * (bb->pressure_raw_data.bottom_right - bb->cal_low_weight.bottom_right) / (float)(bb->cal_medium_weight.bottom_right - bb->cal_low_weight.bottom_right);
	}
	else if(bb->pressure_raw_data.bottom_right <= bb->cal_high_weight.bottom_right) {
		bb->pressure_weight.bottom_right = 17.0 * (bb->pressure_raw_data.bottom_right - bb->cal_medium_weight.bottom_right) / (float)(bb->cal_high_weight.bottom_right - bb->cal_medium_weight.bottom_right) + 17.0;	
	}
	else {
		bb->pressure_weight.bottom_right = 17.0 * (bb->pressure_raw_data.bottom_right - bb->cal_high_weight.bottom_right) / (float)(bb->cal_high_weight.bottom_right - bb->cal_medium_weight.bottom_right) + 34.0;			
	}
	
	// Top Left
	if(bb->pressure_raw_data.top_left <= bb->cal_medium_weight.top_left) {
		bb->pressure_weight.top_left = 17.0 * (bb->pressure_raw_data.top_left - bb->cal_low_weight.top_left) / (float)(bb->cal_medium_weight.top_left - bb->cal_low_weight.top_left);
	}
	else if(bb->pressure_raw_data.top_left <= bb->cal_high_weight.top_left) {
		bb->pressure_weight.top_left = 17.0 * (bb->pressure_raw_data.top_left - bb->cal_medium_weight.top_left) / (float)(bb->cal_high_weight.top_left - bb->cal_medium_weight.top_left) + 17.0;	
	}
	else {
		bb->pressure_weight.top_left = 17.0 * (bb->pressure_raw_data.top_left - bb->cal_high_weight.top_left) / (float)(bb->cal_high_weight.top_left - bb->cal_medium_weight.top_left) + 34.0;			
	}
	
	// Botton Left
	if(bb->pressure_raw_data.bottom_left <= bb->cal_medium_weight.bottom_left) {
		bb->pressure_weight.bottom_left = 17.0 * (bb->pressure_raw_data.bottom_left - bb->cal_low_weight.bottom_left) / (float)(bb->cal_medium_weight.bottom_left - bb->cal_low_weight.bottom_left);
	}
	else if(bb->pressure_raw_data.bottom_left <= bb->cal_high_weight.bottom_left) {
		bb->pressure_weight.bottom_left = 17.0 * (bb->pressure_raw_data.bottom_left - bb->cal_medium_weight.bottom_left) / (float)(bb->cal_high_weight.bottom_left - bb->cal_medium_weight.bottom_left) + 17.0;	
	}
	else {
		bb->pressure_weight.bottom_left = 17.0 * (bb->pressure_raw_data.bottom_left - bb->cal_high_weight.bottom_left) / (float)(bb->cal_high_weight.bottom_left - bb->cal_medium_weight.bottom_left) + 34.0;			
	}
	
	bb->pressure_weight.weight = bb->pressure_weight.top_right + bb->pressure_weight.top_left + bb->pressure_weight.bottom_right + bb->pressure_weight.bottom_left;
}

/**
 *	@brief The balance board disconnected.
 *
 *	@param bb		A pointer to a balance_board_t structure.
 */
void balance_board_disconnected(struct balance_board_t* bb)
{
	memset(bb, 0, sizeof(struct balance_board_t));
}
