/*
 *    wiic_structs.h
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
 *	@brief WiiC structures.
 *
 *	Contains all the data structures, that represents
 *	devices, sensors, and everything necessary. This header 
 *	is included in wiic.h.
 */
struct vec3b_t;
struct orient_t;
struct gforce_t;

/**
 *	@struct read_req_t
 *	@brief Data read request structure.
 */
struct read_req_t {
	wiic_read_cb cb;			/**< read data callback											*/
	byte* buf;					/**< buffer where read data is written							*/
	unsigned int addr;			/**< the offset that the read started at						*/
	unsigned short size;		/**< the length of the data read								*/
	unsigned short wait;		/**< num bytes still needed to finish read						*/
	byte dirty;					/**< set to 1 if not using callback and needs to be cleaned up	*/

	struct read_req_t* next;	/**< next read request in the queue								*/
};

/*** COMMON STRUCTURES ***/

/**
 *	@struct vec2b_t
 *	@brief Unsigned x,y byte vector.
 */
typedef struct vec2b_t {
	byte x, y;
} vec2b_t;


/**
 *	@struct vec3b_t
 *	@brief Unsigned x,y,z byte vector.
 */
typedef struct vec3b_t {
	byte x, y, z;
} vec3b_t;

/**
 *	@struct vec3f_t
 *	@brief Signed x,y,z float struct.
 */
typedef struct vec3f_t {
	float x, y, z;
} vec3f_t;

/**
 *	@struct ang3s_t
 *	@brief RPY short int angles.
 */
typedef struct ang3s_t {
	short roll, pitch, yaw;
} ang3s_t;

/**
 *	@struct ang3f_t
 *	@brief RPY float angles.
 */
typedef struct ang3f_t {
	float roll, pitch, yaw;
} ang3f_t;

/**
 *	@struct orient_t
 *	@brief Orientation struct.
 *
 *	Yaw, pitch, and roll range from -180 to 180 degrees.
 */
typedef struct orient_t {
	struct ang3f_t angle;	/**< roll, pitch and yaw (this may be smoothed if enabled)	*/
} orient_t;

/**
 *	@struct ang_rate_t
 *	@brief Angular rate struct.
 *
 *	Yaw, pitch, and roll rate from -180 to 180 degrees.
 */
typedef struct ang_rate_t {
	struct ang3f_t rate;	/**< roll, pitch and yaw rate (this may be smoothed if enabled)	*/
	struct ang3f_t a_rate;	/**< roll, pitch and yaw rate (unsmoothed)	*/
} ang_rate_t;

/**
 *	@struct vel_t
 *	@brief Velocity struct.
 */
typedef struct vel_t {
	struct ang3s_t vel;		/**< raw rate, this may be smoothed if enabled 	*/
	struct ang3s_t a_vel;	/**< raw rate (unsmoothed)	*/
} vel_t;

/**
 *	@struct gforce_t
 *	@brief Gravity force struct.
 */
typedef struct gforce_t {
	struct vec3f_t vec;		/**< gforce, this may be smoothed if enabled 	*/
	struct vec3f_t a_vec;	/**< gforce (unsmoothed)	*/
} gforce_t;


/**
 *	@struct accel_t
 *	@brief Accelerometer struct. For any device with an accelerometer.
 */
typedef struct accel_t {
	struct vec3b_t cal_zero;		/**< zero calibration					*/
	struct vec3b_t cal_g;			/**< 1g difference around 0cal			*/
	float st_alpha;					/**< alpha value for smoothing [0-1]	*/
} accel_t;

/**
 *	@struct gyro_t
 *	@brief Gyro struct. For any device with a gyroscope.
 */
typedef struct gyro_t {
	struct vec3b_t cal_zero;		/**< zero calibration					*/
	float st_alpha;					/**< alpha value for smoothing [0-1]	*/
} gyro_t;

/**
 *  @struct pressure_t
 *  @brief Pressure sensor struct. Contains four pressure sensor measurements. Used for the Wii Balance Board.
 */
