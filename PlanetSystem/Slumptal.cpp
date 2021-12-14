
/************************************/
/* Namn:	 Joakim Kvistholm		*/
/* Datum:	 2000-08-03				*/
/* Filnamn:	 slumptal.cpp			*/
/************************************/


/* F�r att anropa rand och f�r f� v�rdet p� RAND_MAX */
#include <stdlib.h>

#include "slumptal.h"

/* Slumpar ut ett flyttal som ligger inom vissa	  */
/* gr�nser (som ges av indatan).				  */

double slumptal(double tal_min, double tal_max)
{
	double slumpvarde;	/* En temp-variabel f�r slumpv�rdet */
		
	slumpvarde=rand()/(double)RAND_MAX;	/* V�rde mellan 0 och 1 */

	/* Kollar om de angivit tal_min och tal_max r�tt */
	if(tal_max > tal_min)
		return slumpvarde*(tal_max-tal_min)+tal_min;
	else
		return slumpvarde*(tal_min-tal_max)+tal_max;
}
