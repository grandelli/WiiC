/*
 *    wiicpp.cpp
 *
 *		This file is part of WiiC, written by:
 *			Gabriele Randelli
 *			Email: randelli@dis.uniroma1.it
 *
 *		Copyright 2010
 *		
 *		This file is based on WiiuseCpp, written By:
 *        James Thomas
 *        Email: jt@missioncognition.org
 *
 *    Copyright 2009
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
 */

/*
 * Note:  This C++ library is a (very) thin wrapper of the the WiiC library.
 */

#include <iostream>
#include <unistd.h>
#include "wiicpp.h"

using namespace std;
using namespace WiiC;

/*
 * CButtonBase class methods.
 */
CButtonBase::CButtonBase(void *ButtonsPtr, void *ButtonsHeldPtr, void *ButtonsReleasedPtr)
{
    mpBtnsPtr = ButtonsPtr;
    mpBtnsHeldPtr = ButtonsHeldPtr;
    mpBtnsReleasedPtr = ButtonsReleasedPtr;
}

int CButtonBase::isPressed(int Button)
{
    return (Cast(mpBtnsPtr) & Button) == Button;
}

int CButtonBase::isHeld(int Button)
{
    return (Cast(mpBtnsHeldPtr) & Button) == Button;
}

int CButtonBase::isReleased(int Button)
{
    return (Cast(mpBtnsReleasedPtr) & Button) == Button;
}

int CButtonBase::isJustPressed(int Button)
{
    return ((Cast(mpBtnsPtr) & Button) == Button) && ((Cast(mpBtnsHeldPtr) & Button) != Button);
}

/*
 * Initializers for classes derrived from CButtonBase.
 */
CButtons::CButtons(void *ButtonsPtr, void *ButtonsHeldPtr, void *ButtonsReleasedPtr) :
    CButtonBase(ButtonsPtr, ButtonsHeldPtr, ButtonsReleasedPtr)
{
}

CNunchukButtons::CNunchukButtons(void *ButtonsPtr, void *ButtonsHeldPtr, void *ButtonsReleasedPtr) :
    CButtonBase(ButtonsPtr, ButtonsHeldPtr, ButtonsReleasedPtr)
{
}

CClassicButtons::CClassicButtons(void *ButtonsPtr, void *ButtonsHeldPtr, void *ButtonsReleasedPtr) :
    CButtonBase(ButtonsPtr, ButtonsHeldPtr, ButtonsReleasedPtr)
{
}

CGH3Buttons::CGH3Buttons(void *ButtonsPtr, void *ButtonsHeldPtr, void *ButtonsReleasedPtr) :
    CButtonBase(ButtonsPtr, ButtonsHeldPtr, ButtonsReleasedPtr)
{
}

/*
 * CJoystick class methods.
 */

CJoystick::CJoystick(struct joystick_t *JSPtr)
{
    mpJoystickPtr = JSPtr;
}

void CJoystick::GetMaxCal(int &X, int &Y)
{
    X = mpJoystickPtr->max.x;
    Y = mpJoystickPtr->max.y;
}

void CJoystick::SetMaxCal(int X, int Y)
{
    mpJoystickPtr->max.x = X;
    mpJoystickPtr->max.y = Y;
}

void CJoystick::GetMinCal(int &X, int &Y)
{
    X = mpJoystickPtr->min.x;
    Y = mpJoystickPtr->min.y;
}

void CJoystick::SetMinCal(int X, int Y)
{
    mpJoystickPtr->min.x = X;
    mpJoystickPtr->min.y = Y;
}

void CJoystick::GetCenterCal(int &X, int &Y)
{
    X = mpJoystickPtr->center.x;
    Y = mpJoystickPtr->center.y;
}

void CJoystick::SetCenterCal(int X, int Y)
{
    mpJoystickPtr->center.x = X;
    mpJoystickPtr->center.y = Y;
}

