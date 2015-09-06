/*
 *	balanceboard.h
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
 *	@brief Balance Board expansion device
 */
#ifndef BALANCEBOARD_H_INCLUDED
#define BALANCEBOARD_H_INCLUDED

#include "wiic_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

int balance_board_handshake(struct wiimote_t* wm, struct balance_board_t* bb, byte* data, unsigned short len);

void balance_board_disconnected(struct balance_board_t* bb);

void balance_board_event(struct balance_board_t* bb, byte* msg);

#ifdef __cplusplus
}
#endif

#endif // BALANCEBOARD_H_INCLUDED
