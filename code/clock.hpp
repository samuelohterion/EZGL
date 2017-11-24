#ifndef	CLOCK_HPP
#define	CLOCK_HPP

#include	<sys/time.h>

#ifdef _WINDOWS
	#include <windows.h>
#else
	#include <unistd.h>
#endif

class	Clock{

	public:

		Clock( ):
		startTime( time( ) ) {

		}

	private:

		timeval
		startTime;

	public:

		timeval
		start( ) {

			return startTime = time( );
		}

		std::size_t
		elapsedMicros( ) const {

			timeval
			now_ = time( );

			return ( 1e6 * now_.tv_sec + now_.tv_usec ) - (  1e6 * startTime.tv_sec + startTime.tv_usec );
		}

		timeval
		elapsed( ) {

			timeval
			ret_;

			std::size_t
			microSecs_ = elapsedMicros( );

			ret_.tv_sec  = microSecs_ / 1e6;
			ret_.tv_usec = microSecs_ - ret_.tv_sec * 1e6;

			return ret_;
		}

		static timeval
		time( ) {

			timeval
			ptv;

			gettimeofday( &ptv, 0 );

			return ptv;
		}

		static void
		sleep( double const &p_seconds ) {

			#ifdef _WINDOWS
				Sleep( p_seconds );
			#else
				usleep( p_seconds * 1e6 );
			#endif
		}
};

#endif // CLOCK_HPP