void CJoystick::GetPosition(float &Angle, float &Magnitude)
{
    Angle = mpJoystickPtr->ang;
    Magnitude = mpJoystickPtr->mag;
}

/*
 * CAccelerometer class methods.
 */

CAccelerometer::CAccelerometer(struct accel_t *AccelCalPtr, struct vec3b_t *AccelerationPtr, int *AccelThresholdPtr,
                               struct orient_t *OrientationPtr, float *OrientationThresholdPtr,
                               struct gforce_t *GForcePtr)
{
    mpAccelCalibPtr = AccelCalPtr;
    mpAccelPtr = AccelerationPtr;
    mpOrientPtr = OrientationPtr;
    mpGForcePtr = GForcePtr;
    mpAccelThresholdPtr = AccelThresholdPtr;
    mpOrientThresholdPtr = OrientationThresholdPtr;
}

float CAccelerometer::SetSmoothAlpha(float Alpha)
{
    float old_value;

    old_value = mpAccelCalibPtr->st_alpha;

    mpAccelCalibPtr->st_alpha = Alpha;

    return old_value;
}

float CAccelerometer::GetOrientThreshold()
{
    return *mpOrientThresholdPtr;
}

void CAccelerometer::SetOrientThreshold(float Threshold)
{
    *mpOrientThresholdPtr = Threshold;
}

int CAccelerometer::GetAccelThreshold()
{
    return *mpAccelThresholdPtr;
}

void CAccelerometer::SetAccelThreshold(int Threshold)
{
    *mpAccelThresholdPtr = Threshold;
}

/**
 *
 * @brief Retrieves the smoothed device attitude (pitch, roll, and yaw) computed with an
 * exponential moving average.
 *
 * @param Pitch		[out] Reference variable where the smooth device pitch will be set.
 * @param Roll		[out] Reference variable where the smooth device roll will be set.
 * @param Yaw		[out] Reference variable where the smooth device yaw will be set. Please,
 *						  note that without IR enabled, yaw cannot be retrieved.
 */
void CAccelerometer::GetOrientation(float &Pitch, float &Roll, float &Yaw)
{
    Pitch = mpOrientPtr->angle.pitch;
    Roll = mpOrientPtr->angle.roll;
    Yaw = mpOrientPtr->angle.yaw;
}

void CAccelerometer::GetGravityCalVector(float &X, float &Y, float &Z)
{
    X = mpAccelCalibPtr->cal_g.x;
    Y = mpAccelCalibPtr->cal_g.y;
    Z = mpAccelCalibPtr->cal_g.z;
}

void CAccelerometer::SetGravityCalVector(float X, float Y, float Z)
{
    mpAccelCalibPtr->cal_g.x = X;
    mpAccelCalibPtr->cal_g.y = Y;
    mpAccelCalibPtr->cal_g.z = Z;
}

void CAccelerometer::GetGravityVector(float &X, float &Y, float &Z)
{
    X = mpGForcePtr->vec.x;
    Y = mpGForcePtr->vec.y;
    Z = mpGForcePtr->vec.z;
}

void CAccelerometer::GetRawGravityVector(float &X, float &Y, float &Z)
{
    X = mpGForcePtr->a_vec.x;
    Y = mpGForcePtr->a_vec.y;
    Z = mpGForcePtr->a_vec.z;
}

/*
 * CGyroscope class methods.
 */
CGyroscope::CGyroscope(struct ang3s_t* RawGyro, struct ang3s_t* CalGyro, struct ang3f_t* AngleRate, unsigned char* Mode, struct motion_plus_t* MPPtr, 
	int* GyroThresholdPtr)
{
	mpRawGyro = RawGyro;
	mpCalGyro = CalGyro;
	mpAngleRate = AngleRate;
	mpMode = Mode;
	mpMPPtr = MPPtr;
	mpGyroThresholdPtr = GyroThresholdPtr;
}

