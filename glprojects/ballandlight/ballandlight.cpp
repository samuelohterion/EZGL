#include "ballandlight.hpp"

typedef glm::vec3 V3;
typedef glm::mat4 M4;
BallAndLight::BallAndLight( CStr const & p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ) {
   tex = false;
   ballintr = new Interaction(glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(-.5,3,-5), glm::vec3(0,-2.981,0), .8);

}
void
BallAndLight::init( ) {

	glClearColor( .0f, .0f, .0f, 1.f );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
	glDepthFunc( GL_LESS );

	m = v = p = glm::mat4( 1. );
	nrm = glm::mat3( 1. );
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
	rad = .8,
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
	& vb = glr.vertices( "VERTICES-FlOOR" );

	color  = glm::vec3(1.f, 0.f, 0.f);
	normal = glm::vec3(0.f, 0.f, 1.f);

	vb << glm::vec3(-1, -1, -3) << color << normal <<
		  glm::vec3( 1, -1, -3) << color << normal <<
		  glm::vec3( 1,  3, -3) << color << normal <<
		  glm::vec3(-1,  3, -3) << color << normal;

	vb << GLRenderer::VertexArray::Object( 0, 4, GL_TRIANGLE_FAN);

	normal = glm::vec3(0.f, 1.f, 0.f);

	vb << glm::vec3(-1, -1,  2) << color << normal <<
		  glm::vec3( 1, -1,  2) << color << normal <<
		  glm::vec3( 1, -1, -3) << color << normal <<
		  glm::vec3(-1, -1, -3) << color << normal ;


	vb << GLRenderer::VertexArray::Object( 4, 4, GL_TRIANGLE_FAN);


	glr.vertices( "VERTICES-CUBE" ).
		setUsage( GL_STATIC_DRAW ).
		addAttrib( "vertex", 3, 0 ).addAttrib("normal", 3, 3).addAttrib("coord", 2, 6;

	GLRenderer::VertexArray
	& vc = glr.vertices( "VERTICES-CUBE" );
	float b = 1.;
	glm::vec3
			vUl (-b, -b, b),
			vUr ( b,-b,  b),
			vOr ( b, b,  b),
			vOl (-b, b,  b),
			hUl (-b,-b, -b),
			hUr ( b,-b, -b),
			hOr ( b, b, -b),
			hOl (-b, b, -b);

//front:
	
	glm::vec3 normal (0,0,1);
	
	vc << vUl << normal << glm::vec2(.5, 0.) << vUr << normal << glm::vec2(.75, 0) << vOr << normal << glm::vec2(.75, .25)
	   << vUl << normal << glm::vec2(.5, 0.) << vOr << normal << glm:.vec2(.75, .25) << vOl << normal << glm:.vec2(.5, .25);

	vc << GLRenderer::VertexArray::Object( 0, 6, GL_TRIANGLES);
//left 
	normal = glm:vec3(-1,0,0);
	
	vc << vUl << normal << glm::vec2(.25, .25.) << vOl << normal << glm::vec2(.5, .25) << hOl << normal << glm::vec2(.5, .5)
	   << vUl << normal << glm::vec2(.25, .25.) << hOl << normal << glm::vec2(.5, .5) << hUl << normal << glm::vec2(.25, .5)

	vc << GLRenderer::VertexArray::Object( 6, 12, GL_TRIANGLES);
	
//bottom	
	normal = glm:vec3(0,-1,0);
	
	vc << vUl << normal << glm::vec2(.25, .25.) << vUr << normal << glm::vec2(0., .25) << hUr << normal << glm::vec2(0, .5)
	   << vUl << normal << glm::vec2(.25, .25.) << hUr << normal << glm::vec2(0., .5) <<  hUl << normal << glm::vec2(.25, .5)

	vc << GLRenderer::VertexArray::Object( 12, 16, GL_TRIANGLES);


	glr.frameBuffer( "FRAMEBUFFER-TEXTURE" );

	glr.texture(	"TEXTURE-CUBE",
			new GLRenderer::Texture( "txCube", "../EZGL/glprojects/ballandlight/pix/wuerfelnetz.jpg" ) );

	glr.shader(
		"SHADER-CUBE",
		//Vertex Shader
		"#version 330 core\n"
		"layout( location = 0 ) in vec3 vertex;\n"
		"layout( location = 1 ) in vec3 normal;\n"
		"layout( location = 2 ) in vec2 coord;\n"

		"uniform mat4 mv; \n"
		"uniform mat4 v; \n"
		"uniform mat4 p;\n"
		"uniform vec3 light;\n"
	
		"out VS2FS {\n"
		 "	vec3 vertex, normal;\n"
		 " 	vec4 light;\n"
		 "	vec2 coord;"
		 "	} vs2gs;\n"
		" \n"
		"void main( void ) {\n"
		"	vs2gs.normal   = normalize( transpose(inverse(mv))*vec4(normal,0)).xzy;\n"
		"    	vs2fs.light    = v* vec4( light.xyz, 1 );\n"
		"   	vs2fs.vertex   = (mv * vec4(vertex, 1 )).xyz; \n"
		"   	vs2fs.coord    = coord; \n"
		"    	gl_Position    = p*mv*vec4(vertex, 1);\n"
		"}\n",
	
		//Fragment Shader
		"#version 330 core\n"

		"in VS2FS {\n"
		" 	vec3 vertex,, normal;\n"
		"  	vec4 light;\n"
		"   	vec2 coord;\n"
		"} gs2fs;\n"
		"uniform sampler2D TxCube;\n"
		"\n"
		"out vec4 fColor;\n"
		"\n"
		"void main( void ) {\n"
		"   \n"
		"   vec3  d     = gs2fs.light.xyz - gs2fs.vertex.xyz; \n"
		"   vec3 color  = texture( TxCube, gs2fs.coord ).xyz;  \n"
		"   float a =10*dot(d, gs2fs.normal)/dot(d,d); \n"
		"   fColor = vec4( clamp(a*color.x,0,1),clamp(a*color.y,0,1), clamp(a*color.z,0,1),  1 );\n"
		"}\n",
			GLRenderer::ShaderCode::FROM_CODE ).
			addUniform( "mv", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & mv ).
			addUniform( "p", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & p ).
			addUniform( "v", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & v ).
			addUniform( "light", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & light2 );


	glr.program( "PROGRAM-CUBE" ).
		setVertexArray( "VERTICES-CUBE" ).
		addInTexture( "TEXTURE-CUBE" ).
		setShader( "SHADER-CUBE" ).
		build( );


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
		"uniform vec3 light;\n"
		"uniform vec3 pos;\n"
		"uniform float rad;\n"
		"out vec3 vColor;\n"
		"out vec4 vNormal;\n"
		"out vec4 vVert;\n"
		"out vec4 vPos;\n"
		"out vec4 vLight;\n"
		"out float radB;\n"
		" \n"
		"void main( void ) {\n"
		"    mat4 nrm = transpose( inverse( mv ) );\n"
		"    vNormal  = normalize( nrm * ( vec4( normal, 0 ) ) ) ;\n"
		"    vLight = v* vec4( light.xyz, 1 );\n"
		"    vPos   = v* vec4(pos.xyz, 1);\n"
		"    vVert  = mv * vec4(vertex, 1 ); \n"
		"    radB   = rad;\n"
		"    vec3 vColor = color;\n"
		"    gl_Position = p*mv*vec4(vertex, 1);\n"
		"}\n",

		//Fragment Shader
		"#version 330 core\n"

		"in vec3 vColor;\n"
		"in vec4 vVert;\n"
		"in vec4 vNormal;\n"
		"in vec4 vLight;\n"
		"in vec4 vPos;\n"
		"in float radB;\n"
		"\n"
		"out vec4 fColor;\n"
		"\n"
		"void main( void ) {\n"
		"   vec3  d = vLight.xyz - vVert.xyz; \n"
		"   vec3 hP = vPos.xyz - vLight.xyz;\n"
		"   vec3 color = int(vVert.x*vVert.y*vVert.z )%2==0?vec3(1,1,1):vec3(.2,.4,.3);\n"
		"   hP = cross(hP, d);\n"
		"   float distance = sqrt(dot(hP,hP) )/sqrt(dot(d,d));\n"
		"   float a =10*dot(d, vNormal.xyz)/dot(d,d); \n"
		"   a = distance>radB?a:.2*pow(a,2);\n"
		"   fColor = vec4(clamp(a*color.x,0,1), clamp(a*color.y,0,1), clamp(a*color.z,0,1), 1);\n"
		"}\n",
			GLRenderer::ShaderCode::FROM_CODE ).
			addUniform( "mv", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & mv ).
			addUniform( "p", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & p ).
			addUniform( "v", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & v ).
			addUniform( "pos", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & pos ).
			addUniform( "light", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & light ).
			addUniform( "rad", GLRenderer::Shader::FLOAT, GLRenderer::Shader::SCALAR, & ballintr->rad );

	glr.program( "PROGRAM-FLOOR" ).
		setVertexArray( "VERTICES-FlOOR" ).
		setShader( "SHADER-FLOOR" ).
		build( );

	glr.shader(
		"SHADER-FLOOR-LEFT",
		//Vertex Shader
		"#version 330 core\n"
		"layout( location = 0 ) in vec3 vertex;\n"
		"layout( location = 1 ) in vec3 color;\n"
		"layout( location = 2 ) in vec3 normal;\n"

		"uniform float time;\n"
		"uniform mat4 mv; \n"
		"uniform mat4 v; \n"
		"uniform mat4 m; \n"
		"uniform mat4 p;\n"
		"uniform vec3 light;\n"
		"uniform vec3 pos;\n"
		"uniform float rad;\n"
		"out mat4 mO;\n"
		"out vec3 vColor;\n"
		"out vec4 vNormal;\n"
		"out vec4 vVert;\n"
		"out vec4 vPos;\n"
		"out vec4 vLight;\n"
		"out float radB;\n"
		" \n"
		"void main( void ) {\n"
		"    mat4 nrm = transpose( inverse( mv ) );\n"
		"    mO = m;"
		"    vNormal  = normalize( nrm * ( vec4( normal, 0 ) ) ) ;\n"
		"    vLight = v* vec4( light.xyz, 1 );\n"
		"    vPos   = v* vec4(pos.xyz, 1);\n"
		"    vVert  = mv * vec4(vertex, 1 ); \n"
		"    radB   = rad;\n"
		"    vec3 vColor = color;\n"
		"    gl_Position = p*mv*vec4(vertex, 1);\n"
		"}\n",

		//Fragment Shader
		"#version 330 core\n"

		"in mat4 mO;"
		"in vec3 vColor;\n"
		"in vec4 vVert;\n"
		"in vec4 vNormal;\n"
		"in vec4 vLight;\n"
		"in vec4 vPos;\n"
		"in float radB;\n"
		"\n"
		"out vec4 fColor;\n"
		"\n"
		""
		"float shadow(vec3 lightVecB){\n"
		"  return 1.; \n"
		"}\n"
		"void main( void ) {\n"
		"   vec3  d = vLight.xyz - vVert.xyz; \n"
		"   float t = 1;\n"
		"   vec3 hP = vPos.xyz - vLight.xyz;\n"
		"   vec3 color = int(5*sin(vVert.x)*pow((cos(vVert.x-vVert.y+vVert.z)),2))%2==0?vec3(1,1,1):vec3(.1,.2,.6);\n"
		"   hP = cross(hP, d);\n"
		"   float distance = sqrt(dot(hP,hP) )/sqrt(dot(d,d));\n"
		"   float a =t*10*dot(d, vNormal.xyz)/dot(d,d); \n"
		"   fColor = vec4(clamp(a*color.x,0,1), clamp(a*color.y,0,1), clamp(a*color.z,0,1), 1);\n"
		"}\n",

			GLRenderer::ShaderCode::FROM_CODE ).
			addUniform( "mv", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & mv ).
			addUniform( "p", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & p ).
			addUniform( "v", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & v ).
			addUniform( "m", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & m ).
			addUniform( "pos", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & pos2 ).
			addUniform( "light", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & light2 ).
			addUniform( "rad", GLRenderer::Shader::FLOAT, GLRenderer::Shader::SCALAR, & ballintr->rad );

	glr.program( "PROGRAM-FLOOR-LEFT" ).
		setVertexArray( "VERTICES-FlOOR" ).
		setShader( "SHADER-FLOOR-LEFT" ).
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
		"    vLight  = v* vec4( light.xyz, 1 );\n"
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
		"\n"
		"void main( void ) {\n"
			"vec3  d = vLight.xyz - vVert.xyz; \n"
			"float a =10*dot(d, vNormal.xyz)/dot(d,d); \n"
			"fColor = vec4( clamp(a*vColor.x,0,1),clamp(a*vColor.y,0,1), clamp(a*vColor.z,0,1),  1 );\n"
		"}\n",

		GLRenderer::ShaderCode::FROM_CODE ).
			addUniform( "mv", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & mv ).
			addUniform( "v", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & v ).
			addUniform( "p", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & p ).
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

	light = glm::vec3(1*cos(vcd->time), 1,.5*sin(vcd->time)-1);
	light2 = glm::vec3(1*cos(vcd->time)-1, 1,.5*sin(vcd->time)-1);
	pos = ballintr->Bounds(vcd->time);
	pos2 = glm::vec3(-3 ,1, -6.6);

	v = glm::translate(glm::mat4(1), glm::vec3(1.5,-.5,-1));

	m = glm::translate( glm::mat4( 1. ), pos);

	m = glm::rotate( m, angle, glm::vec3( 0.f, 1.f, 0.f ) );
	mv = v * m;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glr.run( { "PROGRAM-BALLANDLIGHT" } );

	m = glm::translate( glm::mat4( 1. ), glm::vec3(0, 0, -5));

	mv = v * m;
	glr.run( { "PROGRAM-FLOOR" } );

	m = glm::translate( glm::mat4( 1. ), glm::vec3(-3, 0, -5));

	mv = v * m;
	glr.run( { "PROGRAM-FLOOR-LEFT" } );

	m = glm::translate( glm::mat4( 1. ), pos2);

	m = glm::rotate( m, angle, glm::vec3( 1.f, 1.f, 0.f ) );
	mv = v * m;

	glr.run( { "PROGRAM-CUBE" } );
}
void
BallAndLight::resize( int p_width, int p_height ) {
	float
	ratio = ( 1.f * p_width / p_height );
	p = glm::perspective( 45.0f, ratio, 1.0f, 10.f );
}

