/*
 *    wiicpp.h
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
 *		$Header$
 *
 */

#ifndef WIICPP_H_
#define WIICPP_H_

#include <vector>
#include <wiic.h>
#include <dataset.h>
#include <logger.h>

using namespace WiiC;

class CButtonBase
{
public:
    CButtonBase(void *ButtonsPtr, void *ButtonsHeldPtr, void *ButtonsReleasedPtr);

    int isPressed(int Button);
    int isHeld(int Button);
    int isReleased(int Button);
    int isJustPressed(int Button);

private:
    virtual short Cast(void *Ptr) {return *((short *)(Ptr));} // Inlined.

    void *mpBtnsPtr;
    void *mpBtnsHeldPtr;
    void *mpBtnsReleasedPtr;
};

class CButtons : public CButtonBase
{
public:
    enum ButtonDefs
    {
        BUTTON_TWO = WIIMOTE_BUTTON_TWO,
        BUTTON_ONE = WIIMOTE_BUTTON_ONE,
        BUTTON_B = WIIMOTE_BUTTON_B,
        BUTTON_A = WIIMOTE_BUTTON_A,
        BUTTON_MINUS = WIIMOTE_BUTTON_MINUS,
        BUTTON_HOME = WIIMOTE_BUTTON_HOME,
        BUTTON_LEFT = WIIMOTE_BUTTON_LEFT,
        BUTTON_RIGHT = WIIMOTE_BUTTON_RIGHT,
        BUTTON_DOWN = WIIMOTE_BUTTON_DOWN,
        BUTTON_UP = WIIMOTE_BUTTON_UP,
        BUTTON_PLUS = WIIMOTE_BUTTON_PLUS,
        BUTTON_UNKNOWN = WIIMOTE_BUTTON_UNKNOWN,
        BUTTON_ALL = WIIMOTE_BUTTON_ALL
    };

    CButtons(void *ButtonsPtr, void *ButtonsHeldPtr, void *ButtonsReleasedPtr);
};

class CNunchukButtons : public CButtonBase
{
public:
    enum ButtonDefs
    {
        BUTTON_Z = NUNCHUK_BUTTON_Z,
        BUTTON_C = NUNCHUK_BUTTON_C,
        BUTTON_ALL = NUNCHUK_BUTTON_ALL
    };

    CNunchukButtons(void *ButtonsPtr, void *ButtonsHeldPtr, void *ButtonsReleasedPtr);

private:
    short Cast(void *Ptr) {return (short)(*((byte *)(Ptr)));} // Inlined using the different type.
};

class CClassicButtons : public CButtonBase
{
public:
    enum ButtonDefs
    {
        BUTTON_X = CLASSIC_CTRL_BUTTON_X,
        BUTTON_Y = CLASSIC_CTRL_BUTTON_Y,
        BUTTON_B = CLASSIC_CTRL_BUTTON_B,
        BUTTON_A = CLASSIC_CTRL_BUTTON_A,
        BUTTON_MINUS = CLASSIC_CTRL_BUTTON_MINUS,
        BUTTON_HOME = CLASSIC_CTRL_BUTTON_HOME,
        BUTTON_LEFT = CLASSIC_CTRL_BUTTON_LEFT,
        BUTTON_RIGHT = CLASSIC_CTRL_BUTTON_RIGHT,
        BUTTON_DOWN = CLASSIC_CTRL_BUTTON_DOWN,
        BUTTON_UP = CLASSIC_CTRL_BUTTON_UP,
        BUTTON_PLUS = CLASSIC_CTRL_BUTTON_PLUS,
        BUTTON_ZR = CLASSIC_CTRL_BUTTON_ZR,
        BUTTON_ZL = CLASSIC_CTRL_BUTTON_ZL,
        BUTTON_FULL_R = CLASSIC_CTRL_BUTTON_FULL_R,
        BUTTON_FULL_L = CLASSIC_CTRL_BUTTON_FULL_L,
        BUTTON_ALL = CLASSIC_CTRL_BUTTON_ALL
    };

    CClassicButtons(void *ButtonsPtr, void *ButtonsHeldPtr, void *ButtonsReleasedPtr);
};

