#pragma once

#include "ofMain.h"
extern int NPPAlive[];
extern const int numNPPoints;
extern void NPPUpdate (int idx);
extern void NPPDraw (int idx);
extern bool NPPCollisionDetection (int idx);
extern ofImage gyroNom;
 
const int numSounds = 10;

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void mouseMoved(int x, int y);
		
		ofSoundPlayer mySound[numSounds];
		ofSoundPlayer backgroundMusic;
		ofSoundPlayer laugh;
		ofSoundPlayer stretch;
		
		/* void keyPressed(int key);
		void keyReleased(int key);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg); */
};
