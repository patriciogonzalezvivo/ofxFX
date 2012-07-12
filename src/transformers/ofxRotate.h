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
        
        fragmentShader = "#version 120\n\
#extension GL_ARB_texture_rectangle : enable\n\
\n\
uniform sampler2DRect tex0;\n\
uniform vec2 size;\n\
uniform float rotate;\n\
\n\
void main(){\n\
        \n\
        vec2 st = gl_TexCoord[0].xy;\n\
        st.x -= size.x*0.5;\n\
        st.y -= size.y*0.5;\n\
        vec2 uv;\n\
        \n\
        float angle = atan(st.y,st.x);\n\
        float radio = sqrt(dot(st,st));\n\
        \n\
        uv.x = cos( angle - rotate) * radio + size.x*0.5;\n\
        uv.y = sin( angle - rotate) * radio + size.y*0.5;\n\
        \n\
        vec3 color = texture2DRect(tex0, uv).rgb;\n\
        gl_FragColor = vec4(color, 1.0);\n\
}";
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
