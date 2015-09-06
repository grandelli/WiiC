/*
 *    wiic_internal.h
 *
 *	  This file is part of WiiC, written by:
 *		Gabriele Randelli
 *		Email: randelli@dis.uniroma1.it
 *
 *    Copyright 2010
 *		
 *	  This file is based on Wiiuse, written By:
 *		Michael Laforest	< para >
 *		Email: < thepara (--AT--) g m a i l [--DOT--] com >
 *
 *	  Copyright 2006-2007
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	  $Header$
 */

/**
 *	@file
 *	@brief General internal wiic stuff.
 *
 *	Since WiiC is a library, wiic.h is a duplicate
 *	of the API header.
 *
 *	The code that would normally go in that file, but
 *	which is not needed by third party developers,
 *	is put here.
 *
 *	So wiic_internal.h is included by other files
 *	internally, wiic.h is included only here.
 */

#ifndef WIIC_INTERNAL_H_INCLUDED
#define WIIC_INTERNAL_H_INCLUDED

#ifndef __APPLE__
	#include <arpa/inet.h>				/* htons() */
	#include <bluetooth/bluetooth.h>
#endif

#include "definitions.h"

/* WiiC version */
#define WIIC_VERSION					"1.2"
#define WIIC_LOG_VERSION				 1.0

/********************
 *
 *	Wiimote internal codes
 *
 ********************/

/* Communication channels */
#define WM_OUTPUT_CHANNEL			0x11
#define WM_INPUT_CHANNEL			0x13

#define WM_SET_REPORT				0x50

/* commands */
#define WM_CMD_LED					0x11
#define WM_CMD_REPORT_TYPE			0x12
#define WM_CMD_RUMBLE				0x13
#define WM_CMD_IR					0x13
#define WM_CMD_SPEAKER_ENABLE		0x14
#define WM_CMD_CTRL_STATUS			0x15
#define WM_CMD_WRITE_DATA			0x16
#define WM_CMD_READ_DATA			0x17
#define WM_CMD_SPEAKER_DATA			0x18
#define WM_CMD_SPEAKER_MUTE			0x19
#define WM_CMD_IR_2					0x1A

/* input report ids */
#define WM_RPT_CTRL_STATUS			0x20
#define WM_RPT_READ					0x21
#define WM_RPT_WRITE				0x22
#define WM_RPT_BTN					0x30
#define WM_RPT_BTN_ACC				0x31
#define WM_RPT_BTN_ACC_IR			0x33
#define WM_RPT_BTN_EXP				0x34
#define WM_RPT_BTN_ACC_EXP			0x35
#define WM_RPT_BTN_IR_EXP			0x36
#define WM_RPT_BTN_ACC_IR_EXP		0x37

#define WM_BT_INPUT					0x01
#define WM_BT_OUTPUT				0x02

/* Identify the wiimote device by its class */

/*
 * The different codes wrt. to Linux
 * is a bit hard to explain.
 * Looking at Bluetooth CoD format, we have 24 bits.
 * In wiic Linux they are stored in three fields,
 * each one 8bit long. The result number is
 * 0x002504. However, MacOS Bluetooth does
 * not store them in such a way, rather it uses
 * the concept of major service, major class,
 * and minor class, that are respectivelly
 * 11bit, 5bit, and 6bit long. Hence, the 
 * numbers are different.
 * The Wiimote CoD Bluetooth division is the following:
 * 00000000001 00101 000001 00 (major service - major class - minor class - format type)
 * This can also be seen in the WiiC Linux way:
 * 00000000 00100101 00000100 
 */
#ifdef __APPLE__
	#define WM_DEV_MINOR_CLASS				0x01
	#define WM_DEV_MAJOR_CLASS				0x05
	#define WM_DEV_MAJOR_SERVICE			0x01
#else
	#define WM_DEV_CLASS_0				0x04
	#define WM_DEV_CLASS_1				0x25
	#define WM_DEV_CLASS_2				0x00
#endif
#define WM_VENDOR_ID				0x057E
#define WM_PRODUCT_ID				0x0306

/* controller status stuff */
#define WM_MAX_BATTERY_CODE			0xC8

/* offsets in wiimote memory */
#define WM_MEM_OFFSET_CALIBRATION	0x16
#define WM_EXP_MEM_BASE				0x04A40000
#define WM_EXP_MEM_ENABLE			0x04A40040
#define WM_EXP_MEM_CALIBR			0x04A40020
#define WM_EXP_ID					0x04A400FA
#define EXP_HANDSHAKE_LEN					224

