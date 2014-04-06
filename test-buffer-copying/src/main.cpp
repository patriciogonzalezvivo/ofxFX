#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"

// comment out the line below if you want to use a fixed pipeline opengl renderer,
// otherwise leave this line uncommented if you want to use a programmable pipeline opengl renderer.
#define USE_PROGRAMMABLE_RENDERER

#ifdef USE_PROGRAMMABLE_RENDERER
#include "ofGLProgrammableRenderer.h"
#endif

//========================================================================
int main( ){

#ifdef USE_PROGRAMMABLE_RENDERER
    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
#endif
    
        ofAppGlutWindow window;
	ofSetupOpenGL(&window, 300*3, 300, OF_WINDOW);
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