class CGH3Buttons : public CButtonBase
{
public:
    enum ButtonDefs
    {
        BUTTON_STRUM_UP = GUITAR_HERO_3_BUTTON_STRUM_UP,
        BUTTON_STRUM_DOWN = GUITAR_HERO_3_BUTTON_STRUM_DOWN,
        BUTTON_YELLOW = GUITAR_HERO_3_BUTTON_YELLOW,
        BUTTON_GREEN = GUITAR_HERO_3_BUTTON_GREEN,
        BUTTON_BLUE = GUITAR_HERO_3_BUTTON_BLUE,
        BUTTON_RED = GUITAR_HERO_3_BUTTON_RED,
        BUTTON_ORANGE = GUITAR_HERO_3_BUTTON_ORANGE,
        BUTTON_MINUS = GUITAR_HERO_3_BUTTON_MINUS,
        BUTTON_PLUS = GUITAR_HERO_3_BUTTON_PLUS,
        BUTTON_ALL = GUITAR_HERO_3_BUTTON_ALL
    };

    CGH3Buttons(void *ButtonsPtr, void *ButtonsHeldPtr, void *ButtonsReleasedPtr);
};

class CJoystick
{
public:
    CJoystick(struct joystick_t *JSPtr);

    void GetMaxCal(int &X, int &Y);
    void SetMaxCal(int X, int Y);

    void GetMinCal(int &X, int &Y);
    void SetMinCal(int X, int Y);

    void GetCenterCal(int &X, int &Y);
    void SetCenterCal(int X, int Y);

    void GetPosition(float &Angle, float &Magnitude);

private:
    struct joystick_t *mpJoystickPtr;
};

class CAccelerometer
{
public:
    CAccelerometer(struct accel_t *AccelCalPtr,
                  struct vec3b_t *AccelerationPtr, int *AccelThresholdPtr,
                  struct orient_t *OrientationPtr, float *OrientationThresholdPtr,
                  struct gforce_t *GForcePtr);

    float SetSmoothAlpha(float Alpha);

    float GetOrientThreshold();
    void SetOrientThreshold(float Threshold);

    int GetAccelThreshold();
    void SetAccelThreshold(int Threshold);

    void GetOrientation(float &Pitch, float &Roll, float &Yaw);

    void GetGravityCalVector(float &X, float &Y, float &Z);
    void SetGravityCalVector(float X, float Y, float Z);

    void GetGravityVector(float &X, float &Y, float &Z);
	void GetRawGravityVector(float &X, float &Y, float &Z);

private:
    struct accel_t *mpAccelCalibPtr;
    struct vec3b_t *mpAccelPtr;
    struct orient_t *mpOrientPtr;
    struct gforce_t *mpGForcePtr;
    int *mpAccelThresholdPtr;
    float *mpOrientThresholdPtr;
};

class CGyroscope
{
public:
	CGyroscope(struct ang3s_t* RawGyro, struct ang3s_t* CalGyro, struct ang3f_t* AngleRate, unsigned char* Mode, struct motion_plus_t* MPPtr, 
		int* GyroThresholdPtr);
	void GetRawRates(int& Roll, int& Pitch, int& Yaw);
	void GetRates(float& Roll, float& Pitch, float& Yaw);
	void Calibrate();
    int GetGyroThreshold();
    void SetGyroThreshold(int Threshold);

private:
	struct ang3s_t* mpRawGyro;
	struct ang3s_t* mpCalGyro;
	struct ang3f_t* mpAngleRate;
	struct motion_plus_t* mpMPPtr;
	unsigned char* mpMode;
	int* mpGyroThresholdPtr;
};

class CWeightSensor
{
public:
	CWeightSensor(struct pressure_t* RawWeight, struct pressure_t* LowCalWeight, struct pressure_t* MediumCalWeight, struct pressure_t* HighCalWeight, struct pressure_weight_t* Weight, struct balance_board_t* BBPtr);
	void GetRawWeight(int& TopLeft, int& TopRight, int& BottomLeft, int& BottomRight);
	void GetWeight(float& TotalWeight, float& TopLeft, float& TopRight, float& BottomLeft, float& BottomRight);
	void GetLowCalWeight(int& TopLeft, int& TopRight, int& BottomLeft, int& BottomRight);
   void GetMediumCalWeight(int& TopLeft, int& TopRight, int& BottomLeft, int& BottomRight);
   void GetHighCalWeight(int& TopLeft, int& TopRight, int& BottomLeft, int& BottomRight);

private:
	struct pressure_t* mpRawWeight;
	struct pressure_t* mpLowCalWeight;
	struct pressure_t* mpMediumCalWeight;
	struct pressure_t* mpHighCalWeight;
	struct pressure_weight_t* mpWeight;
	struct balance_board_t* mpBBPtr;
};

class CIRDot
{
public:
    CIRDot();
    CIRDot(struct ir_dot_t *DotPtr);
    CIRDot(const CIRDot & copyin);

    int isVisible();
    int GetSize();
    int GetOrder();
    void GetCoordinate(int &X, int &Y);
    void GetRawCoordinate(int &X, int &Y);

private:
    struct ir_dot_t *mpDotPtr;
};

