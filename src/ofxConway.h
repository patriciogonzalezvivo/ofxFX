//
//  ofxConway.h
//
//  Created by kalwalt alias Walter Perdan on 24/12/11
//  Copyright 2011 http://www.kalwaltart.it/ All rights reserved.
//

#ifndef OFXCONWAY
#define OFXCONWAY

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxConway : public ofxFXObject {
public:

    ofxConway(){
        passes = 10;
        nTextures = 1;
        internalFormat = GL_RGBA;
        
        ///\brief Implements in GLSL the rules of the Automata
        ///
        ///1. Any live cell with fewer than two live neighbours dies, as if by loneliness.\n
        ///2. Any live cell with more than three live neighbours dies, as if by overcrowding.\n
        ///3. Any live cell with two or three live neighbours lives, unchanged, to the next generationn
        ///4. Any dead cell with exactly three live neighbours comes to life.\n
        
        fragmentShader = "#version 120\n \
        #extension GL_ARB_texture_rectangle : enable\n \
        \
        uniform sampler2DRect backbuffer; \
        \
        vec4 dead = vec4(1.0,1.0,1.0,1.0); \
        vec4 alive = vec4(0.0,0.0,0.0,1.0); \
        \
        void main(void) { \
            vec2  st = gl_TexCoord[0].st;\
            int sum = 0; \
            vec4 y = texture2DRect(backbuffer, st); \
            \
            if (texture2DRect(backbuffer, st + vec2(-1.0, -1.0)) == alive) ++sum; \
            if (texture2DRect(backbuffer, st + vec2(0.0, -1.0)) == alive) ++sum; \
            if (texture2DRect(backbuffer, st + vec2(1.0, -1.0)) == alive) ++sum; \
            \
            if (texture2DRect(backbuffer, st + vec2(-1.0, 0.0)) == alive) ++sum; \
            if (texture2DRect(backbuffer, st + vec2(1.0, 0.0)) == alive) ++sum; \
            \
            if (texture2DRect(backbuffer, st + vec2(-1.0, 1.0)) == alive) ++sum; \
            if (texture2DRect(backbuffer, st + vec2(0.0, 1.0)) == alive) ++sum; \
            if (texture2DRect(backbuffer, st + vec2(1.0, 1.0)) == alive) ++sum; \
            \
            if (sum < 2) gl_FragColor = dead; \
            else if (sum > 3) gl_FragColor = dead; \
            else if (sum == 3) gl_FragColor = alive; \
            else gl_FragColor = y; \
        }";
    };
    
    void end() {
        textures[0].end();
        ofPopStyle();
		ofPopMatrix();
        
        pingPong.src->begin();
        textures[0].draw(0,0);
        pingPong.src->end();
    }
};
#endif
