//
//  ofxBlur.h
//
//  Created by Patricio González Vivo on 25/11/11.
//  Copyright (c) 2011 http://PatricioGonzalezVivo.com All rights reserved.
//

#ifndef OFXBLUR
#define OFXBLUR

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxBlur : public ofxFXObject {	
public:
    ofxBlur(){
        passes = 1;
        nTextures = 1;
        internalFormat = GL_RGBA;
        
        radius = 10;
    }
    
	ofxBlur& setPasses(int _passes) { this->passes = _passes; return * this;};
	ofxBlur& setRadius(float _radius) { this->radius = _radius; return * this;};
    
    void update(){
        ofPushStyle();
        
        pingPong.src->begin();
        textures[0].draw(0,0);
        pingPong.src->end();
        
        for(int i = 0; i < passes; i++) {
            for(int j = 0; j < 2; j++) {    
                pingPong[(j+1)%2].begin();
                blurShader[j].begin();
                blurShader[j].setUniformTexture("backbuffer", pingPong.src->getTextureReference(), 0 );
                blurShader[j].setUniform1f("radius", radius);
                renderFrame();
                blurShader[j].end();
                pingPong[(j+1)%2].end();
            }
            
            pingPong.swap();
        }
        
        pingPong.swap();
    
        ofPopStyle();
	}

protected:
    void injectShader(){
        textures = new ofFbo[nTextures];
        for( int i = 0; i < nTextures; i++){
            initFbo(textures[i], width, height, internalFormat);
        }
         
        string fragmentHorizontalBlurShader = "#version 120\n \
    	#extension GL_ARB_texture_rectangle : enable\n \
    	\
		uniform sampler2DRect backbuffer;\
		uniform float radius;\
		\
		const float total = (1. + 8. + 28. + 56.) * 2. + 70.;\
		\
		void main(void) {\
            vec2 st = gl_TexCoord[0].st;\
            \
            gl_FragColor += (1. / total) * texture2DRect(backbuffer, st - radius * vec2(4. / 4., 0.));\
            gl_FragColor += (8. / total)  * texture2DRect(backbuffer, st - radius * vec2(3. / 4., 0.));\
            gl_FragColor += (28. / total)  * texture2DRect(backbuffer, st - radius * vec2(2. / 4., 0.));\
            gl_FragColor += (56. / total)  * texture2DRect(backbuffer, st - radius * vec2(1. / 4., 0.));\
            \
            gl_FragColor +=  (70. / total) * texture2DRect(backbuffer, st);\
            \
            gl_FragColor += (1. / total) * texture2DRect(backbuffer, st + radius * vec2(4. / 4., 0.));\
            gl_FragColor += (8. / total)  * texture2DRect(backbuffer, st + radius * vec2(3. / 4., 0.));\
            gl_FragColor += (28. / total)  * texture2DRect(backbuffer, st + radius * vec2(2. / 4., 0.));\
            gl_FragColor += (56. / total)  * texture2DRect(backbuffer, st + radius * vec2(1. / 4., 0.));\
		}";
        blurShader[0].unload();
        blurShader[0].setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentHorizontalBlurShader);
    	blurShader[0].linkProgram();
        
		string fragmentVerticalBlurShader = "#version 120\n \
    	#extension GL_ARB_texture_rectangle : enable\n \
            \
            uniform sampler2DRect backbuffer;\
            uniform float radius;\
            \
            const float total = (1. + 8. + 28. + 56.) * 2. + 70.;\
            \
            void main(void) {\
            vec2 st = gl_TexCoord[0].st;\
            \
            gl_FragColor += (1. / total) * texture2DRect(backbuffer, st - radius * vec2(0., 4. / 4.));\
            gl_FragColor += (8. / total)  * texture2DRect(backbuffer, st - radius * vec2(0., 3. / 4.));\
            gl_FragColor += (28. / total)  * texture2DRect(backbuffer, st - radius * vec2(0., 2. / 4.));\
            gl_FragColor += (56. / total)  * texture2DRect(backbuffer, st - radius * vec2(0., 1. / 4.));\
            \
            gl_FragColor +=  (70. / total) * texture2DRect(backbuffer, st);\
            \
            gl_FragColor += (1. / total) * texture2DRect(backbuffer, st + radius * vec2(0., 4. / 4.));\
            gl_FragColor += (8. / total)  * texture2DRect(backbuffer, st + radius * vec2(0., 3. / 4.));\
            gl_FragColor += (28. / total)  * texture2DRect(backbuffer, st + radius * vec2(0., 2. / 4.));\
            gl_FragColor += (56. / total)  * texture2DRect(backbuffer, st + radius * vec2(0., 1. / 4.));\
		}";
        blurShader[1].unload();
        blurShader[1].setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentVerticalBlurShader);
    	blurShader[1].linkProgram();
    }
    
    ofShader    blurShader[2];
    
	float   radius;
    int     passes;
};
#endif