typedef struct pressure_t {
	unsigned short top_left;
	unsigned short top_right;
	unsigned short bottom_left;
	unsigned short bottom_right;
} pressure_t;

/**
 *  @struct pressure_weight_t
 *  @brief Pressure sensor weight struct. Contains four pressure sensor measurements in Kg. Used for the Wii Balance Board.
 */
typedef struct pressure_weight_t {
	float top_left;
	float top_right;
	float bottom_left;
	float bottom_right;
	float weight; // This is the sum of the weight on the four sensors
} pressure_weight_t;

/* IR correction types */
typedef enum ir_position_t {
	WIIC_IR_ABOVE,
	WIIC_IR_BELOW
} ir_position_t;

/**
 *	@struct ir_dot_t
 *	@brief A single IR source.
 */
typedef struct ir_dot_t {
	byte visible;					/**< if the IR source is visible		*/

	unsigned int x;					/**< interpolated X coordinate			*/
	unsigned int y;					/**< interpolated Y coordinate			*/

	short rx;						/**< raw X coordinate (0-1023)			*/
	short ry;						/**< raw Y coordinate (0-767)			*/

	byte order;						/**< increasing order by x-axis value	*/

	byte size;						/**< size of the IR dot (0-15)			*/
} ir_dot_t;


/**
 *	@struct ir_t
 *	@brief IR struct. Hold all data related to the IR tracking.
 */
typedef struct ir_t {
	struct ir_dot_t dot[4];			/**< IR dots							*/
	byte num_dots;					/**< number of dots at this time		*/

	enum aspect_t aspect;			/**< aspect ratio of the screen			*/

	enum ir_position_t pos;			/**< IR sensor bar position				*/

	unsigned int vres[2];			/**< IR virtual screen resolution		*/
	int offset[2];					/**< IR XY correction offset			*/
	int state;						/**< keeps track of the IR state		*/

	int ax;							/**< absolute X coordinate				*/
	int ay;							/**< absolute Y coordinate				*/

	int x;							/**< calculated X coordinate			*/
	int y;							/**< calculated Y coordinate			*/

	float distance;					/**< pixel distance between first 2 dots*/
	float z;						/**< calculated distance				*/
} ir_t;


/**
 *	@struct joystick_t
 *	@brief Joystick calibration structure.
 *
 *	The angle \a ang is relative to the positive y-axis into quadrant I
 *	and ranges from 0 to 360 degrees.  So if the joystick is held straight
 *	upwards then angle is 0 degrees.  If it is held to the right it is 90,
 *	down is 180, and left is 270.
 *
 *	The magnitude \a mag is the distance from the center to where the
 *	joystick is being held.  The magnitude ranges from 0 to 1.
 *	If the joystick is only slightly tilted from the center the magnitude
 *	will be low, but if it is closer to the outter edge the value will
 *	be higher.
 */
typedef struct joystick_t {
	struct vec2b_t max;				/**< maximum joystick values	*/
	struct vec2b_t min;				/**< minimum joystick values	*/
	struct vec2b_t center;			/**< center joystick values		*/

	float ang;						/**< angle the joystick is being held		*/
	float mag;						/**< magnitude of the joystick (range 0-1)	*/
} joystick_t;


/**
 *	@struct nunchuk_t
 *	@brief Nunchuk expansion device.
 */
typedef struct nunchuk_t {
	struct accel_t accel_calib;		/**< nunchuk accelerometer calibration		*/
	struct joystick_t js;			/**< joystick calibration					*/

	int* flags;						/**< options flag (points to wiimote_t.flags) */

	byte btns;						/**< what buttons have just been pressed	*/
	byte btns_held;					/**< what buttons are being held down		*/
	byte btns_released;				/**< what buttons were just released this	*/

	float orient_threshold;			/**< threshold for orient to generate an event */
	int accel_threshold;			/**< threshold for accel to generate an event */

	struct vec3b_t accel;			/**< current raw acceleration data			*/
	struct orient_t orient;			/**< current orientation on each axis		*/
	struct orient_t a_orient;		/**< current orientation on each axis (unsmoothed)		*/
	struct gforce_t gforce;			/**< current gravity forces on each axis	*/
	struct gforce_t a_gforce;		/**< current gravity forces on each axis (unsmoothed)	*/
} nunchuk_t;


