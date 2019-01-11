#include "ofApp.h"

// This is an example of how to implement something via functions
// if you still feel shaky with using objects.
// 
ofImage gyroNom;

extern int w, h, avatarX, avatarY, avatarZ;

const int numNPPoints = 15; // number of non-player points

// points have the following attributes
ofPoint NPPoints[numNPPoints];  // position
int NPPSpeed[numNPPoints];      // speed
int NPPRadius[numNPPoints];     // radius
int NPPAlive[numNPPoints];      // am I alive or not?

void NPPUpdate (int idx) {
  if (NPPAlive[idx]) {
    int rand = ofRandom(1, 10);
    
    if (rand > 8 && rand % 2 == 1) { // only a small % chance of moving
      NPPoints[idx].x++;
    }
    if (rand > 8 && rand % 2 == 0) { // only a small % chance of moving
      NPPoints[idx].x--;
    }
      
    NPPoints[idx].x = ofClamp(NPPoints[idx].x, 0, w); // keep it on screen
    
    NPPoints[idx].y += NPPSpeed[idx];  // update speed
    
    if (NPPoints[idx].y > h) { // if beyond bottom of screen
      NPPAlive[idx] = false;
    }
    
  } else {
    int rand = ofRandom(1, 10);
    if (rand > 3){  // randomly dont start certain points so drawing staggers
      NPPAlive[idx] = true;
      NPPSpeed[idx] = ofRandom(3, 7);
      NPPoints[idx].x = ofRandom(30, w-30); // start at random point from top
      NPPoints[idx].y = 0;
    }
  }
}

void NPPDraw (int idx) {
  ofColor c = ofColor(128, 0, 250);  // set hsv
  int bDir = (int(NPPoints[idx].y) & 0x100) >> 8;  // breathing direction
  int rDel = (int(NPPoints[idx].y) & 0xF0) >> 4;   // radius delta size
  
  if (bDir == 0) { // breathe radius & hue relative to Y
    NPPRadius[idx] = 20 + rDel;// y pos controls width 
    c.setHue(rDel * 5 + 100); // add delta to min hue
  } else {
    NPPRadius[idx] = 20 + 16 - rDel;
    c.setHue(rDel * -5 + 100 + 75); // minus delta from max hue
  }
  ofSetColor(c); 
  ofCircle (NPPoints[idx], NPPRadius[idx]);
  ofSetColor(255,255,255);
  gyroNom.draw(NPPoints[idx].x-20,NPPoints[idx].y-20,40,40);
}

// Detection for circles is easy: (x2-x1)^2 + (y1-y2)^2 <= (r1+r2)^2
// true if collision detected
bool NPPCollisionDetection (int idx) {
  return (pow((avatarX-NPPoints[idx].x),2) + pow((avatarY-NPPoints[idx].y),2) <= pow((avatarZ+NPPRadius[idx]),2));
}
