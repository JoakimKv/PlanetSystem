
/************************************/
/* Namn:	 Joakim Kvistholm		*/
/* Datum:	 2000-08-03				*/
/* Filnamn:	 slumptal.cpp			*/
/************************************/


/* För att anropa rand och för få värdet på RAND_MAX */
#include <stdlib.h>

#include "slumptal.h"

/* Slumpar ut ett flyttal som ligger inom vissa	  */
/* gränser (som ges av indatan).				  */

double slumptal(double tal_min, double tal_max)
{
	double slumpvarde;	/* En temp-variabel för slumpvärdet */
		
	slumpvarde=rand()/(double)RAND_MAX;	/* Värde mellan 0 och 1 */

	/* Kollar om de angivit tal_min och tal_max rätt */
	if(tal_max > tal_min)
		return slumpvarde*(tal_max-tal_min)+tal_min;
	else
		return slumpvarde*(tal_min-tal_max)+tal_max;
}
