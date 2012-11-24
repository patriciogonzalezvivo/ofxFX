//
//  ofxBloom.h
//  emptyExample
//
//  Created by Patricio González Vivo on 25/11/11.
//  Copyright (c) 2011 PatricioGonzalezVivo.com. All rights reserved.
//

#ifndef OFXROTATE
#define OFXROTATE

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxRotate : public ofxFXObject {
public:
    ofxRotate(){
        angle   = 0.0;
        passes  = 1;
        internalFormat = GL_RGBA;
        
        fragmentShader = STRINGIFY(uniform sampler2DRect tex0;
                                   uniform vec2 size;
                                   uniform float rotate;
                                   
                                   void main(){
                                       
                                       vec2 st = gl_TexCoord[0].xy;
                                       st.x -= size.x*0.5;
                                       st.y -= size.y*0.5;
                                       vec2 uv;
                                       
                                       float angle = atan(st.y,st.x);
                                       float radio = sqrt(dot(st,st));
                                       
                                       uv.x = cos( angle - rotate) * radio + size.x*0.5;
                                       uv.y = sin( angle - rotate) * radio + size.y*0.5;
                                       
                                       vec3 color = texture2DRect(tex0, uv).rgb;
                                       gl_FragColor = vec4(color, 1.0);
                                   });
    }

    void    setRotation(float _angle){angle = _angle;};
    
    void update(){
        ofPushStyle();
        
        pingPong.dst->begin();
        shader.begin();
        shader.setUniformTexture("tex0",  textures[0].getTextureReference(), 0 );
        shader.setUniform2f("size", width,height);
        shader.setUniform1f("rotate", angle);
        renderFrame();
        shader.end();
        pingPong.dst->end();
        
        pingPong.swap();
        
        ofPopStyle();
	}
    
    float   angle;
};
#endif
