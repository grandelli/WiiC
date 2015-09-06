/*
 *    wiic_macros.h
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
 *
 *	@brief WiiC macros and typedef.
 *
 *	Contains all the macros to manage the state of the Wiimote,
 *	its component, and other devices, as well as some useful 
 *	typedef. This header is included in wiic.h.
 */
/* wiimote state flags */
#define WIIMOTE_STATE_DEV_FOUND				0x00001
#define WIIMOTE_STATE_HANDSHAKE				0x00002	/* actual connection exists but no handshake yet */
#define WIIMOTE_STATE_HANDSHAKE_COMPLETE	0x00004	/* actual connection exists but no handshake yet */
#define WIIMOTE_STATE_CONNECTED				0x00008
#define WIIMOTE_STATE_RUMBLE				0x00010
#define WIIMOTE_STATE_ACC					0x00020
#define WIIMOTE_STATE_EXP					0x00040
#define WIIMOTE_STATE_IR					0x00080
#define WIIMOTE_STATE_SPEAKER				0x00100
#define WIIMOTE_STATE_IR_SENS_LVL1			0x00200
#define WIIMOTE_STATE_IR_SENS_LVL2			0x00400
#define WIIMOTE_STATE_IR_SENS_LVL3			0x00800
#define WIIMOTE_STATE_IR_SENS_LVL4			0x01000
#define WIIMOTE_STATE_IR_SENS_LVL5			0x02000
#define WIIMOTE_STATE_EXP_HANDSHAKE			0x04000	/* Added to handle handshaking concurrency between Motion Plus and other expansion types */
#define WIIMOTE_STATE_EXP_FAILED			0x08000
#define WIIMOTE_STATE_MOTION_PLUS			0x10000 /**< MotionPlus state flag, set when the MotionPlus is used */	
#define WIIMOTE_STATE_SPEAKER_MUTE			0x20000 /**< Speaker Mute */	

#define WIIMOTE_INIT_STATES					(WIIMOTE_STATE_IR_SENS_LVL3)

/* macro to manage states */
#define WIIMOTE_IS_SET(wm, s)			((wm->state & (s)) == (s))
#define WIIMOTE_ENABLE_STATE(wm, s)		(wm->state |= (s))
#define WIIMOTE_DISABLE_STATE(wm, s)	(wm->state &= ~(s))
#define WIIMOTE_TOGGLE_STATE(wm, s)		((wm->state & (s)) ? WIIMOTE_DISABLE_STATE(wm, s) : WIIMOTE_ENABLE_STATE(wm, s))

/* macro to manage flags */
#define WIIMOTE_IS_FLAG_SET(wm, s)		((wm->flags & (s)) == (s))
#define WIIMOTE_ENABLE_FLAG(wm, s)		(wm->flags |= (s))
#define WIIMOTE_DISABLE_FLAG(wm, s)		(wm->flags &= ~(s))
#define WIIMOTE_TOGGLE_FLAG(wm, s)		((wm->flags & (s)) ? WIIMOTE_DISABLE_FLAG(wm, s) : WIIMOTE_ENABLE_FLAG(wm, s))

#define NUNCHUK_IS_FLAG_SET(wm, s)		((*(wm->flags) & (s)) == (s))

/* misc macros */
#define WIIMOTE_ID(wm)					(wm->unid)
#define WIIMOTE_IS_CONNECTED(wm)		(WIIMOTE_IS_SET(wm, WIIMOTE_STATE_CONNECTED))

/* led bit masks */
#define WIIMOTE_LED_NONE				0x00
#define WIIMOTE_LED_1					0x10
#define WIIMOTE_LED_2					0x20
#define WIIMOTE_LED_3					0x40
#define WIIMOTE_LED_4					0x80

