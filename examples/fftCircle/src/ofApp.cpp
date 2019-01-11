#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	 

	song.load("song.mp3");
	while (! song.isLoaded()); // spin wheels till loaded
	song.play();
	nBandsToGet = 512;  // up to 512
}

float *val;
//--------------------------------------------------------------
void ofApp::update(){
	
  ofBackground(0x23,0x32,0x37);

  // update the sound playing system:
  // ofSoundUpdate();	
	
  val = ofSoundGetSpectrum(nBandsToGet);  // get array of floats (1 for each band)
}


float getX (int deg, int radius) {
  float x = radius * cos(ofDegToRad(deg)) + ofGetWidth()/2;
  return (x);
}

float getY (int deg, int radius) {
  float y = radius * sin(ofDegToRad(deg)) + ofGetHeight()/2;
  return (y);
}

//--------------------------------------------------------------
void ofApp::draw(){
  int r = 100;  // radius
  
  // draw the fft results:
  ofSetColor(0x98,0x4b,0x43,255);
	
  // cos & sin take RADIANS.  
  // 2pi radians would be 2 x 57.2958 x 3.1416 = 360 degrees
  // Therefore to draw a whole circle you would use 0-TWO_PI to draw a whole circle 
  
  for (int i = 0;i < nBandsToGet; i++){
    float x1 = getX(i, r);
    float y1 = getY(i, r);
    float x2 = getX(i, r + 2000 * val[i/8]);  // up to 2k larger (theoretically) but it will never get even close
    float y2 = getY(i, r + 2000 * val[i/8]);
    ofDrawLine (x1, y1, x2, y2);  // we only care about bottom 8th of the spectrum
  }

}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
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