class CIR
{
public:
    enum BarPositions
    {
        BAR_ABOVE = WIIC_IR_ABOVE,
        BAR_BELOW = WIIC_IR_BELOW
    };

    enum AspectRatioSelections
    {
        ASPECT_4_3 = WIIC_ASPECT_4_3,
        ASPECT_16_9 = WIIC_ASPECT_16_9
    };

    enum OnOffSelection
    {
        OFF = 0,
        ON = 1
    };

    CIR(struct wiimote_t *wmPtr);

    void SetMode(OnOffSelection State);
    void SetVres(unsigned int x, unsigned int y);

    BarPositions GetBarPositionSetting();
    void SetBarPosition(BarPositions PositionSelection);

    AspectRatioSelections GetAspectRatioSetting();
    void SetAspectRatio(AspectRatioSelections AspectRatioSelection );

    void SetSensitivity(int Level);
    int GetSensitivity();

    int GetNumDots();
    std::vector<CIRDot>& GetDots();

    void GetOffset(int &X, int &Y);
    int GetState();
    void GetCursorPositionAbsolute(int &X, int &Y);
    void GetCursorPosition(int &X, int &Y);
    float GetPixelDistance();
    float GetDistance();

private:
    struct wiimote_t *mpWiimotePtr;
    std::vector<CIRDot> mpIRDotsVector;
};

class CNunchuk
{
public:
    CNunchuk(struct expansion_t *ExpPtr);

    CNunchukButtons Buttons;
    CJoystick Joystick;
    CAccelerometer Accelerometer;

private:
    struct nunchuk_t *mpNunchukPtr;
};

class CClassic
{
public:
    CClassic(struct expansion_t *ExpPtr);

    float GetLShoulderButton();
    float GetRShoulderButton();

    CClassicButtons Buttons;
    CJoystick LeftJoystick;
    CJoystick RightJoystick;

private:
    struct classic_ctrl_t *mpClassicPtr;
};

class CGuitarHero3
{
public:
    CGuitarHero3(struct expansion_t *ExpPtr);

    float GetWhammyBar();

    CGH3Buttons Buttons;
    CJoystick Joystick;

private:
    struct guitar_hero_3_t *mpGH3Ptr;
};


class CMotionPlus
{
public:
    CMotionPlus(struct expansion_t* MPPtr);

	void Connect(struct wiimote_t* WiimotePtr);
	void Disconnect(struct wiimote_t* WiimotePtr);
	
    CGyroscope Gyroscope;
private:
    struct motion_plus_t* mpMPPtr;
};

class CBalanceBoard
{
public:
    CBalanceBoard(struct expansion_t* BBPtr);
	
    CWeightSensor WeightSensor;
private:
    struct balance_board_t* mpBBPtr;
};

class CExpansionDevice
{
public:
    enum ExpTypes
    {
        TYPE_NONE = EXP_NONE,
        TYPE_NUNCHUK = EXP_NUNCHUK,
        TYPE_CLASSIC = EXP_CLASSIC,
        TYPE_GUITAR_HERO_3 = EXP_GUITAR_HERO_3,
        TYPE_MOTION_PLUS = EXP_MOTION_PLUS,
		TYPE_BALANCE_BOARD = EXP_BALANCE_BOARD
    };

    CExpansionDevice(struct expansion_t *ExpPtr);

    ExpTypes GetType();

    CNunchuk Nunchuk;
    CClassic Classic;
    CGuitarHero3 GuitarHero3;
    CMotionPlus MotionPlus;
	CBalanceBoard BalanceBoard;

private:
    struct expansion_t *mpExpansionPtr;
};


class CWiimote
{
public:
    enum LEDS
    {
        LED_NONE = WIIMOTE_LED_NONE,
        LED_1 = WIIMOTE_LED_1,
        LED_2 = WIIMOTE_LED_2,
        LED_3 = WIIMOTE_LED_3,
        LED_4 = WIIMOTE_LED_4

    };

    enum Flags
    {
        FLAG_SMOOTHING = WIIC_SMOOTHING,
        FLAG_CONTINUOUS = WIIC_CONTINUOUS,
        FLAG_ORIENT_THRESH = WIIC_ORIENT_THRESH,
        FLAG_INIT_FLAGS = WIIC_INIT_FLAGS
    };

