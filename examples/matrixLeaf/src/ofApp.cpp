#include "ofApp.h"
ofImage leaf;

//--------------------------------------------------------------
void ofApp::setup(){
  leaf.load("leaf.png");
}

float rX = 0;
float rY = 0;
float rZ = 0;
//--------------------------------------------------------------
void ofApp::update(){
  rX++;
  if (ofGetFrameNum() % 3 == 0)
    rY++;
  if (ofGetFrameNum() % 7 == 0)
    rZ++;
}

//--------------------------------------------------------------
void ofApp::draw(){
ofBackground(255);
 
ofPushMatrix();
  ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
  ofScale(0.3, 0.3, 0.3);  // can be 3 dimensional
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
