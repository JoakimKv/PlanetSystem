
/************************************/
/* Namn:	 Joakim Kvistholm		*/
/* Datum:	 2000-08-03				*/
/* Filnamn:	 Planet.cpp				*/
/************************************/


/* To use round */
#include "round.h"

/* To use sqrt, pow */
#include <math.h>
#include <string.h>         /* To use memcpy, memset */
#include <stdlib.h>         /* To use malloc, free   */

#include "CPlanet.h"

///////////////////////////////
// Konstructor
///////////////////////////////


CPlanet::CPlanet()
{
	int count;	// Räknare 

	for (count=0; count < 3; count++)
	{
		// Avrundning av x,y och z koordinaterna är nödvändig 
		m_koord_exakt[count] = (double) 0.0 ;
		m_koord[count] = (long int) round(m_koord_exakt[count]) ;
		m_hast[count] = (double) 0.0 ;
		m_acceleration[count] = (double) 0.0 ;
		m_dW[count] = (double) 0.0 ;
		m_dDeltaW[count] = (double) 0.0 ; 
	}
	m_massa = (double) 1.0 ;
	strcpy(m_szTextureFile, "") ;
	m_iRadius = (double) 1.0 ;
	m_iTextureID = 0 ;
    m_iPlanetTextureIndex = 0 ;
    m_bPlanet = TRUE ;
}


CPlanet::CPlanet(double massa, double radius, char* szTextureFile)
{
	int count;	/* Räknare */

	for (count=0; count < 3; count++)
	{
		/* Avrundning av x,y och z koordinaterna är nödvändig */
		m_koord_exakt[count] = (double) 0.0 ;
		m_koord[count] = (long int) round(m_koord_exakt[count]) ;
		m_hast[count] = (double) 0.0 ;
		m_acceleration[count] = (double) 0.0 ;
		m_dW[count] = (double) 0.0 ;
		m_dDeltaW[count] = (double) 0.0 ; 
	}

	if (szTextureFile != NULL && strlen(szTextureFile) > 0)
	{
		strcpy(m_szTextureFile, szTextureFile) ;
	}
	else
	{
		strcpy(m_szTextureFile, "") ;
	}

	m_massa = massa ;
	m_iRadius = radius ;
	m_iTextureID = 0 ;
    m_iPlanetTextureIndex = 0 ;
    m_bPlanet = TRUE ;
}

CPlanet::CPlanet(int iPlanetTextureIndex, int iTextureID, BOOLEAN bPlanet)
{
	int count;	/* Räknare */

	for (count=0; count < 3; count++)
	{
		/* Avrundning av x,y och z koordinaterna är nödvändig */
		m_koord_exakt[count] = (double) 0.0 ;
		m_koord[count] = (long int) round(m_koord_exakt[count]);
		m_hast[count] = (double) 0.0 ;
		m_acceleration[count] = (double) 0.0 ;
		m_dW[count] = (double) 0.0 ;
		m_dDeltaW[count] = (double) 0.0 ; 
	}

	m_massa = (double) 1.0 ;
	strcpy(m_szTextureFile, "") ;
	m_iRadius = (double) 1.0 ;
	m_iTextureID = iTextureID ;
    m_iPlanetTextureIndex = iPlanetTextureIndex ;
    m_bPlanet = bPlanet ;
}

CPlanet::CPlanet(double koord_exakt[],double hast[],double acc[],double massa)
{
	int count;	/* Räknare */

	for (count=0; count < 3; count++)
	{
		/* Avrundning av x,y och z koordinaterna är nödvändig */
		m_koord_exakt[count] = koord_exakt[count];
		m_koord[count] = (long int) round(koord_exakt[count]);
		m_hast[count] = hast[count];
		m_acceleration[count] = acc[count];
		m_dW[count] = (double) 0.0 ;
		m_dDeltaW[count] = (double) 0.0 ; 
	}

	m_massa = massa ;
	strcpy(m_szTextureFile, "") ;
	m_iRadius = (double) 1.0 ;
}

///////////////////////////////
// Destructor 
///////////////////////////////

CPlanet::~CPlanet()
{
   
}

///////////////////////////////
// Copy constructor 
///////////////////////////////

CPlanet::CPlanet(const CPlanet& PlanetCopy)
: m_massa(PlanetCopy.m_massa), m_iRadius(PlanetCopy.m_iRadius), 
  m_iTextureID(PlanetCopy.m_iTextureID), 
  m_iPlanetTextureIndex(PlanetCopy.m_iPlanetTextureIndex),           
  m_bPlanet(PlanetCopy.m_bPlanet)  
{
	int iCount ;

	for (iCount = 0; iCount < 3; iCount++)
	{
		m_koord[iCount] = PlanetCopy.m_koord[iCount] ;   	                 
		m_koord_exakt[iCount] = PlanetCopy.m_koord_exakt[iCount] ;              	
		m_hast[iCount] = PlanetCopy.m_hast[iCount] ;			             
		m_acceleration[iCount] = PlanetCopy.m_acceleration[iCount] ;	             
		m_dW[iCount] = PlanetCopy.m_dW[iCount] ;
		m_dDeltaW[iCount] = PlanetCopy.m_dDeltaW[iCount] ;    
	}

	if (PlanetCopy.m_szTextureFile != NULL && strlen(PlanetCopy.m_szTextureFile) > 0)
	{
		strcpy(m_szTextureFile, PlanetCopy.m_szTextureFile) ;
	}
	else
	{
		strcpy(m_szTextureFile, "") ;
	}

                  
}

///////////////////////////////
// Assignment operator 
///////////////////////////////