/* button codes */
#define WIIMOTE_BUTTON_TWO				0x0001
#define WIIMOTE_BUTTON_ONE				0x0002
#define WIIMOTE_BUTTON_B				0x0004
#define WIIMOTE_BUTTON_A				0x0008
#define WIIMOTE_BUTTON_MINUS			0x0010
#define WIIMOTE_BUTTON_ZACCEL_BIT6		0x0020
#define WIIMOTE_BUTTON_ZACCEL_BIT7		0x0040
#define WIIMOTE_BUTTON_HOME				0x0080
#define WIIMOTE_BUTTON_LEFT				0x0100
#define WIIMOTE_BUTTON_RIGHT			0x0200
#define WIIMOTE_BUTTON_DOWN				0x0400
#define WIIMOTE_BUTTON_UP				0x0800
#define WIIMOTE_BUTTON_PLUS				0x1000
#define WIIMOTE_BUTTON_ZACCEL_BIT4		0x2000
#define WIIMOTE_BUTTON_ZACCEL_BIT5		0x4000
#define WIIMOTE_BUTTON_UNKNOWN			0x8000
#define WIIMOTE_BUTTON_ALL				0x1F9F

/* nunchuk button codes */
#define NUNCHUK_BUTTON_Z				0x01
#define NUNCHUK_BUTTON_C				0x02
#define NUNCHUK_BUTTON_ALL				0x03

/* classic controller button codes */
#define CLASSIC_CTRL_BUTTON_UP			0x0001
#define CLASSIC_CTRL_BUTTON_LEFT		0x0002
#define CLASSIC_CTRL_BUTTON_ZR			0x0004
#define CLASSIC_CTRL_BUTTON_X			0x0008
#define CLASSIC_CTRL_BUTTON_A			0x0010
#define CLASSIC_CTRL_BUTTON_Y			0x0020
#define CLASSIC_CTRL_BUTTON_B			0x0040
#define CLASSIC_CTRL_BUTTON_ZL			0x0080
#define CLASSIC_CTRL_BUTTON_FULL_R		0x0200
#define CLASSIC_CTRL_BUTTON_PLUS		0x0400
#define CLASSIC_CTRL_BUTTON_HOME		0x0800
#define CLASSIC_CTRL_BUTTON_MINUS		0x1000
#define CLASSIC_CTRL_BUTTON_FULL_L		0x2000
#define CLASSIC_CTRL_BUTTON_DOWN		0x4000
#define CLASSIC_CTRL_BUTTON_RIGHT		0x8000
#define CLASSIC_CTRL_BUTTON_ALL			0xFEFF

/* guitar hero 3 button codes */
#define GUITAR_HERO_3_BUTTON_STRUM_UP	0x0001
#define GUITAR_HERO_3_BUTTON_YELLOW		0x0008
#define GUITAR_HERO_3_BUTTON_GREEN		0x0010
#define GUITAR_HERO_3_BUTTON_BLUE		0x0020
#define GUITAR_HERO_3_BUTTON_RED		0x0040
#define GUITAR_HERO_3_BUTTON_ORANGE		0x0080
#define GUITAR_HERO_3_BUTTON_PLUS		0x0400
#define GUITAR_HERO_3_BUTTON_MINUS		0x1000
#define GUITAR_HERO_3_BUTTON_STRUM_DOWN	0x4000
#define GUITAR_HERO_3_BUTTON_ALL		0xFEFF


/* wiimote option flags */
#define WIIC_SMOOTHING				0x01
#define WIIC_CONTINUOUS				0x04 
#define WIIC_ORIENT_THRESH			0x04
#define WIIC_INIT_FLAGS				(WIIC_SMOOTHING | WIIC_ORIENT_THRESH)

/* expansion codes */
#define EXP_NONE						0
#define EXP_NUNCHUK						1
#define EXP_CLASSIC						2
#define EXP_GUITAR_HERO_3				3
#define EXP_MOTION_PLUS					4
#define EXP_BALANCE_BOARD				5

/**
 *	@brief Check if a button is pressed.
 *	@param dev		Pointer to a wiimote_t or expansion structure.
 *	@param button	The button you are interested in.
 *	@return 1 if the button is pressed, 0 if not.
 */
#define IS_PRESSED(dev, button)		((dev->btns & button) == button)

/**
 *	@brief Check if a button is being held.
 *	@param dev		Pointer to a wiimote_t or expansion structure.
 *	@param button	The button you are interested in.
 *	@return 1 if the button is held, 0 if not.
 */
