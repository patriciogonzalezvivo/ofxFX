//
//  ofxBloom.h
//  emptyExample
//
//  Created by Patricio González Vivo on 25/11/11.
//  Copyright (c) 2011 PatricioGonzalezVivo.com. All rights reserved.
//

#ifndef OFXBLOOM
#define OFXBLOOM


#include "ofMain.h"
#include "ofxFXObject.h"

class ofxBloom : ofxFXObject {	
public:
	void allocate(int width, int height) {
        pingPong.allocate(width,height);
        
        loadShaders();
	}
    
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
        
        pingPong.dst->begin();
        shader.begin();
        pingPong.src->draw(0,0);
        shader.end();
        pingPong.dst->end();
            
        pingPong.swap();
        
        if (bDraw)
            draw();
	}
	
	void draw(int x = 0, int y = 0){ pingPong.dst->draw(x, y);}
    
protected:
    void loadShaders(){
        string fragmentShader = "#version 120\n \
        #extension GL_ARB_texture_rectangle : enable\n \
        \
        uniform sampler2DRect src_tex_unit0;\
        \
        void main(){\
            vec4 sum = vec4(0);\
            vec2 st = gl_TexCoord[0].st;\
            int j;\
            int i;\
            \
            for( i= -4 ;i < 4; i++){\
                for (j = -3; j < 3; j++){\
                    sum += texture2DRect(src_tex_unit0, st + vec2(j, i)*0.004) * 0.25;\
                }\
            }\
            \
            if (texture2DRect(src_tex_unit0, st).r < 0.3){\
                gl_FragColor = sum*sum*0.012 + texture2DRect(src_tex_unit0, st);\
            } else {\
                if (texture2DRect(src_tex_unit0, st).r < 0.5){\
                    gl_FragColor = sum*sum*0.009 + texture2DRect(src_tex_unit0, st);\
                } else {\
                    gl_FragColor = sum*sum*0.0075 + texture2DRect(src_tex_unit0, st);\
                }\
            }\
        }";
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
        shader.linkProgram();
    }
    
    swapBuffer  pingPong;
    ofShader    shader;
};
#endif
