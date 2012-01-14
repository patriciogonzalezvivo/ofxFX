/*
 *  ofxFXObject.h
 *
 *  Created by Patricio González Vivo on 03/07/11.
 *  Copyright 2011 http://PatricioGonzalezVivo.com All rights reserved.
 *
 *
 *  Common structure of ofxFX different effects
 */   

#ifndef OFXFXOBJECT
#define OFXFXOBJECT

#include "ofMain.h"
#include "ofxSwapBuffer.h"

class ofxFXObject{
public:
    ofxFXObject(){
        passes = 1;
        
        fragmentShader = "#version 120\n \
    	#extension GL_ARB_texture_rectangle : enable\n \
        void main(){\
            gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\
        }";
    }
    
    void allocate(int _width, int _height){
        width = _width;
        height = _height;
        
        pingPong.allocate(width, height,GL_RGBA);
        
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
        shader.linkProgram();
    };
    
    void setPasses(int _passes) { passes = _passes; };
    ofTexture& getTextureReference() { return pingPong.dst->getTextureReference(); };
    
    void begin() {
		ofPushStyle();
		ofPushMatrix();
        pingPong.src->begin();
	}
	
	void end() {
        pingPong.src->end();
        ofPopStyle();
		ofPopMatrix();
    }
    
    void update(){
        for(int i = 0; i < passes; i++) {
            pingPong.dst->begin();
            shader.begin();
            shader.setUniformTexture("backbuffer", pingPong.src->getTextureReference(), 0 );
            shader.setUniform2f("resolution", (float)width, (float)height);
            shader.setUniform1f("time", time );
            renderFrame();
            shader.end();
            pingPong.dst->end();
            
            pingPong.swap();
        }
        
        pingPong.swap();
        time += 1.0/ofGetFrameRate();
    };
    
    void draw(int x = 0, int y = 0, float _width = -1, float _height = -1){
        if (_width == -1) _width = width;
        if (_height == -1) _height = height;
        
        ofPushStyle();
        ofEnableAlphaBlending();
        pingPong.dst->draw(x, y, _width, _height);
        ofPopStyle();
    }

    
protected:
    void initFbo(ofFbo & _fbo, int _width, int _height, int _internalformat = GL_RGBA ) {
        _fbo.allocate(_width, _height, _internalformat);
        _fbo.begin();
        ofClear(0,255);
        _fbo.end();
    }
    
    void renderFrame(float _width = -1, float _height = -1){
        if (_width == -1) _width = width;
        if (_height == -1) _height = height;
        
        // Rendering canvas frame in order to make it cleaner to read.
        ofSetColor(255,255,255,255);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        glTexCoord2f(_width, 0); glVertex3f(_width, 0, 0);
        glTexCoord2f(_width, _height); glVertex3f(_width, _height, 0);
        glTexCoord2f(0,_height);  glVertex3f(0,_height, 0);
        glEnd();
    }
    
    ofxSwapBuffer   pingPong;
    ofShader        shader;
    string          fragmentShader;
    float           width, height, time;
    int             passes;
};

#endif
