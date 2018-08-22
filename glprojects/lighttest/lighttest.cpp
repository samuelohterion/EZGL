#include "lighttest.hpp"

LightTest::LightTest( const CStr &p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ) {

}

void
LightTest::init( ) {

	glClearColor( .1f, .02f, .03f, 1. );

	projection = view = model = glm::mat4( 1. );

// -------------------------------------------------------------------------------------
// checkerboard

	glr.vertices( "VERTICES-LIGHTED-CHECKERBOARD-CHECKERBOARD" ).
		setUsage( GL_STATIC_DRAW ).
		addAttrib( "vertex", 2, 0 ) <<
		-1.f << -1.f <<
		+1.f << -1.f <<
		+1.f << +1.f <<
		-1.f << +1.f <<
		GLRenderer::VertexArray::Object( 0, 4, GL_TRIANGLE_FAN );

	glr.shader(
		"SHADER-LIGHTED-CHECKERBOARD-CHECKERBOARD",

		// VERTEX SHADER

		//	version 3.3 core
		"#version 330 core\n"

		//	matrices in every space to show their behavior
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"

		//	light in every space that makes sense to show their behavior
		"uniform vec3 light1InModelSpacePosition;\n"
		"uniform vec3 light2InCameraSpacePosition;\n"
		"uniform vec3 light1InModelSpaceColor;\n"
		"uniform vec3 light2InCameraSpaceColor;\n"

		//	only vertices come in
		"in vec2 vertex;\n"

		//	Vertex To Fragment
		"out VS2FS {\n"

		//	squareCoordsInInitialSpace (0,0) - (1,1)
		"	vec2\n"
		"		squareCoordsInWorldSpace;\n"

		//	tranformed vectors
		"	vec4\n"
		"		vertexInModelSpace,\n"
		"		vertexInCameraSpace,\n"
		"		vertexInProjectionSpace,\n"
		"		normalVectorInModelSpace,"
		"		normalVectorInCameraSpace;\n"
		"} vs2fs;\n"

		//	now the main part (O;
		"void main( ) {\n"

		//	save the xy - coords transformed from -1,+1 to 0,7
		"	vs2fs.squareCoordsInWorldSpace = 4. + 4. * vertex.xy;\n"

		"	vec4\n"
		"		v4 = vec4( vertex.y, 0, vertex.x, 1 );\n"

		//	make some trafos
		"	vs2fs.vertexInModelSpace          = model * v4;\n"
		"	vs2fs.vertexInCameraSpace         = view  * model * vs2fs.vertexInModelSpace;\n"
		"	vs2fs.vertexInProjectionSpace     = projection * vs2fs.vertexInCameraSpace;\n"
		"	vs2fs.normalVectorInModelSpace    = vec4( 0, 1, 0, 0 ) * inverse( model );\n"
		"	vs2fs.normalVectorInCameraSpace   = vec4( 0, 1, 0, 0 ) * inverse( view * model );\n"

		// position of course has to be transformed into projection space
		"	gl_Position = projection * view * model * v4;\n"
		"}\n",

		// FRAGMENT SHADER

		// version 3.3 core
		"#version 330 core\n"

		//	light in every space that makes sense to show their behavior
		"uniform vec3 light1InModelSpacePosition;\n"
		"uniform vec3 light2InCameraSpacePosition;\n"
		"uniform vec3 light1InModelSpaceColor;\n"
		"uniform vec3 light2InCameraSpaceColor;\n"

		// Vertex To Fragment
		"in VS2FS {\n"

		//	squareCoordsInInitialSpace (0,0) - (1,1)
		"	vec2\n"
		"		squareCoordsInWorldSpace;\n"

		//	tranformed vectors
		"	vec4\n"
		"		vertexInModelSpace,\n"
		"		vertexInCameraSpace,\n"
		"		vertexInProjectionSpace,\n"
		"		normalVectorInModelSpace,\n"
		"		normalVectorInCameraSpace;\n"
		"} vs2fs;\n"

		// output color
		"out vec4 fColor;\n"

		// 2d random
		"float random ( in vec2 st) {\n"
			"return fract( sin( dot( st.xy, vec2( 12.9898, 78.233 ) ) ) * 43758.5453123 );\n"
		"}\n"

		// main again
		"void main( ) {\n"
		"	vec3\n"
		"	d1 = light1InModelSpacePosition  - vs2fs.vertexInModelSpace.xyz,\n"
		"	d2 = light2InCameraSpacePosition - vs2fs.vertexInCameraSpace.xyz;\n"

		"	float\n"
		"	a1 = max( 0, .5 * dot( d1, vs2fs.normalVectorInModelSpace.xyz )  / dot( d1, d1 ) ),\n"
		"	a2 = max( 0, .5 * dot( d2, vs2fs.normalVectorInCameraSpace.xyz ) / dot( d2, d2 ) );\n"

		"	fColor = ( ( int( vs2fs.squareCoordsInWorldSpace.x ) + int( vs2fs.squareCoordsInWorldSpace.y ) ) % 2 == 0 )\n"
		"		? vec4( .7, .7, .7, 1. )\n"
		"		: vec4( .3, .3, .3, 1. );\n"
		"	fColor.xyz += vec3( .3 * random( vs2fs.squareCoordsInWorldSpace ) );\n"
		"	fColor.xyz = fColor.xyz * ( pow( a1, 1. ) * light1InModelSpaceColor + pow( a2, 4. ) * light2InCameraSpaceColor );\n"
		"}\n",
		GLRenderer::ShaderCode::FROM_CODE ).
		addUniform( "model",               GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & model ).
		addUniform( "view",                GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & view ).
		addUniform( "projection",          GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & projection ).
		addUniform( "light1InModelSpacePosition",  GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & light1InModelSpacePosition ).
		addUniform( "light2InCameraSpacePosition", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & light2InCameraSpacePosition ).
		addUniform( "light1InModelSpaceColor",  GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & light1InModelSpaceColor ).
		addUniform( "light2InCameraSpaceColor", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & light2InCameraSpaceColor );

	glr.program( "PROGRAM-LIGHTED-CHECKERBOARD-CHECKERBOARD" ).
		setVertexArray( "VERTICES-LIGHTED-CHECKERBOARD-CHECKERBOARD" ).
		setShader( "SHADER-LIGHTED-CHECKERBOARD-CHECKERBOARD" ).
		build( );

/*
	glr.shader(
		"NORMALS-SHADER",

		"#version 330 core\n"
		"precision highp float;\n"
		"precision highp int;\n"
		"in vec2 vertex;\n"
		"out VS2GS {\n"
		"	vec4 vertex, normal, mvn, color;\n"
		"} vs2gs;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main( ) {\n"
		"	vec4\n"
		"		v4 = vec4( vertex.y, 0, vertex.x, 1 );\n"
		"	vs2gs.vertex = projection * model * view * v4;\n"
		"	vs2gs.normal = normalize( vec3( projection * vec4( mat3( transpose( inverse( model * view ) ) ) * normals, 0. ) ) );\n"
		"	vs2gs.mvn    = normalize( vec4( 0, 0, 1, 0 ) * inverse( model * view ) );\n"
		"	vs2gs.color  = color;\n"
		"	gl_Position  = projection * model * view * v4;\n"
		"}\n",

		"#version 330 core\n"
		"precision highp float;\n"
		"precision highp int;\n"
		"layout ( triangles ) in;\n"
		"layout ( line_strip, max_vertices = 6 ) out;\n"
		"in VS2GS {\n"
		"	vec3 vertex;\n"
		"	vec3 normal;\n"
		"	vec3 mvn;\n"
		"	vec3 color;\n"
		"} vs2gs[ ];\n"
		"out GS2FS {\n"
			"vec3 colors;\n"
		"} gs2fsOut;\n"
		"const float MAGNITUDE = 1.;\n"
		"void generateLine( int index ) {\n"
			"gl_Position = gl_in[ index ].gl_Position;\n"
			"gs2fsOut.colors = max( vec3( 0. ), vs2gsIn[ index ].mvn );\n"
			"EmitVertex( );\n"
			"gl_Position = gl_in[ index ].gl_Position + vec4( vs2gsIn[ index ].normals, 0. ) * MAGNITUDE;\n"
			"gs2fsOut.colors = max( vec3( 0. ), vs2gsIn[ index ].mvn );\n"
			"EmitVertex( );\n"
			"EndPrimitive( );\n"
		"}\n"
		"void main( ) {\n"
			"generateLine( 0 ); // first vertex normal\n"
			"generateLine( 1 ); // second vertex normal\n"
			"generateLine( 2 ); // third vertex normal\n"
		"}\n",

		"#version 330 core\n"
		"precision highp float;\n"
		"precision highp int;\n"
		"in GS2FS {\n"
			"vec3 colors;\n"
		"} gs2fsIn;\n"
		"out vec4 fCol;\n"
		"void main( void ) {\n"
			"fCol = vec4( gs2fsIn.colors, 1.f );\n"
		"}\n",
		GLRenderer::ShaderCode::FROM_CODE ).
			addUniform( "p",  GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & p ).
			addUniform( "mv",  GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & mv );

	glr.program( "NORMALS-PROGRAM" ).
		setVertexArray(	"QUAD-VERTICES" ).
		setShader( "NORMALS-SHADER" ).
		build( );
*/

// -------------------------------------------------------------------------------------
// light in model space

	glr.vertices( "VERTICES-LIGHTED-CHECKERBOARD-LIGHT-IN-MODEL-SPACE" ).
		setUsage( GL_STATIC_DRAW ).
		addAttrib( "color", 3, 0 ) <<
		0.f << 0.f << 0.f <<
		GLRenderer::VertexArray::Object( 0, 1, GL_POINTS );

	glr.shader(
		"SHADER-LIGHTED-CHECKERBOARD-LIGHT-IN-MODEL-SPACE",

		// VERTEX SHADER

		//	version 3.3 core
		"#version 330 core\n"

		//	matrices in every space to show their behavior
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		//	light in model space to show their behavior
		"uniform vec3 light1InModelSpacePosition;\n"
		"uniform vec3 light1InModelSpaceColor;\n"

		"in vec3 color;\n"

		//	Vertex To Fragment
		"out VS2FS {\n"

		//	tranformed vectors
		"vec4\n"
		"	color;\n"
		"} vs2fs;\n"

		//	now the main part (O;
		"void main( ) {\n"

		"	gl_PointSize = 20;\n"

		//	make some trafos
		"	vs2fs.color  = vec4( light1InModelSpaceColor, 1 );\n"

		// position of course has to be transformed into projection space
		"	gl_Position = projection * view * vec4( light1InModelSpacePosition, 1 );\n"
		"}\n",

		// FRAGMENT SHADER

		// version 3.3 core
		"#version 330 core\n"

		// Vertex To Fragment
		"in VS2FS {\n"

		//	tranformed vectors
		"vec4\n"
		"		color;\n"
		"} vs2fs;\n"

		// output color
		"out vec4 fColor;\n"

		// main again
		"void main( ) {\n"
		"	vec2 v = 2. * gl_PointCoord.xy - 1.;\n"
		"	float s = dot( v, v );\n"
		"	if( s > .999 )\n"
		"		discard;\n"
		"	fColor = vec4( clamp( 1. * ( 1. - .95 * s ), .1, 1 ) * ( vec3( 1. ) + vs2fs.color.xyz ), 1. );\n"
		"}\n",
		GLRenderer::ShaderCode::FROM_CODE ).
		addUniform( "model",               GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & model ).
		addUniform( "view",                GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & view ).
		addUniform( "projection",          GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & projection ).
		addUniform( "light1InModelSpacePosition",  GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & light1InModelSpacePosition ).
		addUniform( "light1InModelSpaceColor",  GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & light1InModelSpaceColor );


	glr.program( "PROGRAM-LIGHTED-CHECKERBOARD-LIGHT-IN-MODEL-SPACE" ).
		setVertexArray( "VERTICES-LIGHTED-CHECKERBOARD-LIGHT-IN-MODEL-SPACE" ).
		setShader( "SHADER-LIGHTED-CHECKERBOARD-LIGHT-IN-MODEL-SPACE" ).
		build( );

// -------------------------------------------------------------------------------------
// light in camera space

	glr.vertices( "VERTICES-LIGHTED-CHECKERBOARD-LIGHT-IN-CAMERA-SPACE" ).
		setUsage( GL_STATIC_DRAW ).
		addAttrib( "color", 3, 0 ) <<
		0.f << 0.f << 0.f <<
		GLRenderer::VertexArray::Object( 0, 1, GL_POINTS );

	glr.shader(
		"SHADER-LIGHTED-CHECKERBOARD-LIGHT-IN-CAMERA-SPACE",

		// VERTEX SHADER

		//	version 3.3 core
		"#version 330 core\n"

		//	matrices in every space to show their behavior
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		//	light in model space to show their behavior
		"uniform vec3 light2InCameraSpacePosition;\n"
		"uniform vec3 light2InCameraSpaceColor;\n"

		"in vec3 color;\n"

		//	Vertex To Fragment
		"out VS2FS {\n"

		//	tranformed vectors
		"	vec4\n"
		"		color;\n"
		"} vs2fs;\n"

		//	now the main part (O;
		"void main( ) {\n"

		"	gl_PointSize = 20;\n"

		//	make some trafos
		"	vs2fs.color  = vec4( light2InCameraSpaceColor, 1 );\n"

		// position of course has to be transformed into projection space
		"	gl_Position = projection * vec4( light2InCameraSpacePosition, 1 );\n"
		"}\n",

		// FRAGMENT SHADER

		// version 3.3 core
		"#version 330 core\n"

		// Vertex To Fragment
		"in VS2FS {\n"

		//	tranformed vectors
		"vec4\n"
		"		color;\n"
		"} vs2fs;\n"

		// output color
		"out vec4 fColor;\n"

		// main again
		"void main( ) {\n"
		"	vec2 v = 2. * gl_PointCoord.xy - 1.;\n"
		"	float s = dot( v, v );\n"
		"	if( s > .999 )\n"
		"		discard;\n"
		"	fColor = vec4( clamp( 1. * ( 1. - 1. * s ), .1, 1 ) * ( vec3( .5 ) + vs2fs.color.xyz ), 1. );\n"
		"}\n",
		GLRenderer::ShaderCode::FROM_CODE ).
		addUniform( "model",               GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & model ).
		addUniform( "view",                GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & view ).
		addUniform( "projection",          GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & projection ).
		addUniform( "light2InCameraSpacePosition",  GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & light2InCameraSpacePosition ).
		addUniform( "light2InCameraSpaceColor", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & light2InCameraSpaceColor );

	glr.program( "PROGRAM-LIGHTED-CHECKERBOARD-LIGHT-IN-CAMERA-SPACE" ).
		setVertexArray( "VERTICES-LIGHTED-CHECKERBOARD-LIGHT-IN-CAMERA-SPACE" ).
		setShader( "SHADER-LIGHTED-CHECKERBOARD-LIGHT-IN-CAMERA-SPACE" ).
		build( );
/*
	glr.vertices( "VERTICES-LIGHTED-CHECKERBOARD-LIGHT2" ).
		setUsage( GL_STATIC_DRAW ).
		addAttrib( "light1InCameraSpace", 3, 0 ) <<
		0.f << 0.f << 0.f;

	glr.shader(
		"SHADER-LIGHTED-CHECKERBOARD-LIGHT2",

		// VERTEX SHADER

		//	version 3.3 core
		"#version 330 core\n"

		//	matrices in every space to show their behavior
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		//	light in camera space to show their behavior
		"uniform vec3 light2InCameraSpace;\n"

		"in vec3 color;\n"

		//	Vertex To Fragment
		"out VS2FS {\n"

		//	tranformed vectors
		"	vec4\n"
		"		color;\n"
		"} vs2fs;\n"

		//	now the main part (O;
		"void main( ) {\n"

		//	make some trafos
		"	vs2fs.color  = vec4( color, 1 );\n"

		// position of course has to be transformed into projection space
		"	gl_Position = projection * vec4( light2InCameraSpace, 1 );\n"
		"}\n",

		// FRAGMENT SHADER

		// version 3.3 core
		"#version 330 core\n"

		// Vertex To Fragment
		"in VS2FS {\n"

		//	tranformed vectors
		"	vec4\n"
		"		color;\n"
		"} vs2fs;\n"

		// output color
		"out vec4 fColor;\n"

		// main again
		"void main( ) {\n"
		"	fColor = vs2fs.color;\n"
		"}\n",
		GLRenderer::ShaderCode::FROM_CODE ).
		addUniform( "model",               GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & model ).
		addUniform( "view",                GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & view ).
		addUniform( "projection",          GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & projection ).
		addUniform( "light2InCameraSpace", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & light2InCameraSpace );

	glr.program( "PROGRAM-LIGHTED-CHECKERBOARD-LIGHT2" ).
		setVertexArray( "VERTICES-LIGHTED-CHECKERBOARD-LIGHT" ).
		setShader( "SHADER-LIGHTED-CHECKERBOARD-LIGHT2" ).
		build( );
*/
}

