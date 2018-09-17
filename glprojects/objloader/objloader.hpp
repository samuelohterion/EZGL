#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP

#include "../../code/glr.hpp"
#include "../../code/glmprinter.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>

class Obj :
public Named {

	public :

		CStr
		COMMENT_STR [ 1 ] {

			"#"	// comment
		};

		CStr
		VERTEX_DATA_STR [ 4 ] {

			"v",	// Geometric vertices
			"vt",	// Texture vertices
			"vn", // Vertex normals
			"vp" // Parameter space vertices
		};

		CStr
		FREE_FORM_CURVE_SURFACE_ATTRIBUTE_STR [ 4 ] {

			"deg",	// Degree
			"bmat",	// Basis matrix
			"step",	// Step size
			"cstype"	// Curve or surface type
		};

		CStr
		ELEMENTS_STR [ 6 ] {

			"p",		// Point
			"l",		// Line
			"f",		// Face
			"curv",	// Curve
			"curv2",	// 2D curve
			"surf"	// Surface
		};

		CStr
		FREE_FORM_CURVE_SURFACE_BODY_STATEMENT_STR [ 6 ] {

			"parm",	// Parameter values
			"trim",	// Outer trimming loop
			"hole",	// Inner trimming loop
			"scrv",	// Special curve
			"sp",		// Special point
			"end"		// End statement
		};

		CStr
		CONNECTIVITY_BETWEEN_FREE_FORM_SURFACES_STR [ 1 ] {

			"con"	// Connect
		};

		CStr
		GROUPING_STR [ 4 ] {

			"g",	// Group name
			"s",	// Smoothing group
			"mg",	// Merging group
			"o",	// Object name
		};

		CStr
		DISPLAY_RENDER_ATTRIBUTE_STR [ 10 ]{

			"bevel",		// Bevel interpolation
			"c_interp",	// Color interpolation
			"d_interp",	// Dissolve interpolation
			"lod",		// Level of detail
			"usemtl",		// Material name
			"mtllib",		// Material library
			"shadow_obj",	// Shadow casting
			"trace_obj",	//	Ray tracing
			"ctech",		// Curve approximation technique
			"stech"		// Surface approximation technique
		};

	public :

		Obj ( const CStr & p_name );

		~ Obj ( );

	public :

		std::string
		mtllib;

		std::vector< GLfloat >
		v,
		vn,
		vt;

	public :

		bool
		load( CStr & p_filename );
};

class ObjLoader :
public GLProject {

	public:

		ObjLoader ( CStr const & p_name = "ObjLoader", ViewControlData * p_vcd = nullptr );

		~ ObjLoader ( );

	public:

		M4
		model,
		view,
		projection;

		Obj
		* obj;

	public:

		void
		init ( );

		void
		paint ( );

		void
		resize ( int p_width, int p_height );
};

#endif // OBJLOADER_HPP