void CGyroscope::GetRawRates(int& Roll, int& Pitch, int& Yaw)
{
	Roll = mpRawGyro->roll;
	Pitch = mpRawGyro->pitch;
	Yaw = mpRawGyro->yaw;
}

void CGyroscope::GetRates(float& Roll, float& Pitch, float& Yaw)
{
	Roll = mpAngleRate->roll;
	Pitch = mpAngleRate->pitch;
	Yaw = mpAngleRate->yaw;
}

void CGyroscope::Calibrate()
{
	wiic_calibrate_motion_plus(mpMPPtr);	
}

int CGyroscope::GetGyroThreshold()
{
    return *mpGyroThresholdPtr;
}

void CGyroscope::SetGyroThreshold(int Threshold)
{
    *mpGyroThresholdPtr = Threshold;
}


/*
 * CWeightSensor class methods.
 */
CWeightSensor::CWeightSensor(struct pressure_t* RawWeight, struct pressure_t* LowCalWeight, struct pressure_t* MediumCalWeight, struct pressure_t* HighCalWeight, struct pressure_weight_t* Weight, struct balance_board_t* BBPtr)
{
	mpRawWeight = RawWeight;
	mpLowCalWeight = LowCalWeight;
	mpMediumCalWeight = MediumCalWeight;
	mpHighCalWeight = HighCalWeight;
	mpWeight = Weight;
	mpBBPtr = BBPtr;
}

void CWeightSensor::GetRawWeight(int& TopLeft, int& TopRight, int& BottomLeft, int& BottomRight)
{
	TopLeft = mpRawWeight->top_left;
	TopRight = mpRawWeight->top_right;
	BottomLeft = mpRawWeight->bottom_left;
	BottomRight = mpRawWeight->bottom_right;
}

void CWeightSensor::GetLowCalWeight(int& TopLeft, int& TopRight, int& BottomLeft, int& BottomRight)
{
	TopLeft = mpLowCalWeight->top_left;
	TopRight = mpLowCalWeight->top_right;
	BottomLeft = mpLowCalWeight->bottom_left;
	BottomRight = mpLowCalWeight->bottom_right;
}

void CWeightSensor::GetMediumCalWeight(int& TopLeft, int& TopRight, int& BottomLeft, int& BottomRight)
{
	TopLeft = mpMediumCalWeight->top_left;
	TopRight = mpMediumCalWeight->top_right;
	BottomLeft = mpMediumCalWeight->bottom_left;
	BottomRight = mpMediumCalWeight->bottom_right;
}

void CWeightSensor::GetHighCalWeight(int& TopLeft, int& TopRight, int& BottomLeft, int& BottomRight)
{
	TopLeft = mpHighCalWeight->top_left;
	TopRight = mpHighCalWeight->top_right;
	BottomLeft = mpHighCalWeight->bottom_left;
	BottomRight = mpHighCalWeight->bottom_right;
}

void CWeightSensor::GetWeight(float& TotalWeight, float& TopLeft, float& TopRight, float& BottomLeft, float& BottomRight)
{
	TotalWeight = mpWeight->weight;
	TopLeft = mpWeight->top_left;
	TopRight = mpWeight->top_right;
	BottomLeft = mpWeight->bottom_left;
	BottomRight = mpWeight->bottom_right;
}


/*
 * CIRDot class methods.
 */

CIRDot::CIRDot()
{
    mpDotPtr = NULL;
}

CIRDot::CIRDot(struct ir_dot_t *DotPtr)
{
    mpDotPtr = DotPtr;
}

CIRDot::CIRDot(const CIRDot &copyin) // Copy constructor to handle pass by value.
{
    mpDotPtr = copyin.mpDotPtr;
}

int CIRDot::isVisible()
{
    return mpDotPtr->visible;
}

int CIRDot::GetSize()
{
    return mpDotPtr->size;
}

int CIRDot::GetOrder()
{
    return mpDotPtr->order;
}