#define IS_HELD(dev, button)			((dev->btns_held & button) == button)

/**
 *	@brief Check if a button is released on this event.					\n\n
 *			This does not mean the button is not pressed, it means		\n
 *			this button was just now released.
 *	@param dev		Pointer to a wiimote_t or expansion structure.
 *	@param button	The button you are interested in.
 *	@return 1 if the button is released, 0 if not.
 *
 */
#define IS_RELEASED(dev, button)		((dev->btns_released & button) == button)

/**
 *	@brief Check if a button has just been pressed this event.
 *	@param dev		Pointer to a wiimote_t or expansion structure.
 *	@param button	The button you are interested in.
 *	@return 1 if the button is pressed, 0 if not.
 */
#define IS_JUST_PRESSED(dev, button)	(IS_PRESSED(dev, button) && !IS_HELD(dev, button))

/**
 *	@brief Return the IR sensitivity level.
 *	@param wm		Pointer to a wiimote_t structure.
 *	@param lvl		[out] Pointer to an int that will hold the level setting.
 *	If no level is set 'lvl' will be set to 0.
 */
#define WIIC_GET_IR_SENSITIVITY(wm, lvl)									\
			do {														\
				if ((wm->state & WIIMOTE_STATE_IR_SENS_LVL1) == WIIMOTE_STATE_IR_SENS_LVL1) 		*lvl = 1;	\
				else if ((wm->state & WIIMOTE_STATE_IR_SENS_LVL2) == WIIMOTE_STATE_IR_SENS_LVL2) 	*lvl = 2;	\
				else if ((wm->state & WIIMOTE_STATE_IR_SENS_LVL3) == WIIMOTE_STATE_IR_SENS_LVL3) 	*lvl = 3;	\
				else if ((wm->state & WIIMOTE_STATE_IR_SENS_LVL4) == WIIMOTE_STATE_IR_SENS_LVL4) 	*lvl = 4;	\
				else if ((wm->state & WIIMOTE_STATE_IR_SENS_LVL5) == WIIMOTE_STATE_IR_SENS_LVL5) 	*lvl = 5;	\
				else									*lvl = 0;		\
			} while (0)

#define WIIC_USING_ACC(wm)			((wm->state & WIIMOTE_STATE_ACC) == WIIMOTE_STATE_ACC)
#define WIIC_USING_EXP(wm)			((wm->state & WIIMOTE_STATE_EXP) == WIIMOTE_STATE_EXP)
#define WIIC_USING_IR(wm)				((wm->state & WIIMOTE_STATE_IR) == WIIMOTE_STATE_IR)
#define WIIC_USING_SPEAKER(wm)		((wm->state & WIIMOTE_STATE_SPEAKER) == WIIMOTE_STATE_SPEAKER)
#define WIIC_SPEAKER_MUTED(wm)		((wm->state & WIIMOTE_STATE_SPEAKER_MUTE) == WIIMOTE_STATE_SPEAKER_MUTE)
#define WIIC_USING_MOTION_PLUS(wm)	((wm->state & WIIMOTE_STATE_MOTION_PLUS) == WIIMOTE_STATE_MOTION_PLUS)
#define WIIC_IS_LED_SET(wm, num)		((wm->leds & WIIMOTE_LED_##num) == WIIMOTE_LED_##num)

/** TYPEDEFS */
typedef unsigned char byte;
typedef char sbyte;

/**
 *      @brief Callback that handles a read event.
 *
 *      @param wm               Pointer to a wiimote_t structure.
 *      @param data             Pointer to the filled data block.
 *      @param len              Length in bytes of the data block.
 *
 *      @see wiic_init()
 *
 *      A registered function of this type is called automatically by the wiiuse
 *      library when the wiimote has returned the full data requested by a previous
 *      call to wiiuse_read_data().
 */
struct wiimote_t;
typedef void (*wiic_read_cb)(struct wiimote_t* wm, byte* data, unsigned short len);

/**
 *	@enum aspect_t
 *	@brief Screen aspect ratio.
 */
typedef enum aspect_t {
	WIIC_ASPECT_4_3,
	WIIC_ASPECT_16_9
} aspect_t;
