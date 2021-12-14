
// CELESTIALTEXTURENAME, NROFPLANETTEXTURES, NROFSUNTEXTURES
// and MAXFILEPATHLENGTH are defined here.
#define NROFPLANETTEXTURES 6
#define NROFSUNTEXTURES 2
#define MAXTEXTUREFILELENGTH 64


#include <stdio.h>


/* PlanetSystem är en klass som innehåller */
/* information om planetsystemet.		   */
#include "CPlanet.h"

class CPlanetSystem
{
	public:

		///////////////////////////////
		// Konstruktorer
		///////////////////////////////

		CPlanetSystem(double dt = 0.01, double GKonst = 1000.0, long antSol = 1,
					  long antPlan = 1, double mindist = 1.0) ;

		///////////////////////////////
		// Destruktor
		///////////////////////////////

		~CPlanetSystem();

		///////////////////////////////
		// Publika Medlemsfunktioner
		///////////////////////////////
	public:
		/* Initierar planetsystemet med minsta och största massan */
		/* på planeter givna av minPlan och maxPlan, samt den     */
		/* största och minsta massan på stjärnorna ges av minSol  */
		/* och maxSol.											  */
		void initiera(double minPlan,double maxPlan,double minSol,
					  double maxSol,long max_koord[]) ;

		/* Saves the texture data for the planets and suns */
		void setAllTextures(char szPlanetTextures[NROFPLANETTEXTURES][MAXTEXTUREFILELENGTH],
						    char szSunTextures[NROFSUNTEXTURES][MAXTEXTUREFILELENGTH])	;
		
		/* Loads the bitmap texture in the CPlanetClass memory */
		int LoadBitmapTexture(BOOLEAN bPlanet, int iIndex,
							  char* szFileName) ;

		/* Bind the texture in OpenGL for the planet or sun */
		void bindTextureForPlanetOrSun(BOOLEAN bPlanet, int iNrPlanet) ;

		/* Get the bitmap width of the planet or sun index */
		int getBitmapTextureWidth(BOOLEAN bPlanet, int iPlanNr) ;

		/* Get the bitmap Height of the planet or sun index  */
		int getBitmapTextureHeight(BOOLEAN bPlanet, int iPlanNr) ;

		/* Beräknar accelerationerna för planeterna */
		void planAcc(void);

		/* Beräknar accelerationerna för solarna */
		void solAcc(void);
		
		/* Utför en förflyttning av planeterna och solarna */
		void enForflyttning(void);

		/* Lagrar information om en planets position */
		void get_planet(int index, long *position) ;

		/* Lagrar information om en solens position */
		void get_sol(int index, long *position) ;

		/* Gets the planet with index = iIndex */
		CPlanet getPlanet(int iIndex) ; 

		/* Gets the sun with index = iIndex */
		CPlanet getSun(int iIndex) ; 

		/* Returnerar antal planeter vi har */
		long  get_antalPlaneter() { return m_antPlan ; }
		
		/* Returnerar antal solar vi har */
		long  get_antalSolar() { return m_antSol ; }

		/* Returns the scaling of the planet's rotation or the sun's rotation */
		int  getRotationScalingPlanetsOrSuns(BOOLEAN bPlanet) 
		{
			// Planet
			if (bPlanet)
			{
				return m_iRotationScalePlanet ;
			}
			else
			{
				return m_iRotationScaleSun ;
			}
		}

		/* Sets the scaling of the planet's rotation or the sun's rotation */
		void  setRotationScalingPlanetsOrSuns(int iRotationScale, BOOLEAN bPlanet) 
		{
			// Planet
			if (bPlanet)
			{
				m_iRotationScalePlanet = iRotationScale ;
			}
			// Sun
			else
			{
				m_iRotationScaleSun = iRotationScale ;
			}
		}
		
		/* Set planet's texture */
		void setPlanetTexture(int index, char* szTextureFile) ;

		/* Get planet's texture (filename) */
		char* getPlanetTexture(int index) ;

		/* Set sun's texture */
		void setSunTexture(int index, char* szTextureFile) ;

		/* Get sun's texture (filename) */
		char* getSunTexture(int index) ;

		/* Set planet's or sun's texture id */
		void setPlanetOrSunTextureID(int iNrPlan, int iID, BOOLEAN bPlanet) ;

		/* Get planet's or sun's texture id */
		int getPlanetOrSunTextureID(int iNrPlan, BOOLEAN bPlanet) ;


