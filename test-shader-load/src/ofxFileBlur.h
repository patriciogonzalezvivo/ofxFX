#pragma once

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxFileBlur : public ofxFXObject {
    public:
        float fade;
        void setFade(float _fade) { fade = _fade;};

        ofxFileBlur(){
            passes = 3;
            internalFormat = GL_RGBA;
            fade = 0.03f;
            loadVersioned("", "blur_gl2", "blur_gl3");
        }

    protected:
        void injectUniforms() {
            shader.setUniform1f("fade", fade);
        }
};