const CPlanet& CPlanet::operator= (const CPlanet& PlanetCopy)
{
	int iCount ;

	if (this != &PlanetCopy)
	{
		for (iCount = 0; iCount < 3; iCount++)
		{
			m_koord[iCount] = PlanetCopy.m_koord[iCount] ;   	                 
			m_koord_exakt[iCount] = PlanetCopy.m_koord_exakt[iCount] ;              	
			m_hast[iCount] = PlanetCopy.m_hast[iCount] ;			             
			m_acceleration[iCount] = PlanetCopy.m_acceleration[iCount] ;
			m_dW[iCount] = PlanetCopy.m_dW[iCount] ;
			m_dDeltaW[iCount] = PlanetCopy.m_dDeltaW[iCount] ; 
		}

		if (PlanetCopy.m_szTextureFile != NULL && strlen(PlanetCopy.m_szTextureFile) > 0)
		{
			strcpy(m_szTextureFile, PlanetCopy.m_szTextureFile) ;
		}
		else
		{
			strcpy(m_szTextureFile, "") ;
		}

		m_massa = PlanetCopy.m_massa ;
		m_iRadius = PlanetCopy.m_iRadius ;                       
		m_iTextureID = PlanetCopy.m_iTextureID ;                    
		m_iPlanetTextureIndex = PlanetCopy.m_iPlanetTextureIndex ;           
		m_bPlanet = PlanetCopy.m_bPlanet ;
	}
	return *this ;
}

///////////////////////////////
// Publika medlemsfunktioner
///////////////////////////////

/* Change texture file */
void CPlanet::setTexture(char* pszTextureFile)
{
	int strLen = 0 ;
    
	if (pszTextureFile == NULL)
	{
		strLen = 0 ;
		memset(m_szTextureFile, 0, MAXFILELENGTH) ;
		strcpy(m_szTextureFile, "") ;
	}
	else
	{
	    strLen = strlen(pszTextureFile) ;
		memset(m_szTextureFile, 0, MAXFILELENGTH) ;
		strcpy(m_szTextureFile, pszTextureFile) ;
	}
}

/* Get the texture's filename */
char* CPlanet::getTexture(void)
{
	return m_szTextureFile ;
}

/* Set the textures index to the planet or sun */
void CPlanet::setTextureIndex(int TextureIndex)
{
    m_iPlanetTextureIndex = TextureIndex ;
}

/* Get the textures index to the planet or sun */
int CPlanet::getTextureIndex(void)
{
	return m_iPlanetTextureIndex ;
}

/* Set the textures id of the planet */
void CPlanet::setTextureID(int TextureID)
{
    m_iTextureID = TextureID ;
}

/* Get the texture id of the planet */
int CPlanet::getTextureID(void)
{
	return m_iTextureID ;
}

/* Get the information if we have a planet or sun */
BOOLEAN CPlanet::getInfoIfPlanetOrSun(void)
{
	return m_bPlanet ;
}

/* Set the information if we have a planet or sun */
void CPlanet::setInfoIfPlanetOrSun(BOOLEAN bPlanet)
{
    m_bPlanet = bPlanet ;
}

/* Changes the exact and the display coordinates (as integer). */
void CPlanet::changeKoord(double koord_exakt[])
{
	int count;	/* Räknare */
	
	for (count=0; count < 3; count++)
	{
		m_koord_exakt[count]=koord_exakt[count];
		m_koord[count]=(long int)round(koord_exakt[count]);
	}	
}

/* Obtains the exact coordinates. */
void CPlanet::getCoordinates(double dPos[3])
{
	int iCount ;

	for (iCount = 0 ; iCount < 3 ; ++iCount)
	{
		dPos[iCount] = m_koord_exakt[iCount] ;
	}
}
		
/* Ändrar hastigheten */
void CPlanet::changeHast(double hast[])
{
	int count;	/* Räknare */
	
	for (count=0; count < 3; count++)
	{
		m_hast[count] = hast[count];
	}	
}

/* Ändrar accelerationen */
void CPlanet::changeAcc(double acc[])
{
	int count;	/* Räknare */
	
	for (count=0; count < 3; count++)
	{
		m_acceleration[count] = acc[count];
	}
}

/* Räknar ut avståndet mellan två planeter */
double CPlanet::distance(CPlanet *plan2, double mindist)
{
	double dist;	/* Temp-variabel för att lagra avståndet */
	int count;		/* Räknare */

	dist=0;
	
	for (count=0; count<3; count++)
	{
		dist += pow((m_koord_exakt[count]-plan2->m_koord_exakt[count]),2);
	}
	dist=sqrt(dist);

	if (dist > mindist)
	{
		return dist;
	}
	else
	{
		return mindist;
	}
}

/* Räknar ut och updaterar den nya positionen för en planet     */
void CPlanet::updatePos(double dt) 
{
	int count; /* Räknare */

	for (count=0; count < 3; count++)
	{
		m_koord_exakt[count] += m_acceleration[count]*dt*dt/2.0 + 
			                    m_hast[count]*dt;
		m_koord[count] = (long) round(m_koord_exakt[count]);
	}	
}

/* Räknar ut och updaterar den nya hastigheten för en planet    */
void CPlanet::updateHast(double dt)
{
	int count; /* Räknare */

	for (count=0; count < 3; count++)
	{
		m_hast[count] += m_acceleration[count]*dt;
	}
}

/* Räknar ut accelerationsbidraget från en planet */
void CPlanet::delAcc(CPlanet *plan2, double mindist, 
					 double G, double delta_acc[])
{
	int count;							/* Räknare */

	for (count=0; count < 3; count++)
	{
		delta_acc[count]=G*plan2->getMassa() * (-1) *
			( m_koord_exakt[count]-plan2->m_koord_exakt[count] ) /
			  pow( distance(plan2,mindist) , 3 );
	}
}