void
LightTest::paint( ) {

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

//	look from a certain point
	view =
		glm::lookAt(
			glm::vec3( 0, 3, 3 ), // Camera is at (4,3,3), in World Space
			glm::vec3( 0, 0, 0 ), // and looks at the origin
			glm::vec3( 0, 1, 0 )  // Head is up (set to 0,-1,0 to look upside-down)
	);

	model = glm::mat4( 1. );

	model = glm::rotate( model, .1f * vcd->time, glm::vec3( 0., 1., 0. ) );

	glr.run( { "PROGRAM-LIGHTED-CHECKERBOARD-CHECKERBOARD" } );

	model = glm::rotate( model, .5f * 3.1415f, glm::vec3( 1., 0., 0. ) );
	model = glm::translate( model, glm::vec3( 0., -1., -1. ) );

	glr.run( { "PROGRAM-LIGHTED-CHECKERBOARD-CHECKERBOARD" } );

	model = glm::rotate( model, .5f * 3.1415f, glm::vec3( 0., 0., 1. ) );
	model = glm::translate( model, glm::vec3( +1., -1., 0. ) );

	glr.run( { "PROGRAM-LIGHTED-CHECKERBOARD-CHECKERBOARD" } );

//	view = glm::rotate(
//			glm::translate(
//				glm::mat4( 1. ),
//				glm::vec3( 0.f, 0.f, -5.f ) ),
//			0.f * sinf( vcd->time ),
//			glm::vec3( 0., 1.f, 0.f ) );

	glr.run( {
		"PROGRAM-LIGHTED-CHECKERBOARD-CHECKERBOARD" } );

	light1InModelSpaceColor  = glm::vec3( 1.0f, .50f, .25f );
	light2InCameraSpaceColor = glm::vec3( .25f, .50f, 1.0f );

	model = glm::mat4( 1. );

	light1InModelSpacePosition  = glm::vec3( model * glm::vec4( .9f * cosf( 8.f * .665f * vcd->time ), 1.f + .9f * sinf( .665f * vcd->time ), .9f * sinf( 8.f * .665f * vcd->time ), 1.f ) );
	light2InCameraSpacePosition = glm::vec3( view * model * glm::vec4( .5f * sinf( sinf( vcd->time ) ), 2.f - .5f * cosf( sinf( vcd->time ) ), 1.f, 1.f ) );

	glEnable( GL_VERTEX_ATTRIB_ARRAY_NORMALIZED );
	glEnable( GL_VERTEX_PROGRAM_POINT_SIZE );

	glr.run( {
		"PROGRAM-LIGHTED-CHECKERBOARD-LIGHT-IN-MODEL-SPACE",
		"PROGRAM-LIGHTED-CHECKERBOARD-LIGHT-IN-CAMERA-SPACE"
	} );

	glDisable( GL_VERTEX_ATTRIB_ARRAY_NORMALIZED );
	glDisable( GL_VERTEX_PROGRAM_POINT_SIZE );
}

void
LightTest::resize( int p_width, int p_height ) {

	float
	ratio = ( 1.f * p_width / p_height );

	projection = glm::perspective( 45.0f, ratio, 1.0f, 100.f );
}
