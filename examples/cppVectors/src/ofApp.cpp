#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

}

//--------------------------------------------------------------
void ofApp::update(){

}

vector<string> myList;

//--------------------------------------------------------------
void ofApp::draw(){
  string myString;
  cout << "Enter a name or command" << endl;
  cin >> myString;
  
  if (myString == "list") {
    for(int i=0; i < myList.size(); i++) {
        cout << myList.at(i) << endl;  // or use regular syntax
    }
    
    return;
  }
  
  if (myString == "listEasy") {
    for(string name : myList) {
        cout << name << endl;
    }
    cout << endl;
    cout << "The first element is: " << myList.front() << endl;
    cout << "The last element is: " << myList.back() << endl;
    cout << "List length is: " << myList.size() << endl;
    
    return;
  }
  
   if (myString == "pop") {
    myList.pop_back();
    return;
  }
 
  if (myString == "rewrite") {
    myList.at(0) = "michael";
    return;
  }
 
  
  myList.push_back(myString);
  cout << "Done adding content." << endl;
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
