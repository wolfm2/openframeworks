// =============================================================================
//
// Source code for section 1.ii.e. Fleeing Triangle Brush from the Introduction
// to Graphics chapter of ofBook (https://github.com/openframeworks/ofBook).
//
// Copyright (c) 2014 Michael Hadley, mikewesthad.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================

#include "ofApp.h"

#include <iostream>
using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(1024,768);

    ofSetFrameRate(60); // Limit the speed of our program to 60 frames per second

    //ofSetBackgroundAuto(false); // Stop the background from being redrawn each frame
    // We still want to draw on a black background, so we need to draw
    // the background before we do anything with the brush
    ofBackground(0);
}

//--------------------------------------------------------------
void ofApp::update(){
}

int rotation = 0;
float angle = 0;
ofVec2f v1(0,0);
ofVec2f v2(0,0);  // mouse
ofVec2f delta(0,0);

ofVec2f p1(6.25,  0);
ofVec2f p2(0, -25);
ofVec2f p3(-6.25,  0);

float prevAngle = 0;

//--------------------------------------------------------------
void ofApp::draw(){
     int alpha = 150;
     
     // take dist between mouse and tip of triangle and move a fraction of that
     delta = (v2 - p2) * .1; // move all relative to same point.
     p1 += delta;
     p2 += delta;
     p3 += delta;
     
     
     ofVec2f vArr[] = {p1,p2,p3};
     
            ofColor aqua(0, 252, 255, alpha);
            ofColor purple(198, 0, 205, alpha);
            ofColor inbetween = aqua.getLerped(purple, ofRandom(1.0));
            ofSetColor(inbetween);

	    v1.average(vArr, 3);  // git middle of triangle
	    
	    // ANGLE ROTATION
	    // undo last rotation
	    p1.rotate(prevAngle * -1, v1);  
     	    p2.rotate(prevAngle * -1, v1);
            p3.rotate(prevAngle * -1, v1);
	    
	    // rotate to new mouse coords
	    // angle set by mouseMoved()
	    p1.rotate(angle, v1);  // rotate this vector around its middle (v1) towards the mouse
     	    p2.rotate(angle, v1);
            p3.rotate(angle, v1);
	    prevAngle = angle;  // save the old one

	    
            ofDrawTriangle(p1, p2, p3); // draw new mouse
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
  v2.set(x,y);				// mouse
  
  // get angle to point at
/*
  ofVec2f right(0,-1);  // point up
  
  angle = right.angle(v2-v1);		// put at origin - get angle from control
  if (angle < 0)
    angle = 360 + angle;
  //cout << x << " " << y << " angle: " << angle << "\n";
*/
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