void CIRDot::GetCoordinate(int &X, int &Y)
{
    X = mpDotPtr->x;
    Y = mpDotPtr->y;
}

void CIRDot::GetRawCoordinate(int &X, int &Y)
{
    X = mpDotPtr->rx;
    Y = mpDotPtr->ry;
}

/*
 * CIR class methods.
 */

CIR::CIR(struct wiimote_t *wmPtr)
{
    mpWiimotePtr = wmPtr;
}

void CIR::SetMode(CIR::OnOffSelection State)
{
    wiic_set_ir(mpWiimotePtr, State);
}

void CIR::SetVres(unsigned int x, unsigned int y)
{
    wiic_set_ir_vres(mpWiimotePtr, x, y);
}

CIR::BarPositions CIR::GetBarPositionSetting()
{
    return (CIR::BarPositions) mpWiimotePtr->ir.pos;
}

void CIR::SetBarPosition(CIR::BarPositions PositionSelection)
{
    wiic_set_ir_position(mpWiimotePtr, (ir_position_t) PositionSelection);
}

CIR::AspectRatioSelections CIR::GetAspectRatioSetting()
{
    return (CIR::AspectRatioSelections) mpWiimotePtr->ir.aspect;
}

void CIR::SetAspectRatio(CIR::AspectRatioSelections AspectRatioSelection)
{
    wiic_set_aspect_ratio(mpWiimotePtr, (enum aspect_t) AspectRatioSelection);
}

void CIR::SetSensitivity(int Level)
{
    wiic_set_ir_sensitivity(mpWiimotePtr, Level);
}

int CIR::GetSensitivity()
{
    int level = 0;

	 WIIC_GET_IR_SENSITIVITY(mpWiimotePtr, &level);

    return level;
}

int CIR::GetNumDots()
{
    return mpWiimotePtr->ir.num_dots;
}

std::vector<CIRDot>& CIR::GetDots()
{
    int index;

    // Empty the array of irdots before reloading
    mpIRDotsVector.clear();

    for(index = 0 ; index < 4 ; index++)
    {
        CIRDot dot((struct ir_dot_t *) (&(mpWiimotePtr->ir.dot[index])));
		if(dot.isVisible())
        	mpIRDotsVector.push_back(dot);
    }

    return mpIRDotsVector;
}

void CIR::GetOffset(int &X, int &Y)
{
    X = mpWiimotePtr->ir.offset[0];
    Y = mpWiimotePtr->ir.offset[1];
}

int CIR::GetState()
{
    return mpWiimotePtr->ir.state;
}

void CIR::GetCursorPositionAbsolute(int &X, int &Y)
{
    X = mpWiimotePtr->ir.ax;
    Y = mpWiimotePtr->ir.ay;
}

void CIR::GetCursorPosition(int &X, int &Y)
{
    X = mpWiimotePtr->ir.x;
    Y = mpWiimotePtr->ir.y;
}

float CIR::GetPixelDistance()
{
    return mpWiimotePtr->ir.distance;
}

float CIR::GetDistance()
{
    return mpWiimotePtr->ir.z;
}

/*
 * CExpansionDevice class methods.  This is a container class so there is not much.
 */

CExpansionDevice::CExpansionDevice(struct expansion_t * ExpPtr) :
    Nunchuk(ExpPtr),Classic(ExpPtr),GuitarHero3(ExpPtr),MotionPlus(ExpPtr),BalanceBoard(ExpPtr)
{
    mpExpansionPtr = ExpPtr;
}

CExpansionDevice::ExpTypes CExpansionDevice::GetType()
{
    return (CExpansionDevice::ExpTypes) (mpExpansionPtr->type);
}

/*
 * CNunchuk class methods.
 */

