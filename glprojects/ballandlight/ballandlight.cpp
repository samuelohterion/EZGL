#include "ballandlight.hpp"

typedef glm::vec3 V3;
typedef glm::mat4 M4;
BallAndLight::BallAndLight( CStr const & p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ) {
   ballintr = new Interaction(glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,3,-4), glm::vec3(0,-2.981,0));
}
void
BallAndLight::init( ) {

    glClearColor( .0f, .0f, .0f, 1.f );
    glEnable( GL_DEPTH_TEST );
    glDisable( GL_CULL_FACE );
    glDepthFunc( GL_LESS );

    m = v = p = glm::mat4( 1. );
    nrm = glm::mat3( 1. );
    light = glm::vec3(0,0,.5);

    glr.vertices( "VERTICES-BALLANDLIGHT" ).
        setUsage( GL_STATIC_DRAW ).
        addAttrib( "vertex", 3, 0 ).addAttrib( "color", 3, 3 ).addAttrib("normal", 3, 6);

    GLRenderer::VertexArray
    & va = glr.vertices( "VERTICES-BALLANDLIGHT" );

    int
    longS = 64,
    latS = 32,
    vxCount = 1,
    latd = 1;
    float
    rad = 1,
    factLong = 2.f*3.14159265f/longS,
    factLat = 3.14159265f/latS,
    x,y,z;

    glm::vec3
    normal = glm::vec3( rad*1.f, 0.f, 0.f ),
    color = glm::vec3( .1, .7, .5);
    va << normal << color<< normal;

    float
    cl = cosf(latd*factLat),
    sl = sinf(latd*factLat);

    for(int longid = longS; longid >= 0; --longid){
        x = rad * cl;
        y = rad* sl*sinf(factLong*longid);
        z = rad* sl*cosf(factLong*longid);
        va << x << y << z << color << x << y << z;
        ++vxCount;
    }
    va << GLRenderer::VertexArray::Object( 0, vxCount, GL_TRIANGLE_FAN );
    int off = vxCount;

    for (latd; latd < latS; latd++){

        float
        sa = sinf((latd-1)*factLat),
        ca = cosf((latd-1)*factLat),
        sb = sinf((latd-0)*factLat),
        cb = cosf((latd-0)*factLat);

        for(int longid = 0; longid <=longS; ++longid){
            float
            xA = rad*cb,
            yA = rad*sb*sinf(factLong*longid),
            zA = rad*sb*cosf(factLong*longid),
            xB = rad*ca,
            yB = rad*sa*sinf(factLong*(1+longid)),
            zB = rad*sa*cosf(factLong*(1+longid));

            va << xA << yA << zA << color << xA << yA << zA<<
                  xB << yB << zB << color << xB << yB << zB;

            vxCount +=2;
        }

     }
     va << GLRenderer::VertexArray::Object( off, vxCount, GL_TRIANGLE_STRIP );

     off = vxCount;

     ++vxCount;
     ++latd;
     cl = cosf(latd*factLat);
     sl = sinf(latd*factLat);

        for(int longid = 0; longid <= longS; ++longid){
            x = rad * cl;
            y = rad* sl*sinf(factLong*longid);
            z = rad* sl*cosf(factLong*longid);
            va << x << y << z << color << x << y << z;
            if(longid==0)
                 va << -normal.x << normal.y << normal.z << color << -normal.x << normal.y << normal.z;
           ++vxCount;

        }

    va << GLRenderer::VertexArray::Object( off, vxCount, GL_TRIANGLE_FAN );

    glr.vertices( "VERTICES-FlOOR" ).
        setUsage( GL_STATIC_DRAW ).
        addAttrib( "vertex", 3, 0 ).addAttrib( "color", 3, 3 ).addAttrib("normal", 3, 6);

    GLRenderer::VertexArray
    & vb = glr.vertices( "VERTICES-FLOOR" );

    color = glm::vec3(0.1,0.2,0.8);
    normal = glm::vec3(0,1,0);

    vb << glm::vec3(-1, -1, 0) << color << normal <<
          glm::vec3( 1, -1, 0) << color << normal <<
          glm::vec3( 1,  1, 0) << color << normal <<
          glm::vec3(-1,  1, 0) << color << normal;

    vb << GLRenderer::VertexArray::Object( 0, 4, GL_TRIANGLE_STRIP );


    glr.shader(
        "SHADER-FLOOR",
        //Vertex Shader
        "#version 330 core\n"
        "layout( location = 0 ) in vec3 vertex;\n"
        "layout( location = 1 ) in vec3 color;\n"
        "layout( location = 2 ) in vec3 normal;\n"

        "uniform float time;\n"
        "uniform mat4 mv; \n"
        "uniform mat4 v; \n"
        "uniform mat4 p;\n"
        "uniform mat4 m;\n"
        "uniform vec3 light;\n"
        "out vec3 vColor;\n"
        "out vec4 vNormal;\n"
        "out vec4 vLight;\n"
        "out vec4 vVert;\n"
        " \n"
        "void main( void ) {\n"
        "    vColor = color;\n"
        "    mat4 nrm = transpose( inverse( mv ) );\n"
        "    vNormal  = normalize( nrm * ( vec4( normal, 0 ) ) ) ;\n"
        "    vLight  = m* vec4( light.xyz, 1 );\n"
        "    vVert   = mv * vec4(vertex, 1 ); \n"
        "    gl_Position = p * mv * vec4( vertex, 1. );\n"
        "}\n",

        //Fragment Shader
        "#version 330 core\n"
        "in vec3 vColor;\n"
        "in vec4 vVert;\n"
        "in vec4 vNormal;\n"
        "in vec4 vLight;\n"
        "out vec4 fColor;\n"
        "void main( void ) {\n"
            "vec3  d = vLight.xyz - vVert.xyz; \n"
            "float a =10*dot(d, vNormal.xyz)/dot(d,d); \n"
            "fColor = vec4( vColor,  1 );\n"
        "}\n",
            GLRenderer::ShaderCode::FROM_CODE ).
            addUniform( "mv", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & mv ).
            addUniform( "m", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & v ).
            addUniform( "p", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & p ).
            addUniform( "m", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & m ).
            addUniform( "light", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & light ).
            addUniform( "time", GLRenderer::Shader::FLOAT, GLRenderer::Shader::SCALAR, & vcd->time );

    glr.program( "PROGRAM-FLOOR" ).
        setVertexArray( "VERTICES-FLOOR" ).
        setShader( "SHADER-FLOOR" ).
        build( );

    glr.shader(
        "SHADER-BALLANDLIGHT",
        //Vertex Shader
        "#version 330 core\n"
        "layout( location = 0 ) in vec3 vertex;\n"
        "layout( location = 1 ) in vec3 color;\n"
        "layout( location = 2 ) in vec3 normal;\n"

        "uniform float time;\n"
        "uniform mat4 mv; \n"
        "uniform mat4 v; \n"
        "uniform mat4 p;\n"
        "uniform mat4 m;\n"
        "uniform vec3 light;\n"
        "out vec3 vColor;\n"
        "out vec4 vNormal;\n"
        "out vec4 vLight;\n"
        "out vec4 vVert;\n"
        " \n"
        "void main( void ) {\n"
        "    vColor = color;\n"
        "    mat4 nrm = transpose( inverse( mv ) );\n"
        "    vNormal  = normalize( nrm * ( vec4( normal, 0 ) ) ) ;\n"
        "    vLight  = m* vec4( light.xyz, 1 );\n"
        "    vVert   = mv * vec4(vertex, 1 ); \n"
        "    gl_Position = p * mv * vec4( vertex, 1. );\n"
        "}\n",

        //Fragment Shader
        "#version 330 core\n"
        "in vec3 vColor;\n"
        "in vec4 vVert;\n"
        "in vec4 vNormal;\n"
        "in vec4 vLight;\n"
        "out vec4 fColor;\n"
        "void main( void ) {\n"
            "vec3  d = vLight.xyz - vVert.xyz; \n"
            "float a =10*dot(d, vNormal.xyz)/dot(d,d); \n"
            "fColor = vec4( clamp(a*vColor.x,0,1),clamp(a*vColor.y,0,1), clamp(a*vColor.z,0,1),  1 );\n"
        "}\n",

        GLRenderer::ShaderCode::FROM_CODE ).
            addUniform( "mv", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & mv ).
            addUniform( "m", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & v ).
            addUniform( "p", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & p ).
            addUniform( "m", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & m ).
            addUniform( "light", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & light ).
            addUniform( "time", GLRenderer::Shader::FLOAT, GLRenderer::Shader::SCALAR, & vcd->time );

    glr.program( "PROGRAM-BALLANDLIGHT" ).
        setVertexArray( "VERTICES-BALLANDLIGHT" ).
        setShader( "SHADER-BALLANDLIGHT" ).
        build( );
        ballintr->curTime = vcd->time;
    }

void
BallAndLight::paint( ) {

    float
    angle = 2* vcd->time;

    light = glm::vec3(0,0, .5);
    glm::vec3 pos = ballintr->Bounds(vcd->time);

    v = glm::mat4( 1. );
    v = glm::translate( glm::mat4( 1. ), pos);

    v = glm::rotate( v, angle, glm::vec3( 0.f, 1.f, 0.f ) );
    mv = v * m;

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glr.run( { "PROGRAM-BALLANDLIGHT" } );
    glr.run( { "PROGRAM-FLOOR" } );
}
void
BallAndLight::resize( int p_width, int p_height ) {
    float
    ratio = ( 1.f * p_width / p_height );
    p = glm::perspective( 45.0f, ratio, 1.0f, 10.f );
}

