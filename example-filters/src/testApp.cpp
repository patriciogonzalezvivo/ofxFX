#define STRINGIFY(A) #A

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
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
    
    selection = 1;
}

//--------------------------------------------------------------
void testApp::update(){
    video.update();
    if (video.isFrameNew() ){
        lut.setTexture(video.getTextureReference());
        lut.update();
        
        mult.setTexture(lut.getTextureReference(),1);
        
        if ( selection == 0 ){
            bloom << lut ;
            bloom.update();
        } else if ( selection == 1 ){
            blur.setRadius(sin(beat)*10);
            blur << lut;
            mult.setTexture(blur.getTextureReference(),2);
            blur.update();
        } else if ( selection == 2 ){
            bokeh.setRadius(abs(sin(beat)*10));
            bokeh << lut;
            mult.setTexture(bokeh.getTextureReference(),2);
            bokeh.update();
        } else if ( selection == 3 ){
            glow.setRadius(sin(beat)*15);
            glow << lut;
            mult.setTexture(glow.getTextureReference(),2);
            glow.update();
        } else if ( selection == 4 ){
            unsharp.setFade(sin(beat));
            unsharp << lut;
            mult.setTexture(unsharp.getTextureReference(),2);
            unsharp.update();
        }
        
        mult.update();
    }
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    beat += 1.0/ofGetFrameRate();
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
    
    
    
    ofPopMatrix();
    
    if ( selection == 0 ){
        ofDrawBitmapString("Bloom", 15,30);
    } else if ( selection == 1 ){
        ofDrawBitmapString("Blur at: " + ofToString(sin(beat)*10),15,30);
    } else if ( selection == 2 ){
        ofDrawBitmapString("Bokeh at: "+ ofToString( abs(sin(beat)*10)), 15,30);
    } else if ( selection == 3 ){
        ofDrawBitmapString("Glow at: "+ ofToString(sin(beat)*15), 15,30);
    } else if ( selection == 4 ){
        ofDrawBitmapString("Unsharp at: "+ ofToString(sin(beat)), 15,30);
    } else {
        ofDrawBitmapString("No Filter", 15,30);
    }
    ofDrawBitmapString(dir.getName(dirLoadIndex), 15,15);
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

