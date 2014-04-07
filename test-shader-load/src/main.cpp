#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"

// This program should look exactly the same whether this is commented out or not.
// Defined -> OpenGL3 is used. Not defined -> OpenGL2
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
	ofRunApp(new ofApp());

}
