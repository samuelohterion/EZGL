#ifndef DUMMY_HPP
#define DUMMY_HPP
#include "code/glr.hpp"

class
Triangle {

	public:

		Triangle ( glm::vec3 const & p_a, glm::vec3 const & p_b, glm::vec3 const & p_c, glm::vec3 const & p_col ) :
		a ( p_a ),
		b ( p_b ),
		c ( p_c ),
		col( p_col ),
		t ( nullptr ) {

		}

		~ Triangle ( ) {

			if ( ! t )

				return;

			for ( GLuint i = 0; i < 4; ++ i ) {

				delete t [ i ];

				t [ i ] = nullptr;
			}

			delete [ ] t;

			t = nullptr;
		}

		glm::vec3
		a,
		b,
		c,
		col;

		Triangle
		* * t;

	public:

		void
		divide ( GLsizei p_count ) {

			if ( -- p_count < 0 ) {

				return;
			}

			glm::vec3
			ab = 1.f * ( .51f * a + .49f * b ),
			bc = 1.f * ( .51f * b + .49f * c ),
			ca = 1.f * ( .51f * c + .49f * a ),
			ba = 1.f * ( .49f * a + .51f * b ),
			cb = 1.f * ( .49f * b + .51f * c ),
			ac = 1.f * ( .49f * c + .51f * a ),
			ab2 = 1.f * ( .5f * a + .5f * b ),
			bc2 = 1.f * ( .5f * b + .5f * c ),
			ca2 = 1.f * ( .5f * c + .5f * a );

			t = new Triangle * [ 4 ];

			t [ 0 ] = new Triangle ( glm::normalize( a ), glm::normalize( ac ), glm::normalize( ab ), col );
			t [ 1 ] = new Triangle ( glm::normalize( b ), glm::normalize( ba ), glm::normalize( bc ), col );
			t [ 2 ] = new Triangle ( glm::normalize( c ), glm::normalize( cb ), glm::normalize( ca ), col );
			t [ 3 ] = new Triangle ( glm::normalize( ca2 ), glm::normalize( bc2 ), glm::normalize( ab2 ), col );

			t [ 0 ] -> divide ( p_count );
			t [ 1 ] -> divide ( p_count );
			t [ 2 ] -> divide ( p_count );
			t [ 3 ] -> divide ( p_count );
		}

		GLR::VertexArray
		& fill ( GLR::VertexArray & p_va ) {

			if ( t ) {

				for ( GLsizei i = 0; i < 4; ++ i ) {

					t [ i ] -> fill ( p_va );
				}
			}
			else {

//                p_va << glm::normalize( a ) << glm::normalize( b ) << glm::normalize( c );
//				glm::vec3
//				col( 1.f * rand( ) / RAND_MAX, 1.f * rand( ) / RAND_MAX, 1.f * rand( ) / RAND_MAX );

				p_va << a << col << b << col << c << col;
			}

			return p_va;
		}
};

class
Tetrahedron {

	public:

		Tetrahedron ( Triangle const & p_a, Triangle const & p_b, Triangle const & p_c, Triangle const & p_d ) :
		a ( p_a ),
		b ( p_b ),
		c ( p_c ),
		d ( p_d ) {

		}

		~ Tetrahedron ( ) {

		}

		Triangle
		a,
		b,
		c,
		d;

	public:

		void
		divide ( GLsizei p_count ) {

			a.divide( p_count );
			b.divide( p_count );
			c.divide( p_count );
			d.divide( p_count );
		}

		GLR::VertexArray
		& fill ( GLR::VertexArray & p_va ) {

			return d.fill( c.fill( b.fill( a.fill( p_va ) ) ) );
		}
};

class Dummy :
public GLProject {

	public:

		Dummy ( CStr const & p_name = "Dummy", ViewControlData *p_vcd = nullptr );

		~ Dummy( );

	public:

		glm::mat4
		m,//odel
		v,//iew
		p,//projection
		mvp;//model-view-projection

		Tetrahedron
		tetrahedron;

	public:

		void
		init( );

		void
		paint( );

		void
		resize( int p_width, int p_height );

};

#endif // COLOREDTRIANGLE3D_HPP
