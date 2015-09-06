/*
 *    dynamics.h
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
 *	@brief Handles the dynamics of the wiimote.
 *
 *	The file includes functions that handle the dynamics
 *	of the wiimote.  Such dynamics include orientation and
 *	motion sensing.
 */

#ifndef DYNAMICS_H_INCLUDED
#define DYNAMICS_H_INCLUDED

#include "wiic_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

void calculate_orientation(struct vec3f_t* in, struct ang3f_t* out);
void calculate_gforce(struct accel_t* ac, struct vec3b_t* accel, struct gforce_t* gforce, int smooth);
void calc_joystick_state(struct joystick_t* js, float x, float y);
void apply_smoothing(struct gforce_t* gforce, float alpha);

#ifdef __cplusplus
}
#endif

#endif // DYNAMICS_H_INCLUDED