#define WM_REG_IR					0x04B00030
#define WM_REG_IR_BLOCK1			0x04B00000
#define WM_REG_IR_BLOCK2			0x04B0001A
#define WM_REG_IR_MODENUM			0x04B00033

/* 
 * Motion Plus (please note that the first 
 * "04" in these addresses
 * is to enable register write) 
 */
#define WM_MOTION_PLUS_INIT			0x04A600F0
#define WM_MOTION_PLUS_ID_ADDR		0x04A400FA	/** This register contains the Motion Plus ID, if correctly activated */
#define WM_MOTION_PLUS_ENABLE		0x04A600FE // We're using the unencrypted version
#define WM_MOTION_PLUS_DISABLE		0x04A400F0

/* ir block data */
#define WM_IR_BLOCK1_LEVEL1			"\x02\x00\x00\x71\x01\x00\x64\x00\xfe"
#define WM_IR_BLOCK2_LEVEL1			"\xfd\x05"
#define WM_IR_BLOCK1_LEVEL2			"\x02\x00\x00\x71\x01\x00\x96\x00\xb4"
#define WM_IR_BLOCK2_LEVEL2			"\xb3\x04"
#define WM_IR_BLOCK1_LEVEL3			"\x02\x00\x00\x71\x01\x00\xaa\x00\x64"
#define WM_IR_BLOCK2_LEVEL3			"\x63\x03"
#define WM_IR_BLOCK1_LEVEL4			"\x02\x00\x00\x71\x01\x00\xc8\x00\x36"
#define WM_IR_BLOCK2_LEVEL4			"\x35\x03"
#define WM_IR_BLOCK1_LEVEL5			"\x07\x00\x00\x71\x01\x00\x72\x00\x20"
#define WM_IR_BLOCK2_LEVEL5			"\x1f\x03"

#define WM_IR_TYPE_BASIC			0x01
#define WM_IR_TYPE_EXTENDED			0x03

/* controller status flags for the first message byte */
/* bit 1 is unknown */
#define WM_CTRL_STATUS_BYTE1_ATTACHMENT			0x02
#define WM_CTRL_STATUS_BYTE1_SPEAKER_ENABLED	0x04
#define WM_CTRL_STATUS_BYTE1_IR_ENABLED			0x08
#define WM_CTRL_STATUS_BYTE1_LED_1				0x10
#define WM_CTRL_STATUS_BYTE1_LED_2				0x20
#define WM_CTRL_STATUS_BYTE1_LED_3				0x40
#define WM_CTRL_STATUS_BYTE1_LED_4				0x80

/* aspect ratio */
#define WM_ASPECT_16_9_X	660
#define WM_ASPECT_16_9_Y	370
#define WM_ASPECT_4_3_X		560
#define WM_ASPECT_4_3_Y		420


/**
 *	Expansion stuff
 */

/* encrypted expansion id codes (located at 0x04A400FC) */
#define EXP_ID_CODE_NUNCHUK					0x9A1EFEFE
#define EXP_ID_CODE_CLASSIC_CONTROLLER		0x9A1EFDFD
#define EXP_ID_CODE_GUITAR					0x9A1EFDFB
/* unencrypted expansion id codes */
#define EXP_ID_CODE_MOTION_PLUS				0xA4200405 /** Motion Plus ID (when activated) */
#define EXP_ID_BALANCE_BOARD				0xA4200402

/********************
 *
 *	End Wiimote internal codes
 *
 ********************/

/*
 *	Smooth tilt calculations are computed with the
 *	exponential moving average formula:
 *		St = St_last + (alpha * (tilt - St_last))
 *	alpha is between 0 and 1
 */
#define WIIC_DEFAULT_SMOOTH_ALPHA			0.9f

#include "wiic.h"

#ifdef __cplusplus
extern "C" {
#endif

/* not part of the api */
int wiic_set_report_type(struct wiimote_t* wm);
void wiic_send_next_pending_read_request(struct wiimote_t* wm);
int wiic_send(struct wiimote_t* wm, byte report_type, byte* msg, int len);
int wiic_read_data_cb(struct wiimote_t* wm, wiic_read_cb read_cb, byte* buffer, unsigned int offset, unsigned short len);

#ifdef __cplusplus
}
#endif

#endif /* WIIC_INTERNAL_H_INCLUDED */
