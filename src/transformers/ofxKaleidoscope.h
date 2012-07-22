//
//  ofxBloom.h
//  emptyExample
//
//  Created by Patricio González Vivo on 25/11/11.
//  Copyright (c) 2011 PatricioGonzalezVivo.com. All rights reserved.
//

#ifndef OFXKALEIDOSCOPE
#define OFXKALEIDOSCOPE

#include "ofMain.h"
#include "ofxRotate.h"

class ofxKaleidoscope: public ofxRotate {	
public:
    ofxKaleidoscope(){
        angle   = 0.0;
        setTotalFaces(5);
        
        passes  = 1;
        internalFormat = GL_RGBA;
        
        fragmentShader = "#version 120\n\
        #extension GL_ARB_texture_rectangle : enable\n\
        \n\
        uniform sampler2DRect tex0;\n\
        uniform vec2 offset;\n\
        uniform vec2 size;\n\
        uniform float rotate;\n\
        \n\
        void main(){\n\
        \n\
        vec2 st = gl_TexCoord[0].xy;\n\
        st.x -= size.x*0.5 + offset.x;\n\
        st.y -= size.y*0.5 + offset.y;\n\
        vec2 uv;\n\
        \n\
        float angle = atan(st.y,st.x);\n\
        float radio = sqrt(dot(st,st));\n\
        \n\
        uv.x = cos( angle - rotate) * radio + size.x*0.5 + offset.x;\n\
        uv.y = sin( angle - rotate) * radio + size.y*0.5 + offset.y;\n\
        \n\
        vec3 color = texture2DRect(tex0, uv).rgb;\n\
        gl_FragColor = vec4(color, 1.0);\n\
        }";
        
        offset = ofPoint(0,0);
    }
    
    void setFacesRotation(float _angle){facesRotation = _angle;};
    
    void update(){
        ofPushStyle();
        
        pingPong.dst->begin();
        ofClear(0,0);
        shader.begin();
        shader.setUniformTexture("tex0",  textures[0].getTextureReference(), 0 );
        shader.setUniform2f("size", width,height);
        shader.setUniform2f("offset", offset.x, offset.y);
        shader.setUniform1f("rotate", angle);
        
        ofVec2f radio = ofVec2f(0,MIN(width,height)*-0.5);
        //  A
        ofVec2f A= ofVec2f(width*0.5 + offset.x , height*0.5 + offset.y );
        ofVec2f p[2];
        //  B
        p[0] = radio + A;
        //  C
        p[1] = radio.getRotated(facesAngle) + A;
        
        radio = radio.getRotatedRad(facesRotation);
        
        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(A.x, A.y);
        glVertex3f(A.x, A.y, 0.0);
        for(int i = 0; i < faces+1; i++){    
            glTexCoord2f(p[i%2].x, p[i%2].y);
            ofVec2f e = radio.getRotated(facesAngle*i) + A;
            glVertex3f(e.x,e.y, 0.0);
        } 
        glEnd();
        
        shader.end();
        pingPong.dst->end();
        
        pingPong.swap();
        
        ofPopStyle();
	}
    
    void    setTotalFaces(float _faces){
        faces = _faces*2;
        facesAngle = 360/faces;
    };

    ofPoint offset;
private:
    int     faces;
    float   facesAngle;
    float   facesRotation;
    
};
#endif
