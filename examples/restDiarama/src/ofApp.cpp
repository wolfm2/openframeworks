#include "ofApp.h" 
#include "ofxJSON.h"

ofxJSONElement json;	// JSON reader
ofColor dayFade;	// an alpha box we draw over the landscape to simulate night
int dfAlpha;		// dayfade opacity

#define TYPE_CBODY 0
#define TYPE_CLOUD 1
#define TYPE_ANIMAL 2

// simple constructor
actor::actor(){  
  active=false;
}  

void actor::setup(int idx){
  
  // 1 celestial body, 6 clouds, the rest = animals
  if (idx == 0)
    type = TYPE_CBODY;
  else if (idx < 6)
    type = TYPE_CLOUD;
  else {
    type = TYPE_ANIMAL;
  }
  
  top = true;		// everything but animals
  day = false;

  plank.load("images/plank.png");
  
}

void actor::activate() {

  player.load("sound/ratchet.mp3");  // play this sound when plank enters viewport
  player.setVolume(0.6f);
  player.play();
  
  if (type == TYPE_CBODY) {	// set specific icon and looping sound for day or night
    if (day) {
      player.load("sound/day.mp3");
      icon.load("images/sun.png");
    } else {
      player.load("sound/night.mp3");
      icon.load("images/moon.png");
    }
    player.setLoop(true);
    player.play(); 
    
    xDest = 100;
    left = true;
  }
  
  // c++ always rounds down so we do this trick for a boolean    
  if (type == TYPE_ANIMAL) {  
    top = ((int)ofRandom(0,10) % 2);	  // animals can come from top or bottom
    left = ((int)ofRandom(0,10) % 2);		// moving left to right
  }
  
  height = ofRandom(-50, 50);  // plank height
  
  if (top)			// where to draw the pic
    pos.y = -350 + height;
  else
    pos.y = ofGetHeight() - 150 + height;
    
  if (left)			// which side to start the pic
    pos.x =  -10;
  else
    pos.x = ofGetWidth() + 10;
  
  if (type == TYPE_ANIMAL) {	// Animals move.  Set end destination for x axis
   
    string fName;  		// file name of animal icon
    int animal = ofRandom(0,3);
    if (day)
      fName += "d";
    else
      fName += "n";
    
    if (top)
      fName += "t";
    else
      fName += "b";
    
    fName += "" + to_string(animal) + ".png";
    icon.load("images/animals/" + fName);	// load random animal
    // TODO add sounds for each animal
  
    if (left) {
      xDest = pos.x + ofRandom(20, 300);	// destination to zeno to
    } else {
      icon.mirror(false, true);			// face the right direction
      xDest = pos.x + ofRandom(-300, -20);
    }
  }
  
  active=true;
}

#define SEC_IN_DAY 86164
void actor::update(int time, int sunrise, int sunset){  

  // Update day
  day = false;	
  if (time >= sunrise && time <= sunset) {
    day = true;
  } 
  
  if (!active)
    return;
  
  float weight = ofRandom(.001, .1);  // randomize how fast we ease 
  pos.x += (xDest - pos.x) * weight;  // ease to destination
  
  if (type == TYPE_ANIMAL) {  		// when dest achieved...
    if (left && (pos.x >= xDest - 1)) {	// set new dest moving right
      xDest = pos.x + ofRandom(20, 300);
    }
    if (!left && (pos.x <= xDest + 1)) { // set new dest moving left
      xDest = pos.x + ofRandom(-300, -20);
    }
  }
  
  // if offscreen, deactivate
  if (pos.x <= -200 || pos.x >= ofGetWidth() + 200)
    active = false;
}  

void actor::draw(){
  if (!active)
    return;
    
  plank.draw(pos);
  
  ofPoint iPos;	// icon position
  iPos = pos;	// plank position
  
  if (top) {  // set icon atop plank
    iPos.x -= icon.getWidth()/2;
    iPos.y += plank.getHeight() - icon.getHeight()/2;
  } else {
    iPos.x -= icon.getWidth()/2;
    iPos.y -= icon.getHeight()/2;
  }
    
  icon.draw(iPos);  // draw icon
}  



#define MAX_ACT 14
#define ONE_HRS  3600
#define SIX_HRS 21600

actor a[MAX_ACT];

ofSoundPlayer background;
ofImage landscape;
ofTrueTypeFont title;

int curTime = 1487358400;  // default current time before we JSON sync 2/17/2017, 2:06:40 PM
int sunrise = 1487343485;  // TODO do rooster sound at sunrise?
int sunset =  1487382729;  // TODO do wolf sound at sunset?

//--------------------------------------------------------------  
void ofApp::setup(){  
    ofRegisterURLNotification(this);  
    cout << "starting\n";
    
    ofSetFrameRate(30);
    
    for (int idx=0; idx < MAX_ACT; idx++)
      a[idx].setup(idx);
      
    background.load("sound/ticktock.mp3");
    background.setLoop(true);
    background.play();
    landscape.load("images/forest.jpg");
    
    // init dayFade alpha with clamping
    dfAlpha = ofMap(curTime, sunset - ONE_HRS, sunset + ONE_HRS, 0, 230, true);
    title.load("baskvl.ttf", 60); // font setup
}  


void ofApp::urlResponse(ofHttpResponse & response){ 
	if(response.status==200 ){  
		cout << response.request.name << endl;  
        	cout << response.data.getText() << endl;  
        	
        	if (json.parse(response.data.getText())) {  // parse url returns T/F
        	  sunrise = json["daily"]["data"][0]["sunriseTime"].asInt();  
        	  sunset = json["daily"]["data"][0]["sunsetTime"].asInt();
        	  curTime = json["currently"]["time"].asInt();
        	  
        	  cout << "Set sunrise epoch to: " << sunrise << endl;
        	  cout << "Set sunset epoch to : " << sunset << endl;
        	  cout << "Set curtime epoch to: " << curTime << endl;
        	  
        	} else {
        	  cout << "Parsing Failed." << endl;
        	}
        	a[0].active = false;
	}else{  
		cout << response.status << " " << response.error << endl;  
	}  
}  


//--------------------------------------------------------------  
void ofApp::update(){  

  for (int idx=0; idx < MAX_ACT; idx++)
    a[idx].update(curTime, sunrise, sunset);
    
  if (!a[0].active) { // start sun / moon
    cout << "started cbody" << endl;
    a[0].activate();
  }
  
  // random animal respawn    
  for (int idx=6; idx < MAX_ACT; idx++) {
    int respawn = (int)ofRandom(0,400);
    if (!a[idx].active && respawn == 0)
      a[idx].activate();
  }
}  

//--------------------------------------------------------------  
void ofApp::draw(){  
  landscape.draw(0, 0, ofGetWidth(), ofGetHeight());
  
  dfAlpha = ofMap(curTime, sunset - ONE_HRS, sunset + ONE_HRS, 0, 230, true); // set dayFade alpha with clamping
  dayFade = ofColor(33, 38, 58, dfAlpha);  // midnight blue / grey
  ofSetColor(dayFade);
  ofDrawRectangle(0,0,ofGetWidth(),ofGetHeight());
  
  ofSetColor(255);  // DO THIS BEFORE DRAWING IMGs
  for (int idx=0; idx < MAX_ACT; idx++)
    a[idx].draw();
  
  int fontFade = 0;  // alpha for title font
  if (ofGetFrameNum() <= 120){
    fontFade = ofMap(ofGetFrameNum(), 0,  120, 0, 255, true); // going up
  } else {
    fontFade = ofMap(ofGetFrameNum(), 120,  240, 255, 0, true); // going down
  }
  ofSetColor(255,255,255, fontFade);
  title.drawString("Clockwork DiaRama", (ofGetWidth()/2)-500, (ofGetHeight()/2)-45);
  ofSetColor(255);
}  

