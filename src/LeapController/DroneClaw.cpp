/******************************************************************************\
* Copyright (C) 2012-2014 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/
//change

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//#include <cstring>
#include <string.h>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include "Leap.h"
#include <termios.h>

typedef unsigned char byte;

using namespace Leap;
using namespace std;

//set up the serial port
//const string PORT = "/dev/cu.usbserial-DN01AJVC";
int arduino = open( "/dev/cu.usbserial-DN01AJVC" ,O_RDWR | O_NOCTTY | O_NDELAY);

long map(long x, long in_min, long in_max, long out_min, long out_max);


class SampleListener : public Listener {
  public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
    virtual void onFocusGained(const Controller&);
    virtual void onFocusLost(const Controller&);
    virtual void onDeviceChange(const Controller&);
    virtual void onServiceConnect(const Controller&);
    virtual void onServiceDisconnect(const Controller&);

  private:
};

const string fingerNames[] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
//const string boneNames[] = {"Metacarpal", "Proximal", "Middle", "Distal"};
const string stateNames[] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};

//Initialize the Leap controller
void SampleListener::onInit(const Controller& controller) {
  cout << "Initialized" << endl;
}

//Connect the Leap controller
void SampleListener::onConnect(const Controller& controller) {
  cout << "Connected" << endl;
  //enable the following getures to be used with the Leap controller
  /*Disabled for now
  controller.enableGesture(Gesture::TYPE_CIRCLE);
  controller.enableGesture(Gesture::TYPE_KEY_TAP);
  controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
  controller.enableGesture(Gesture::TYPE_SWIPE);
  */
}

//Disconnect the Leap controller
void SampleListener::onDisconnect(const Controller& controller) {
  // Note: not dispatched when running in a debugger.
  cout << "Disconnected" << endl;
}

//Exit the Leap process
void SampleListener::onExit(const Controller& controller) {
  cout << "Exited" << endl;
}

//Listen to frames, this is where most of the processing happens.
void SampleListener::onFrame(const Controller& controller) {
  // Get the most recent frame and report some basic information
  //int arduino = open( "/dev/cu.usbserial-DN01AJVC" ,O_RDWR | O_NOCTTY | O_NDELAY);
  const Frame frame = controller.frame();
  cout << "Frame id: " << frame.id()
            << ", timestamp: " << frame.timestamp()
            << ", hands: " << frame.hands().count()
            << ", extended fingers: " << frame.fingers().extended().count()
            //<< ", tools: " << frame.tools().count()
            //<< ", gestures: " << frame.gestures().count()
            << endl;

  HandList hands = frame.hands();
  for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
    // Get the first hand
    const Hand hand = *hl;
    string handType = hand.isLeft() ? "Left hand" : "Right hand";
    cout << string(2, ' ') << handType << ", id: " << hand.id()
              << ", palm position: " << hand.palmPosition() << endl;
      cout << string(2, ' ') << ", grab strength: " << hand.grabStrength() << endl;

    // Get the hand's normal vector and direction
    const Vector normal = hand.palmNormal();
    const Vector direction = hand.direction();

    // Calculate the hand's pitch, roll, and yaw angles
    cout << string(2, ' ') <<  "pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "
              << "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
              << "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << endl;
 
    // Outbut to the arduino ***TEST***
      int DroneGrab = hand.grabStrength() * 75 + 45;
      //string angleString = to_string(DroneGrab) + " \n";
      cout << "Drone Grab angle: " << DroneGrab << endl;
      //write(arduino,&angleString,sizeof(angleString));   //original line
      byte stream[3] = {0x00, 0x02, (byte) DroneGrab};
      write(arduino,&stream,sizeof(stream));
  }
  // Get gestures
  /* Disabled for now
  const GestureList gestures = frame.gestures();
  for (int g = 0; g < gestures.count(); ++g) {
    Gesture gesture = gestures[g];

    switch (gesture.type()) {
      case Gesture::TYPE_CIRCLE:
      {
        CircleGesture circle = gesture;
        string clockwiseness;

        if (circle.pointable().direction().angleTo(circle.normal()) <= PI/2) {
          clockwiseness = "clockwise";
        } else {
          clockwiseness = "counterclockwise";
        }

        // Calculate angle swept since last frame
        float sweptAngle = 0;
        if (circle.state() != Gesture::STATE_START) {
          CircleGesture previousUpdate = CircleGesture(controller.frame(1).gesture(circle.id()));
          sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * PI;
        }
        cout << string(2, ' ')
                  << "Circle id: " << gesture.id()
                  << ", state: " << stateNames[gesture.state()]
                  << ", progress: " << circle.progress()
                  << ", radius: " << circle.radius()
                  << ", angle " << sweptAngle * RAD_TO_DEG
                  <<  ", " << clockwiseness << endl;
        break;
      }
      case Gesture::TYPE_SWIPE:
      {
        SwipeGesture swipe = gesture;
        cout << string(2, ' ')
          << "Swipe id: " << gesture.id()
          << ", state: " << stateNames[gesture.state()]
          << ", direction: " << swipe.direction()
          << ", speed: " << swipe.speed() << endl;
        break;
      }
      case Gesture::TYPE_KEY_TAP:
      {
        KeyTapGesture tap = gesture;
        cout << string(2, ' ')
          << "Key Tap id: " << gesture.id()
          << ", state: " << stateNames[gesture.state()]
          << ", position: " << tap.position()
          << ", direction: " << tap.direction()<< endl;
        break;
      }
      case Gesture::TYPE_SCREEN_TAP:
      {
        ScreenTapGesture screentap = gesture;
        cout << string(2, ' ')
          << "Screen Tap id: " << gesture.id()
          << ", state: " << stateNames[gesture.state()]
          << ", position: " << screentap.position()
          << ", direction: " << screentap.direction()<< endl;
        break;
      }
      default:
        cout << string(2, ' ')  << "Unknown gesture type." << endl;
        break;
    }
  }

  if (!frame.hands().isEmpty() || !gestures.isEmpty()) {
    cout << endl;
  }
*/
}

void SampleListener::onFocusGained(const Controller& controller) {
  cout << "Focus Gained" << endl;
}

void SampleListener::onFocusLost(const Controller& controller) {
  cout << "Focus Lost" << endl;
}

void SampleListener::onDeviceChange(const Controller& controller) {
  cout << "Device Changed" << endl;
  const DeviceList devices = controller.devices();

  for (int i = 0; i < devices.count(); ++i) {
    cout << "id: " << devices[i].toString() << endl;
    cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << endl;
  }
}

void SampleListener::onServiceConnect(const Controller& controller) {
  cout << "Service Connected" << endl;
}

void SampleListener::onServiceDisconnect(const Controller& controller) {
  cout << "Service Disconnected" << endl;
}


int main(int argc, char** argv) {
  
    struct termios options;  // Terminal options
    fcntl(arduino, F_SETFL,0);
    tcgetattr(arduino, &options);
    cfsetospeed(&options, B115200);
    cfsetispeed(&options, B115200);
    tcsetattr(arduino, TCSANOW, &options);
  // Create a sample listener and controller
  SampleListener listener;
  Controller controller;

  // Have the sample listener receive events from the controller
  controller.addListener(listener);

  if (argc > 1 && strcmp(argv[1], "--bg") == 0)
    controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);
  // Keep this process running until Enter is pressed
  cout << "Press Enter to quit..." << endl;
  cin.get();

  // Remove the sample listener when done
  controller.removeListener(listener);
  close(arduino);
  return 0;
}


long map(long x, long in_min, long in_max, long out_min, long out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min; }