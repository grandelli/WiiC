/*
 *	motionplus.c
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
 *	@brief Motion Plus expansion device.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "definitions.h"
#include "wiic_internal.h"
#include "dynamics.h"
#include "events.h"
#include "motionplus.h"

/**
 *	@brief Convert raw data in deg/sec angular rates.
 *
 *	@param mp		A pointer to a motionplus_t structure.
 *	
 *	Subtract calibration data from raw data, and convert the difference in deg/sec
 *	angular rates. The function also considers the fast/slow rotation mode
 *	and performs very simple filtering for slow rotations.
 */
void calculate_gyro_rates(struct motion_plus_t* mp, struct ang3s_t* in, struct ang3f_t* out)
{
	short int tmp_r, tmp_p, tmp_y;
	float tmp_roll, tmp_pitch, tmp_yaw;
	
	// We consider calibration data
	tmp_r = in->roll - mp->cal_gyro.roll;
	tmp_p = in->pitch - mp->cal_gyro.pitch;
	tmp_y = in->yaw - mp->cal_gyro.yaw;

	// We convert to degree/sec according to fast/slow mode
	if(mp->acc_mode & 0x04)
		tmp_roll = tmp_r / 20.0;
	else
		tmp_roll = tmp_r / 4.0;

	if(mp->acc_mode & 0x02)
		tmp_pitch = tmp_p / 20.0;
	else
		tmp_pitch = tmp_p / 4.0;
		
	if(mp->acc_mode & 0x01)
		tmp_yaw = tmp_y / 20.0;
	else
		tmp_yaw = tmp_y / 4.0;
	
	out->roll = tmp_roll;
	out->pitch = tmp_pitch;
	out->yaw = tmp_yaw;
}

/**
 *	@brief Handle Motion Plus event.
 *
 *	@param mp		A pointer to a motionplus_t structure.
 *	@param msg		The message specified in the event packet.
 */
void motion_plus_event(struct motion_plus_t* mp, byte* msg) 
{
	// Check if the gyroscope is in fast or slow mode (0 if rotating fast, 1 if slow or still)
	mp->acc_mode = ((msg[4] & 0x2) << 1) | ((msg[3] & 0x1) << 1) | ((msg[3] & 0x2) >> 1); 
	
	mp->a_raw_gyro.roll = ((msg[4] & 0xFC) << 6) | msg[1];
	mp->a_raw_gyro.pitch = ((msg[5] & 0xFC) << 6) | msg[2];
	mp->a_raw_gyro.yaw = ((msg[3] & 0xFC) << 6) | msg[0];

	// First calibration
	if((mp->a_raw_gyro.roll > 5000) && (mp->a_raw_gyro.pitch > 5000) && (mp->a_raw_gyro.yaw > 5000) && 
		!(mp->cal_gyro.roll) && !(mp->cal_gyro.pitch) && !(mp->cal_gyro.yaw)) {
		wiic_calibrate_motion_plus(mp);	
	}
	
	if(mp->smooth)
		motion_plus_apply_smoothing(mp);
	else {
		mp->raw_gyro.roll = mp->a_raw_gyro.roll;
		mp->raw_gyro.pitch = mp->a_raw_gyro.pitch;
		mp->raw_gyro.yaw = mp->a_raw_gyro.yaw;
	}
				
	// Calculate angular rates in deg/sec and performs some simple filtering (both unsmoothed and smoothed)
	calculate_gyro_rates(mp,&(mp->a_raw_gyro),&(mp->a_gyro_rate));
	calculate_gyro_rates(mp,&(mp->raw_gyro),&(mp->gyro_rate));
}

/**
 *	@brief Apply smoothing to the Motion Plus gyroscopes.
 *
 *	@param mp		Pointer to a motion_plus_t structure.
 *
 */
void motion_plus_apply_smoothing(struct motion_plus_t *mp)
{
	float alpha = mp->smooth_alpha;
	
	mp->raw_gyro.roll = alpha*mp->raw_gyro.roll + (1.0-alpha)*mp->a_raw_gyro.roll;
	mp->raw_gyro.pitch = alpha*mp->raw_gyro.pitch + (1.0-alpha)*mp->a_raw_gyro.pitch;
	mp->raw_gyro.yaw = alpha*mp->raw_gyro.yaw + (1.0-alpha)*mp->a_raw_gyro.yaw;
}

/**
 *	@brief Handle the handshake from the Motion Plus.
 *
 *	@param wm		A pointer to a wiimote_t structure.
 *	@param data		The data that should contain the Motion Plus ID to check.
 *	@param len		The length of the data block, in bytes.
 *
 *	@return	Returns 1 if handshake was successful, 0 if not.
 */
