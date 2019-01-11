#include "ofApp.h"

ofSoundPlayer boop;

//--------------------------------------------------------------
void ofApp::setup(){
  // load every sound you will need here
  boop.loadSound("boopBoop.wav");
}

int x = 100, y = 100;
int inc = 1;  // our increment will change!

//--------------------------------------------------------------
void ofApp::update(){
  
  // CLASSWORK: Comment out the "if" below after you run it.
  // Use ofGetWidth(), *=, and a logical "or" to make the ball 
  // change direction and play the sound when you hit a side.
  
  // How would you speed the ball up?
  
  if (x%500 == 0) {  // every 500th update, play sound
    boop.play();
  }
  
  x += inc; // add inc to x
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofSetColor(255,0,255);
  ofDrawCircle(x,y,50);
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
