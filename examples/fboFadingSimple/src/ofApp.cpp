#include "ofApp.h"

// Track mouse with fade

ofFbo fbo;  // framebuffer object
ofColor ball(100,0,200);  // purple
ofColor black(0,0,0);

//--------------------------------------------------------------
void ofApp::setup(){
  //fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);  // lower precision. leaves artifacts. way faster.
  fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F); // higher precision alpha (no artifacts)
  
  fbo.begin();
    ofClear(255,255,255, 0);
  fbo.end();
  
  ofSetBackgroundColor(black);
}

//--------------------------------------------------------------
void ofApp::update(){
}

int mouseX = 0;
int mouseY = 0;

//--------------------------------------------------------------
void ofApp::draw(){
  fbo.begin();
    ofSetColor(black, 10); // background color with alpha 
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(ball);
    ofNoFill();
    ofDrawCircle(mouseX, mouseY, 50);
    ofFill();
  fbo.end();
  
  ofSetColor(255,255,255);  // always set the color before drawing the fbo
  fbo.draw(0,0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
  mouseX = x;
  mouseY = y;
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
