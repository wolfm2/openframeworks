#pragma once

#include "ofMain.h"

class particle {
private:
		ofPoint pos;
		ofPoint vel;
		ofColor color;
		int rad = 20;
		void hit();
		void distanceSensor();
		bool inCol = true;
		
public:
		particle();
		ofSoundPlayer tone;
		bool doTone = false;
		void setup(int idx);
		void update();
		void draw();
		int getX();
		int getY();
		int getR();
};

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		

};
