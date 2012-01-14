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
        \
        uniform float time;\
        uniform vec2 mouse;\
        uniform vec2 resolution;\
        \
        float box( vec2 p, vec4 rect ){ \
            float trim = min(rect.z, rect.w) * 0.5;\
            float minX = min(p.x - rect.x, rect.x + rect.z - p.x);\
            float minY = min(p.y - rect.y, rect.y + rect.w - p.y);\
            return ((minX > 0.0) && (minY > 0.0) && ((minX + minY) > trim)) ? 1.0 : 0.0;\
        }\
        \
        float digit( vec2 p, vec4 dim, float d){\
            d = (d - mod(d,1.0)) / 10.0;\
            d = mod( d, 1.0 );\
            \
            p.xy -= dim.xy;\
            p.xy /= dim.zw;\
            \
            float c = 0.0;\
            \
            c += box( p, vec4( 0.05, 0.9, 0.9, 0.1 ) ) * ( cos( (0.85*d+0.1)*30.0) - sin(pow(d,1.0)) < 0.0 ? 1.0 : 0.0 );\
            \
            c += box( p, vec4( 0.05, 0.45, 0.9, 0.1 ) ) * ( min( pow(6.0*d,2.0), pow(20.0*(d-0.7),2.0) ) < 1.0 ? 0.0 : 1.0 );\
            \
            c += box( p, vec4( 0.05, 0.0, 0.9, 0.1 ) ) * ( max( cos(18.6*pow(d,0.75)), 1.0-pow(40.0*(d-0.8),2.0)) > 0.0 ? 1.0 : 0.0 );\
            \
            c += box( p, vec4( 0.0, 0.08, 0.1, 0.39 ) ) * ( cos( d * 30.0 ) * abs(d-0.4) > 0.1 ? 1.0 : 0.0 );\
            \
            c += box( p, vec4( 0.9, 0.08, 0.1, 0.39) ) * ( pow( 4.0*d-0.8, 2.0) > 0.1 ? 1.0 : 0.0 );\
            \
            c += box( p, vec4( 0.0, 0.52, 0.1, 0.39 ) ) * ( sin((d-0.05)*10.5) - 12.0*sin(pow(d,10.0)) > 0.0 ? 0.0 : 1.0 );\
            \
            c += box( p, vec4( 0.9, 0.52, 0.1, 0.39 ) ) * ( pow( d-0.55, 2.0 ) > 0.02 ? 1.0 : 0.0 );\
            \
            return c;\
        }\
        \
        void main( void ){\
            vec2 p = (gl_FragCoord.xy / resolution.xy);\
            p.y = 1.0 - p.y;\
            \
            float c= 0.0;\
            c += ( time < 100.0 ) ? 0.0 : digit( p, vec4( 0.05, 0.4, 0.2, 0.35 ), time/100.0 );\
            c += ( time < 10.0) ? 0.0 : digit( p, vec4( 0.27, 0.4, 0.2, 0.35 ), time/10.0 );\
            c += digit( p, vec4( 0.5, 0.4, 0.2, 0.35 ), time );\
            c += box( p, vec4( 0.71, 0.4, 0.03, 0.035 ) );\
            c += digit( p, vec4( 0.75, 0.4, 0.2, 0.35 ), time*10.0 );\
            gl_FragColor = vec4( 0.0, c * 0.5, c, 1.0 )*(abs(sin(time*0.5))+0.5);\
        }";
    }
    
    void allocate(int _width, int _height, int _internalformat = GL_RGBA){
        width = _width;
        height = _height;
        
        pingPong.allocate(width, height, _internalformat);
        initFbo(texture, width, height, _internalformat);
        
        shader.unload();
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
        shader.linkProgram();
    };
    
    void injectShader(string fragShader){
        fragmentShader = fragShader;
        
        shader.unload();
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
        shader.linkProgram();
    }
    
    void setPasses(int _passes) { passes = _passes; };
    ofTexture& getTextureReference() { return pingPong.dst->getTextureReference(); };
    
    void begin() {
		ofPushStyle();
		ofPushMatrix();
        texture.begin();
        ofClear(0,255);
	}
	
	void end() {
        texture.end();
        ofPopMatrix();
        ofPopStyle();
    }
    
    void update(){
        for(int i = 0; i < passes; i++) {
            pingPong.dst->begin();
            shader.begin();
            shader.setUniformTexture("backbuffer", pingPong.src->getTextureReference(), 0 );
            shader.setUniformTexture("tex", texture.getTextureReference(), 1 );
            shader.setUniform1f("time", (float)time );
            shader.setUniform2f("resolution", (float)width, (float)height);
            shader.setUniform2f("mouse", (float)ofGetMouseX(), (float)ofGetMouseY());
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
    ofFbo           texture;
    ofShader        shader;
    string          fragmentShader;
    float           width, height, time;
    int             passes;
};

#endif
