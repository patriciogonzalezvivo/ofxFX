#define STRINGIFY(A) #A

#include "ofApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    
    width = 640;
    height = 480;
    video.initGrabber(width , height);
    
    map.loadImage("map.jpg");
    polaroid.loadImage("polaroid.png");
    
    mult.allocate(width, height);
    mult.setTexture(map.getTexture());
    
    lut.allocate(width,height);
    
    bloom.allocate(width, height);
    gaussianBlur.allocate(width, height);
    bokeh.allocate(width, height);
    glow.allocate(width, height);
    blur.allocate(width, height);
    
    dir.allowExt("cube");
	dir.listDir("LUTs/");
	dir.sort();
    
	if (dir.size()>0) {
		dirLoadIndex=0;
        lut.loadLUT(dir.getPath(dirLoadIndex));
	}
    
    selection = -1;
}

//--------------------------------------------------------------
void testApp::update(){
    video.update();
    if (video.isFrameNew() ){
        lut.setTexture(video.getTexture());
        lut.update();
        
        mult.setTexture(lut.getTexture(),1);
        
        if ( selection == -1 ){         //  NO FILTER
            
            mult.setTexture(lut.getTexture(),2);
        } else if ( selection == 0 ){   // BLOOM
            bloom << lut ;
            bloom.update();
            mult.setTexture(bloom.getTexture(),2);
        } else if ( selection == 1 ){   // GAUSSIAN BLUR
            gaussianBlur.setRadius(sin( ofGetElapsedTimef() )*10);
            gaussianBlur << lut;
            gaussianBlur.update();
            mult.setTexture(gaussianBlur.getTexture(),2);
        } else if ( selection == 2 ){   // BOKEH
            bokeh.setRadius(abs(sin( ofGetElapsedTimef() )*10));
            bokeh << lut;
            bokeh.update();
            mult.setTexture(bokeh.getTexture(),2);
        } else if ( selection == 3 ){   // GLOW
            glow.setRadius(sin( ofGetElapsedTimef() )*15);
            glow << lut;
            glow.update();
            mult.setTexture(glow.getTexture(),2);
        } else if ( selection == 4 ){   // BLUR
            blur.setFade(sin( ofGetElapsedTimef() ));
            blur << lut;
            blur.update();
            mult.setTexture(blur.getTexture(),2);
        }
        
        mult.update();
    }
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackgroundGradient(ofColor::gray, ofColor::black);
        
	ofSetColor(255);
	
    ofPushMatrix();
	ofTranslate(ofGetWindowWidth()*0.5f, ofGetWindowHeight()*0.5f, 0);
    
	mult.draw(-height*0.5,-height*0.5, height,height);
    
    ofSetColor(255, 255);
    polaroid.draw(-polaroid.getWidth()*0.46,-polaroid.getHeight()*0.43,polaroid.getWidth()*0.95,polaroid.getHeight()*0.95);
    
    ofSetColor(50, 255);
    ofDrawBitmapString("LUT: " + dir.getName(dirLoadIndex), -220 ,280);
    if ( selection == 0 ){
        ofDrawBitmapString("FILTER: Bloom", -220 ,300);
    } else if ( selection == 1 ){
        ofDrawBitmapString("FILTER: Gaussian Blur at: " + ofToString(sin( ofGetElapsedTimef() )*10),-220 ,300);
    } else if ( selection == 2 ){
        ofDrawBitmapString("FILTER: Bokeh at: "+ ofToString( abs(sin( ofGetElapsedTimef() )*10)), -220 ,300);
    } else if ( selection == 3 ){
        ofDrawBitmapString("FILTER: Glow at: "+ ofToString(sin( ofGetElapsedTimef() )*15), -220 ,300);
    } else if ( selection == 4 ){
        ofDrawBitmapString("FILTER: Blur (smooth/sharp) at: "+ ofToString(sin( ofGetElapsedTimef() )), -220 ,300);
    } else {
        ofDrawBitmapString("FILTER: No Filter", -220 ,300);
    }
    
    ofPopMatrix();
    
    ofSetColor(255, 255);
    ofDrawBitmapString("Press LEFT/RIGHT keys to change between filters", 15,15);