CNunchuk::CNunchuk(struct expansion_t *ExpPtr):
    Buttons((void *) &(ExpPtr->nunchuk.btns), (void *) &(ExpPtr->nunchuk.btns_held),
            (void *) &(ExpPtr->nunchuk.btns_released)), Joystick(&(ExpPtr->nunchuk.js)),
            Accelerometer(&(ExpPtr->nunchuk.accel_calib), &(ExpPtr->nunchuk.accel), &(ExpPtr->nunchuk.accel_threshold),
                          &(ExpPtr->nunchuk.orient), &(ExpPtr->nunchuk.orient_threshold), &(ExpPtr->nunchuk.gforce))
{
}

/*
 * CClassic class methods.
 */

CClassic::CClassic(struct expansion_t *ExpPtr):
    Buttons((void *) &(ExpPtr->classic.btns), (void *) &(ExpPtr->classic.btns_held), (void *) &(ExpPtr->classic.btns_released)),LeftJoystick(&(ExpPtr->classic.ljs)), RightJoystick(&(ExpPtr->classic.rjs))
{
    // Initialize the expansion pointer.
    mpClassicPtr = &(ExpPtr->classic);
}

float CClassic::GetLShoulderButton()
{
    return mpClassicPtr->l_shoulder;
}

float CClassic::GetRShoulderButton()
{
    return mpClassicPtr->r_shoulder;
}

/*
 * CGuitarHero3 class methods.
 */

CGuitarHero3::CGuitarHero3(struct expansion_t *ExpPtr):
    Buttons((void *) &(ExpPtr->gh3.btns), (void *) &(ExpPtr->gh3.btns_held), (void *) &(ExpPtr->gh3.btns_released)),
            Joystick(&(ExpPtr->gh3.js))
{
    // Initialize the expansion pointer.
    mpGH3Ptr = &(ExpPtr->gh3);
}

float CGuitarHero3::GetWhammyBar()
{
    return mpGH3Ptr->whammy_bar;
}


/*
 * CMotionPlus class methods.
 */
CMotionPlus::CMotionPlus(struct expansion_t *ExpPtr):
	Gyroscope(&(ExpPtr->mp.raw_gyro),&(ExpPtr->mp.cal_gyro),&(ExpPtr->mp.gyro_rate),(unsigned char*)&(ExpPtr->mp.acc_mode),&(ExpPtr->mp),
	&(ExpPtr->mp.raw_gyro_threshold))
{
	mpMPPtr = &(ExpPtr->mp);
}

void CMotionPlus::Connect(struct wiimote_t* WiimotePtr)
{
	wiic_set_motion_plus(WiimotePtr,1);
}

void CMotionPlus::Disconnect(struct wiimote_t* WiimotePtr)
{
	wiic_set_motion_plus(WiimotePtr,0);
}


/*
 * CBalanceBoard class methods.
 */
CBalanceBoard::CBalanceBoard(struct expansion_t *ExpPtr):
	WeightSensor(&(ExpPtr->bb.pressure_raw_data),&(ExpPtr->bb.cal_low_weight),&(ExpPtr->bb.cal_medium_weight),&(ExpPtr->bb.cal_high_weight),&(ExpPtr->bb.pressure_weight),&(ExpPtr->bb))
{
    // Initialize the expansion pointer.
	 mpBBPtr = &(ExpPtr->bb);
} 


/*
 * CWiimote class methods.
 */

CWiimote::CWiimote() : // SWIG insisted it exist for the vectors. Hopefully it will only be used for copy.
    IR(NULL), Buttons(NULL, NULL, NULL), Accelerometer((accel_t*) NULL, (vec3b_t*) NULL, (int*) &(mpTempInt),
                                                       (orient_t*) NULL, (float*) &(mpTempFloat), (gforce_t*) NULL),
            ExpansionDevice(NULL)
{
    mpWiimotePtr = NULL;
}

