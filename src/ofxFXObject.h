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

class swapBuffer {
public:
    void allocate( int _width, int _height, int _internalformat = GL_RGBA, float _dissipation = 1.0f){
        for(int i = 0; i < 2; i++){
            FBOs[i].allocate(_width,_height, _internalformat );
            FBOs[i].begin();
            ofClear(0,255);
            FBOs[i].end();
        }
        
        flag = 0;
        swap();
        flag = 0;
        
        diss = _dissipation;
    }
        
    void swap(){
        src = &(FBOs[(flag)%2]);
        dst = &(FBOs[++(flag)%2]);
    }
    
    ofFbo& operator[]( int n ){ return FBOs[n];}
    
    ofFbo   *src;       // Source       ->  Ping
    ofFbo   *dst;       // Destination  ->  Pong
    
    float   diss;       // Dissipation

private:
    ofFbo   FBOs[2];    // Real addresses of ping/pong FBO´s  
    int     flag;       // Integer for making a quick swap
};


class ofxFXObject{
    /*
public:
    ofxFXObject(){
        // Here it loads the geo, vert and frag shaders.
    }
    void allocate(int _width, int _height){
        width = _width;
        height = _height;
        
        // Here also will be the FBO allocation and cleaning
    };
    
    void draw(int x = 0, int y = 0);
     */
    
protected:
    
    void initFbo(ofFbo & _fbo, int _width, int _height, int _internalformat = GL_RGBA ) {
        _fbo.allocate(_width, _height, _internalformat);
        _fbo.begin();
        ofClear(0,255);
        _fbo.end();
    }
    
    void renderFrame(float _width = -1, float _height = -1)
    {
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
    
protected:
    float width,height;
};

#endif
