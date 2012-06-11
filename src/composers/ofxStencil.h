//
//  ofxStencil.h
//  TUIOpainter
//
//  Created by Patricio González Vivo on 6/9/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//

#ifndef TUIOpainter_ofxStencil_h
#define TUIOpainter_ofxStencil_h

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxStencil : public ofxFXObject {	
public:
    ofxStencil(){
        passes = 1;
        internalFormat = GL_RGBA;
        
        fragmentShader = "#version 120\n\
        #extension GL_ARB_texture_rectangle : enable\n\
        \n\
        uniform sampler2DRect tex0;\n\
        uniform sampler2DRect tex1;\n\
        \n\
        void main (void){\n\
            vec2 st = gl_TexCoord[0].st;\n\
            vec4 image = texture2DRect(tex0, st);\n\
            vec4 stencil = texture2DRect(tex1, st);\n\
            \n\
            gl_FragColor = vec4(image.rgb*(1.0-stencil.r)*image.a, (1.0-stencil.r)*image.a);\n\
        }\n";
    }
    
    void    begin(int _texNum = 0 ) {
        if ((_texNum < nTextures) && ( _texNum >= 0)){
            ofPushStyle();
            ofPushMatrix();
            textures[_texNum].begin();
            ofClear(0,0);
        }
    }
    
    void    end(int _texNum = 0) {
        if ((_texNum < nTextures) && ( _texNum >= 0)){
            textures[_texNum].end();
            ofPopMatrix();
            ofPopStyle();
        }
    }
    
    void    update(){
        ofEnableAlphaBlending();
        
        pingPong.dst->begin();
        ofClear(0,0);
        ofSetColor(255,255);
        
        shader.begin();
        shader.setUniformTexture("tex0", textures[0].getTextureReference(), 0);
        shader.setUniformTexture("tex1", textures[1].getTextureReference(), 1);
        renderFrame();
        shader.end();
        pingPong.dst->end();
    };
    
    void    draw(int _x, int _y){
        pingPong.dst->draw(_x,_y);
    }
};

#endif