		/* Set planet's or sun's texture index */
		void setPlanetOrSunTextureIndex(int iNrPlan, int iIndex, BOOLEAN bPlanet) ;

		/* Get planet's or sun's texture index */
		int getPlanetOrSunTextureIndex(int iNrPlan, BOOLEAN bPlanet) ;

		/* Set texture id for databas */
		void setTextureIDForDataBas(int iIndex, int iTextureID, BOOLEAN bPlanet) ;

		/* Get texture id for databas */
		int  getTextureIDFromDataBas(int iIndex, BOOLEAN bPlanet) ;

		/* Set texture index for databas */
		void  setBitmapInfoHeaderForDataBas(int iIndex, BITMAPINFOHEADER BitmapInfoHeader, 
											BOOLEAN bPlanet) ;

		/* Get texture index for databas */
		BITMAPINFOHEADER getBitmapInfoHeaderFromDataBas(int iIndex, BOOLEAN bPlanet) ;

		/* Add a new planet or sun. */
		void addNewPlanetOrSun(int iIndex, BOOLEAN bPlanet) ;

		/* Add planets or suns. */
		BOOLEAN addPlanetsOrSuns(int iNrOfPlanOrSun, BOOLEAN bPlanet) ;

		/* Delete one planet or sun. */
		void deletePlanetOrSun(int iPlanOrSunNr, BOOLEAN bPlanet) ;

		/* Get number of collisions between planets or suns and other planets or suns. */
		int getNumberOfCollisions(BOOLEAN bPlanet1, BOOLEAN bPlanet2) ;

		/* Check if one more collision exists between planets and suns (for one combination of */
		/* planets or suns) and update the planetsystem if one exists. */
		BOOLEAN checkIfMoreCollisionsAndUpdate(BOOLEAN bPlanet1, BOOLEAN bPlanet2) ;

		/* Check for collisions between planets and suns. */
		void checkForCollisionAndUpdate() ;

		/* Calculate the distance between a sun or planet with another sun or planet. */
		double distanceBetweenTwoPlanetsOrSuns(CPlanet* plan1, CPlanet* plan2, BOOLEAN bMinDist) ;

	private:

		///////////////////////////////
		// Medlemsvariabler
		///////////////////////////////

		double  m_mindist;
		double  m_dt;				                                         /* Tidsintervall                               */
		double  m_GKonst;			                                         /* Gravitationskonstanten                      */
		CPlanet **m_planeter;		                                         /* Planeterna                                  */
		CPlanet **m_solar ;			                                         /* Solarna                                     */
		long    m_antPlan, m_antSol ;                                        /* Antalet planeter och solar                  */
		long    m_antPlan_VectSize, m_antSol_VectSize ;                      /* The sizes of the planet and the sun vectors */
		double  m_dMinPlan, m_dMaxPlan, m_dMinSol, m_dMaxSol ;               /* Limiting values on the masses               */
		long    m_plMax_Koord[3] ;										     /* Limiting values on the coordinates          */
		int     m_piPlanetTextureID[NROFPLANETTEXTURES] ;                    /* Texture id:s data for planets               */
		int     m_piSunTextureID[NROFSUNTEXTURES] ;                          /* Texture id:s data for suns                  */									                                   
		BITMAPINFOHEADER  m_pPlanetsBitmapInfoHeader[NROFPLANETTEXTURES] ;   /* BITMAPINFOHEADER for planets                */
		BITMAPINFOHEADER  m_pSunsBitmapInfoHeader[NROFSUNTEXTURES] ;         /* BITMAPINFOHEADER for suns                   */

	public:
		unsigned char*    m_ppucPlanetTexture[NROFPLANETTEXTURES] ;          /* Texture data for planets                    */
		unsigned char*    m_ppucSunTexture[NROFSUNTEXTURES] ;                /* Texture data for suns                       */
		char m_szPlanetTextures[NROFPLANETTEXTURES][MAXTEXTUREFILELENGTH] ;  /* Saved texture names to the planets          */
		char m_szSunTextures[NROFSUNTEXTURES][MAXTEXTUREFILELENGTH] ;        /* Saved texture names to the suns             */
		int  m_iRotationScalePlanet, m_iRotationScaleSun ;					 /* Uniform scaling of the rotations of planets */
		                                                                     /* and suns.                                   */
} ;
