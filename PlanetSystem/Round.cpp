
/************************************/
/* Namn:	 Joakim Kvistholm		*/
/* Datum:	 2000-08-03				*/
/* Filnamn:	 round.cpp				*/
/************************************/


/* F�r att anropa fmod och fabs			 */
#include <math.h>

#include "round.h"	

double round( double d )
{
	double fract ;	/* Decimaldelen */
	int tecken ;	/* Tecknet p� talet */
	
	/* H�mta decimaldelen */
	fract = fmod(d,1) ;
	
	/* Om talet �r negativt eller positivt unders�ks */
	if( fract < 0 )
		tecken = -1 ;
	else 
		tecken = 1 ;

	/* Ska vi avrunda upp�t eller ner�t unders�ks */
	if( fabs((2*fract))>1.0 )
		return d+tecken-fract ;
	else
		return d-fract ;
}
