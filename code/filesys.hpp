#ifndef FILESYS_HPP
#define FILESYS_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <regex>
#include <unistd.h>

class FileSys {

	public :

		static void
		changeDir ( std::string const & p_path ) {

			chdir ( p_path.c_str ( ) );
		}

		static std::string
		currentPath ( ) {

			char
			fn[ 0x200 ];

			getcwd( fn, 0x200 );

			return std::string ( fn );
		}

		static void
		extractFile ( std::string & p_file, std::string const & p_filename ) {

			std::size_t
			lof = p_filename.find_last_of( "/\\" );

			p_file = p_filename.substr( lof + 1 );
		}

		static void
		extractPath ( std::string & p_path, std::string const & p_filename ) {

			std::size_t
			lof = p_filename.find_last_of( "/\\" );

			p_path = p_filename.substr( 0, lof );
		}

		static void
		extractPathAndFile ( std::string & p_path, std::string & p_file, std::string const & p_filename ) {

			std::size_t
			lof = p_filename.find_last_of( "/\\" );

			p_path = p_filename.substr( 0, lof );
			p_file = p_filename.substr( lof + 1 );
		}
};
#endif // FILESYS_HPP
