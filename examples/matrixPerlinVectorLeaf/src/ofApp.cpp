#include "ofApp.h"

vector<leaf> leaves;

leaf::leaf(){
  leafImg.load("leaf.png");
  
  // while(!leafImg.getWidth());  // loop here till loaded
  loc -= ofPoint(leafImg.getWidth()/2, leafImg.getHeight()/2);
  sTime = ofGetElapsedTimef();
  sWeight = ofRandom(0.1, 0.4);
  fallSpeed = ofRandom(2, 8);
}

void leaf::update(){
  float time = sTime + ofGetElapsedTimef();
  rX = ofSignedNoise(time * 0.5) * 400.0;  // rotate +- 400deg
  rY = ofSignedNoise(time * 0.3) * 400.0;
  rZ = ofSignedNoise(time * 0.9) * 400.0;
  dScale = (1 - ofNoise(time * 0.2)) * sWeight;
  //dScaleX = (1 - ofNoise(time * 0.2)) * sWeight;
  //dScaleY = (1 - ofNoise(time * 0.4)) * sWeight; 
  //dScaleZ = (1 - ofNoise(time * 0.6)) * sWeight; 
  
  fallWiggle = ofSignedNoise(time * 0.6) * ofRandom(20, 70);
  loc += ofPoint(fallWiggle, fallSpeed);
}

void leaf::draw() {
  ofPushMatrix();
    float z = ofSignedNoise((ofGetElapsedTimef() + sTime) * 0.9) * 400;
    ofTranslate(loc.x, loc.y, z);
    // ofScale(dScaleX, dScaleY, dScaleZ);  // can be 3 dimensional
    ofScale(dScale, dScale, dScale);
    ofRotateX(rX);
    ofRotateY(rY);
    ofRotateZ(rZ);
    leafImg.draw(0 - leafImg.getWidth()/2, 0 - leafImg.getHeight()/2);  // draw at new 0,0
  ofPopMatrix();
}

ofTrueTypeFont title;
ofImage bg;

void ofApp::add(ofPoint l){
  leaf newLeaf;
  newLeaf.loc = l;
  leaves.push_back(newLeaf);    // gets copied so newLeaf can go out of scope
}

void ofApp::setup(){
  title.load("baskvl.ttf", 60); // font setup
  bg.load("bg.jpg");
}

//--------------------------------------------------------------
void ofApp::update(){  
  for(int i; i < leaves.size(); i++) {
    leaves[i].update();
    if (leaves[i].loc.y > ofGetHeight()) {  // age it out
      leaves.erase(leaves.begin() + i);
    }
  }
}

//--------------------------------------------------------------
void ofApp::draw(){
  bg.draw(0,0);

  for(leaf l : leaves) {
    l.draw();
  }
  
  
  ofPushStyle();
  int fontFade = 0;  // alpha for title font
    if (ofGetFrameNum() <= 200){
      fontFade = ofMap(ofGetFrameNum(), 0,  200, 0, 255, true); // going up
    } else {
      fontFade = ofMap(ofGetFrameNum(), 200,  400, 255, 0, true); // going down
    }
    ofSetColor(0,0,0, fontFade);
    title.drawString("Windy Autumn", (ofGetWidth()/2)-500, (ofGetHeight()/2)-45);
  ofPopStyle();
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
  add(ofPoint(x, y));
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
