//
//  ofxConway.cpp
//
//  Created by kalwalt alias Walter Perdan on 24/12/11
//  Copyright 2011 http://www.kalwaltart.it/ All rights reserved.
//

#include "ofxConway.h"

ofxConway::ofxConway(){
    iterations = 10;

    ///\brief Implements in GLSL the rules of the Automata
    ///
    ///1. Any live cell with fewer than two live neighbours dies, as if by loneliness.\n
    ///2. Any live cell with more than three live neighbours dies, as if by overcrowding.\n
    ///3. Any live cell with two or three live neighbours lives, unchanged, to the next generationn
    ///4. Any dead cell with exactly three live neighbours comes to life.\n

    string comwayFragmentShader="#version 120\n \
        #extension GL_ARB_texture_rectangle : enable\n \
        \
        uniform sampler2DRect texture_A; \
        \
        int sum; \
        vec4 dead = vec4(1.0,1.0,1.0,1.0); \
        vec4 alive = vec4(0.0,0.0,0.0,1.0); \
        void main(void) { \
        sum=0; \
        vec4 y = texture2DRect(texture_A, gl_TexCoord[0].st); \
        \
        if (texture2DRect(texture_A, gl_TexCoord[0].st + vec2(-1.0, -1.0))==alive) ++sum; \
        if (texture2DRect(texture_A, gl_TexCoord[0].st + vec2(0.0, -1.0))==alive) ++sum; \
        if (texture2DRect(texture_A, gl_TexCoord[0].st + vec2(1.0, -1.0))==alive) ++sum; \
        \
        if (texture2DRect(texture_A, gl_TexCoord[0].st + vec2(-1.0, 0.0))==alive) ++sum; \
        if (texture2DRect(texture_A, gl_TexCoord[0].st + vec2(1.0, 0.0))==alive) ++sum; \
        \
        if (texture2DRect(texture_A, gl_TexCoord[0].st + vec2(-1.0, 1.0))==alive) ++sum; \
        if (texture2DRect(texture_A, gl_TexCoord[0].st + vec2(0.0, 1.0))==alive) ++sum; \
        if (texture2DRect(texture_A, gl_TexCoord[0].st + vec2(1.0, 1.0))==alive) ++sum; \
        \
        if (sum<2) gl_FragColor = dead; \
        else if (sum>3) gl_FragColor = dead; \
        else if (sum==3) gl_FragColor = alive; \
        else gl_FragColor = y; \
        }";

    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, comwayFragmentShader);
    shader.linkProgram();
}

void ofxConway::allocate(int _width, int _height){
    width = _width;
    height = _height;

    pingPong.allocate(width, height,GL_RGBA);

}

void ofxConway::begin(){
    pingPong[0].begin();
}

void ofxConway::end(bool drawIt){
    pingPong[0].end();
    update();
    if (drawIt)
        draw();
}

void ofxConway::update(){
    for( int i = 0; i < iterations ; i++ ){
        pingPong.dst->begin();
        ofClear(0,0,0,255);
        shader.begin();
        shader.setUniformTexture("texture_A", pingPong.src->getTextureReference(), 0 );
        renderFrame();
        shader.end();
        pingPong.dst->end();

        pingPong.swap();
	}
}