/**
 *	@struct classic_ctrl_t
 *	@brief Classic controller expansion device.
 */
typedef struct classic_ctrl_t {
	short btns;						/**< what buttons have just been pressed	*/
	short btns_held;				/**< what buttons are being held down		*/
	short btns_released;			/**< what buttons were just released this	*/

	float r_shoulder;				/**< right shoulder button (range 0-1)		*/
	float l_shoulder;				/**< left shoulder button (range 0-1)		*/

	struct joystick_t ljs;			/**< left joystick calibration				*/
	struct joystick_t rjs;			/**< right joystick calibration				*/
} classic_ctrl_t;


/**
 *	@struct guitar_hero_3_t
 *	@brief Guitar Hero 3 expansion device.
 */
typedef struct guitar_hero_3_t {
	short btns;						/**< what buttons have just been pressed	*/
	short btns_held;				/**< what buttons are being held down		*/
	short btns_released;			/**< what buttons were just released this	*/

	float whammy_bar;				/**< whammy bar (range 0-1)					*/

	struct joystick_t js;			/**< joystick calibration					*/
} guitar_hero_3_t;

/**
 *	@struct balance_board_t
 *	@brief Balance Board expansion device.
 */
typedef struct balance_board_t {
	struct pressure_t cal_low_weight; // equivalent 0Kg
	struct pressure_t cal_medium_weight; // equivalent 17Kg
	struct pressure_t cal_high_weight; // equivalent 34Kg
	struct pressure_t pressure_raw_data; // Actual reading
	struct pressure_weight_t pressure_weight; // In Kg
} balance_board_t;

/**
 *	@struct motion_plus_t
 *	@brief Motion Plus expansion device.
 */
typedef struct motion_plus_t {
	struct ang3s_t a_raw_gyro;			/**< current raw gyroscope data (unsmoothed) */
	struct ang3s_t raw_gyro;			/**< current raw gyroscope data (smoothed, if enabled) */
	struct ang3s_t cal_gyro;			/**< calibration raw gyroscope data */
	struct ang3f_t a_gyro_rate;		/**< current gyro angle rate (unsmoothed) */
	struct ang3f_t gyro_rate;		/**< current gyro angle rate (smoothed, if enabled) */
	struct orient_t orient;				/**< current orientation on each axis using Motion Plus gyroscopes */
	byte acc_mode;						/**< Fast/slow rotation mode for roll, pitch and yaw (0 if rotating fast, 1 if slow or still) */
	int raw_gyro_threshold;			/**< threshold for gyroscopes to generate an event */
	int smooth;					/**< smoothing enabled/disabled */
	float smooth_alpha;				/**< smoothness alpha parameter*/
} motion_plus_t;


/**
 *	@struct expansion_t
 *	@brief Generic expansion device plugged into wiimote.
 */
typedef struct expansion_t {
	int type;						/**< type of expansion attached				*/

	union {
		struct nunchuk_t nunchuk;
		struct classic_ctrl_t classic;
		struct guitar_hero_3_t gh3;
		struct motion_plus_t mp;
		struct balance_board_t bb;
	};
} expansion_t;


/**
 *	@struct wiimote_state_t
 *	@brief Significant data from the previous event.
 */
typedef struct wiimote_state_t {
	/* expansion_t */
	float exp_ljs_ang;
	float exp_rjs_ang;
	float exp_ljs_mag;
	float exp_rjs_mag;
	unsigned short exp_btns;
	struct orient_t exp_orient;
	struct vec3b_t exp_accel;
	float exp_r_shoulder;
	float exp_l_shoulder;
	
	/* motion_plus_t */
	byte mp_acc_mode;
	struct ang3s_t mp_raw_gyro;
	
	/* balance_board_t */
	struct pressure_t pressure_raw_data;

	/* ir_t */
	int ir_ax;
	int ir_ay;
	float ir_distance;

	/* wiimote_t */
	struct orient_t orient;
	unsigned short btns;
	struct vec3b_t accel;
} wiimote_state_t;


