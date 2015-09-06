/*
 *	robot.cpp
 *
 *	This file is part of WiiC, written by:
 *		Gabriele Randelli
 *		Email: randelli@dis.uniroma1.it
 *
 *	Copyright 2010
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
 */
#include <fstream>
#include <unistd.h>
#include <wiicpp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>

using namespace std;

#define MAX_SPEED	0.5		// m/sec
#define MAX_JOG		30.0	// deg

float pitch, roll, yaw;
float x, y, z;
float robotSpeed, robotJog;

void acc2robot(CWiimote &wm)
{
	bool enable = wm.Buttons.isPressed(CButtons::BUTTON_B);
	
	if(enable) {
		// Acquire smoothed gravity vector
		wm.Accelerometer.GetGravityVector(x, y, z);
	
		// Acquire orientation
        wm.Accelerometer.GetOrientation(pitch, roll, yaw);
        
		// JOG
		if(fabs(x) <= 0.25)  // Still
			robotJog = 0.0;	
		else 
			robotJog = MAX_JOG*roll*2.0/M_PI;

		robotJog > MAX_JOG ? robotJog = MAX_JOG : robotJog;
		robotJog < -MAX_JOG ? robotJog = -MAX_JOG : robotJog;

		// SPEED
		if(fabs(y) <= 0.25)  // Still
			robotSpeed = 0.0;	
		else 
			robotSpeed = MAX_SPEED*pitch*2.0/M_PI;

		robotSpeed > MAX_SPEED ? robotSpeed = MAX_SPEED : robotSpeed;
		robotSpeed < -MAX_SPEED ? robotSpeed = -MAX_SPEED : robotSpeed;	
	}
	else {
		pitch = roll = yaw = x = y = z = 0.0;
		robotSpeed = robotJog = 0.0;
	}
}

int main(int argc, char** argv)
{
    CWii wii; // Defaults to 4 remotes
    std::vector<CWiimote>::iterator i;
    int reloadWiimotes = 0;
    int numFound;
    int index;
    bool help = false;
    roll = pitch = yaw = x = y = z = robotSpeed = robotJog = 0.0; // Init

	string option; 
	if(argc == 2) {
		option = string(argv[1]);
		if(option == "help") {
			cout << endl << "wiic-robot is a simple teleoperation interface to control a robot with a Wiimote." << endl;
			cout << "wiic-robot is part of WiiC (http://wiic.sf.net)" << endl << endl;
			cout << "Usage: wiic-robot" << endl;
		
			return 1;
		}
	}
	
	// Find and connect to the wiimotes
    std::vector<CWiimote>& wiimotes = wii.FindAndConnect(1);
	
	if(!wiimotes.size()) {
		cout << "Error: no connected Wiimote" << endl;
		return 1;		
	}
	CWiimote& wiimote = wiimotes[0];
	
    do
    {
        if(reloadWiimotes)
        {
            // Regenerate the list of wiimotes
            wiimotes = wii.GetWiimotes();
            reloadWiimotes = 0;
        }

        //Poll the wiimotes to get the status like pitch or roll
        if(wii.Poll())
        {
            for(i = wiimotes.begin(); i != wiimotes.end(); ++i)
            {
                // Use a reference to make working with the iterator handy.
                CWiimote & wiimote = *i;
                switch(wiimote.GetEvent())
                {

                    case CWiimote::EVENT_EVENT:
                        acc2robot(wiimote);
                        break;

                    case CWiimote::EVENT_DISCONNECT:
                    case CWiimote::EVENT_UNEXPECTED_DISCONNECT:
                        reloadWiimotes = 1;
                        break;

                    default:
                        break;
                }                
            }
        }

    } while(wiimotes.size()); // Go so long as there are wiimotes left to poll

    return 0;
}
