//
//  ofxBlur.h
//
//  Created by Patricio González Vivo on 25/11/11.
//  Copyright (c) 2011 PatricioGonzalezVivo.com. All rights reserved.
//

#ifndef OFXBLUR
#define OFXBLUR

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxBlur : public ofxFXObject {	
public:
    
	void allocate(int width, int height) {
        pingPong.allocate(width,height);
        
        passes = 1;
        radius = 3;
        
        loadShaders();
	}
    
	ofxBlur& setPasses(int _passes) { this->passes = _passes; return * this;};
	ofxBlur& setRadius(float _radius) { this->radius = _radius; return * this;};
    
    ofTexture& getTextureReference() { return pingPong[1].getTextureReference(); };

	void begin() {
		ofPushStyle();
		ofPushMatrix();
        pingPong[1].begin();
	}
	
	void end(bool bDraw = true) {
        pingPong[1].end();
        
        ofPopStyle();
		ofPopMatrix();
        
        for(int i = 0; i < passes; i++) {
            for(int j = 0; j < 2; j++) {    
                pingPong[j].begin();
                blurShader[j].begin();
                blurShader[j].setUniform1f("radius", radius);
                pingPong[(j+1)%2].draw(0,0);
                blurShader[j].end();
                pingPong[j].end();
            }
        }
        
        if (bDraw)
            draw();
	}
	
	void draw(int x = 0, int y = 0){ pingPong[1].draw(x, y);}
    
protected:
    void loadShaders(){
        string fragmentHorizontalBlurShader = "#version 120\n \
    	#extension GL_ARB_texture_rectangle : enable\n \
    	\
		uniform sampler2DRect tex;\
		uniform float radius;\
		\
		const float total = (1. + 8. + 28. + 56.) * 2. + 70.;\
		\
		void main(void) {\
        vec2 st = gl_TexCoord[0].st;\
        \
        gl_FragColor += (1. / total) * texture2DRect(tex, st - radius * vec2(4. / 4., 0.));\
        gl_FragColor += (8. / total)  * texture2DRect(tex, st - radius * vec2(3. / 4., 0.));\
        gl_FragColor += (28. / total)  * texture2DRect(tex, st - radius * vec2(2. / 4., 0.));\
        gl_FragColor += (56. / total)  * texture2DRect(tex, st - radius * vec2(1. / 4., 0.));\
        \
        gl_FragColor +=  (70. / total) * texture2DRect(tex, st);\
        \
        gl_FragColor += (1. / total) * texture2DRect(tex, st + radius * vec2(4. / 4., 0.));\
        gl_FragColor += (8. / total)  * texture2DRect(tex, st + radius * vec2(3. / 4., 0.));\
        gl_FragColor += (28. / total)  * texture2DRect(tex, st + radius * vec2(2. / 4., 0.));\
        gl_FragColor += (56. / total)  * texture2DRect(tex, st + radius * vec2(1. / 4., 0.));\
		}";
        blurShader[0].setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentHorizontalBlurShader);
    	blurShader[0].linkProgram();
        
		string fragmentVerticalBlurShader = "#version 120\n \
    	#extension GL_ARB_texture_rectangle : enable\n \
    	\
		uniform sampler2DRect tex;\
		uniform float radius;\
		\
		const float total = (1. + 8. + 28. + 56.) * 2. + 70.;\
		\
		void main(void) {\
        vec2 st = gl_TexCoord[0].st;\
        \
        gl_FragColor += (1. / total) * texture2DRect(tex, st - radius * vec2(0., 4. / 4.));\
        gl_FragColor += (8. / total)  * texture2DRect(tex, st - radius * vec2(0., 3. / 4.));\
        gl_FragColor += (28. / total)  * texture2DRect(tex, st - radius * vec2(0., 2. / 4.));\
        gl_FragColor += (56. / total)  * texture2DRect(tex, st - radius * vec2(0., 1. / 4.));\
        \
        gl_FragColor +=  (70. / total) * texture2DRect(tex, st);\
        \
        gl_FragColor += (1. / total) * texture2DRect(tex, st + radius * vec2(0., 4. / 4.));\
        gl_FragColor += (8. / total)  * texture2DRect(tex, st + radius * vec2(0., 3. / 4.));\
        gl_FragColor += (28. / total)  * texture2DRect(tex, st + radius * vec2(0., 2. / 4.));\
        gl_FragColor += (56. / total)  * texture2DRect(tex, st + radius * vec2(0., 1. / 4.));\
		}";
        blurShader[1].setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentVerticalBlurShader);
    	blurShader[1].linkProgram();
    }
    
    swapBuffer pingPong;
    ofShader blurShader[2];
    
	float   radius;
    int     passes;
};
#endif