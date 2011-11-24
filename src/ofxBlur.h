#ifndef OFXBLUR
#define OFXBLUR

#include "ofMain.h"

class ofxBlur {
private:
	ofFbo ping, pong;
	ofShader horizontalBlur, verticalBlur;
	int passes;
	float radius;
	
public:
	void setup(int width, int height) {
		ping.allocate(width, height);
		pong.allocate(width, height);

		string fragmentHorizontalBlurShader = "#version 120\n \
    	#extension GL_ARB_texture_rectangle : enable\n \
    	\
		uniform sampler2DRect tex;\
		uniform float radius;\
		\
		const float total = (1. + 8. + 28. + 56.) * 2. + 70.;\
		\
		void main(void) {\
			vec2 st = gl_TexCoord[0].st;\
			\
			gl_FragColor += (1. / total) * texture2DRect(tex, st - radius * vec2(4. / 4., 0.));\
			gl_FragColor += (8. / total)  * texture2DRect(tex, st - radius * vec2(3. / 4., 0.));\
			gl_FragColor += (28. / total)  * texture2DRect(tex, st - radius * vec2(2. / 4., 0.));\
			gl_FragColor += (56. / total)  * texture2DRect(tex, st - radius * vec2(1. / 4., 0.));\
			\
			gl_FragColor +=  (70. / total) * texture2DRect(tex, st);\
			\
			gl_FragColor += (1. / total) * texture2DRect(tex, st + radius * vec2(4. / 4., 0.));\
			gl_FragColor += (8. / total)  * texture2DRect(tex, st + radius * vec2(3. / 4., 0.));\
			gl_FragColor += (28. / total)  * texture2DRect(tex, st + radius * vec2(2. / 4., 0.));\
			gl_FragColor += (56. / total)  * texture2DRect(tex, st + radius * vec2(1. / 4., 0.));\
		}";
    	horizontalBlur.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentHorizontalBlurShader);
    	horizontalBlur.linkProgram();

		string fragmentVerticalBlurShader = "#version 120\n \
    	#extension GL_ARB_texture_rectangle : enable\n \
    	\
		uniform sampler2DRect tex;\
		uniform float radius;\
		\
		const float total = (1. + 8. + 28. + 56.) * 2. + 70.;\
		\
		void main(void) {\
			vec2 st = gl_TexCoord[0].st;\
			\
			gl_FragColor += (1. / total) * texture2DRect(tex, st - radius * vec2(0., 4. / 4.));\
			gl_FragColor += (8. / total)  * texture2DRect(tex, st - radius * vec2(0., 3. / 4.));\
			gl_FragColor += (28. / total)  * texture2DRect(tex, st - radius * vec2(0., 2. / 4.));\
			gl_FragColor += (56. / total)  * texture2DRect(tex, st - radius * vec2(0., 1. / 4.));\
			\
			gl_FragColor +=  (70. / total) * texture2DRect(tex, st);\
			\
			gl_FragColor += (1. / total) * texture2DRect(tex, st + radius * vec2(0., 4. / 4.));\
			gl_FragColor += (8. / total)  * texture2DRect(tex, st + radius * vec2(0., 3. / 4.));\
			gl_FragColor += (28. / total)  * texture2DRect(tex, st + radius * vec2(0., 2. / 4.));\
			gl_FragColor += (56. / total)  * texture2DRect(tex, st + radius * vec2(0., 1. / 4.));\
		}";
    	verticalBlur.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentVerticalBlurShader);
    	verticalBlur.linkProgram();
	}
	
	void setPasses(int passes) {
		this->passes = passes;
	}
	void setRadius(float radius) {
		this->radius = radius;
	}

	void begin() {
		ofPushStyle();
		ofPushMatrix();
		pong.begin();
	}
	
	void end(bool draw = true) {
		pong.end();
		
		glColor4f(1, 1, 1, 1);
		for(int i = 0; i < passes; i++) {	
			ping.begin();
			horizontalBlur.begin();
			horizontalBlur.setUniform1f("radius", radius);
			pong.draw(0, 0);
			horizontalBlur.end();
			ping.end();
			
			pong.begin();
			verticalBlur.begin();
			verticalBlur.setUniform1f("radius", radius);
			ping.draw(0, 0);
			verticalBlur.end();
			pong.end();
		}
		
		if(draw) {
			pong.draw(0, 0);
		}
		ofPopStyle();
		ofPopMatrix();
	}
	
	ofTexture& getTexture() {
		return pong.getTextureReference();
	}
	
	void draw(){
		pong.draw(0, 0);
	}
};
#endif