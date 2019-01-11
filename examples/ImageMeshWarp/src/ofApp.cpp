#include "ofApp.h"

ofImage img;
ofMesh mesh;

//--------------------------------------------------------------
void ofApp::setup(){
    img.load("check.jpg");
    //img.allocate(256, 256, OF_IMAGE_COLOR);
    //int i = 0;
    //while ( i < img.getPixels().size() ) {
    //    img.getPixels()[i] = abs(sin( float(i) / 18.f )) * 255.f;
    //    i++;
    //}
    //img.reloadTexture();

    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

    mesh.addVertex(ofVec2f(10, 10));
    mesh.addVertex(ofVec2f(410, 10));
    mesh.addVertex(ofVec2f(410, 410));
    mesh.addVertex(ofVec2f(10, 410));
    mesh.addVertex(ofVec2f(10, 10));

    mesh.addTexCoord(ofVec2f(0, 0));
    mesh.addTexCoord(ofVec2f(256, 0));
    mesh.addTexCoord(ofVec2f(256, 256));
    mesh.addTexCoord(ofVec2f(0, 256));
    mesh.addTexCoord(ofVec2f(0, 0));
}

int x= 410;

//--------------------------------------------------------------
void ofApp::update(){
  mesh.setVertex(2, ofVec2f(x, 410));
  mesh.setVertex(3, ofVec2f(x++ - 400, 410)); 
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(255, 255, 255); // background white

    img.bind();
    mesh.draw();
    img.unbind();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
x = 410;

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
