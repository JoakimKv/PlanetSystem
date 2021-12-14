
/************************************/
/* Namn:	 Joakim Kvistholm		*/
/* Datum:	 2000-08-03				*/
/* Filnamn:	 round.cpp				*/
/************************************/


/* För att anropa fmod och fabs			 */
#include <math.h>

#include "round.h"	

double round( double d )
{
	double fract ;	/* Decimaldelen */
	int tecken ;	/* Tecknet på talet */
	
	/* Hämta decimaldelen */
	fract = fmod(d,1) ;
	
	/* Om talet är negativt eller positivt undersöks */
	if( fract < 0 )
		tecken = -1 ;
	else 
		tecken = 1 ;

	/* Ska vi avrunda uppåt eller neråt undersöks */
	if( fabs((2*fract))>1.0 )
		return d+tecken-fract ;
	else
		return d-fract ;
}
