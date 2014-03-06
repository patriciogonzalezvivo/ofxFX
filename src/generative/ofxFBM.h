//
//  ofxNoise.h
//
//  Created by Patricio Gonzalez Vivo on 4/12/12.
//  Copyright (c) 2012 http://www.PatricioGonzalezVivo.com. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxFXObject.h"

#define STRINGIFY(A) #A

class ofxFBM : public ofxFXObject {
public:
    
    ofxFBM(){
        zoom = 9.0;
        speed = 0.5;
        internalFormat = GL_RGBA;
        
        fragmentShader = STRINGIFY(uniform  float   zoom;
                                   uniform  float   time;
                                   uniform  float   speed;
                                   uniform  vec2    resolution;
                                   
                                   float rand(vec2 co){
                                       return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
                                   }
                                   
                                   float noise2f( in vec2 p ){
                                       vec2 ip = vec2(floor(p));
                                       vec2 u = fract(p);
                                       u = u*u*(3.0-2.0*u);
                                       
                                       float res = mix(mix(rand(ip),
                                                           rand(ip+vec2(1.0,0.0)),u.x),
                                                       mix(rand(ip+vec2(0.0,1.0)),
                                                           rand(ip+vec2(1.0,1.0)),u.x),
                                                       u.y);
                                       return res*res;
                                   }
                                   
                                   float fbm(vec2 c) {
                                       float f = 0.0;
                                       float w = 1.0;
                                       
                                       for (int i = 0; i < 8; i++) {
                                           f += w*noise2f(c);
                                           c *=2.0;
                                           w*=0.5;
                                       }
                                       return f;
                                   }
                                   
                                   vec2 cMul(vec2 a, vec2 b) {
                                       return vec2( a.x*b.x -  a.y*b.y,a.x*b.y + a.y * b.x);
                                   }
                                   
                                   float pattern(  vec2 p, out vec2 q, out vec2 r ){
                                       q.x = fbm( p  +0.00*time);
                                       q.y = fbm( p + vec2(1.0));
                                       
                                       r.x = fbm( p +1.0*q + vec2(1.7,9.2)+0.15*time );
                                       r.y = fbm( p+ 1.0*q + vec2(8.3,2.8)+0.126*time);
                                       return fbm(p +1.0*r + speed* time);
                                   }
                                   
                                   void main() {
                                       vec2 q;
                                       vec2 r;
                                       vec2 c = zoom*100.0 * gl_FragCoord.xy / resolution.xy;
                                       float noise = pattern( c * 0.01, q, r);
                                       
                                       gl_FragColor = vec4( vec3( noise ), 1.0);
                                   }
                                   );
    };
    
    void setZoom(float _zoom){zoom = _zoom;};
    
    void clear(){
        for(int i = 0; i < 2; i++){
            pingPong[i].begin();
            ofClear(255,255);
            pingPong[i].end();
        }
    }
    
    void update(){
        pingPong.dst->begin();
        ofClear(0,0);
        shader.begin();
        
        shader.setUniform1f("time", ofGetElapsedTimef());
        shader.setUniform1f("zoom", zoom);
        shader.setUniform1f("speed", speed);
        shader.setUniform2f("resolution", (float)width, (float)height);
        shader.setUniformTexture("backbuffer", pingPong.src->getTextureReference(), 0 );
        
        pingPong.src->draw(0,0);
        
        shader.end();
        pingPong.dst->end();
        
        pingPong.swap();
    };
    
    void draw(int _x = 0, int _y = 0, float _width = -1, float _height = -1){
        if (_width == -1) _width = width;
        if (_height == -1) _height = height;
        
        getTextureReference().draw(_x, _y, _width, _height);
    }
    
    float   zoom, speed;
};