CWiimote::CWiimote(struct wiimote_t *wmPtr):
    IR(wmPtr), Buttons((void *) &(wmPtr->btns), (void *) &(wmPtr->btns_held), (void *) &(wmPtr->btns_released)),
            Accelerometer((accel_t*) &(wmPtr->accel_calib), (vec3b_t*) &(wmPtr->accel),
                          (int*) &(wmPtr->accel_threshold), (orient_t*) &(wmPtr->orient),
                          (float*) &(wmPtr->orient_threshold), (gforce_t*) &(wmPtr->gforce)), 
            ExpansionDevice((struct expansion_t*) &(wmPtr->exp))
{
    mpWiimotePtr = wmPtr;
    logger.SetDeviceAddress(string(GetAddress()));
}

CWiimote::CWiimote(const CWiimote &copyin) : // Copy constructor to handle pass by value.
            IR(copyin.mpWiimotePtr),
            Buttons((void *) &(copyin.mpWiimotePtr->btns), (void *) &(copyin.mpWiimotePtr->btns_held),
                    (void *) &(copyin.mpWiimotePtr->btns_released)),
            Accelerometer((accel_t*) &(copyin.mpWiimotePtr->accel_calib), (vec3b_t*) &(copyin.mpWiimotePtr->accel),
                          (int*) &(copyin.mpWiimotePtr->accel_threshold), (orient_t*) &(copyin.mpWiimotePtr->orient),
                          (float*) &(copyin.mpWiimotePtr->orient_threshold), (gforce_t*) &(copyin.mpWiimotePtr->gforce)),
            ExpansionDevice((struct expansion_t*) &(copyin.mpWiimotePtr->exp)), logger(copyin.logger)
{
    mpWiimotePtr = copyin.mpWiimotePtr;
    logger = copyin.logger;
}

void CWiimote::Disconnected()
{
    wiic_disconnected(mpWiimotePtr);
}

void CWiimote::SetRumbleMode(CWiimote::OnOffSelection State)
{
    wiic_rumble(mpWiimotePtr, State);
}

void CWiimote::ToggleRumble()
{
    wiic_toggle_rumble(mpWiimotePtr);
}

bool CWiimote::isRumbleEnabled()
{
    return WIIMOTE_IS_SET(mpWiimotePtr,WIIMOTE_STATE_RUMBLE);
}

int CWiimote::GetLEDs()
{
    return mpWiimotePtr->leds;
}

void CWiimote::SetLEDs(int LEDs)
{
    wiic_set_leds(mpWiimotePtr, LEDs);
}

float CWiimote::GetBatteryLevel()
{
    return mpWiimotePtr->battery_level;
}

int CWiimote::GetHandshakeState()
{
    return mpWiimotePtr->handshake_state;
}

CWiimote::EventTypes CWiimote::GetEvent()
{
    return (CWiimote::EventTypes) mpWiimotePtr->event;
}

const unsigned char *CWiimote::GetEventBuffer()
{
    return mpWiimotePtr->event_buf;
}

void CWiimote::Log()
{
	logger.InitLog();
	
	if(logType & WIIC_LOG_ACC) {
		float x, y, z;
		Accelerometer.GetGravityVector(x,y,z);
		logger.LogAcc(x,y,z);
	}
	
	if(logType & WIIC_LOG_GYRO) {			
		float roll, pitch, yaw;
		ExpansionDevice.MotionPlus.Gyroscope.GetRates(roll,pitch,yaw);
		logger.LogGyro(roll,pitch,yaw);
	}			
}

void CWiimote::SetMotionSensingMode(CWiimote::OnOffSelection State)
{
    wiic_motion_sensing(mpWiimotePtr, State);
}

void CWiimote::EnableMotionPlus(CWiimote::OnOffSelection State)
{
    if(State == CWiimote::ON)
    	ExpansionDevice.MotionPlus.Connect(mpWiimotePtr);
    else
    	ExpansionDevice.MotionPlus.Disconnect(mpWiimotePtr);
}

void CWiimote::EnableSpeaker(CWiimote::OnOffSelection State)
{
    wiic_set_speaker(mpWiimotePtr, State);
}

