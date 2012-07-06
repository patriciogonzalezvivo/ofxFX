#define STRINGIFY(A) #A

#include "testApp.h"

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
    mult.setTexture(map.getTextureReference());
    
    lut.allocate(width,height);
    
    bloom.allocate(width, height);
    blur.allocate(width, height);
    bokeh.allocate(width, height);
    glow.allocate(width, height);
    unsharp.allocate(width, height);
    
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
        lut.setTexture(video.getTextureReference());
        lut.update();
        
        mult.setTexture(lut.getTextureReference(),1);
        
        if ( selection == -1 ){         //  NO FILTER
            
            mult.setTexture(lut.getTextureReference(),2);
        } else if ( selection == 0 ){   // BLOOM
            bloom << lut ;
            bloom.update();
            mult.setTexture(bloom.getTextureReference(),2);
        } else if ( selection == 1 ){   // BLUR
            blur.setRadius(sin( ofGetElapsedTimef() )*10);
            blur << lut;
            blur.update();
            mult.setTexture(blur.getTextureReference(),2);
        } else if ( selection == 2 ){   // BOKEH
            bokeh.setRadius(abs(sin( ofGetElapsedTimef() )*10));
            bokeh << lut;
            bokeh.update();
            mult.setTexture(bokeh.getTextureReference(),2);
        } else if ( selection == 3 ){   // GLOW
            glow.setRadius(sin( ofGetElapsedTimef() )*15);
            glow << lut;
            glow.update();
            mult.setTexture(glow.getTextureReference(),2);
        } else if ( selection == 4 ){   // UNSHARP
            unsharp.setFade(sin( ofGetElapsedTimef() ));
            unsharp << lut;
            unsharp.update();
            mult.setTexture(unsharp.getTextureReference(),2);
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
        ofDrawBitmapString("FILTER: Blur at: " + ofToString(sin( ofGetElapsedTimef() )*10),-220 ,300);
    } else if ( selection == 2 ){
        ofDrawBitmapString("FILTER: Bokeh at: "+ ofToString( abs(sin( ofGetElapsedTimef() )*10)), -220 ,300);
    } else if ( selection == 3 ){
        ofDrawBitmapString("FILTER: Glow at: "+ ofToString(sin( ofGetElapsedTimef() )*15), -220 ,300);
    } else if ( selection == 4 ){
        ofDrawBitmapString("FILTER: Unsharp at: "+ ofToString(sin( ofGetElapsedTimef() )), -220 ,300);
    } else {
        ofDrawBitmapString("FILTER: No Filter", -220 ,300);
    }
    
    ofPopMatrix();
    
    ofSetColor(255, 255);
    ofDrawBitmapString("Press LEFT/RIGHT keys to change between filters", 15,15);
    ofDrawBitmapString("Press UP/DOWN keys to change between LUT«s", 15,30);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	switch (key) {
		case OF_KEY_UP:
			dirLoadIndex++;
			if (dirLoadIndex>=dir.size()) {
				dirLoadIndex=0;
			}
			lut.loadLUT(dir.getPath(dirLoadIndex));
			
			break;
		case OF_KEY_DOWN:
			dirLoadIndex--;
			if (dirLoadIndex<0) {
				dirLoadIndex=dir.size()-1;
			}
			lut.loadLUT(dir.getPath(dirLoadIndex));
			break;
        case OF_KEY_RIGHT:
            selection++;
            break;
        case OF_KEY_LEFT:
            selection--;
            break;
		default:
			break;
	}
    
    if (selection >= 5)
        selection = -1;
    else if ( selection < -1)
        selection = 4;
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

