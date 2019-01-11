#include "ofApp.h"

float x, y, angle = 0;
//--------------------------------------------------------------
void ofApp::setup(){
  
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------

void ofApp::draw(){
  
  int radius = 100;
  // cos & sin take RADIANS.  
  // 2pi radians would be 2 x 57.2958 x 3.1416 = 360 degrees
  // Therefore to draw a whole circle you would use 0-TWO_PI to draw a whole circle 
  x = radius * cos( ofDegToRad(angle) ) + ofGetWidth()/2;
  y = radius * sin( ofDegToRad(angle) ) + ofGetHeight()/2;
  
  angle += 1;
  if (angle >= 360) angle = 0; // TWO_PI is a whole circle
  
  ofSetColor(255, 0, 0);
  ofCircle(x, y, 20);
  
  ofSetColor(0, 255, 0);
  ofCircle(ofGetWidth()/2, y, 20); // sin only
  
  ofSetColor(0, 0, 255);
  ofCircle(x, ofGetHeight()/2, 20); // cos only
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