void CWiimote::MuteSpeaker(CWiimote::OnOffSelection State)
{
    wiic_mute_speaker(mpWiimotePtr, State);
}

void CWiimote::PlaySound()
{
	wiic_sound(mpWiimotePtr);
}

void CWiimote::ReadData(unsigned char *Buffer, unsigned int Offset, unsigned int Length)
{
    wiic_read_data(mpWiimotePtr, Buffer, Offset, Length);
}

void CWiimote::WriteData(unsigned int Address, unsigned char *Data, unsigned int Length)
{
    wiic_write_data(mpWiimotePtr, Address, Data, Length);
}

void CWiimote::UpdateStatus()
{
    wiic_status(mpWiimotePtr);
}

int CWiimote::GetID()
{
    return mpWiimotePtr->unid;
}

const char* CWiimote::GetAddress()
{
    return mpWiimotePtr->bdaddr_str;
}

int CWiimote::GetState()
{
    return mpWiimotePtr->state;
}

int CWiimote::GetFlags()
{
    return mpWiimotePtr->flags;
}

int CWiimote::SetFlags(int Enable, int Disable)
{
    return wiic_set_flags(mpWiimotePtr, Enable, Disable);
}

void CWiimote::SetSmoothing(bool Smooth)
{
	if(Smooth)
		wiic_set_flags(mpWiimotePtr, WIIC_SMOOTHING, 0);
	else
		wiic_set_flags(mpWiimotePtr, 0, WIIC_SMOOTHING);
}

void CWiimote::Resync()
{
    wiic_resync(mpWiimotePtr);
}

void CWiimote::Disconnect()
{
    wiic_disconnect(mpWiimotePtr);
}

int CWiimote::isUsingACC()
{
    return WIIC_USING_ACC(mpWiimotePtr) != 0;
}

int CWiimote::isUsingEXP()
{
    return WIIC_USING_EXP(mpWiimotePtr) != 0;
}

int CWiimote::isUsingIR()
{
    return WIIC_USING_IR(mpWiimotePtr) != 0;
}

int CWiimote::isUsingMotionPlus()
{
	return WIIC_USING_MOTION_PLUS(mpWiimotePtr) != 0;
}

int CWiimote::isUsingSpeaker()
{
    return WIIC_USING_SPEAKER(mpWiimotePtr) != 0;
}

int CWiimote::isSpeakerMuted()
{
    return WIIC_SPEAKER_MUTED(mpWiimotePtr) != 0;
}

int CWiimote::isLEDSet(int LEDNum)
{
    int result = 0;

    switch(LEDNum)
    {
        case 1:
            result = (mpWiimotePtr->leds & LED_1) != 0;
            break;
        case 2:
            result = (mpWiimotePtr->leds & LED_2) != 0;
            break;
        case 3:
            result = (mpWiimotePtr->leds & LED_3) != 0;
            break;
        case 4:
            result = (mpWiimotePtr->leds & LED_4) != 0;
        default:
            result = 0;
    }

    return result;
}

/*
 * Wii Class Methods
 */

CWii::CWii()
{
    mpWiimoteArraySize = 4;
    mpWiimoteArray = wiic_init(mpWiimoteArraySize);
}

CWii::CWii(int MaxNumWiimotes)
{
    mpWiimoteArraySize = MaxNumWiimotes;
    mpWiimoteArray = wiic_init(mpWiimoteArraySize);
}

CWii::~CWii()
{
    wiic_cleanup((struct wiimote_t**) mpWiimoteArray, mpWiimoteArraySize);
}

void CWii::RefreshWiimotes()
{
    int index;

    // This approach is a bit wasteful but it will work.  The other
    // option is to force the user to handle disconnect events to remove
    // wiimotes from the array.
    mpWiimotesVector.clear();

    for(index = 0; index < mpWiimoteArraySize; index++)
    {
        if((mpWiimoteArray[index]->state & WIIMOTE_STATE_CONNECTED) != 0)
        {
            CWiimote wm(mpWiimoteArray[index]);
            mpWiimotesVector.push_back(wm);
        }
    }
}

