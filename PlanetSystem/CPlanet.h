
/************************************/
/* Namn:	 Joakim Kvistholm		*/
/* Datum:	 2000-08-03				*/
/* Filnamn:	 Planet.h				*/
/************************************/

/* Planet �r en klass som inneh�ller		*/
/* information om en planet.			    */

#include <windows.h>

#define MAXFILELENGTH 64

#include "COpenGLTexture.h"

// OpenGL defined here.
#ifndef ___OPENGL
#define ___OPENGL

#include <gl/gl.h>  // For OpenGL calls.
#include <gl/glu.h> // Only needed for extra functions to OpenGL.

#endif


/* PlanetSystem �r en v�nklass				*/
class CPlanetSystem ;

class CPlanet
{

	///////////////////////////////
	// V�nklass
	///////////////////////////////

	friend CPlanetSystem ;

	public:

		///////////////////////////////
		// Konstruktorer
		///////////////////////////////
        CPlanet() ;
		CPlanet(int iPlanetTextureIndex, int iTextureID, BOOLEAN bPlanet) ;
		CPlanet(double massa, double radius, char* szTextureFile) ;
		CPlanet(double koord_exakt[],double hast[],double acc[],double massa) ;

		///////////////////////////////
		// Copy constructor 
		///////////////////////////////

		CPlanet(const CPlanet& PlanetCopy) ;

		///////////////////////////////
		// Assignment operator 
		///////////////////////////////

		const CPlanet& operator= (const CPlanet& PlanetCopy) ;

		///////////////////////////////
		// Destruktor
		///////////////////////////////

		~CPlanet() ;

		///////////////////////////////
		// Public Member functions
		///////////////////////////////

		/* Get the information if we have a planet or sun */
		BOOLEAN getInfoIfPlanetOrSun(void) ;

		/* Set the information if we have a planet or sun */
		void setInfoIfPlanetOrSun(BOOLEAN bPlanet) ;

		/* Set the textures index to the planet or sun */
		void setTextureIndex(int TextureIndex) ;

		/* Get the textures index to the planet or sun */
		int getTextureIndex(void) ;

		/* Sets the textures id to planet */
		void setTextureID(int TextureID) ;

		/* Get the texture id of the planet */
		int getTextureID(void) ;

		/* Change texture file */
		void setTexture(char* pszTextureFile) ;

        /* Get the texture's filename */
		char* getTexture(void) ;

		/* �ndrar koordinaterna */
		void changeKoord(double koord_exakt[]) ;

		/* Obtains the exact coordinates. */		
		void getCoordinates(double dPos[3]) ;

		/* �ndrar hastigheten */
		void changeHast(double hast[]) ;

		/* �ndrar accelerationen */
		void changeAcc(double acc[]) ;

		/* �ndrar massan */
		void changeMassa(double massa) { m_massa=massa ; } 

		/* Returnerar x, y och z koordinaterna f�r planeten */
		long int* getKoord(void) { return m_koord ; }
			
		/* Returnerar de exakta x, y och z koordinaterna f�r planeten */
		double* getKoord_Exakt(void) { return m_koord_exakt ; }

		/* Returnerar hastigheten i x -, y - och z - led */
		void getVelocity(double dVelocity[3]) 
		{   int iCount ;

			for (iCount = 0; iCount <3; ++iCount)
			{
				dVelocity[iCount] = m_hast[iCount];
			}
		} ;

		/* Returnerar accelerationen i x -, y - och z - led */
		double* getAcc(void) { return m_acceleration ; }

		/* Returns the mass of the planet */
		double getMassa(void) { return m_massa ; }

		/* Returns the radius of the planet */
		double getRadius(void) { return m_iRadius ; }

		/* Change the radius of the planet */
		void setRadius(double Radius) 
		{ 
			if (Radius <= 0.0)
				m_iRadius = (double) 1.0 ;
			else
				m_iRadius = Radius ; 
		}

		/* R�knar ut avst�ndet mellan tv� planeter        */
		/* mindist anger minsta avst�ndet f�r vilken      */
		/* den enkla gravitatrionslagen skall g�lla       */ 
		double distance(CPlanet *plan2, double mindist) ;

		/* R�knar ut och updaterar den nya positionen f�r en planet  */
		/* d� accelerationen �r given								 */
		/* mindist anger minsta avst�ndet f�r vilken				 */
		/* den enkla gravitatrionslagen skall g�lla					 */
		/* dt �r tidsintervallet f�r updateringen					 */
		void updatePos(double dt) ; 

		/* R�knar ut och updaterar den nya hastigheten f�r en planet */
		/* d� accelerationen �r given								 */
		/* mindist anger minsta avst�ndet f�r vilken				 */
		/* den enkla gravitatrionslagen skall g�lla					 */
		/* dt �r tidsintervallet f�r updateringen					 */
		void updateHast(double dt) ;

		/* R�knar ut accelerationsbidraget fr�n en planet			 */
		/* mindist anger minsta avst�ndet f�r vilken				 */
		/* den enkla gravitatrionslagen skall g�lla					 */
		/* G �r gravitationskonstanten, delta_acc[] = en vektor med  */
		/* accelerationsbidragen i x, y och z						 */													 
		void delAcc(CPlanet *plan2, double mindist, double G, 
					double delta_acc[]) ;

	public:
		/* Instance variables */
		double         m_dW[3] ;                         /* The rotation of the planet                        */
		double         m_dDeltaW[3] ;                    /* The speed of rotation of the planet.              */


	private:
		/* Instance variabler */
		long int       m_koord[3];   	                 /* The coordinates of the planets in x,y and z       */
		double         m_koord_exakt[3];              	 /* The position in x,y and z				          */
		double         m_hast[3];			             /* The velocity in x,y and z				          */
		double         m_acceleration[3];	             /* The acceleration in x,y and z			          */
		double         m_massa;		  	                 /* The mass of the planet					          */
		char           m_szTextureFile[MAXFILELENGTH] ;  /* The name of the planet texture			          */
		double         m_iRadius ;                       /* The radius of the planet					      */
		int		       m_iTextureID ;                    /* The texture ID of the planet					  */
		int		       m_iPlanetTextureIndex ;           /* The texture index of the planet	or sun            */
		BOOLEAN        m_bPlanet ;                       /* Classifies the object as a planet or sun          */
} ;