//--------------------------------------------------------------  
void ofApp::keyPressed(int key){  

    if (key == '1')
      curTime -= 600;  // minus 10 min  
    if (key == '2')
      curTime += 600;  // add 10 min
    if (key == '3') {    // sync time with json weather data
      // NYC LAT/LONG == 40.7128 N, 74.0059
      string url = "https://api.darksky.net/forecast/9189df2d918db076965b80892e8b0256/40.7128,-74.0059";
      ofLoadURLAsync(url, "async_req"); 
    }
      
    bool day = a[0].day; // save previous value
    
    a[0].update(curTime, sunrise, sunset);
    if (day != a[0].day)  // if you trip over sunset, re-init celestial body
      a[0].active = false;
      
    if (curTime < sunrise)  // impose early limit
      curTime = sunrise;
      
    if (curTime > sunset + SIX_HRS)  // impose late limit
      curTime = sunset + SIX_HRS;
      
    cout << "Current epoch time is: " << curTime << endl;
    
    // TODO implement rain(with transparent lines), night stars (with circles), clouds, wind
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
void ofApp::windowResized(int w, int h){  

}  

//--------------------------------------------------------------  
void ofApp::gotMessage(ofMessage msg){  

}  

//--------------------------------------------------------------  
void ofApp::dragEvent(ofDragInfo dragInfo){   

}  

/*

{
  "latitude": 37.8267,
  "longitude": -122.4233,
  "timezone": "America\/Los_Angeles",
  "offset": -8,
  "currently": {
    "time": 1487366701,
    "summary": "Light Rain",
    "icon": "rain",
    "nearestStormDistance": 0,
    "precipIntensity": 0.0135,
    "precipIntensityError": 0.0013,
    "precipProbability": 1,
    "precipType": "rain",
    "temperature": 56.22,
    "apparentTemperature": 56.22,
    "dewPoint": 53.34,
    "humidity": 0.9,
    "windSpeed": 15.45,
    "windBearing": 163,
    "visibility": 6.28,
    "cloudCover": 1,
    "pressure": 987.86,
    "ozone": 344.13
  },
  "minutely": {
    "summary": "Light rain stopping in 15 min.",
    "icon": "rain",
    "data": [
      {
        "time": 1487366700,
        "precipIntensity": 0.0135,
        "precipIntensityError": 0.0013,
        "precipProbability": 1,
        "precipType": "rain"
      },
      {
        "time": 1487366760,
        "precipIntensity": 0.0142,
        "precipIntensityError": 0.0017,
        "precipProbability": 1,
        "precipType": "rain"
      },
      {
        "time": 1487366820,
        "precipIntensity": 0.0136,
        "precipIntensityError": 0.0015,
        "precipProbability": 1,
        "precipType": "rain"
      },
      {
        "time": 1487366880,
        "precipIntensity": 0.0134,
        "precipIntensityError": 0.0017,
        "precipProbability": 1,
        "precipType": "rain"
      },
      {
        "time": 1487366940,
        "precipIntensity": 0.0136,
        "precipIntensityError": 0.0019,
        "precipProbability": 1,
        "precipType": "rain"
      },
      {
        "time": 1487367000,
        "precipIntensity": 0.013,
        "precipIntensityError": 0.0019,
        "precipProbability": 1,
        "precipType": "rain"
      },
      {
        "time": 1487367060,
        "precipIntensity": 0.0128,
        "precipIntensityError": 0.0021,
        "precipProbability": 1,
        "precipType": "rain"
      },
      {
        "time": 1487367120,
        "precipIntensity": 0.0118,
        "precipIntensityError": 0.0023,
        "precipProbability": 1,
        "precipType": "rain"
      },
      {
        "time": 1487367180,
        "precipIntensity": 0.0112,
        "precipIntensityError": 0.0025,
        "precipProbability": 1,
        "precipType": "rain"
      },
      {
        "time": 1487367240,
        "precipIntensity": 0.0099,
        "precipIntensityError": 0.0027,
        "precipProbability": 0.98,
        "precipType": "rain"
      },
      {
        "time": 1487367300,
        "precipIntensity": 0.0088,
        "precipIntensityError": 0.0028,
        "precipProbability": 0.91,
        "precipType": "rain"
      },
      {
        "time": 1487367360,
        "precipIntensity": 0.0079,
        "precipIntensityError": 0.0028,
        "precipProbability": 0.77,
        "precipType": "rain"
      },
      {
        "time": 1487367420,
        "precipIntensity": 0.0074,
        "precipIntensityError": 0.0028,
        "precipProbability": 0.67,
        "precipType": "rain"
      },
      {
        "time": 1487367480,
        "precipIntensity": 0.0069,
        "precipIntensityError": 0.0027,
        "precipProbability": 0.59,
        "precipType": "rain"
      },
      {
        "time": 1487367540,
        "precipIntensity": 0.0063,
        "precipIntensityError": 0.0027,
        "precipProbability": 0.48,
        "precipType": "rain"
      },
      {
        "time": 1487367600,
        "precipIntensity": 0.006,
        "precipIntensityError": 0.0027,
        "precipProbability": 0.43,
        "precipType": "rain"
      },
      {
        "time": 1487367660,
        "precipIntensity": 0.0056,
        "precipIntensityError": 0.0026,
        "precipProbability": 0.37,
        "precipType": "rain"
      },
      {
        "time": 1487367720,
        "precipIntensity": 0.0053,
        "precipIntensityError": 0.0026,
        "precipProbability": 0.33,
        "precipType": "rain"
      },
      {
        "time": 1487367780,
        "precipIntensity": 0.0052,
        "precipIntensityError": 0.0026,
        "precipProbability": 0.3,
        "precipType": "rain"
      },
      {
        "time": 1487367840,
        "precipIntensity": 0.0049,
        "precipIntensityError": 0.0025,
        "precipProbability": 0.27,
        "precipType": "rain"
      },
      {
        "time": 1487367900,
        "precipIntensity": 0.0047,
        "precipIntensityError": 0.0024,
        "precipProbability": 0.25,
        "precipType": "rain"
      },
      {
        "time": 1487367960,
        "precipIntensity": 0.0045,
        "precipIntensityError": 0.0024,
        "precipProbability": 0.24,
        "precipType": "rain"
      },
      {
        "time": 1487368020,
        "precipIntensity": 0.0044,
        "precipIntensityError": 0.0024,
        "precipProbability": 0.22,
        "precipType": "rain"
      },
      {
        "time": 1487368080,
        "precipIntensity": 0.0044,
        "precipIntensityError": 0.0024,
        "precipProbability": 0.22,
        "precipType": "rain"
      },
      {
        "time": 1487368140,
        "precipIntensity": 0.0043,
        "precipIntensityError": 0.0023,
        "precipProbability": 0.2,
        "precipType": "rain"
      },
      {
        "time": 1487368200,
        "precipIntensity": 0.0043,
        "precipIntensityError": 0.0023,
        "precipProbability": 0.2,
        "precipType": "rain"
      },
      {
        "time": 1487368260,
        "precipIntensity": 0.0043,
        "precipIntensityError": 0.0023,
        "precipProbability": 0.19,
        "precipType": "rain"
      },
      {
        "time": 1487368320,
        "precipIntensity": 0.0043,
        "precipIntensityError": 0.0023,
        "precipProbability": 0.19,
        "precipType": "rain"
      },
      {
        "time": 1487368380,
        "precipIntensity": 0.0043,
        "precipIntensityError": 0.0023,
        "precipProbability": 0.18,
        "precipType": "rain"
      },
      {
        "time": 1487368440,
        "precipIntensity": 0.0043,
        "precipIntensityError": 0.0023,
        "precipProbability": 0.17,
        "precipType": "rain"
      },
      {
        "time": 1487368500,
        "precipIntensity": 0.0043,
        "precipIntensityError": 0.0023,
        "precipProbability": 0.17,
        "precipType": "rain"
      },
      {
        "time": 1487368560,
        "precipIntensity": 0.0043,
        "precipIntensityError": 0.0023,
        "precipProbability": 0.16,
        "precipType": "rain"
      },
      {
        "time": 1487368620,
        "precipIntensity": 0.0044,
        "precipIntensityError": 0.0023,
        "precipProbability": 0.16,
        "precipType": "rain"
      },
      {
        "time": 1487368680,
        "precipIntensity": 0.0044,
        "precipIntensityError": 0.0023,
        "precipProbability": 0.16,
        "precipType": "rain"
      },
      {
        "time": 1487368740,
        "precipIntensity": 0.0044,
        "precipIntensityError": 0.0023,
        "precipProbability": 0.15,
        "precipType": "rain"
      },
      {
        "time": 1487368800,
        "precipIntensity": 0.0045,
        "precipIntensityError": 0.0023,
        "precipProbability": 0.15,
        "precipType": "rain"
      },
      {
        "time": 1487368860,
        "precipIntensity": 0.0045,
        "precipIntensityError": 0.0023,
        "precipProbability": 0.14,
        "precipType": "rain"
      },
      {
        "time": 1487368920,
        "precipIntensity": 0.0046,
        "precipIntensityError": 0.0023,
        "precipProbability": 0.14,
        "precipType": "rain"
      },
      {
        "time": 1487368980,
        "precipIntensity": 0.0045,
        "precipIntensityError": 0.0024,
        "precipProbability": 0.13,
        "precipType": "rain"
      },
      {
        "time": 1487369040,
        "precipIntensity": 0.0046,
        "precipIntensityError": 0.0024,
        "precipProbability": 0.13,
        "precipType": "rain"
      },
      {
        "time": 1487369100,
        "precipIntensity": 0.0047,
        "precipIntensityError": 0.0024,
        "precipProbability": 0.13,
        "precipType": "rain"
      },
      {
        "time": 1487369160,
        "precipIntensity": 0.0048,
        "precipIntensityError": 0.0024,
        "precipProbability": 0.14,
        "precipType": "rain"
      },
      {
        "time": 1487369220,
        "precipIntensity": 0.0049,
        "precipIntensityError": 0.0025,
        "precipProbability": 0.14,
        "precipType": "rain"
      },
      {
        "time": 1487369280,
        "precipIntensity": 0.005,
        "precipIntensityError": 0.0025,
        "precipProbability": 0.14,
        "precipType": "rain"
      },
      {
        "time": 1487369340,
        "precipIntensity": 0.0051,
        "precipIntensityError": 0.0026,
        "precipProbability": 0.14,
        "precipType": "rain"
      },
      {
        "time": 1487369400,
        "precipIntensity": 0.0052,
        "precipIntensityError": 0.0026,
        "precipProbability": 0.14,
        "precipType": "rain"
      },
      {
        "time": 1487369460,
        "precipIntensity": 0.0054,
        "precipIntensityError": 0.0027,
        "precipProbability": 0.15,
        "precipType": "rain"
      },
      {
        "time": 1487369520,
        "precipIntensity": 0.0055,
        "precipIntensityError": 0.0027,
        "precipProbability": 0.15,
        "precipType": "rain"
      },
      {
        "time": 1487369580,
        "precipIntensity": 0.0055,
        "precipIntensityError": 0.0027,
        "precipProbability": 0.15,
        "precipType": "rain"
      },
      {
        "time": 1487369640,
        "precipIntensity": 0.0057,
        "precipIntensityError": 0.0028,
        "precipProbability": 0.16,
        "precipType": "rain"
      },
      {
        "time": 1487369700,
        "precipIntensity": 0.0058,
        "precipIntensityError": 0.0028,
        "precipProbability": 0.16,
        "precipType": "rain"
      },
      {
        "time": 1487369760,
        "precipIntensity": 0.0059,
        "precipIntensityError": 0.0029,
        "precipProbability": 0.17,
        "precipType": "rain"
      },
      {
        "time": 1487369820,
        "precipIntensity": 0.0061,
        "precipIntensityError": 0.0029,
        "precipProbability": 0.18,
        "precipType": "rain"
      },
      {
        "time": 1487369880,
        "precipIntensity": 0.0062,
        "precipIntensityError": 0.0029,
        "precipProbability": 0.19,
        "precipType": "rain"
      },
      {
        "time": 1487369940,
        "precipIntensity": 0.0063,
        "precipIntensityError": 0.003,
        "precipProbability": 0.2,
        "precipType": "rain"
      },
      {
        "time": 1487370000,
        "precipIntensity": 0.0064,
        "precipIntensityError": 0.003,
        "precipProbability": 0.2,
        "precipType": "rain"
      },
      {
        "time": 1487370060,
        "precipIntensity": 0.0065,
        "precipIntensityError": 0.003,
        "precipProbability": 0.21,
        "precipType": "rain"
      },
      {
        "time": 1487370120,
        "precipIntensity": 0.0066,
        "precipIntensityError": 0.003,
        "precipProbability": 0.22,
        "precipType": "rain"
      },
      {
        "time": 1487370180,
        "precipIntensity": 0.0066,
        "precipIntensityError": 0.003,
        "precipProbability": 0.22,
        "precipType": "rain"
      },
      {
        "time": 1487370240,
        "precipIntensity": 0.0067,
        "precipIntensityError": 0.003,
        "precipProbability": 0.23,
        "precipType": "rain"
      },
      {
        "time": 1487370300,
        "precipIntensity": 0.0068,
        "precipIntensityError": 0.0031,
        "precipProbability": 0.24,
        "precipType": "rain"
      }
    ]
  },
  "hourly": {
    "summary": "Rain until this evening, starting again tonight, and breezy starting tomorrow morning.",
    "icon": "rain",
    "data": [
      {
        "time": 1487365200,
        "summary": "Rain",
        "icon": "rain",
        "precipIntensity": 0.0535,
        "precipProbability": 0.73,
        "precipType": "rain",
        "temperature": 56.05,
        "apparentTemperature": 56.05,
        "dewPoint": 53.41,
        "humidity": 0.91,
        "windSpeed": 15.3,
        "windBearing": 162,
        "visibility": 5.74,
        "cloudCover": 1,
        "pressure": 987.91,
        "ozone": 341.08
      },
      {
        "time": 1487368800,
        "summary": "Drizzle",
        "icon": "rain",
        "precipIntensity": 0.008,
        "precipProbability": 0.58,
        "precipType": "rain",
        "temperature": 56.46,
        "apparentTemperature": 56.46,
        "dewPoint": 53.24,
        "humidity": 0.89,
        "windSpeed": 15.66,
        "windBearing": 164,
        "visibility": 7.03,
        "cloudCover": 1,
        "pressure": 987.8,
        "ozone": 348.39
      },
      {
        "time": 1487372400,
        "summary": "Light Rain",
        "icon": "rain",
        "precipIntensity": 0.032,
        "precipProbability": 0.66,
        "precipType": "rain",
        "temperature": 56.71,
        "apparentTemperature": 56.71,
        "dewPoint": 53.22,
        "humidity": 0.88,
        "windSpeed": 14.06,
        "windBearing": 164,
        "visibility": 7.21,
        "cloudCover": 1,
        "pressure": 987.62,
        "ozone": 356.48
      },
      {
        "time": 1487376000,
        "summary": "Light Rain",
        "icon": "rain",
        "precipIntensity": 0.015,
        "precipProbability": 0.56,
        "precipType": "rain",
        "temperature": 56.76,
        "apparentTemperature": 56.76,
        "dewPoint": 53.08,
        "humidity": 0.87,
        "windSpeed": 12.79,
        "windBearing": 165,
        "visibility": 7.14,
        "cloudCover": 0.98,
        "pressure": 987.45,
        "ozone": 364.2
      },
      {
        "time": 1487379600,
        "summary": "Light Rain",
        "icon": "rain",
        "precipIntensity": 0.0106,
        "precipProbability": 0.51,
        "precipType": "rain",
        "temperature": 56.89,
        "apparentTemperature": 56.89,
        "dewPoint": 52.95,
        "humidity": 0.87,
        "windSpeed": 11.98,
        "windBearing": 165,
        "visibility": 6.76,
        "cloudCover": 0.95,
        "pressure": 987.46,
        "ozone": 371.74
      },
      {
        "time": 1487383200,
        "summary": "Light Rain",
        "icon": "rain",
        "precipIntensity": 0.0093,
        "precipProbability": 0.49,
        "precipType": "rain",
        "temperature": 56.26,
        "apparentTemperature": 56.26,
        "dewPoint": 52.38,
        "humidity": 0.87,
        "windSpeed": 11.55,
        "windBearing": 164,
        "visibility": 6.39,
        "cloudCover": 0.9,
        "pressure": 987.68,
        "ozone": 378.92
      },
      {
        "time": 1487386800,
        "summary": "Drizzle",
        "icon": "rain",
        "precipIntensity": 0.0083,
        "precipProbability": 0.43,
        "precipType": "rain",
        "temperature": 55.7,
        "apparentTemperature": 55.7,
        "dewPoint": 51.77,
        "humidity": 0.87,
        "windSpeed": 9.54,
        "windBearing": 158,
        "visibility": 6.24,
        "cloudCover": 0.87,
        "pressure": 988.23,
        "ozone": 384.06
      },
      {
        "time": 1487390400,
        "summary": "Mostly Cloudy",
        "icon": "partly-cloudy-night",
        "precipIntensity": 0.0049,
        "precipProbability": 0.2,
        "precipType": "rain",
        "temperature": 55.15,
        "apparentTemperature": 55.15,
        "dewPoint": 51.43,
        "humidity": 0.87,
        "windSpeed": 9.13,
        "windBearing": 153,
        "visibility": 6.39,
        "cloudCover": 0.86,
        "pressure": 989.05,
        "ozone": 386.27
      },
      {
        "time": 1487394000,
        "summary": "Mostly Cloudy",
        "icon": "partly-cloudy-night",
        "precipIntensity": 0.0029,
        "precipProbability": 0.09,
        "precipType": "rain",
        "temperature": 54.82,
        "apparentTemperature": 54.82,
        "dewPoint": 51.12,
        "humidity": 0.87,
        "windSpeed": 8.85,
        "windBearing": 147,
        "visibility": 6.71,
        "cloudCover": 0.85,
        "pressure": 990.02,
        "ozone": 386.43
      },
      {
        "time": 1487397600,
        "summary": "Mostly Cloudy",
        "icon": "partly-cloudy-night",
        "precipIntensity": 0.0026,
        "precipProbability": 0.08,
        "precipType": "rain",
        "temperature": 54.48,
        "apparentTemperature": 54.48,
        "dewPoint": 50.51,
        "humidity": 0.86,
        "windSpeed": 7.74,
        "windBearing": 140,
        "visibility": 7,
        "cloudCover": 0.85,
        "pressure": 990.93,
        "ozone": 385.48
      },
      {
        "time": 1487401200,
        "summary": "Mostly Cloudy",
        "icon": "partly-cloudy-night",
        "precipIntensity": 0.0028,
        "precipProbability": 0.09,
        "precipType": "rain",
        "temperature": 54.04,
        "apparentTemperature": 54.04,
        "dewPoint": 50.16,
        "humidity": 0.87,
        "windSpeed": 5.62,
        "windBearing": 133,
        "visibility": 6.87,
        "cloudCover": 0.87,
        "pressure": 991.69,
        "ozone": 383.88
      },
      {
        "time": 1487404800,
        "summary": "Drizzle",
        "icon": "rain",
        "precipIntensity": 0.0063,
        "precipProbability": 0.29,
        "precipType": "rain",
        "temperature": 53.91,
        "apparentTemperature": 53.91,
        "dewPoint": 50.06,
        "humidity": 0.87,
        "windSpeed": 3.18,
        "windBearing": 114,
        "visibility": 6.48,
        "cloudCover": 0.91,
        "pressure": 992.41,
        "ozone": 381.17
      },
      {
        "time": 1487408400,
        "summary": "Drizzle",
        "icon": "rain",
        "precipIntensity": 0.0083,
        "precipProbability": 0.43,
        "precipType": "rain",
        "temperature": 53.64,
        "apparentTemperature": 53.64,
        "dewPoint": 49.82,
        "humidity": 0.87,
        "windSpeed": 2.23,
        "windBearing": 73,
        "visibility": 6.21,
        "cloudCover": 0.95,
        "pressure": 993.18,
        "ozone": 376.92
      },
      {
        "time": 1487412000,
        "summary": "Light Rain",
        "icon": "rain",
        "precipIntensity": 0.0111,
        "precipProbability": 0.52,
        "precipType": "rain",
        "temperature": 53.58,
        "apparentTemperature": 53.58,
        "dewPoint": 49.95,
        "humidity": 0.87,
        "windSpeed": 2.46,
        "windBearing": 2,
        "visibility": 6.34,
        "cloudCover": 0.97,
        "pressure": 994.01,
        "ozone": 370.09
      },
      {
        "time": 1487415600,
        "summary": "Light Rain",
        "icon": "rain",
        "precipIntensity": 0.0147,
        "precipProbability": 0.56,
        "precipType": "rain",
        "temperature": 53.6,
        "apparentTemperature": 53.6,
        "dewPoint": 50.14,
        "humidity": 0.88,
        "windSpeed": 5.37,
        "windBearing": 335,
        "visibility": 6.54,
        "cloudCover": 0.98,
        "pressure": 994.88,
        "ozone": 361.72
      },
      {
        "time": 1487419200,
        "summary": "Drizzle",
        "icon": "rain",
        "precipIntensity": 0.009,
        "precipProbability": 0.48,
        "precipType": "rain",
        "temperature": 53.63,
        "apparentTemperature": 53.63,
        "dewPoint": 49.82,
        "humidity": 0.87,
        "windSpeed": 7.9,
        "windBearing": 328,
        "visibility": 6.87,
        "cloudCover": 0.97,
        "pressure": 995.74,
        "ozone": 354.49
      },
      {
        "time": 1487422800,
        "summary": "Drizzle",
        "icon": "rain",
        "precipIntensity": 0.0053,
        "precipProbability": 0.23,
        "precipType": "rain",
        "temperature": 53.58,
        "apparentTemperature": 53.58,
        "dewPoint": 49.84,
        "humidity": 0.87,
        "windSpeed": 9,
        "windBearing": 325,
        "visibility": 6.91,
        "cloudCover": 0.95,
        "pressure": 996.54,
        "ozone": 349.43
      },
      {
        "time": 1487426400,
        "summary": "Drizzle",
        "icon": "rain",
        "precipIntensity": 0.005,
        "precipProbability": 0.21,
        "precipType": "rain",
        "temperature": 53.49,
        "apparentTemperature": 53.49,
        "dewPoint": 49.64,
        "humidity": 0.87,
        "windSpeed": 10.87,
        "windBearing": 320,
        "visibility": 7.06,
        "cloudCover": 0.92,
        "pressure": 997.36,
        "ozone": 345.52
      },
      {
        "time": 1487430000,
        "summary": "Mostly Cloudy",
        "icon": "partly-cloudy-night",
        "precipIntensity": 0.0047,
        "precipProbability": 0.19,
        "precipType": "rain",
        "temperature": 53.8,
        "apparentTemperature": 53.8,
        "dewPoint": 49.63,
        "humidity": 0.86,
        "windSpeed": 12.31,
        "windBearing": 317,
        "visibility": 7.73,
        "cloudCover": 0.9,
        "pressure": 998.22,
        "ozone": 342.29
      },
      {
        "time": 1487433600,
        "summary": "Mostly Cloudy",
        "icon": "partly-cloudy-day",
        "precipIntensity": 0.0041,
        "precipProbability": 0.16,
        "precipType": "rain",
        "temperature": 54.24,
        "apparentTemperature": 54.24,
        "dewPoint": 49.71,
        "humidity": 0.85,
        "windSpeed": 14.04,
        "windBearing": 315,
        "visibility": 8.47,
        "cloudCover": 0.9,
        "pressure": 999.19,
        "ozone": 340.06
      },
      {
        "time": 1487437200,
        "summary": "Mostly Cloudy",
        "icon": "partly-cloudy-day",
        "precipIntensity": 0.0032,
        "precipProbability": 0.11,
        "precipType": "rain",
        "temperature": 54.89,
        "apparentTemperature": 54.89,
        "dewPoint": 49.25,
        "humidity": 0.81,
        "windSpeed": 15.53,
        "windBearing": 315,
        "visibility": 9.08,
        "cloudCover": 0.9,
        "pressure": 1000.2,
        "ozone": 338.52
      },
      {
        "time": 1487440800,
        "summary": "Mostly Cloudy",
        "icon": "partly-cloudy-day",
        "precipIntensity": 0.0023,
        "precipProbability": 0.06,
        "precipType": "rain",
        "temperature": 55.56,
        "apparentTemperature": 55.56,
        "dewPoint": 48.95,
        "humidity": 0.78,
        "windSpeed": 16.59,
        "windBearing": 315,
        "visibility": 9.48,
        "cloudCover": 0.82,
        "pressure": 1001.08,
        "ozone": 336.27
      },
      {
        "time": 1487444400,
        "summary": "Breezy and Mostly Cloudy",
        "icon": "wind",
        "precipIntensity": 0.0016,
        "precipProbability": 0.03,
        "precipType": "rain",
        "temperature": 56.74,
        "apparentTemperature": 56.74,
        "dewPoint": 49.24,
        "humidity": 0.76,
        "windSpeed": 18.69,
        "windBearing": 311,
        "visibility": 10,
        "cloudCover": 0.75,
        "pressure": 1001.71,
        "ozone": 332.41
      },
      {
        "time": 1487448000,
        "summary": "Breezy and Mostly Cloudy",
        "icon": "wind",
        "precipIntensity": 0.0011,
        "precipProbability": 0.02,
        "precipType": "rain",
        "temperature": 57.66,
        "apparentTemperature": 57.66,
        "dewPoint": 49.05,
        "humidity": 0.73,
        "windSpeed": 19.15,
        "windBearing": 308,
        "visibility": 10,
        "cloudCover": 0.65,
        "pressure": 1002.2,
        "ozone": 327.84
      },
      {
        "time": 1487451600,
        "summary": "Breezy and Partly Cloudy",
        "icon": "wind",
        "precipIntensity": 0,
        "precipProbability": 0,
        "temperature": 56.39,
        "apparentTemperature": 56.39,
        "dewPoint": 47.11,
        "humidity": 0.71,
        "windSpeed": 19.55,
        "windBearing": 306,
        "visibility": 10,
        "cloudCover": 0.56,
        "pressure": 1002.64,
        "ozone": 323.86
      },
      {
        "time": 1487455200,
        "summary": "Breezy and Partly Cloudy",
        "icon": "wind",
        "precipIntensity": 0,
        "precipProbability": 0,
        "temperature": 55.46,
        "apparentTemperature": 55.46,
        "dewPoint": 46.48,
        "humidity": 0.72,
        "windSpeed": 19.89,
        "windBearing": 305,
        "visibility": 10,
        "cloudCover": 0.41,
        "pressure": 1003.02,
        "ozone": 321.09
      },
      {
        "time": 1487458800,
        "summary": "Breezy and Partly Cloudy",
        "icon": "wind",
        "precipIntensity": 0,
        "precipProbability": 0,
        "temperature": 54.2,
        "apparentTemperature": 54.2,
        "dewPoint": 45.6,
        "humidity": 0.73,
        "windSpeed": 20.09,
        "windBearing": 304,
        "visibility": 10,
        "cloudCover": 0.28,
        "pressure": 1003.35,
        "ozone": 318.91
      },
      {
        "time": 1487462400,
        "summary": "Breezy",
        "icon": "wind",
        "precipIntensity": 0,
        "precipProbability": 0,
        "temperature": 53.09,
        "apparentTemperature": 53.09,
        "dewPoint": 44.9,
        "humidity": 0.74,
        "windSpeed": 19.82,
        "windBearing": 303,
        "visibility": 10,
        "cloudCover": 0.19,
        "pressure": 1003.81,
        "ozone": 316.76
      },
      {
        "time": 1487466000,
        "summary": "Breezy",
        "icon": "wind",
        "precipIntensity": 0,
        "precipProbability": 0,
        "temperature": 52.16,
        "apparentTemperature": 52.16,
        "dewPoint": 44.51,
        "humidity": 0.75,
        "windSpeed": 18.78,
        "windBearing": 303,
        "visibility": 10,
        "cloudCover": 0.14,
        "pressure": 1004.5,
        "ozone": 314.47
      },
      {
        "time": 1487469600,
        "summary": "Clear",
        "icon": "clear-night",
        "precipIntensity": 0,
        "precipProbability": 0,
        "temperature": 51.42,
        "apparentTemperature": 51.42,
        "dewPoint": 44.53,
        "humidity": 0.77,
        "windSpeed": 17.26,
        "windBearing": 302,
        "visibility": 10,
        "cloudCover": 0.1,
        "pressure": 1005.3,
        "ozone": 312.2
      },
      {
        "time": 1487473200,
        "summary": "Clear",
        "icon": "clear-night",
        "precipIntensity": 0,
        "precipProbability": 0,
        "temperature": 50.99,
        "apparentTemperature": 50.99,
        "dewPoint": 44.6,
        "humidity": 0.79,
        "windSpeed": 15.81,
        "windBearing": 301,
        "visibility": 10,
        "cloudCover": 0.11,
        "pressure": 1006.1,
        "ozone": 309.9
      },
      {
        "time": 1487476800,
        "summary": "Clear",
        "icon": "clear-night",
        "precipIntensity": 0,
        "precipProbability": 0,
        "temperature": 50.76,
        "apparentTemperature": 50.76,
        "dewPoint": 44.67,
        "humidity": 0.8,
        "windSpeed": 14.73,
        "windBearing": 299,
        "visibility": 10,
        "cloudCover": 0.11,
        "pressure": 1006.88,
        "ozone": 307.47
      },
      {
        "time": 1487480400,
        "summary": "Clear",
        "icon": "clear-night",
        "precipIntensity": 0,
        "precipProbability": 0,
        "temperature": 50.48,
        "apparentTemperature": 50.48,
        "dewPoint": 44.62,
        "humidity": 0.8,
        "windSpeed": 13.62,
        "windBearing": 296,
        "visibility": 10,
        "cloudCover": 0.11,
        "pressure": 1007.65,
        "ozone": 305.02
      },
      {
        "time": 1487484000,
        "summary": "Clear",
        "icon": "clear-night",
        "precipIntensity": 0,
        "precipProbability": 0,
        "temperature": 50.18,
        "apparentTemperature": 50.18,
        "dewPoint": 44.5,
        "humidity": 0.81,
        "windSpeed": 12.32,
        "windBearing": 291,
        "visibility": 10,
        "cloudCover": 0.19,
        "pressure": 1008.35,
        "ozone": 302.8
      },
      {
        "time": 1487487600,
        "summary": "Partly Cloudy",
        "icon": "partly-cloudy-night",
        "precipIntensity": 0,
        "precipProbability": 0,
        "temperature": 49.99,
        "apparentTemperature": 45.77,
        "dewPoint": 44.45,
        "humidity": 0.81,
        "windSpeed": 10.81,
        "windBearing": 279,
        "visibility": 10,
        "cloudCover": 0.41,
        "pressure": 1008.98,
        "ozone": 301.15
      },
      {
        "time": 1487491200,
        "summary": "Mostly Cloudy",
        "icon": "partly-cloudy-night",
        "precipIntensity": 0,
        "precipProbability": 0,
        "temperature": 50.81,
        "apparentTemperature": 50.81,
        "dewPoint": 45.35,
        "humidity": 0.81,
        "windSpeed": 10.25,
        "windBearing": 263,
        "visibility": 10,
        "cloudCover": 0.71,
        "pressure": 1009.53,
        "ozone": 299.74
      },
      {
        "time": 1487494800,
        "summary": "Mostly Cloudy",
        "icon": "partly-cloudy-night",
        "precipIntensity": 0,
        "precipProbability": 0,
        "temperature": 51.33,
        "apparentTemperature": 51.33,
        "dewPoint": 46.09,
        "humidity": 0.82,
        "windSpeed": 10.27,
        "windBearing": 249,
        "visibility": 10,
        "cloudCover": 0.92,
        "pressure": 1009.92,
        "ozone": 297.87
      },
      {
        "time": 1487498400,
        "summary": "Overcast",
        "icon": "cloudy",
        "precipIntensity": 0.0009,
        "precipProbability": 0.01,
        "precipType": "rain",
        "temperature": 51.66,
        "apparentTemperature": 51.66,
        "dewPoint": 46.87,
        "humidity": 0.84,
        "windSpeed": 10.8,
        "windBearing": 237,
        "visibility": 10,
        "cloudCover": 0.99,
        "pressure": 1010.15,
        "ozone": 295.15
      },
      {
        "time": 1487502000,
        "summary": "Overcast",
        "icon": "cloudy",
        "precipIntensity": 0.0011,
        "precipProbability": 0.02,
        "precipType": "rain",
        "temperature": 51.97,
        "apparentTemperature": 51.97,
        "dewPoint": 47.75,
        "humidity": 0.85,
        "windSpeed": 11.21,
        "windBearing": 225,
        "visibility": 10,
        "cloudCover": 0.98,
        "pressure": 1010.28,
        "ozone": 291.98
      },
      {
        "time": 1487505600,
        "summary": "Overcast",
        "icon": "cloudy",
        "precipIntensity": 0.0016,
        "precipProbability": 0.03,
        "precipType": "rain",
        "temperature": 52.2,
        "apparentTemperature": 52.2,
        "dewPoint": 48.48,
        "humidity": 0.87,
        "windSpeed": 11.93,
        "windBearing": 217,
        "visibility": 10,
        "cloudCover": 0.96,
        "pressure": 1010.41,
        "ozone": 288.82
      },
      {
        "time": 1487509200,
        "summary": "Overcast",
        "icon": "cloudy",
        "precipIntensity": 0.0027,
        "precipProbability": 0.08,
        "precipType": "rain",
        "temperature": 52.29,
        "apparentTemperature": 52.29,
        "dewPoint": 48.99,
        "humidity": 0.89,
        "windSpeed": 12.27,
        "windBearing": 210,
        "visibility": 8.91,
        "cloudCover": 0.96,
        "pressure": 1010.58,
        "ozone": 285.73
      },
      {
        "time": 1487512800,
        "summary": "Overcast",
        "icon": "cloudy",
        "precipIntensity": 0.0043,
        "precipProbability": 0.17,
        "precipType": "rain",
        "temperature": 52.24,
        "apparentTemperature": 52.24,
        "dewPoint": 49.33,
        "humidity": 0.9,
        "windSpeed": 12.37,
        "windBearing": 204,
        "visibility": 7.41,
        "cloudCover": 0.95,
        "pressure": 1010.76,
        "ozone": 282.65
      },
      {
        "time": 1487516400,
        "summary": "Drizzle",
        "icon": "rain",
        "precipIntensity": 0.0056,
        "precipProbability": 0.25,
        "precipType": "rain",
        "temperature": 52.22,
        "apparentTemperature": 52.22,
        "dewPoint": 49.42,
        "humidity": 0.9,
        "windSpeed": 12.62,
        "windBearing": 198,
        "visibility": 6.29,
        "cloudCover": 0.95,
        "pressure": 1011,
        "ozone": 279.88
      },
      {
        "time": 1487520000,
        "summary": "Drizzle",
        "icon": "rain",
        "precipIntensity": 0.0061,
        "precipProbability": 0.28,
        "precipType": "rain",
        "temperature": 52.36,
        "apparentTemperature": 52.36,
        "dewPoint": 49.24,
        "humidity": 0.89,
        "windSpeed": 13.17,
        "windBearing": 192,
        "visibility": 5.75,
        "cloudCover": 0.95,
        "pressure": 1011.37,
        "ozone": 277.45
      },
      {
        "time": 1487523600,
        "summary": "Drizzle",
        "icon": "rain",
        "precipIntensity": 0.0063,
        "precipProbability": 0.29,
        "precipType": "rain",
        "temperature": 52.7,
        "apparentTemperature": 52.7,
        "dewPoint": 48.99,
        "humidity": 0.87,
        "windSpeed": 13.89,
        "windBearing": 188,
        "visibility": 5.59,
        "cloudCover": 0.96,
        "pressure": 1011.79,
        "ozone": 275.32
      },
      {
        "time": 1487527200,
        "summary": "Drizzle",
        "icon": "rain",
        "precipIntensity": 0.0069,
        "precipProbability": 0.33,
        "precipType": "rain",
        "temperature": 53.12,
        "apparentTemperature": 53.12,
        "dewPoint": 48.87,
        "humidity": 0.85,
        "windSpeed": 14.69,
        "windBearing": 184,
        "visibility": 5.92,
        "cloudCover": 0.97,
        "pressure": 1012.07,
        "ozone": 273.65
      },
      {
        "time": 1487530800,
        "summary": "Drizzle",
        "icon": "rain",
        "precipIntensity": 0.0085,
        "precipProbability": 0.44,
        "precipType": "rain",
        "temperature": 53.72,
        "apparentTemperature": 53.72,
        "dewPoint": 49.01,
        "humidity": 0.84,
        "windSpeed": 15.65,
        "windBearing": 182,
        "visibility": 7.32,
        "cloudCover": 0.98,
        "pressure": 1012.11,
        "ozone": 272.62
      },
      {
        "time": 1487534400,
        "summary": "Light Rain",
        "icon": "rain",
        "precipIntensity": 0.0106,
        "precipProbability": 0.51,
        "precipType": "rain",
        "temperature": 54.7,
        "apparentTemperature": 54.7,
        "dewPoint": 49.57,
        "humidity": 0.83,
        "windSpeed": 16.57,
        "windBearing": 180,
        "visibility": 9.21,
        "cloudCover": 0.98,
        "pressure": 1012,
        "ozone": 272.05
      },
      {
        "time": 1487538000,
        "summary": "Light Rain",
        "icon": "rain",
        "precipIntensity": 0.0127,
        "precipProbability": 0.54,
        "precipType": "rain",
        "temperature": 55.26,
        "apparentTemperature": 55.26,
        "dewPoint": 49.95,
        "humidity": 0.82,
        "windSpeed": 16.95,
        "windBearing": 179,
        "visibility": 10,
        "cloudCover": 0.99,
        "pressure": 1011.78,
        "ozone": 271.57
      }
    ]
  },
  "daily": {
    "summary": "Rain today through Wednesday, with temperatures peaking at 60\u00b0F on Monday.",
    "icon": "rain",
    "data": [
      {
        "time": 1487318400,
        "summary": "Rain until evening, starting again overnight.",
        "icon": "rain",
        "sunriseTime": 1487343485,
        "sunsetTime": 1487382729,
        "moonPhase": 0.72,
        "precipIntensity": 0.0342,
        "precipIntensityMax": 0.1031,
        "precipIntensityMaxTime": 1487340000,
        "precipProbability": 0.79,
        "precipType": "rain",
        "temperatureMin": 52.94,
        "temperatureMinTime": 1487332800,
        "temperatureMax": 56.91,
        "temperatureMaxTime": 1487354400,
        "apparentTemperatureMin": 52.94,
        "apparentTemperatureMinTime": 1487332800,
        "apparentTemperatureMax": 56.91,
        "apparentTemperatureMaxTime": 1487354400,
        "dewPoint": 51.97,
        "humidity": 0.9,
        "windSpeed": 8.67,
        "windBearing": 136,
        "visibility": 6.76,
        "cloudCover": 0.89,
        "pressure": 993.47,
        "ozone": 340.3
      },
      {
        "time": 1487404800,
        "summary": "Drizzle in the morning and breezy until evening.",
        "icon": "rain",
        "sunriseTime": 1487429811,
        "sunsetTime": 1487469193,
        "moonPhase": 0.75,
        "precipIntensity": 0.0033,
        "precipIntensityMax": 0.0147,
        "precipIntensityMaxTime": 1487415600,
        "precipProbability": 0.56,
        "precipType": "rain",
        "temperatureMin": 49.99,
        "temperatureMinTime": 1487487600,
        "temperatureMax": 57.66,
        "temperatureMaxTime": 1487448000,
        "apparentTemperatureMin": 45.77,
        "apparentTemperatureMinTime": 1487487600,
        "apparentTemperatureMax": 57.66,
        "apparentTemperatureMaxTime": 1487448000,
        "dewPoint": 47.54,
        "humidity": 0.8,
        "windSpeed": 12.67,
        "windBearing": 308,
        "visibility": 8.8,
        "cloudCover": 0.59,
        "pressure": 1001.14,
        "ozone": 332.93
      },
      {
        "time": 1487491200,
        "summary": "Rain throughout the day and breezy overnight.",
        "icon": "rain",
        "sunriseTime": 1487516137,
        "sunsetTime": 1487555657,
        "moonPhase": 0.78,
        "precipIntensity": 0.0308,
        "precipIntensityMax": 0.1344,
        "precipIntensityMaxTime": 1487570400,
        "precipProbability": 0.81,
        "precipType": "rain",
        "temperatureMin": 50.81,
        "temperatureMinTime": 1487491200,
        "temperatureMax": 59.02,
        "temperatureMaxTime": 1487574000,
        "apparentTemperatureMin": 50.81,
        "apparentTemperatureMinTime": 1487491200,
        "apparentTemperatureMax": 59.02,
        "apparentTemperatureMaxTime": 1487574000,
        "dewPoint": 50.45,
        "humidity": 0.87,
        "windSpeed": 11.93,
        "windBearing": 186,
        "visibility": 6.61,
        "cloudCover": 0.97,
        "pressure": 1010.94,
        "ozone": 277.58
      },
      {
        "time": 1487577600,
        "summary": "Rain and breezy throughout the day.",
        "icon": "rain",
        "sunriseTime": 1487602461,
        "sunsetTime": 1487642121,
        "moonPhase": 0.81,
        "precipIntensity": 0.0716,
        "precipIntensityMax": 0.1384,
        "precipIntensityMaxTime": 1487624400,
        "precipProbability": 0.81,
        "precipType": "rain",
        "temperatureMin": 56.75,
        "temperatureMinTime": 1487646000,
        "temperatureMax": 60.46,
        "temperatureMaxTime": 1487588400,
        "apparentTemperatureMin": 56.75,
        "apparentTemperatureMinTime": 1487646000,
        "apparentTemperatureMax": 60.46,
        "apparentTemperatureMaxTime": 1487588400,
        "dewPoint": 54.85,
        "humidity": 0.91,
        "windSpeed": 20.81,
        "windBearing": 199,
        "visibility": 6.84,
        "cloudCover": 0.95,
        "pressure": 1013.27,
        "ozone": 285.6
      },
      {
        "time": 1487664000,
        "summary": "Rain throughout the day and breezy starting in the evening.",
        "icon": "rain",
        "sunriseTime": 1487688784,
        "sunsetTime": 1487728584,
        "moonPhase": 0.84,
        "precipIntensity": 0.0317,
        "precipIntensityMax": 0.114,
        "precipIntensityMaxTime": 1487732400,
        "precipProbability": 0.8,
        "precipType": "rain",
        "temperatureMin": 51.66,
        "temperatureMinTime": 1487689200,
        "temperatureMax": 58.66,
        "temperatureMaxTime": 1487714400,
        "apparentTemperatureMin": 51.66,
        "apparentTemperatureMinTime": 1487689200,
        "apparentTemperatureMax": 58.66,
        "apparentTemperatureMaxTime": 1487714400,
        "dewPoint": 50.34,
        "humidity": 0.81,
        "windSpeed": 13.13,
        "windBearing": 204,
        "cloudCover": 0.58,
        "pressure": 1018.92,
        "ozone": 335.49
      },
      {
        "time": 1487750400,
        "summary": "Light rain until evening and breezy in the afternoon.",
        "icon": "rain",
        "sunriseTime": 1487775106,
        "sunsetTime": 1487815047,
        "moonPhase": 0.87,
        "precipIntensity": 0.0116,
        "precipIntensityMax": 0.039,
        "precipIntensityMaxTime": 1487750400,
        "precipProbability": 0.68,
        "precipType": "rain",
        "temperatureMin": 50.19,
        "temperatureMinTime": 1487833200,
        "temperatureMax": 56.32,
        "temperatureMaxTime": 1487750400,
        "apparentTemperatureMin": 50.19,
        "apparentTemperatureMinTime": 1487833200,
        "apparentTemperatureMax": 56.32,
        "apparentTemperatureMaxTime": 1487750400,
        "dewPoint": 45.69,
        "humidity": 0.77,
        "windSpeed": 16.38,
        "windBearing": 279,
        "cloudCover": 0.34,
        "pressure": 1019.6,
        "ozone": 336.82
      },
      {
        "time": 1487836800,
        "summary": "Partly cloudy in the morning.",
        "icon": "partly-cloudy-day",
        "sunriseTime": 1487861427,
        "sunsetTime": 1487901509,
        "moonPhase": 0.9,
        "precipIntensity": 0,
        "precipIntensityMax": 0,
        "precipProbability": 0,
        "temperatureMin": 45.33,
        "temperatureMinTime": 1487847600,
        "temperatureMax": 56.96,
        "temperatureMaxTime": 1487890800,
        "apparentTemperatureMin": 41.53,
        "apparentTemperatureMinTime": 1487847600,
        "apparentTemperatureMax": 56.96,
        "apparentTemperatureMaxTime": 1487890800,
        "dewPoint": 36.27,
        "humidity": 0.6,
        "windSpeed": 7.23,
        "windBearing": 4,
        "cloudCover": 0.08,
        "pressure": 1023.11,
        "ozone": 316.51
      },
      {
        "time": 1487923200,
        "summary": "Clear throughout the day.",
        "icon": "clear-day",
        "sunriseTime": 1487947748,
        "sunsetTime": 1487987971,
        "moonPhase": 0.94,
        "precipIntensity": 0.0011,
        "precipIntensityMax": 0.0016,
        "precipIntensityMaxTime": 1487980800,
        "precipProbability": 0.03,
        "precipType": "rain",
        "temperatureMin": 41.72,
        "temperatureMinTime": 1487948400,
        "temperatureMax": 57.25,
        "temperatureMaxTime": 1487980800,
        "apparentTemperatureMin": 39.3,
        "apparentTemperatureMinTime": 1487948400,
        "apparentTemperatureMax": 57.25,
        "apparentTemperatureMaxTime": 1487980800,
        "dewPoint": 37.74,
        "humidity": 0.65,
        "windSpeed": 2.13,
        "windBearing": 66,
        "cloudCover": 0.05,
        "pressure": 1020.31,
        "ozone": 309.39
      }
    ]
  },
  "alerts": [
    {
      "title": "Wind Advisory",
      "regions": [
        "East Bay Hills and the Diablo Range",
        "East Bay Interior Valleys",
        "Mountains Of San Benito County And Interior Monterey County Including Pinnacles National Monument",
        "Northern Monterey Bay",
        "San Francisco Bay Shoreline",
        "San Fransisco Peninsula Coast",
        "Santa Clara Valley Including San Jose",
        "Santa Cruz Mountains",
        "Southern Salinas Valley, Arroyo Seco and Lake San Antonio"
      ],
      "severity": "advisory",
      "time": 1487362260,
      "expires": 1487383200,
      "description": "...Strong and gusty southeast winds expected through Friday\nafternoon and evening...\n.A potent storm system will result in strong and gusty southerly\nwinds across much of the San Francisco Bay Area and Monterey Bay\nArea today. The strongest winds are expected along the Big Sur\nCoast... in the Santa Lucia Mountains and in the northern Salinas\nValley. Winds will gradually decrease late this afternoon as the\nsystem pushes inland.\n...WIND ADVISORY REMAINS IN EFFECT UNTIL 6 PM PST THIS EVENING...\n* TIMING...Through late Friday afternoon.\n* WINDS...Southeast winds 20 to 30 MPH with gusts greater than\n45 MPH.\n* IMPACTS...Gusty winds will make driving difficult especially\nfor drivers of high profile vehicles. Since the soils are\nsaturated... there is a heightened potential for downed trees\nand power outages.",
      "uri": "https:\/\/alerts.weather.gov\/cap\/wwacapget.php?x=CA12583EA19DCC.WindAdvisory.12583EAE1CA0CA.MTRNPWMTR.e71058f9a6b57883d978695e577e021c"
    },
    {
      "title": "Wind Advisory",
      "regions": [
        "Coastal North Bay Including Point Reyes National Seashore",
        "North Bay Interior Valleys",
        "North Bay Mountains",
        "San Francisco"
      ],
      "severity": "advisory",
      "time": 1487362260,
      "expires": 1487368800,
      "description": "...Strong and gusty southeast winds expected through Friday\nafternoon and evening...\n.A potent storm system will result in strong and gusty southerly\nwinds across much of the San Francisco Bay Area and Monterey Bay\nArea today. The strongest winds are expected along the Big Sur\nCoast... in the Santa Lucia Mountains and in the northern Salinas\nValley. Winds will gradually decrease late this afternoon as the\nsystem pushes inland.\n...WIND ADVISORY IN EFFECT UNTIL 2 PM PST THIS AFTERNOON...\nThe National Weather Service in San Francisco has issued a Wind\nAdvisory...which is in effect until 2 PM PST this afternoon.\n* TIMING...Through late Friday afternoon.\n* WINDS...Southeast winds 20 to 35 MPH with gusts greater than\n45 MPH.\n* IMPACTS...Gusty winds will make driving difficult especially\nfor drivers of high profile vehicles. Since the soils are\nsaturated... there is a heightened potential for downed trees\nand power outages.",
      "uri": "https:\/\/alerts.weather.gov\/cap\/wwacapget.php?x=CA12583EA19DCC.WindAdvisory.12583EA1E7A0CA.MTRNPWMTR.40d69c868d281e0d7330d703c571eb31"
    }
  ],
  "flags": {
    "sources": [
      "darksky",
      "lamp",
      "gfs",
      "cmc",
      "nam",
      "rap",
      "rtma",
      "sref",
      "fnmoc",
      "isd",
      "madis",
      "nearest-precip",
      "nwspa"
    ],
    "darksky-stations": [
      "KMUX",
      "KDAX"
    ],
    "lamp-stations": [
      "KAPC",
      "KCCR",
      "KHWD",
      "KLVK",
      "KNUQ",
      "KOAK",
      "KPAO",
      "KSFO",
      "KSQL"
    ],
    "isd-stations": [
      "724943-99999",
      "745039-99999",
      "745045-99999",
      "745060-23239",
      "745065-99999",
      "994016-99999",
      "994033-99999",
      "994036-99999",
      "997734-99999",
      "998163-99999",
      "998197-99999",
      "998476-99999",
      "998477-99999",
      "998479-99999",
      "999999-23239",
      "999999-23272"
    ],
    "madis-stations": [
      "AU915",
      "C5988",
      "C8158",
      "C9629",
      "CQ147",
      "D5422",
      "D8008",
      "E0426",
      "E9227",
      "FTPC1",
      "GGBC1",
      "OKXC1",
      "OMHC1",
      "PPXC1",
      "SFOC1",
      "TIBC1"
    ],
    "units": "us"
  }
}

*/

