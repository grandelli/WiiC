/*
 *	speaker.h
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
#ifndef SPEAKER_H_INCLUDED
#define SPEAKER_H_INCLUDED

#include "wiic_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

void wiic_set_speaker(struct wiimote_t* wm, int status);
void wiic_mute_speaker(struct wiimote_t* wm, int status);
void wiic_sound(struct wiimote_t* wm);

#ifdef __cplusplus
}
#endif

#endif // SPEAKER_H_INCLUDED

