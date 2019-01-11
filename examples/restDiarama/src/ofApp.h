#pragma once

#include "ofMain.h"

class actor {
	public:
		actor();
		void setup(int idx);
		void update(int time, int sunrise, int sunset);
		void draw();
		void activate();
		
		ofImage plank;	// wood peice
		ofImage icon;	// icon at end of plank
		bool top;	// moving on top?
		bool left;	// lTr or rTl movement
		ofPoint pos;	// where is plank now?
		int xDest;	// where should it ease to?	
		
		bool active;	// is it alive	
		bool day;	// show daytime or nighttime celestial body
		int type;	// animal cloud or cbody
		int height;	// randomized height of plank
		// int speed;     TODO implement clouds
		// int constant;
		
		
		ofSoundPlayer player;
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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        	void urlResponse(ofHttpResponse & response);  
};


