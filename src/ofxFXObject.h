/*
 *  ofxFXObject.h
 *
 *  Created by Patricio González Vivo on 03/07/11.
 *  Copyright 2011 http://www.PatricioGonzalezVivo.com All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 *  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 *  OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 *  OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  ************************************************************************************
 *
 *  Definition: Parent class of all the other effect. It also can be use in order to compile single and simple
 *  shaders as the ones you could find at: Ricardo Caballero´s webGL Sandbox http://mrdoob.com/projects/glsl_sandbox/ 
 *  and ShaderToy by Inigo Quilez http://www.iquilezles.org/apps/shadertoy/
 *  
 */   

#ifndef OFXFXOBJECT
#define OFXFXOBJECT

#include "ofMain.h"
#include "ofxSwapBuffer.h"

#define STRINGIFY(A) #A

class ofxFXObject{
public:
    
    ofxFXObject();
    ofxFXObject(ofxFXObject& parent);
    ~ofxFXObject();
    
    ofxFXObject& operator =(ofxFXObject& parent);
    ofxFXObject& operator >>(ofxFXObject& parent) { parent.setTexture( getTextureReference() ); return * this;};
    ofxFXObject& operator <<(ofxFXObject& parent){ setTexture( parent.getTextureReference() ); return * this;};
    ofTexture& operator[](int _nText){ if ((_nText < nTextures) && (_nText >= 0) ) return textures[_nText].getTextureReference(); else return NULL; };

    virtual void allocate(int _width, int _height, int _internalFormat);;
    void allocate(int _width, int _height);
    
    virtual bool injectShader(string fragShader);
    virtual bool injectShader();
    
    void setPasses(int _passes) { passes = _passes; };
    void setInternalFormat(int _internalFormat) { internalFormat = _internalFormat; injectShader(); };
    void setTexture(ofTexture& tex, int _texNum = 0);
    
    void begin(int _texNum = 0);
	void end(int _texNum = 0);
    
    float       getTime() const {return time;};
    int         getPasses() const { return passes; };
    int         getInternalFormat() const { return internalFormat; };
    ofVec2f     getResolution() const { return ofVec2f(width,height);};
    string      getFragShaderCode() const { return fragmentShader; };
    ofTexture&  getTextureReference() const { return pingPong.dst->getTextureReference(); };
    
    void update();
    void draw(int x = 0, int y = 0, float _width = -1, float _height = -1);
    
protected:
    virtual void initFbo(ofFbo & _fbo, int _width, int _height, int _internalformat = GL_RGBA );
    virtual void renderFrame(float _width = -1, float _height = -1);
    
    ofxSwapBuffer   pingPong;
    ofFbo           *textures;
    ofShader        shader;
    string          fragmentShader;
    float           width, height, time;
    int             nTextures, passes, internalFormat;
};

#endif
