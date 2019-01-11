#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){	 

	song.load("song.mp3");
	while (! song.isLoaded()); // spin wheels till loaded
	song.play();
	nBandsToGet = 360;  // up to 512
}

float *val;
//--------------------------------------------------------------
void ofApp::update(){
	
	ofBackground(0,82,165);

	// update the sound playing system:
	// ofSoundUpdate();	
	

	
	val = ofSoundGetSpectrum(nBandsToGet);  // get array of floats (1 for each band)
}

//--------------------------------------------------------------
void ofApp::draw(){

	
	// draw the fft resutls:
	ofSetColor(0,197,144,255);
	
	for (int i = 0;i < nBandsToGet; i++){
		// (we use negative height here, because we want to flip them
		// because the top corner is 0,0)
		ofDrawRectangle(100+i*2,ofGetHeight()-100,2,(-1 * val[i]) * 8000);
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

