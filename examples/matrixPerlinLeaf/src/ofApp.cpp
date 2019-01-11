#include "ofApp.h"
ofImage leaf;

//--------------------------------------------------------------
void ofApp::setup(){
  leaf.load("leaf.png");
}

float rX = 0;
float rY = 0;
float rZ = 0;
float dScaleX = 1; // delta scale
float dScaleY = 1;
float dScaleZ = 1;
//--------------------------------------------------------------
void ofApp::update(){  
  float time = ofGetElapsedTimef();
  rX = ofSignedNoise(time * 0.5, time * 0.5, time * 0.5) * 400.0;  // rotate +- 400deg
  // https://www.reddit.com/r/gamedev/comments/4y23pn/how_do_i_generate_deterministic_noise_that/
  rY = ofSignedNoise(time * 0.3) * 400.0;
  rZ = ofSignedNoise(time * 0.9) * 400.0;
  dScaleX = 1 - ofNoise(time * 0.2);
  dScaleY = 1 - ofNoise(time * 0.4); 
  dScaleZ = 1 - ofNoise(time * 0.6); 
}

//--------------------------------------------------------------
void ofApp::draw(){
ofBackground(255);
 
ofPushMatrix();
  ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
  ofScale(dScaleX, dScaleY, dScaleZ);  // can be 3 dimensional
  ofRotateX(rX);
  ofRotateY(rY);
  ofRotateZ(rZ);
  leaf.draw(0 - leaf.getWidth()/2, 0 - leaf.getHeight()/2);  // draw at new 0,0
ofPopMatrix();
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
