# ShaderFun
A ShaderToy like standalone Fun  

You can test your fragment shaders by editing the file "frag.glsl".  

# INPUTS  
## UNIFORMS  
### TIME  
#### time  
`uniform in float time;`  

* The float value "time" stores the running time in seconds.  
* Its precision is microseconds.  

### SCREEN SIZE  
#### dims  
`uniform vec2 dims;`  

* The vec2 "dims" stores the screen size 
    * x = width  
    * y = height  

#### rdims
`uniform vec2 rdims;`  

* The value "rdims" stores the reciproke screen sizes rdims = 1. / dims.  

### MOUSE  
#### mouse  
`uniform vec2 mouse;`  

* The vec2 "mouse" stores the current position of the mouse cursor.  

## VERTEX ATTRIBUTES  
### POSITION  
#### pos  
`in vec2 pos;`  

* The vec2 "pos" stores the fragment coordinates in screen space.  