    enum EventTypes
    {
        EVENT_NONE = WIIC_NONE,
        EVENT_EVENT = WIIC_EVENT,
        EVENT_STATUS = WIIC_STATUS,
        EVENT_CONNECT = WIIC_CONNECT,
        EVENT_DISCONNECT = WIIC_DISCONNECT,
        EVENT_UNEXPECTED_DISCONNECT = WIIC_UNEXPECTED_DISCONNECT,
        EVENT_READ_DATA = WIIC_READ_DATA,
        EVENT_NUNCHUK_INSERTED = WIIC_NUNCHUK_INSERTED,
        EVENT_NUNCHUK_REMOVED = WIIC_NUNCHUK_REMOVED,
        EVENT_CLASSIC_CTRL_INSERTED = WIIC_CLASSIC_CTRL_INSERTED,
        EVENT_CLASSIC_CTRL_REMOVED = WIIC_CLASSIC_CTRL_REMOVED,
        EVENT_GUITAR_HERO_3_CTRL_INSERTED = WIIC_GUITAR_HERO_3_CTRL_INSERTED,
        EVENT_GUITAR_HERO_3_CTRL_REMOVED = WIIC_GUITAR_HERO_3_CTRL_REMOVED,
        EVENT_MOTION_PLUS_INSERTED = WIIC_MOTION_PLUS_INSERTED,
        EVENT_MOTION_PLUS_REMOVED = WIIC_MOTION_PLUS_REMOVED,
		  EVENT_BALANCE_BOARD_INSERTED = WIIC_BALANCE_BOARD_INSERTED,
		  EVENT_BALANCE_BOARD_REMOVED = WIIC_BALANCE_BOARD_REMOVED
    };

    enum OnOffSelection
    {
        OFF = 0,
        ON = 1
    };

    static const int EVENT_BUFFER_LENGTH = MAX_PAYLOAD;

    CWiimote();
    CWiimote(struct wiimote_t *wmPtr);
    CWiimote(const CWiimote & copyin);
    ~CWiimote() { }

    void Disconnected();

    void SetRumbleMode(OnOffSelection State);
    void ToggleRumble();
    bool isRumbleEnabled();

    int GetLEDs();
    void SetLEDs(int LEDs);

    float GetBatteryLevel();

    int GetHandshakeState();

    EventTypes GetEvent();
    const unsigned char *GetEventBuffer();

	void SetSmoothing(bool Smooth);

    void SetMotionSensingMode(OnOffSelection State);
    void EnableMotionPlus(OnOffSelection State);

	/* Logging methods */
	inline void LogStart(int type =WIIC_LOG_NONE, const string& file ="") { 
		logType = type;
		logger.SetLogLevel(WIIC_LOG_START, type, file);
	}
	inline void LogStop() { logger.SetLogLevel(WIIC_LOG_STOP); }
	inline void Log(); 
	
	/* Timestamp methods */
	inline struct timeval GetTimestamp() const { return mpWiimotePtr->timestamp; }

	void EnableSpeaker(OnOffSelection State);
	void MuteSpeaker(OnOffSelection State);
	void PlaySound();

    void ReadData(unsigned char *Buffer, unsigned int Offset, unsigned int Length);
    void WriteData(unsigned int Address, unsigned char *Data, unsigned int Length);

    void UpdateStatus();

    int GetID();
    const char* GetAddress();

    int GetState();

    int GetFlags();
    int SetFlags(int Enable, int Disable);

    void Resync();

    void Disconnect();

    int isUsingACC();
    int isUsingEXP();
    int isUsingIR();
    int isUsingSpeaker();
	int isSpeakerMuted();
    int isUsingMotionPlus();
    int isLEDSet(int LEDNum);
	inline bool isLogEnabled() { return logger.isLogEnabled(); }

    CIR IR;
    CButtons Buttons;
    CAccelerometer Accelerometer;
    CExpansionDevice ExpansionDevice;

    struct wiimote_t *mpWiimotePtr; // Pointer to the wm structure
private:
    int mpTempInt;
    float mpTempFloat;
    
	Logger logger;
	int logType;
};

class CWii
{
public:

    CWii();
    CWii(int MaxNumCWiimotes);
    virtual ~CWii();

    int GetNumConnectedWiimotes();

    void RefreshWiimotes();

    CWiimote& GetByID(int ID, int Refresh=1);
    std::vector<CWiimote>& GetWiimotes(int Refresh=1);

    int Find(int timeout);
	int LoadRegisteredWiimotes();
    std::vector<CWiimote>& Connect(bool autoreconnect =false);
	std::vector<CWiimote>& FindAndConnect(int timeout =5, bool rumbleAck = true, bool autoreconnect =false);

    int Poll();

private:
    struct wiimote_t **mpWiimoteArray;
    int mpWiimoteArraySize;
    std::vector<CWiimote> mpWiimotesVector;
};

#endif /* WIICPP_H_ */
