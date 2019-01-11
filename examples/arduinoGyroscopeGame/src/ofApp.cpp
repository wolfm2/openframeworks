#include "ofApp.h"
#include <string>


// sounds:
// Setuniman Everyday 1D16
// PaulMorek Nom A
// frosthardr balloon
// Robinhood76 02098 cartoon laughter

ofTrueTypeFont myfont;
ofFbo fbo;

string staging;  // buffer for creating new strings
string actual;   // actual buffer to print

int w, h, X, Y, Z, midX, midY, avatarX, avatarY, avatarZ = 10;

ofSerial serial;

void testApp::setup(){
  
  ofSetFrameRate( 60 );

  w = ofGetWidth();
  h = ofGetHeight();
  //fbo.allocate(w, h);

  myfont.loadFont("arial.ttf", 12);

  midX = ofGetWidth()/2;
  midY = ofGetHeight()/2;

  avatarX = midX; // start the circle in the middle
  avatarY = midY;

  // setting up the serial device
  //serial.setup("/dev/ttyACM0", 9600); // load a specific serial port when not on RPi
  serial.setup(0, 9600); //open device 0 @ 9600 baud
  
  gyroNom.loadImage("gyro.png");  // 2nd PART 1 of 4
  
  
  // Load Sounds
  char fName[20];
  for (int i = 0; i < numSounds; i++) {
    sprintf (fName, "%d.wav", i);
    mySound[i].loadSound(fName);
  }
  
  backgroundMusic.loadSound("background.wav");
  laugh.loadSound("laugh.wav");
  stretch.loadSound("stretch.wav");
  stretch.setVolume(.2);  // make the stretch sound quieter than NOM NOM NOM
  
  while (!backgroundMusic.isLoaded());  // block till loaded
  backgroundMusic.setLoop(true);
  backgroundMusic.play();
}

int parseInt (string del, string str) {

  std::size_t found = str.find(del);
  int rv = atoi(str.c_str() + found + del.length());
  return (rv);
}

void testApp::update(){

  // 2nd PART 2 of 4
  for (int i = 0; i < numNPPoints; i++) { // update all the points
    NPPUpdate(i);
  }
  
  
  // every time we update, look for a character
  // bug: should read all chars in buffer
  int ttl = 100; // read x chars max

  char ch;
  while ((ch=serial.readByte())>0 && ttl-->0 && ch != '\n') {
    staging += ch;
  }

  if (ch == '\n') { // if we are done, create string to print

    // parse gyro input by delimiters

    // actual = string(buffer);  // uncomment this if you want to save the raw string for more processing

    X = parseInt("X:", staging); // relative value of x 
    Y = parseInt("Y:", staging); // relative value of y 
    Z = parseInt("Z:", staging); // relative value of z
    
    char buffer[300];
    sprintf(buffer, "%d %d %d", X, Y, Z);

    avatarX += X;
    avatarY += Y + 3;  // my Y is -3 at rest... so normalize it here!
    
    if (Z > 250) { // doing excercise!
      avatarZ = 10; 
      laugh.play();
    }
    
    avatarX = ofClamp(avatarX, 0, ofGetWidth());
    avatarY = ofClamp(avatarY, 0, ofGetHeight());
    staging = "";

  }

  // 2nd PART 3 of 4
  for (int i = 0; i < numNPPoints; i++) { // check for collisions
  
    if (NPPCollisionDetection(i)) { 
      // printf("boop! I hit you...\n");
      
      NPPAlive[i] = false;  // food is eaten... forever gone
      avatarZ += 10;         // make avatar bigger
      avatarZ = ofClamp(avatarZ, 10, 300);  
      mySound[int(ofRandom(0,9))].play();
      stretch.play();
    }
  } 
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

    avatarX = x;
    avatarY = y;

}

void testApp::keyPressed(int key){
  if (key == 'z') { // weight reduction
    avatarZ = 10;
    laugh.play();
  }
}

void testApp::draw(){
  
  // draw background
  ofColor light = ofColor(128,128,150);
  ofColor dark = ofColor(220,220,250);
  ofBackgroundGradient(light, dark, OF_GRADIENT_LINEAR);

  // bug? for some reason changing colors in fbo does not work
  //fbo.begin();  // in draw:
  //ofClear(255,255,255, 0); // clear prevous stuff
  
  ofSetColor(0, 0, 255); // blue font
  myfont.drawString("Sky Gyros: The GAME!! \n\n"
                    "As you can see, after the rainy Monday I had gyros \n"
                    "on my mind and I ate waaaaaay to much.  The game \n"
                    "can be played with a mouse and 'Z' key for excercise, \n"
                    "or via the Arduino with attached gyro.  Moving the \n"
                    "gyro's X and Y move your avatar, and dramatically \n"
                    "twisting in Z does excercise.\n\n"
                    "There is no winning or losing... Tasty gyros make everyone a winner!", 10,20);
  myfont.drawString(actual, midX, midY);

  ofSetColor(255,0,0);
  
  ofCircle (avatarX, avatarY, avatarZ);
  
  // 2nd PART 4 of 4
  for (int i = 0; i < numNPPoints; i++) { // draw all the points
    NPPDraw(i); 
  }
  
  
  //fbo.end();

  //fbo.draw( 0, 0 );
}
