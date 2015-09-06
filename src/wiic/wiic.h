/*
 *    wiic.h
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
 *	@brief API header file.
 *
 *	If this file is included from inside the wiiuse source
 *	and not from a third party program, then wiimote_internal.h
 *	is also included which extends this file.
 */
#ifndef WIIC_H_INCLUDED
#define WIIC_H_INCLUDED

#ifdef __APPLE__
	/* mac */
	#include <CoreFoundation/CoreFoundation.h>
	#include <IOBluetooth/IOBluetoothUserLib.h>
#else
	/* nix */
	#include <bluetooth/bluetooth.h>
#endif

#ifdef WIIC_INTERNAL_H_INCLUDED
	#define WCONST
#else
	#define WCONST		const
#endif

/*
 *	Largest known payload is 21 bytes.
 *	Add 2 for the prefix and round up to a power of 2.
 */
#define MAX_PAYLOAD			32

/*****************************************
 *
 *	Include API specific stuff
 *
 *****************************************/

#define WIIC_EXPORT_DECL
#define WIIC_IMPORT_DECL

#ifdef WIIC_COMPILE_LIB
	#define WIIC_EXPORT WIIC_EXPORT_DECL
#else
	#define WIIC_EXPORT WIIC_IMPORT_DECL
#endif

#include "wiic_macros.h"
#include "wiic_structs.h"
#include "wiic_functions.h"


#endif /* WIIC_H_INCLUDED */