int motion_plus_handshake(struct wiimote_t* wm, byte* data, unsigned short len) 
{
	WIIMOTE_DISABLE_STATE(wm, WIIMOTE_STATE_EXP);
	WIIMOTE_DISABLE_STATE(wm, WIIMOTE_STATE_EXP_FAILED);
	WIIMOTE_DISABLE_STATE(wm, WIIMOTE_STATE_EXP_HANDSHAKE);

	unsigned int val = (data[2] << 24) | (data[3] << 16) | (data[4] << 8) | data[5];

	if(val == EXP_ID_CODE_MOTION_PLUS) {
		/* handshake done */
		wm->event = WIIC_MOTION_PLUS_INSERTED;
		wm->exp.type = EXP_MOTION_PLUS;
		
		WIIMOTE_ENABLE_STATE(wm,WIIMOTE_STATE_EXP);
		WIIMOTE_ENABLE_STATE(wm,WIIMOTE_STATE_MOTION_PLUS);
		
		// Init gyroscopes
		wm->exp.mp.cal_gyro.roll = 0;
		wm->exp.mp.cal_gyro.pitch = 0;
		wm->exp.mp.cal_gyro.yaw = 0;
		wm->exp.mp.orient.angle.roll = 0.0;
		wm->exp.mp.orient.angle.pitch = 0.0;
		wm->exp.mp.orient.angle.yaw = 0.0;
		wm->exp.mp.raw_gyro.roll = wm->exp.mp.a_raw_gyro.roll = 0;
		wm->exp.mp.raw_gyro.pitch = wm->exp.mp.a_raw_gyro.pitch = 0;
		wm->exp.mp.raw_gyro.yaw = wm->exp.mp.a_raw_gyro.yaw = 0;
		wm->exp.mp.gyro_rate.roll = wm->exp.mp.a_gyro_rate.roll = 0.0;
		wm->exp.mp.gyro_rate.pitch = wm->exp.mp.a_gyro_rate.pitch = 0.0;
		wm->exp.mp.gyro_rate.yaw = wm->exp.mp.a_gyro_rate.yaw = 0.0;
		wm->exp.mp.raw_gyro_threshold = 15;
		wm->exp.mp.smooth = 1;
		wm->exp.mp.smooth_alpha = MP_SMOOTH_ALPHA;
		
		// Calibration (will be done as soon as we receive the first event)
		WIIC_INFO("Please, wait for gyro calibration...\n");
		sleep(1);
	}
	else {
		WIIC_ERROR("Unable to activate Motion Plus");
		wiic_set_motion_plus(wm,0);
		return 0;
	}
	
	return 1;
}

/**
 *	@brief Control the Motion Plus support.
 *
 *	@param wm		Pointer to a wiimote_t structure.
 *	@param status	Flag to control if the support should be enabled or not (1 to enable, 0 to disable).
 */
void wiic_set_motion_plus(struct wiimote_t *wm, int status)
{
	byte val;
	byte* buf = 0;
	byte* tmp = 0;

	// If we're handshaking other expansions, than skip this
	if(WIIMOTE_IS_SET(wm,WIIMOTE_STATE_EXP_HANDSHAKE))
		return;

	WIIMOTE_ENABLE_STATE(wm, WIIMOTE_STATE_EXP_HANDSHAKE);
	if(status) {
		// We initialize the motion plus
		val = 0x55;
		wiic_write_data(wm,WM_MOTION_PLUS_INIT,&val,1);
		usleep(10000);

		// We initialize the motion plus
		val = 0x04;
		wiic_write_data(wm,WM_MOTION_PLUS_ENABLE,&val,1);
		usleep(10000);

		// Callback to check if the init process performed right
		tmp = (byte*)malloc(sizeof(byte)*6);
		wiic_read_data_cb(wm, motion_plus_handshake, tmp, WM_MOTION_PLUS_ID_ADDR, 6);
	}
	else {
		disable_expansion(wm);
		WIIMOTE_DISABLE_STATE(wm,WIIMOTE_STATE_MOTION_PLUS);
		val = 0x55;
		wiic_write_data(wm,WM_MOTION_PLUS_DISABLE,&val,1);
		WIIMOTE_DISABLE_STATE(wm, WIIMOTE_STATE_EXP_HANDSHAKE);
	}
}


/**
 *	@brief Calibrate the Motion Plus gyroscopes.
 *
 *	@param mp		Pointer to a motion_plus_t structure.
 *
 *  This should be called only after receiving the first values
 *	from the Motion Plus.
 */
void wiic_calibrate_motion_plus(struct motion_plus_t *mp)
{
	mp->cal_gyro.roll = mp->raw_gyro.roll = mp->a_raw_gyro.roll ;
	mp->cal_gyro.pitch = mp->raw_gyro.pitch = mp->a_raw_gyro.pitch ;
	mp->cal_gyro.yaw = mp->raw_gyro.yaw = mp->a_raw_gyro.yaw ;
}


/**
 *	@brief Notification of Motion Plus disconnection.
 *
 *	@param mp	A pointer to a motion_plus_t structure.
 */
void motion_plus_disconnected(struct motion_plus_t* mp)
{
	WIIC_DEBUG("Motion plus disconnected");
	memset(mp, 0, sizeof(struct motion_plus_t));
}

/**
 *	@brief	Set the gyroscope event threshold.
 *
 *	@param wm			Pointer to a wiimote_t structure.
 *	@param threshold	The decimal place that should be considered a significant change.
 */
void wiic_set_mp_threshold(struct wiimote_t* wm, int threshold) {
	if (!wm)	return;

	wm->exp.mp.raw_gyro_threshold = threshold;
}

/**
 *	@brief	Enable the gyroscope smoothing (through an exponential moving average)
 *
 *	@param wm			Pointer to a wiimote_t structure.
 *	@param status		1 to enable - 0 to disable
 *  @param alpha		Alpha smoothness parameter (between 0.0 and 1.0)
 */
void wiic_set_mp_smooth(struct wiimote_t* wm, int status, float alpha) {
	wm->exp.mp.smooth = status;
	wm->exp.mp.smooth_alpha = alpha;
}
