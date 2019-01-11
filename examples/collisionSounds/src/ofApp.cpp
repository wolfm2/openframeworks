#include "ofApp.h"
#include <iostream>
#include <stdio.h>

const int numParticles = 20;
ofFbo fbo;
particle myParticle[numParticles];

ofShader shaderBlur;

// present private internal data in a safe way
int particle::getX() {
	return (pos.x);
}

int particle::getY() {
	return (pos.y);
}

int particle::getR() {
	return (rad);
}

particle::particle(){
	vel=ofPoint(ofRandom(-4,4), ofRandom(-4,4), 0);  // set up our velocity
}

void particle::setup(int idx){
	pos=ofPoint(ofGetWidth()/2, ofGetHeight()/2);  // start from center
	color.setHsb((255 / numParticles) * idx,255,255);  // set color
}

void particle::update(){
	
	pos += vel;  // update position
	
	// if you hit any of the four walls, bounce!
	if (pos.x > ofGetWidth()) { 
		pos.x = ofGetWidth();
		vel.x *= -1;
		hit();
	}
	
	if (pos.x < 0) {
		pos.x = 0;
		vel.x *= -1;
		hit();
	}
	
	if (pos.y > ofGetHeight()) {
		pos.y = ofGetHeight();
		vel.y *= -1;
		hit();
	}
	
	if (pos.y < 0) {
		pos.y = 0;
		vel.y *= -1;
		hit();
	}  

	// did our latest movement cause a collision?
	distanceSensor();
}

void particle::distanceSensor () {
	int x, y, r;  // foreign particle
	bool curCol = false; // any collisions at all? 
	
	for (int i = 0; i < numParticles; i++) {
		
		if (&myParticle[i] == this)  // comparing against ourselves, skip
		  continue;
		 
		// Get the other particles relevant info
		x = myParticle[i].getX();
		y = myParticle[i].getY();
		r = myParticle[i].getR();
		
		float pxDistance = sqrt(pow((x-pos.x),2) + pow((pos.y-y),2)) - (r+rad);
		
		if (pxDistance < 50) {
			fbo.begin();
			
			ofSetColor(ofColor::gray);
			ofDrawLine(pos.x, pos.y, x, y);
    
			fbo.end();
		}
		
		if (pxDistance <= 0) {  // if current particle is overlapping w another
			curCol = true;
		}
	}
	
	if (curCol == false) {  // if none detected reset inCollision var
		inCol = false;
	}
	
	if (inCol == false && curCol) { 
		fbo.begin();
		ofSetColor(ofColor::red);
		ofDrawCircle(pos,rad);
		fbo.end();
		
		vel.x *= -1; // bounce x
		vel.y *= -1; // bounce y
		pos += vel;  // apply it and hope for the best
		
		inCol = true;
	}
}

// run every time there is a collision with a wall
void particle::hit(){
	doTone = true;  // set up a tone for next metronome tick
	
	fbo.begin();  // start writing to the FRAMEBUFFER
	ofSetColor(ofColor::gray);
	// draw a line visualizing where we hit
	if (pos.x == ofGetWidth() || pos.x == 0)
	  ofDrawLine(0, pos.y, ofGetWidth(), pos.y);
	else
	  ofDrawLine(pos.x, 0, pos.x, ofGetHeight());
	fbo.end();  // stop framebuffer
}

void particle::draw() {
	
#if 1
	ofSetColor(color); // happy dots
	ofDrawCircle(pos, rad);
#else
	ofSetColor(ofColor::brown); // spookier dots
	ofDrawLine(pos.x + ofRandom(-9, 10), pos.y + ofRandom(-9, 10), pos.x + ofRandom(-9, 10), pos.y + ofRandom(-9, 10));
#endif

}

const int fr = 60;  // frame rate
int frCount = 0;	// count of current framerate

//--------------------------------------------------------------
ofSoundPlayer metronome;
ofTrueTypeFont myfont;

void ofApp::setup(){
	char fName[20];  // sound file name buffer
	
	shaderBlur.load("shadersGL3/shaderBlurX");
	//shaderBlur.load("shadersES2/shaderBlurY");

	
	ofBackground(0,0,0);  // set bg to black
	ofSetFrameRate(fr);
	myfont.loadFont("arial.ttf", 12);
	metronome.loadSound("0.wav");  // plays every tick
	metronome.setVolume(1);
	
	// set up sound for all particles
	for (int i = 0; i < numParticles; i++) {
		myParticle[i].setup(i);
		sprintf (fName, "%d.wav", i);
		myParticle[i].tone.loadSound(fName);
		myParticle[i].tone.setVolume(.5);
	}
	
	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);  // get space to draw on
	fbo.begin();
	ofClear(255,255,255,0);  // clear GPU memory
	fbo.end();
}

//--------------------------------------------------------------
void ofApp::update(){
	// call all the updates
	for (int i = 0; i < numParticles; i++) {
		myParticle[i].update();
	}
	
	if (frCount-- <= 0) {  // only play tones on a meter
		frCount = fr / 4;  // set tempo here: currently every .25 second
		for (int i = 0; i < numParticles; i++) {
			if (myParticle[i].doTone) {  // if this particle hit a wall
				myParticle[i].tone.play();
				myParticle[i].doTone = false;
			}
		}
		metronome.play();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	fbo.begin();
//	ofEnableAlphaBlending();  // do fading of primitives
	
	//ofSetColor(ofColor::black, 5);	// what is the difference between these two? 
	ofSetColor(255,255,255, 15);	// floating point errors/artifacts can look cool!

	ofFill(); // defaults to fill now
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());	
	
//	ofDisableAlphaBlending();
	
	fbo.end();
	
	shaderBlur.begin();
	shaderBlur.setUniform1f("blurAmnt", 9);

	//fSetColor(255,255,255); // draw fbo, all colors, totally opaque
	ofSetColor(ofColor::white);
	fbo.draw(0,0); // draw fbo on our screen
	
	shaderBlur.end();
	
	ofSetColor(ofColor::purple);
	myfont.drawString("Cobwebs: Algorithmically Generated Music & Visuals", 10,20);
	
	for (int i = 0; i < numParticles; i++) {  // draw all the particles over the fbo
		myParticle[i].draw();
	}
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
