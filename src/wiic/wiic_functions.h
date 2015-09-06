/*
 *    wiic_functions.h
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
 *	@brief WiiC public functions.
 *
 *	Contains all the public functions, available for third
 *	party applications. This header is included in wiic.h.
 */
#ifdef __cplusplus
extern "C" {
#endif

/* wiic.c */
WIIC_EXPORT extern const char* wiic_version();

WIIC_EXPORT extern struct wiimote_t** wiic_init(int wiimotes);
WIIC_EXPORT extern void wiic_disconnected(struct wiimote_t* wm);
WIIC_EXPORT extern void wiic_cleanup(struct wiimote_t** wm, int wiimotes);
WIIC_EXPORT extern void wiic_rumble(struct wiimote_t* wm, int status);
WIIC_EXPORT extern void wiic_toggle_rumble(struct wiimote_t* wm);
WIIC_EXPORT extern void wiic_set_leds(struct wiimote_t* wm, int leds);
WIIC_EXPORT extern void wiic_motion_sensing(struct wiimote_t* wm, int status);
WIIC_EXPORT extern int wiic_read_data(struct wiimote_t* wm, byte* buffer, unsigned int offset, unsigned short len);
WIIC_EXPORT extern int wiic_write_data(struct wiimote_t* wm, unsigned int addr, byte* data, byte len);
WIIC_EXPORT extern void wiic_status(struct wiimote_t* wm);
WIIC_EXPORT extern struct wiimote_t* wiic_get_by_id(struct wiimote_t** wm, int wiimotes, int unid);
WIIC_EXPORT extern int wiic_set_flags(struct wiimote_t* wm, int enable, int disable);
WIIC_EXPORT extern float wiic_set_smooth_alpha(struct wiimote_t* wm, float alpha);
WIIC_EXPORT extern void wiic_set_orient_threshold(struct wiimote_t* wm, float threshold);
WIIC_EXPORT extern void wiic_resync(struct wiimote_t* wm);
WIIC_EXPORT extern void wiic_set_accel_threshold(struct wiimote_t* wm, int threshold);
WIIC_EXPORT extern void wiic_update_timestamp(struct wiimote_t* wm);

/* connect.c */
WIIC_EXPORT extern int wiic_find(struct wiimote_t** wm, int max_wiimotes, int timeout);
WIIC_EXPORT extern int wiic_load(struct wiimote_t** wm);
WIIC_EXPORT extern int wiic_connect(struct wiimote_t** wm, int wiimotes, int autoreconnect);
WIIC_EXPORT extern int wiic_connect_single(struct wiimote_t* wm, char* address, int autoreconnect);
WIIC_EXPORT extern void wiic_disconnect(struct wiimote_t* wm);

/* events.c */
WIIC_EXPORT extern int wiic_poll(struct wiimote_t** wm, int wiimotes);

/* ir.c */
WIIC_EXPORT extern void wiic_set_ir(struct wiimote_t* wm, int status);
WIIC_EXPORT extern void wiic_set_ir_vres(struct wiimote_t* wm, unsigned int x, unsigned int y);
WIIC_EXPORT extern void wiic_set_ir_position(struct wiimote_t* wm, enum ir_position_t pos);
WIIC_EXPORT extern void wiic_set_aspect_ratio(struct wiimote_t* wm, enum aspect_t aspect);
WIIC_EXPORT extern void wiic_set_ir_sensitivity(struct wiimote_t* wm, int level);

/* nunchuk.c */
WIIC_EXPORT extern void wiic_set_nunchuk_orient_threshold(struct wiimote_t* wm, float threshold);
WIIC_EXPORT extern void wiic_set_nunchuk_accel_threshold(struct wiimote_t* wm, int threshold);

/* speaker.c */
WIIC_EXPORT extern void wiic_set_speaker(struct wiimote_t* wm, int status);
WIIC_EXPORT extern void wiic_mute_speaker(struct wiimote_t* wm, int status);
WIIC_EXPORT extern void wiic_sound(struct wiimote_t* wm);

/* motionplus.c */
WIIC_EXPORT extern void wiic_set_motion_plus(struct wiimote_t* wm, int status);
WIIC_EXPORT extern void wiic_calibrate_motion_plus(struct motion_plus_t* mp);
WIIC_EXPORT extern void wiic_set_mp_threshold(struct wiimote_t* wm, int threshold);

#ifdef __cplusplus
}
#endif
