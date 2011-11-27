### ofxFX ###

This addons originaly born while I was working on [Efecto Mariposa](http://patriciogonzalezvivo.com/butterfly.html "Efecto Mariposa") and I need to do fast GPU image processing throught GLSL Shaders.
Basically they are well-known algorithms re-writed on GLSL version 120 for ARB rectangular textures.
They could be optimized A LOT so feel free to collaborate or add more of them.

# Portability #
Each ofxFX class have a fragment, vertex o geometry Shader embebed on it´s code. So you don´t need to add any kind of .frag , .vert or .geom on your /data forlder

# Coherence # 
The implementation of each ofxFX class it´s consistent with each other. All of them are extensions of ofxFXObject

* All of them need to be allocated first in order to 
``fx.allocate(width,height);`

* All of them have a ofxFbo style. So you can draw or render directly inside of them. Allowing you to easyly interact with them
``fx.begin();
ofEllipse(50,50,10,10);
fx.end();´´

Watch some of this classes in action [HERE](http://www.patriciogonzalezvivo.com/blog/?p=488)

![ofxFlocking](http://patriciogonzalezvivo.com/images/flock.png) ![ofxFluid](http://patriciogonzalezvivo.com/images/fluid.png) ![ofxGlow](http://patriciogonzalezvivo.com/images/glow.png) ![ofxGrayScott](http://patriciogonzalezvivo.com/images/grayScott.png) ![ofxWater](http://patriciogonzalezvivo.com/images/water.png)
