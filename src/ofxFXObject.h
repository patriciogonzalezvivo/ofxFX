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
 *  Definition: Parent class of all other effects. It can be used in order to compile simple shaders such
 *  as the ones you could find at: Ricardo Caballero´s webGL Sandbox http://mrdoob.com/projects/glsl_sandbox/ 
 *  and ShaderToy by Inigo Quilez http://www.iquilezles.org/apps/shadertoy/
 *  
 */   

#ifndef OFXFXOBJECT
#define OFXFXOBJECT

#include "ofMain.h"
#include "ofxSwapBuffer.h"

#define STRINGIFY(A) #A

class ofxFXObject: public ofBaseHasTexture {
public:
    
    ofxFXObject();
    ofxFXObject(ofxFXObject& parent);
    virtual ~ofxFXObject();

    ofxFXObject&    operator =(ofxFXObject& parent);
    ofxFXObject&    operator <<(ofTexture& _texture){ setTexture( _texture ); update(); return * this;};
    ofxFXObject&    operator <<(ofBaseHasTexture& _texture){ setTexture( _texture.getTextureReference() ); update(); return * this;};
    ofTexture&      operator[](int _nText){ if ((_nText < nTextures) && (_nText >= 0) ) return textures[_nText].getTextureReference(); };

    virtual void    allocate(int _width, int _height, int _internalFormat);;
    virtual void    allocate(int _width, int _height);
    
    // Load shaders from your bin/data directory. Works in the same way as ofShader::load.
    // Pass empty strings for the shaders you don't implement.
    virtual bool    loadVersioned(string glESPath = "", string gl2Path = "", string gl3Path = "");
    // Load a single shader by path, you have to make sure yourself the correct
    // GLSL version is used if you call this function.
    virtual bool    load(string path);
    // If you've loaded a shader from file, you can use this to reload it.
    virtual bool    reload(); // reloads the shader from the same file path as above
    // Forcefully sets the shader code to be used.
    virtual bool    setCode(string fragShader, string vertShader = "");
    // Load the shader code into a shader, allocate ofFbos, etc.
    virtual bool    compileCode();

    virtual void    setUseTexture(bool bUseTex){ };
    
    void            setPasses(int _passes) { passes = _passes; };
    void            setInternalFormat(int _internalFormat) { internalFormat = _internalFormat; compileCode(); };
    virtual void    setTexture(ofBaseDraws& tex, int _texNum = 0);
    
    virtual void    begin(int _texNum = 0);
    virtual void    end(int _texNum = 0);
    
    bool            compiled() const{ return bFine; };
    string          getCode() const { return fragmentShader; };
    string          getVertexCode() const { return vertexShader; };
    float           getWidth() const { return width;};
    float           getHeight() const { return height;};
    int             getPasses() const { return passes; };
    ofVec2f         getResolution() const { return ofVec2f(width,height);};
    int             getInternalFormat() const { return internalFormat; };
    int             getNumberOfCalledTextures() const { return nTextures; };
    
    ofxSwapBuffer&  getSwapBuffer() { return pingPong; };
    ofFbo*          getBackBuffer() const { return pingPong.src; };
    virtual ofTexture & getTextureReference() { return pingPong.dst->getTextureReference(); };
    
    void            clear(int alpha = 255){ pingPong.clear(alpha); } 
    
    virtual void    update();
    void            draw(ofRectangle &_rect){ draw(_rect.x,_rect.y,_rect.width,_rect.height);};
    void            draw(int _x = 0, int _y = 0, float _width = -1, float _height = -1);
    
    int             passes;
    
protected:
    virtual void    initFbo(ofFbo & _fbo, int _width, int _height, int _internalformat = GL_RGBA );
    virtual void    renderFrame(float _width = -1, float _height = -1);
    virtual void    injectUniforms() {};
    
    ofxSwapBuffer   pingPong;
    ofFbo           *textures;
    ofShader        shader;
    int             nTextures, internalFormat, width, height;
    bool            bFine;
    bool            bUpdate;

    bool            needsFboResize();
    void            selectShaderSource();
    string          shaderFilePath;

    // Shader Source Strings: These will always contain the strings used
    // for the shader source after compileCode has been run.
    string          fragmentShader;
    string          vertexShader;

    // Constructor Shader Source Strings: These strings can be specified
    // by inheritors in their constructors, and will be loaded when
    // appropriate. If any of the above strings are empty, the ones
    // below are tried. If shaders are loaded from file, these are also
    // ignored.
    string          glESFragmentShader;
    string          glESVertexShader;

    string          gl2FragmentShader;
    string          gl2VertexShader;

    string          gl3VertexShader;
    string          gl3FragmentShader; // Simply ignore setting this if you want a passthrough shader! :D

    // Geometry shaders not supported at the moment!
};

#endif