int CWii::GetNumConnectedWiimotes()
{
    int index;
    int count = 0;

    for(index = 0; index < mpWiimoteArraySize; index++)
    {
        if((mpWiimoteArray[index]->state & WIIMOTE_STATE_CONNECTED) != 0)
            count++;
    }

    return count;
}

CWiimote& CWii::GetByID(int UnID, int Refresh)
{
    std::vector<CWiimote>::iterator i;

    if(Refresh)
        RefreshWiimotes();

    for(i = mpWiimotesVector.begin(); i != mpWiimotesVector.end(); ++i)
    {
        if((*i).GetID() == UnID)
            return *i;
    }

    return *mpWiimotesVector.begin(); // Return the first one if it was not found.
}

std::vector<CWiimote>& CWii::GetWiimotes(int Refresh)
{
    if(Refresh)
        RefreshWiimotes();

    return mpWiimotesVector;
}

int CWii::Find(int Timeout)
{
    return wiic_find((struct wiimote_t**) mpWiimoteArray, mpWiimoteArraySize, Timeout);
}

int CWii::LoadRegisteredWiimotes()
{
	return wiic_load((struct wiimote_t**) mpWiimoteArray);
}

/**
 *
 * @brief Connects to the found Wii devices.
 *
 * @param autoreconnect	[in] Automatically attempt to re-connect a Wii device in case of unexpected disconnection (default is disabled)
 */
std::vector<CWiimote>& CWii::Connect(bool autoreconnect)
{
    int numConnected = 0;
    int index;

    mpWiimotesVector.clear();

    numConnected = wiic_connect((struct wiimote_t**) mpWiimoteArray, mpWiimoteArraySize, autoreconnect);

    for(index = 0; index < numConnected; index++)
    {
        CWiimote wm(mpWiimoteArray[index]);
        mpWiimotesVector.push_back(wm);
    }

    return mpWiimotesVector;
}

/**
 *
 * @brief Finds up to five devices and automatically connect to all of them.
 *
 * @param timeout	[in] Timeout for the discovery step (default is 5 seconds) 
 * @param rumbleAck	[in] Each found and connected device will receive a small rumble ack (deafult is enabled)
 * @param autoreconnect	[in] Automatically attempt to re-connect a Wii device in case of unexpected disconnection (default is disabled)
 */
std::vector<CWiimote>& CWii::FindAndConnect(int timeout, bool rumbleAck, bool autoreconnect)
{
    std::vector<CWiimote>::iterator i;
    int numFound = 0;
    int index;

    //Find the wiimote
    numFound = Find(timeout);

    // Search for up to five seconds;
    cout << "Found " << numFound << " wiimotes" << endl;
    cout << "Connecting to wiimotes..." << endl;

    // Connect to the wiimote
    Connect(autoreconnect);

	cout << "Connected to " << (unsigned int)mpWiimotesVector.size() << " wiimotes" << endl;

    // Setup the wiimotes
    for(index = 0, i = mpWiimotesVector.begin(); i != mpWiimotesVector.end(); ++i, ++index)
    {
        // Use a reference to make working with the iterator handy.
        CWiimote & wiimote = *i;

        //Rumble for 0.2 seconds as a connection ack
		if(rumbleAck) {
        	wiimote.SetRumbleMode(CWiimote::ON);
        	usleep(200000);
        	wiimote.SetRumbleMode(CWiimote::OFF);
		}
    }

	return mpWiimotesVector;
}

int CWii::Poll()
{
	int events = wiic_poll((struct wiimote_t**) mpWiimoteArray, mpWiimoteArraySize);

	// Logging
	for(std::vector<CWiimote>::iterator i = mpWiimotesVector.begin() ; i != mpWiimotesVector.end() ; ++i)
		if((*i).isLogEnabled())
			(*i).Log();
		
	return events;
}
