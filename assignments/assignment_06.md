### Points: 5
### Assignment

Assemble at least one scene from your story with a title in a font of your choice.

Choose an active element (not static scenery).

Write the code necessary to move it with the arrow keys or mouse *dragging*.

Choose a second element that moves to a given x,y when the mouse button is *clicked*.

When key 'f' is pressed, toggle writing to a fading fbo instead of directly to the screen. 

#### Starter Code

```c
#include "testApp.h"  
  
ofPoint myMouse;  
  
//--------------------------------------------------------------  
void testApp::setup(){  
  
}  
  
//--------------------------------------------------------------  
void testApp::update(){  
  
}  
  
//--------------------------------------------------------------  
void testApp::draw(){  
  
	  
	ofSetColor(255, 0, 0);  
	ofCircle(myMouse,40);  
}  
  
//--------------------------------------------------------------  
void testApp::keyPressed(int key){  
  
	int step = 10;  
	if(key == 'w') myMouse.y = myMouse.y - step;  
	if(key == 's') myMouse.y = myMouse.y + step;  
	if(key == 'a') myMouse.x = myMouse.x - step;  
	if(key == 'd') myMouse.x = myMouse.x + step;  
	  
}  
  
//--------------------------------------------------------------  
void testApp::keyReleased(int key){  
  
}  
  
//--------------------------------------------------------------  
void testApp::mouseMoved(int x, int y ){  
  
}  
  
//--------------------------------------------------------------  
void testApp::mouseDragged(int x, int y, int button){  
  
	myMouse.x = x;  
	myMouse.y = y;  
}  
  
//--------------------------------------------------------------  
void testApp::mousePressed(int x, int y, int button){  
  
}  
  
//--------------------------------------------------------------  
void testApp::mouseReleased(int x, int y, int button){  
  
}  
  
//--------------------------------------------------------------  
void testApp::windowResized(int w, int h){  
  
}  
  
//--------------------------------------------------------------  
void testApp::gotMessage(ofMessage msg){  
  
}  
  
//--------------------------------------------------------------  
void testApp::dragEvent(ofDragInfo dragInfo){   
  
}  
```
Taken from: https://forum.openframeworks.cc/t/mouse-and-keyboard-control/9227

