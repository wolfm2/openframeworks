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

agent::agent(){
  agentVect.set(-1,-1);	// init start location of agent (hidden from view)
  endpoint.set(0,0);	// where we are going to
  
  initAgentVectors();
}

// reset vectors back to origin and pointing up
void agent::initAgentVectors(){
  p1.set(6.25,  0);	// triangle pointing up at origin
  p2.set(0, -25);
  p3.set(-6.25,  0);
}

void agent::update(){
  ofVec2f middle;
  
  float angle = calcAlignment(); // angle needed to point at dest
  
  initAgentVectors();
  
  p1 += agentVect;  // move to current location 
  p2 += agentVect;
  p3 += agentVect;
  
  ofVec2f vArr[] = {p1,p2,p3};
  middle.average(vArr, 3);  // middle of our triangle
  
  p1.rotate(angle, middle); // rotate all vectors relative to midpoint
  p2.rotate(angle, middle);
  p3.rotate(angle, middle);
  
  // agentVect += (endpoint - p2) * .1;
}

void agent::draw(){
  ofVec2f offscreen(-1,-1); 
  if (agentVect != offscreen) {  // if not offscreen, draw it
    ofDrawTriangle(p1, p2, p3);
  }
}

void agent::set(float X, float Y){
  agentVect.set(X,Y);	// init start location of agent
}

void agent::setDest(float X, float Y){
  endpoint.set(X,Y);	// init end location of agent
}

float agent::calcAlignment(){
  ofVec2f top(0,-1);				// top of origin used as control
  
  float angle = top.angle(endpoint - agentVect);	// put at origin - get angle from control
  if (angle < 0)				// because it goes from (-180 - 180) rather than (0 - 360)
    angle = 360 + angle;
    
  return (angle);
}

void agent::calcCohesion(){
}

void agent::calcDispersion(){
}

#define MAX_AGENTS 100		// how many agents we can have max
agent Agents[MAX_AGENTS];	// storage (memory) space for all the unique details of every agent
int curAgent = 0;			// what agent we are dealing with


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

  for (int i=0; i < MAX_AGENTS; i++) {
    Agents[i].setDest(mouseX, mouseY);
    Agents[i].update();
  }
}

//--------------------------------------------------------------
void ofApp::draw(){

	int alpha = 150;
	ofColor orange(255, 125, 0, alpha);
	ofColor purple(198, 0, 205, alpha);
	ofColor inbetween = orange.getLerped(purple, ofRandom(1.0));
	ofSetColor(inbetween);

  for (int i=0; i < MAX_AGENTS; i++) {
    Agents[i].draw();
  }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

    Agents[curAgent].set(mouseX, mouseY);
    curAgent++;
    if (curAgent >= MAX_AGENTS) {  // make sure it does not go over
      curAgent = 0;
    }
    
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
