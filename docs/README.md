# ShaderFun
A ShaderToy like standalone Fun  

You can test your fragment shaders by editing the file "frag.glsl".  

# INPUTS  
## UNIFORMS  
### TIME  
#### time  
`uniform in float time;`  

The float value "time" stores the running time in seconds.  
Its precision is microseconds.  

### SCREEN SIZE  
#### dims  
`unfiorm vec2 dims;`  

The float value "dims.x" is the screen width.  
The float value "dims.y" is the screen height.  
`unfiorm vec2 rdims;`  

The value "dims.x" is the reciproke screen width = 1. / dims.x.  
The value "dims.y" is the reciproke screen height = 1. / dims.y.  

### MOUSE  
#### mouse  
`uniform vec2 mouse;`  

The vec2 "mouse" stores the current position of the mouse cursor.  

## VERTEX ATTRIBUTES  
### POSITION  
#### pos  
`in vec2 pos;`  

The vec2 "pos" stores the fragment coordinates in screen space.  