/**
 *	@enum WIIC_EVENT_TYPE
 *	@brief Events that wiic can generate from a poll.
 */
typedef enum WIIC_EVENT_TYPE {
	WIIC_NONE = 0,
	WIIC_EVENT,
	WIIC_STATUS,
	WIIC_CONNECT,
	WIIC_DISCONNECT,
	WIIC_UNEXPECTED_DISCONNECT,
	WIIC_READ_DATA,
	WIIC_NUNCHUK_INSERTED,
	WIIC_NUNCHUK_REMOVED,
	WIIC_CLASSIC_CTRL_INSERTED,
	WIIC_CLASSIC_CTRL_REMOVED,
	WIIC_GUITAR_HERO_3_CTRL_INSERTED,
	WIIC_GUITAR_HERO_3_CTRL_REMOVED,
	WIIC_MOTION_PLUS_INSERTED,
	WIIC_MOTION_PLUS_REMOVED,
	WIIC_BALANCE_BOARD_INSERTED,
	WIIC_BALANCE_BOARD_REMOVED
} WIIC_EVENT_TYPE;

/**
 *	@struct wiimote_t
 *	@brief Wiimote structure.
 */
typedef struct wiimote_t {
	WCONST int unid;						/**< user specified id						*/

	#ifdef __APPLE__
		WCONST IOBluetoothDeviceRef device;    	/**  Device reference object                */
		WCONST CFStringRef address;            	/**  MacOS-like device address string       */
		WCONST IOBluetoothL2CAPChannelRef inputCh;		/**  Input L2CAP channel					*/	
		WCONST IOBluetoothL2CAPChannelRef outputCh;	/**  Output L2CAP channel					*/
		WCONST IOBluetoothUserNotificationRef disconnectionRef;	/**  Disconnection Notification Reference **/
		WCONST void* connectionHandler; /** Wiimote connection handler for MACOSX **/
	#elif LINUX
		WCONST bdaddr_t bdaddr;				/**< bt address								*/
		WCONST int out_sock;				/**< output socket							*/
		WCONST int in_sock;					/**< input socket 							*/	
	#endif
	
	WCONST char bdaddr_str[18];			/**< readable bt address					*/
	WCONST struct wiimote_state_t lstate;	/**< last saved state						*/
	WCONST int state;						/**< various state flags					*/
	WCONST int flags;						/**< options flag							*/
	WCONST int autoreconnect;				/**< auto-reconnect the device in case of unexpected disconnection */
	WCONST byte handshake_state;			/**< the state of the connection handshake	*/
	
	WCONST byte leds;						/**< currently lit leds						*/
	WCONST float battery_level;				/**< battery level							*/

	WCONST struct read_req_t* read_req;		/**< list of data read requests				*/
	WCONST struct expansion_t exp;			/**< wiimote expansion device				*/

	WCONST struct accel_t accel_calib;		/**< wiimote accelerometer calibration		*/
	WCONST struct vec3b_t accel;			/**< current raw acceleration data			*/
	WCONST struct orient_t orient;			/**< current orientation on each axis (smoothed and unsmoothed)		*/
	WCONST struct gforce_t gforce;			/**< current gravity forces on each axis (smoothed and unsmoothed)	*/
	WCONST float orient_threshold;			/**< threshold for orient to generate an event */
	WCONST int accel_threshold;				/**< threshold for accel to generate an event */

	WCONST struct ir_t ir;					/**< IR data								*/

	WCONST unsigned short btns;				/**< what buttons have just been pressed	*/
	WCONST unsigned short btns_held;		/**< what buttons are being held down		*/
	WCONST unsigned short btns_released;	/**< what buttons were just released this	*/

	WCONST WIIC_EVENT_TYPE event;			/**< type of event that occured				*/
	WCONST byte event_buf[MAX_PAYLOAD];		/**< event buffer							*/
	
	struct timeval timestamp; 				/**< Absolute timestamp (relative to the most recent data) */
} wiimote;
