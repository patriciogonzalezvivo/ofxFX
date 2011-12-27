//
//  ofxOldTv.h
//
//
//  Created by Patricio González Vivo on 25/11/11.
//  Copyright (c) 2011 http://PatricioGonzalezVivo.com All rights reserved.
//
//  Based on http://www.iquilezles.org/apps/shadertoy/ Postprocessing shader
//

#ifndef OFXOLDTV
#define OFXOLDTV

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxOldTv : public ofxFXObject {	
public:
	void allocate(int width, int height) {
        pingPong.allocate(width,height);
        loadShaders();
        time = 0.0;
        brightness = 0.2;
        rows = 5.0;
	}
    
    ofxOldTv& setBrightness(int _brightness) { brightness = _brightness; return * this;};
	ofxOldTv& setRows(float _rows) { rows = _rows; return * this;};
    ofTexture& getTextureReference() { return pingPong.dst->getTextureReference(); };
    
	void begin() {
        pingPong.swap();
		ofPushStyle();
		ofPushMatrix();
        pingPong.src->begin();
	}
	
	void end(bool bDraw = true) {
        pingPong.src->end();
        
        ofPopStyle();
		ofPopMatrix();
        
        time += 1/ofGetFrameRate() ;
        
        pingPong.dst->begin();
        shader.begin();
        shader.setUniform1f("time", time );
        shader.setUniform1f("brightness", brightness ); 
        shader.setUniform1f("rows", rows );
        pingPong.src->draw(0,0);
        shader.end();
        pingPong.dst->end();
            
        if (bDraw)
            draw();
	}
	
	void draw(int x = 0, int y = 0){ pingPong.dst->draw(x, y);}
    
protected:
    void loadShaders(){
        string fragmentShader = "#version 120\n \
        #extension GL_ARB_texture_rectangle : enable\n \
        \
        uniform float time;\
        uniform float brightness;\
        uniform float rows;\
        uniform sampler2DRect tex;\
        \
        void main(void){\
            vec2 st = gl_TexCoord[0].st;\
            vec2 uv = 0.5 + (1.0/st-0.5)*( (1.0-brightness) + brightness*sin(0.2*time) );\
            \
            vec3 col;\
            \
            col.r = texture2DRect(tex, vec2(st.x+0.003,st.y)).r;\
            col.g = texture2DRect(tex, vec2(st.x+0.000,st.y)).g;\
            col.b = texture2DRect(tex, vec2(st.x-0.003,st.y)).b;\
            \
            col = clamp(col*0.5+0.5*col*col*1.2,0.0,1.0);\
            \
            col *= 0.5 + 0.5*16.0*uv.x*uv.y*(1.0-uv.x)*(1.0-uv.y);\
            \
            col *= vec3(0.8,1.0,0.7);\
            \
            col *= 0.9+0.1*sin(10.0*time+st.y*rows);\
            \
            col *= 0.97+0.03*sin(110.0*time);\
            \
            gl_FragColor = vec4(col,1.0);\
        }";
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
        shader.linkProgram();
    }
    
    swapBuffer  pingPong;
    ofShader    shader;
    float       time,brightness,rows;
};
#endif