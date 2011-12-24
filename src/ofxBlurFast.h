//
//  ofxBlurFast.h
//  emptyExample
//
//  Created by Patricio González Vivo on 25/11/11.
//  Copyright (c) 2011 http://PatricioGonzalezVivo.com All rights reserved.
//
//  It´s faster because it´s simple matrix convultion in one pass... 
//  it don´t have different horizontal and vertical shaders
//

#ifndef OFXBLURFAST
#define OFXBLURFAST

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxBlurFast : ofxFXObject {	
public:
	void allocate(int width, int height) {
        pingPong.allocate(width,height);
        
        fade = 1.f;
        passes = 3;
        
        loadShaders();
	}
    
	ofxBlurFast& setPasses(int _passes) { passes = _passes; return * this;};
	ofxBlurFast& setFade(float _fade) { fade = _fade; return * this;};
    
    ofTexture& getTextureReference() { return pingPong.dst->getTextureReference(); };
    
	void begin() {
		ofPushStyle();
		ofPushMatrix();
        pingPong.src->begin();
	}
	
	void end(bool bDraw = true) {
        pingPong.src->end();
        
        ofPopStyle();
		ofPopMatrix();
        
        for(int i = 0; i < passes; i++) {
            pingPong.dst->begin();
            blurShader.begin();
            blurShader.setUniform1f("fade_const", fade);
            pingPong.src->draw(0,0);
            blurShader.end();
            pingPong.dst->end();
            
            pingPong.swap();
        }
        
        if (bDraw)
            draw();
	}
	
	void draw(int x = 0, int y = 0){ pingPong.dst->draw(x, y);}
    
protected:
    void loadShaders(){
        string fragmentBlurShader = "#version 120\n \
        #extension GL_ARB_texture_rectangle : enable\n \
        \
        float kernel[9];\
        \
        uniform sampler2DRect tex;\
        uniform float fade_const;\
        \
        vec2 offset[9];\
        \
        void main(void){\
        vec2  st = gl_TexCoord[0].st;\
        vec4 sum = vec4(0.0);\
        \
        offset[0] = vec2(-1.0, -1.0);\
        offset[1] = vec2(0.0, -1.0);\
        offset[2] = vec2(1.0, -1.0);\
        \
        offset[3] = vec2(-1.0, 0.0);\
        offset[4] = vec2(0.0, 0.0);\
        offset[5] = vec2(1.0, 0.0);\
        \
        offset[6] = vec2(-1.0, 1.0);\
        offset[7] = vec2(0.0, 1.0);\
        offset[8] = vec2(1.0, 1.0);\
        \
        kernel[0] = 1.0/16.0;   kernel[1] = 2.0/16.0;   kernel[2] = 1.0/16.0;\
        kernel[3] = 2.0/16.0;   kernel[4] = 4.0/16.0;   kernel[5] = 2.0/16.0;\
        kernel[6] = 1.0/16.0;   kernel[7] = 2.0/16.0;   kernel[8] = 1.0/16.0;\
        \
        int i = 0;\
        for (i = 0; i < 4; i++){\
        vec4 tmp = texture2DRect(tex, st + offset[i]);\
        sum += tmp * kernel[i];\
        }\
        \
        for (i = 5; i < 9; i++){\
        vec4 tmp = texture2DRect(tex, st + offset[i]);\
        sum += tmp * kernel[i];\
        }\
        \
        vec4 color0 = texture2DRect(tex, st + offset[4]);\
        sum += color0 * kernel[4];\
        \
        gl_FragColor = (1.0 - fade_const) * color0 +  fade_const * vec4(sum.rgb, color0.a);\
        }";
        blurShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentBlurShader);
        blurShader.linkProgram();
    }
    
    swapBuffer  pingPong;
    ofShader    blurShader;
	float       fade;
    int         passes;
};
#endif
