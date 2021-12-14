
/************************************/
/* Namn:	 Joakim Kvistholm		*/
/* Datum:	 2000-08-03				*/
/* Filnamn:	 PlanetSystem.cpp		*/
/************************************/


#include <math.h>			/* För att anropa abs, sqrt, pow */

#include <stdlib.h>			/* För att anropa rand           */
#include <time.h>			/* För att anropa time           */
#include "round.h"			/* För att anropa round	         */
#include "slumptal.h"		/* För att anropa slumptal       */
#include <memory.h>         /* För att anropa memcpy, memset */
#include <assert.h>         /* För att anropa assert         */
#include <crtdbg.h>         /* För att anropa _free_dbg      */

#include "CPlanetSystem.h"

// MAXFILEPATHLENGTH, NROFPLANETTEXTURES and NROFSUNTEXTURES
// defined in the file: CPlanetSystem.h.


///////////////////////////////
// Konstruktorer
///////////////////////////////

CPlanetSystem::CPlanetSystem(double dt,double GKonst,long antSol,
				 		     long antPlan, double mindist)
{
	int iCount ;

	m_dt		= dt ;
	m_GKonst	= GKonst ;
	m_antSol_VectSize  = m_antSol  = antSol ;
	m_antPlan_VectSize = m_antPlan = antPlan ;
	m_mindist	= mindist ;

	/* Initializing data (possible starting values) */
	for (iCount = 0; iCount < 3 ; iCount++)
	{
		m_plMax_Koord[iCount] = 100 ; 
	}
	m_dMinPlan = 100 ;
	m_dMaxPlan = 400 ;
	m_dMinSol = 20000 ;
	m_dMaxSol = 80000 ;

	// Reset planet textures.
	memset(m_szPlanetTextures[iCount], 0, NROFPLANETTEXTURES)  ; 

	// Reset sun textures.
	memset(m_szSunTextures[iCount], 0, NROFSUNTEXTURES) ; 

	// Set the scaling factors for the rotations to one.
	m_iRotationScalePlanet = m_iRotationScaleSun = 1 ;
}

///////////////////////////////
// Destruktor
///////////////////////////////

CPlanetSystem::~CPlanetSystem()
{
	int i ;

	for(i=0;i<m_antPlan;i++)
		delete m_planeter[i] ;

	free(m_planeter) ;

	for(i=0;i<m_antSol;i++)
		delete m_solar[i];

	free(m_solar) ;


	
	if (m_ppucPlanetTexture != NULL)
	{
		for (i = 0; i < NROFPLANETTEXTURES; i++)
		{
			if (m_ppucPlanetTexture[i] != NULL)
			{
				free(m_ppucPlanetTexture[i]) ;
			}
		}
	}

	if (m_ppucSunTexture != NULL)
	{
		for (i = 0; i < NROFSUNTEXTURES; i++)
		{
			if (m_ppucSunTexture[i] != NULL)
			{
				free(m_ppucSunTexture[i]) ;		
			}
		}
	}
}

///////////////////////////////
// Publika Medlemsfunktioner
///////////////////////////////

/* Initierar planetsystemet */
void CPlanetSystem::initiera(double minPlan,double maxPlan,double minSol,
							 double maxSol,long max_koord[])
{
	long count1, count2, iCount ;	/* Räknare */
	long int koord[3];		        /* Utritningskoordinater i x,y och z */
	double koord_exakt[3];	        /* Position i x,y och z */
	double hast[3];			        /* Hastighet i x,y och z */
	double acc[3];			        /* Accelerationen i x,y och z */
	double massa;			        /* Massan på planeten */

	srand(time(NULL));		        /* Sår ett slumpfrö */

	m_planeter = (CPlanet **) malloc(sizeof(CPlanet*) * m_antPlan) ;
	m_solar = (CPlanet **) malloc(sizeof(CPlanet*) * m_antSol) ;

	/* Saving the initializing data */
	for (count1 = 0; count1 < 3 ; count1++)
	{
		m_plMax_Koord[count1] = max_koord[count1] ; 
	}
	m_dMinPlan = minPlan ;
	m_dMaxPlan = maxPlan ;
	m_dMinSol = minSol ;
	m_dMaxSol = maxSol ;

	/* Solar */
	
	/* Uppräkning av solarna */
	for (count1=0; count1 < m_antSol; count1++)	
	{
		/* Uppräkning över x,y och z */
		for (count2=0; count2 < 3; count2++)	
		{
			if (m_antSol==1)
				koord_exakt[count2]=(double)(max_koord[count2]/2);
			else
				koord_exakt[count2] = slumptal((double) (max_koord[count2]*0.25),
										 	   (double) (max_koord[count2]*0.75));

			koord[count2] = (long) round(koord_exakt[count2]);

			/* En sol (fixera solen) */
			if (m_antSol == 1)
			{
				hast[count2] = 0;
			}
			/* Flera solar (ge dem hastighet) */
			else
			{
				hast[count2] = 0.5 * slumptal(-1,1);
			}

			acc[count2] = 0;
		}
		massa = slumptal(minSol,maxSol);

		m_solar[count1] = new CPlanet(koord_exakt, hast, acc, massa) ;		
		m_solar[count1]->setRadius(1.0 * pow(massa, (double) 1.0/3.0)) ;
		m_solar[count1]->setInfoIfPlanetOrSun(FALSE) ; // It's a sun

		// Rotation of the sun.
		for (iCount = 0 ; iCount < 3; ++iCount)
		{
			// Creates rotation
			m_solar[count1]->m_dW[iCount] = (double) 0.0 ;
			m_solar[count1]->m_dDeltaW[iCount] = 1.0 * slumptal(-0.05, 0.05) ;			
		}

	}

	/* Planeter */

	/* Uppräkning av planeterna */
	for(count1=0; count1 < m_antPlan; count1++) 
	{
		/* Uppräkning över x,y och z */
		for (count2=0; count2 < 3; count2++) 
		{
			koord_exakt[count2] = slumptal(0.0,(double)max_koord[count2]);
			koord[count2]= (long) round(koord_exakt[count2]);
			hast[count2] = 2.0 * slumptal(-20,20);
			acc[count2] = 0;
		}
		massa = slumptal(minPlan, maxPlan);

		m_planeter[count1] = new CPlanet(koord_exakt,hast,acc,massa) ;
		m_planeter[count1]->setRadius(1.0 * pow(massa, (double) 1.0/3.0)) ;
		m_planeter[count1]->setInfoIfPlanetOrSun(TRUE) ;  // It's a planet

		// Rotation of the planet.
		for (iCount = 0 ; iCount < 3; ++iCount)
		{
			// Creates rotation
			m_planeter[count1]->m_dW[iCount] = (double) 0.0 ;
			m_planeter[count1]->m_dDeltaW[iCount] = 1.0 * slumptal(-0.45, 0.45) ;			
		}
	}
}

/* Saves the texture data for the planets and suns */
void CPlanetSystem::setAllTextures(char szPlanetTextures[NROFPLANETTEXTURES][MAXTEXTUREFILELENGTH],
						           char szSunTextures[NROFSUNTEXTURES][MAXTEXTUREFILELENGTH])
{
	int NrOfPlanets = 0 ;
	int NrOfSuns = 0 ;
	int count, randNr ;
	int iTextureID ;

	for (count = 0 ; count < NROFPLANETTEXTURES; count++)   // Planet
	{
		// Copy the texture names (planets).
		strcpy(m_szPlanetTextures[count], szPlanetTextures[count]) ;
		iTextureID = LoadBitmapTexture(TRUE, count, szPlanetTextures[count]) ;
        setTextureIDForDataBas(count, iTextureID, TRUE) ;  // Planet
	}

	for (count = 0 ; count < NROFSUNTEXTURES; count++)    // Sun
	{
		// Copy the texture names (suns).
		strcpy(m_szSunTextures[count], szSunTextures[count]) ;
		iTextureID = LoadBitmapTexture(FALSE, count, szSunTextures[count]) ;
        setTextureIDForDataBas(count, iTextureID, FALSE) ; // Sun
	}


	NrOfPlanets = (int) get_antalPlaneter() ;
	NrOfSuns    = (int) get_antalSolar() ;

    //glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping

	// Set the planets textures.
	for (count = 0 ; count < NrOfPlanets ; count++)  // Planet
	{
		randNr = (int) round(slumptal(0.0, (double) (NROFPLANETTEXTURES - 1))) ;
		setPlanetOrSunTextureIndex(count, randNr, TRUE) ;
		setPlanetTexture(count, szPlanetTextures[randNr]) ;
		setPlanetOrSunTextureID(count, getTextureIDFromDataBas(randNr, TRUE), TRUE) ;  // Planet
	}

	// Set the suns textures first.
	for (count = 0 ; count < NrOfSuns ; count++)   // Sun
	{
		randNr = (int) round(slumptal(0.0, (double) (NROFSUNTEXTURES - 1))) ;
		setPlanetOrSunTextureIndex(count, randNr, FALSE) ;
		setSunTexture(count, szSunTextures[randNr]) ;
		setPlanetOrSunTextureID(count, getTextureIDFromDataBas(randNr, FALSE), FALSE) ;   // Sun
	}
}

/* Loads the bitmap texture in the CPlanetClass memory */
int CPlanetSystem::LoadBitmapTexture(BOOLEAN bPlanet, int iIndex,
					                 char* szFileName)
{
    int i, j = 0 ; // Index variables
    FILE *pFile;   // File pointer
	int iStrLen = 0 ;
	int iWidth, iHeight ;
    
    // windows.h gives us these types to work with the Bitmap files
    BITMAPFILEHEADER fileheader; 
    BITMAPINFOHEADER infoheader;
    RGBTRIPLE rgb;

    num_texture++ ; // The counter of the current texture is increased

    if((szFileName == NULL) || ((pFile = fopen(szFileName, "rb")) == NULL)) 
		return (-1); // Open the file for reading
    
    fread(&fileheader, sizeof(fileheader), 1, pFile); // Read the fileheader
    
    fseek(pFile, sizeof(fileheader), SEEK_SET); // Jump the fileheader
    fread(&infoheader, sizeof(infoheader), 1, pFile); // and read the infoheader

	if (bPlanet) // Planet
	{
		// Copy the info header for planets
		setBitmapInfoHeaderForDataBas(iIndex, infoheader, TRUE) ;

		// Now we need to allocate the memory for our image (width * height * color deep)
		m_ppucPlanetTexture[iIndex] = (byte *) malloc(infoheader.biWidth * infoheader.biHeight * 4) ;

		// And fill it with zeros
		memset(m_ppucPlanetTexture[iIndex], 0, infoheader.biWidth * infoheader.biHeight * 4);
 
		// At this point we can read every pixel of the image
		for (i = 0; i < infoheader.biWidth * infoheader.biHeight; i++)
		{            
				// We load an RGB value from the file
				fread(&rgb, sizeof(rgb), 1, pFile); 

				// And store it
				m_ppucPlanetTexture[iIndex][j+0] = rgb.rgbtRed;   // Red component
				m_ppucPlanetTexture[iIndex][j+1] = rgb.rgbtGreen; // Green component
				m_ppucPlanetTexture[iIndex][j+2] = rgb.rgbtBlue;  // Blue component
				m_ppucPlanetTexture[iIndex][j+3] = 255;           // Alpha value
				j += 4 ; // Go to the next position
		}
	}
	else
	{
		// Copy the info header for suns
		setBitmapInfoHeaderForDataBas(iIndex, infoheader, FALSE) ;

		// Now we need to allocate the memory for our image (width * height * color deep)
		m_ppucSunTexture[iIndex] = (byte *) malloc(infoheader.biWidth * infoheader.biHeight * 4);
		// And fill it with zeros
		memset(m_ppucSunTexture[iIndex], 0, infoheader.biWidth * infoheader.biHeight * 4);
 
		// At this point we can read every pixel of the image
		for (i = 0; i < infoheader.biWidth * infoheader.biHeight; i++)
		{            
				// We load an RGB value from the file
				fread(&rgb, sizeof(rgb), 1, pFile); 

				// And store it
				m_ppucSunTexture[iIndex][j+0] = rgb.rgbtRed;   // Red component
				m_ppucSunTexture[iIndex][j+1] = rgb.rgbtGreen; // Green component
				m_ppucSunTexture[iIndex][j+2] = rgb.rgbtBlue;  // Blue component
				m_ppucSunTexture[iIndex][j+3] = 255;           // Alpha value
				j += 4 ; // Go to the next position
		}
	}
    fclose(pFile); // Closes the file stream 
	
	glBindTexture(GL_TEXTURE_2D, num_texture) ; // Bind the ID texture specified by the 2nd parameter

    // The next commands sets the texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // If the u,v coordinates overflow the range 0,1 the image is repeated
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // The magnification function ("linear" produces better results)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); //The minifying function

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // We don't combine the color with the original surface color, use only the texture map.

	if (bPlanet) // Planet
	{
		iWidth = infoheader.biWidth ;
		iHeight = infoheader.biHeight ;

		// Finally we define the 2d texture
		glTexImage2D(GL_TEXTURE_2D, 0, 4, iWidth, iHeight, 0, 
			         GL_RGBA, GL_UNSIGNED_BYTE, m_ppucPlanetTexture[iIndex]) ;


		// And create 2d mipmaps for the minifying function
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, iWidth, iHeight, GL_RGBA, 
			              GL_UNSIGNED_BYTE, m_ppucPlanetTexture[iIndex]) ;

	}
	else  // Sun
	{
		iWidth = infoheader.biWidth ;
		iHeight = infoheader.biHeight ;

		// Finally we define the 2d texture
		glTexImage2D(GL_TEXTURE_2D, 0, 4, iWidth, iHeight, 0, 
			         GL_RGBA, GL_UNSIGNED_BYTE, m_ppucSunTexture[iIndex]) ;

		// And create 2d mipmaps for the minifying function
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, iWidth, iHeight, GL_RGBA, 
			              GL_UNSIGNED_BYTE, m_ppucSunTexture[iIndex]) ;

	}
    return (num_texture) ; // Returns the current texture OpenGL ID	
}

/* Bind the texture in OpenGL for the planet or sun */
void CPlanetSystem::bindTextureForPlanetOrSun(BOOLEAN bPlanet, int iNrPlanet)
{
	int              iIndex, iID ;
	BITMAPINFOHEADER BitmapInfoHeader ;


	if (bPlanet)   // Planet
	{
		iIndex = getPlanetOrSunTextureIndex(iNrPlanet, TRUE) ;
		BitmapInfoHeader = getBitmapInfoHeaderFromDataBas(iIndex, TRUE) ;
		iID = getPlanetOrSunTextureID(iNrPlanet, TRUE) ;
	}
	else           // Sun
	{
		iIndex = getPlanetOrSunTextureIndex(iNrPlanet, FALSE) ;
		BitmapInfoHeader = getBitmapInfoHeaderFromDataBas(iIndex, FALSE) ;
		iID = getPlanetOrSunTextureID(iNrPlanet, FALSE) ;
	}
}

/* Beräknar accelerationerna för planeterna */
void CPlanetSystem::planAcc(void)
{
	/* Tempvektorer för acceleration */
	double acc[3], delacc[3];
	int count_1, count_2, count_3;	/* Räknare */

	/* Planet i */
	for (count_1=0; count_1 < m_antPlan; count_1++) 
	{
		memset(delacc,0,sizeof(double)*3);	/* Nollställer delacc vektorn */
		memset(acc,0,sizeof(double)*3);		/* Nollställer acc vektorn */

		/* Planet j */		
		for (count_2=0; count_2 < m_antPlan; count_2++) 

		{				
			if (count_1!=count_2)
			{
				/* Planet j:s accelerationsbidrag till planet i */
				/* ges av delacc								*/
				m_planeter[count_1]->delAcc(m_planeter[count_2], 
											m_mindist, m_GKonst, delacc);
				for (count_3=0; count_3 < 3; count_3++)
				{
					acc[count_3] += delacc[count_3];
				}
			}
		}

		/* Accelerationsbidraget från planeterna */
		m_planeter[count_1]->changeAcc(acc) ;

		memset(delacc,0,sizeof(double)*3);	/* Nollställer delacc vektorn */
		memset(acc,0,sizeof(double)*3);		/* Nollställer acc vektorn */

		/* Sol j */
		for (count_2=0; count_2 < m_antSol; count_2++) 
		{
			/* Sol j:s accelerationsbidrag till planet i ges av delacc */
			m_planeter[count_1]->delAcc(m_solar[count_2], m_mindist,
										m_GKonst,delacc);
			for (count_3=0; count_3 < 3; count_3++)
			{
				acc[count_3] += delacc[count_3];
			}
		}
		
		/* Lägg till accelerationsbidraget från solarna */
		for (count_3=0; count_3 < 3; count_3++)
		{
			acc[count_3] += m_planeter[count_1]->getAcc()[count_3];
		}

		m_planeter[count_1]->changeAcc(acc) ;
	}	
}

/* Beräknar accelerationerna för solarna */
void CPlanetSystem::solAcc(void)
{
	/* Tempvektorer för acceleration */
	double acc[3], delacc[3];
	int count_1, count_2, count_3;	/* Räknare */

	/* Sol i */
	for (count_1=0; count_1 < m_antSol; count_1++) 
	{
		memset(delacc,0,sizeof(double)*3);	/* Nollställer delacc vektorn */
		memset(acc,0,sizeof(double)*3);		/* Nollställer acc vektorn */

		/* Sol j */		
		for (count_2=0; count_2 < m_antSol; count_2++) 

		{				
			if (count_1!=count_2)
			{
				/* Sol j:s accelerationsbidrag till sol i ges av delacc */
				m_solar[count_1]->delAcc(m_solar[count_2], m_mindist,
										 m_GKonst,delacc);
				for (count_3=0; count_3 < 3; count_3++)
				{
					acc[count_3] += delacc[count_3];
				}
			}
		}

		/* Accelerationsbidraget från solarna */
		m_solar[count_1]->changeAcc(acc) ;

		memset(delacc,0,sizeof(double)*3);	/* Nollställer delacc vektorn */
		memset(acc,0,sizeof(double)*3);		/* Nollställer acc vektorn */

		/* Planet j */
		for (count_2=0; count_2 < m_antPlan; count_2++) 
		{
			/* Planet j:s accelerationsbidrag till sol i ges av delacc */
			m_solar[count_1]->delAcc(m_planeter[count_2], m_mindist,
									 m_GKonst,delacc);
			for (count_3=0; count_3 < 3; count_3++)
			{
				acc[count_3] += delacc[count_3];
			}
		}
		
		/* Lägg till accelerationsbidraget från planeterna */
		for (count_3=0; count_3 < 3; count_3++)
		{
			acc[count_3] += m_solar[count_1]->getAcc()[count_3];
		}

		m_solar[count_1]->changeAcc(acc) ;
	}	
}

/* Utför förflyttningen av himlakropparna under tidssteget m_dt */
void CPlanetSystem::enForflyttning(void)
{
	int count, iCount ;	/* Räknare */
	double dRotationScalePlanet = 1.0, dRotationScaleSun = 1.0 ; /* Scaling the rotations */

	// Check if correct indata was obtained.
	if (m_iRotationScalePlanet > 0)
	{
		dRotationScalePlanet = (double) m_iRotationScalePlanet ;
	}

	if (m_iRotationScaleSun > 0)
	{
		dRotationScaleSun = (double) m_iRotationScaleSun ;
	}

	/* Planeter */

	/* Beräknar accelerationen för planeterna */
	planAcc();

	/* Beräknar hastigheten och positionerna för planeterna */
	for (count=0; count < m_antPlan; count++)
	{
		m_planeter[count]->updateHast(m_dt);
		m_planeter[count]->updatePos(m_dt);

		// Rotation of the planet.
		for (iCount = 0 ; iCount < 3; ++iCount)
		{
			// Creates rotation
			m_planeter[count]->m_dW[iCount] += (m_planeter[count]->m_dDeltaW[iCount] / dRotationScalePlanet) ;
			m_planeter[count]->m_dW[iCount] = fmod(m_planeter[count]->m_dW[iCount], 360.0) ;
		}
	}

	/* Solar */

	/* Beräknar accelerationen för solarna */
	solAcc();

	/* Beräknar hastigheten och positionerna för solarna */
	for (count=0; count < m_antSol; count++)
	{
		m_solar[count]->updateHast(m_dt);
		m_solar[count]->updatePos(m_dt);

		// Rotation of the sun.
		for (iCount = 0 ; iCount < 3; ++iCount)
		{
			// Creates rotation
			m_solar[count]->m_dW[iCount] += (m_solar[count]->m_dDeltaW[iCount] / dRotationScaleSun) ;
			m_solar[count]->m_dW[iCount] = fmod(m_solar[count]->m_dW[iCount], 360.0) ;
		}
	}
}

/* Returnerar koordinaterna (i position) för planet nr index */
void CPlanetSystem::get_planet(int index, long *position)
{
	long* tmp ;

	/* Kontroll om vi indexerat rätt */
	assert(index >=0 && index < m_antPlan) ;

	tmp = m_planeter[index]->getKoord() ;

    memcpy(position, tmp, sizeof(long) * 3) ; 
}

/* Returnerar koordinaterna (i position) för sol nr index */
void CPlanetSystem::get_sol(int index, long *position)
{
	long* tmp ;

	/* Kontroll om vi indexerat rätt */
	assert(index >=0 && index < m_antSol) ;

	tmp = m_solar[index]->getKoord() ;

    memcpy(position, tmp, sizeof(long) * 3) ; 
}

/* Set planet's or sun's texture index */
void CPlanetSystem::setPlanetOrSunTextureIndex(int iNrPlan, int iIndex, BOOLEAN bPlanet)
{
	if (bPlanet)   // Planet
	{
		m_planeter[iNrPlan]->setTextureIndex(iIndex) ;
	}
	else    // Sun
	{
		m_solar[iNrPlan]->setTextureIndex(iIndex) ;
	}
}

/* Get planet's or sun's texture index */
int CPlanetSystem::getPlanetOrSunTextureIndex(int iNrPlan, BOOLEAN bPlanet)
{
	int iIndex ;

	if (bPlanet)   // Planet
	{
		iIndex = m_planeter[iNrPlan]->getTextureIndex() ;
	}
	else   // Sun
	{
		iIndex = m_solar[iNrPlan]->getTextureIndex() ;
	}
	return iIndex ;
}

/* Set planet's or sun's texture id */
void CPlanetSystem::setPlanetOrSunTextureID(int iNrPlan, int iID, BOOLEAN bPlanet)
{
	if (bPlanet)   // Planet
	{
		m_planeter[iNrPlan]->setTextureID(iID) ;
	}
	else   // Sun
	{
		m_solar[iNrPlan]->setTextureID(iID) ;
	}
}

/* Get planet's or sun's texture id */
int CPlanetSystem::getPlanetOrSunTextureID(int iNrPlan, BOOLEAN bPlanet)
{
	int iID ;

	if (bPlanet)   // Planet
	{
		iID = m_planeter[iNrPlan]->getTextureID() ;
	}
	else  // Sun
	{
		iID = m_solar[iNrPlan]->getTextureID() ;
	}
	return iID ;
}

/* Set planet's texture */
void CPlanetSystem::setPlanetTexture(int index, char* szTextureFile)
{
	m_planeter[index]->setTexture(szTextureFile) ;
}

/* Get planet's texture (filename) */
char* CPlanetSystem::getPlanetTexture(int index)
{
	return m_planeter[index]->getTexture() ;
}

/* Set sun's texture */
void CPlanetSystem::setSunTexture(int index, char* szTextureFile)
{
	m_solar[index]->setTexture(szTextureFile) ;
}

/* Get sun's texture (filename) */
char* CPlanetSystem::getSunTexture(int index)
{
	return m_solar[index]->getTexture() ;
}

/* Set texture id for databas */
void CPlanetSystem::setTextureIDForDataBas(int iIndex, int iTextureID, BOOLEAN bPlanet)
{
	if (bPlanet)   // Planet
	{
		m_piPlanetTextureID[iIndex] = iTextureID ;
	}
	else           // Sun
	{
		m_piSunTextureID[iIndex] = iTextureID ;
	}
}

/* Get texture id for databas */
int  CPlanetSystem::getTextureIDFromDataBas(int iIndex, BOOLEAN bPlanet) 
{
	int iTextureID ;

	if (bPlanet)   // Planet
	{
		iTextureID = m_piPlanetTextureID[iIndex] ;
	}
	else           // Sun
	{
		iTextureID = m_piSunTextureID[iIndex] ;
	}
	return iTextureID ;
}

/* Set texture index for databas */
void  CPlanetSystem::setBitmapInfoHeaderForDataBas(int iIndex, 
												   BITMAPINFOHEADER BitmapInfoHeader, 
												   BOOLEAN bPlanet)
{
	if (bPlanet)   // Planet
	{
		m_pPlanetsBitmapInfoHeader[iIndex] = BitmapInfoHeader ;
	}
	else           // Sun
	{
		m_pSunsBitmapInfoHeader[iIndex] = BitmapInfoHeader ;
	}
}

/* Get texture index for databas */
BITMAPINFOHEADER CPlanetSystem::getBitmapInfoHeaderFromDataBas(int iIndex, BOOLEAN bPlanet)
{
	BITMAPINFOHEADER BitmapInfoHeader ;

	if (bPlanet)   // Planet
	{
		BitmapInfoHeader = m_pPlanetsBitmapInfoHeader[iIndex] ;
	}
	else           // Sun
	{
		BitmapInfoHeader = m_pSunsBitmapInfoHeader[iIndex] ;
	}
	return BitmapInfoHeader ;
}

/* Gets the planet with index = iIndex */
CPlanet CPlanetSystem::getPlanet(int iIndex) 
{         
	return *(this->m_planeter[iIndex]) ; 
}

/* Gets the sun with index = iIndex */
CPlanet CPlanetSystem::getSun(int iIndex)
{
	return *(this->m_solar[iIndex]) ; 
}

/* Get the bitmap width of the planet or sun index */
int CPlanetSystem::getBitmapTextureWidth(BOOLEAN bPlanet, int iPlanNr)
{
	int iIndex ;

	if (bPlanet)
	{
		iIndex = getPlanet(iPlanNr).getTextureIndex() ;
		return m_pPlanetsBitmapInfoHeader[iIndex].biWidth ;
	}
	else
	{
		iIndex = getSun(iPlanNr).getTextureIndex() ;
		return m_pSunsBitmapInfoHeader[iIndex].biWidth ;
	}
}

/* Get the bitmap Height of the planet or sun index  */
int CPlanetSystem::getBitmapTextureHeight(BOOLEAN bPlanet, int iPlanNr)
{
	int iIndex ;

	if (bPlanet)
	{
		iIndex = getPlanet(iPlanNr).getTextureIndex() ;
		return m_pPlanetsBitmapInfoHeader[iIndex].biHeight ;
	}
	else
	{
		iIndex = getSun(iPlanNr).getTextureIndex() ;
		return m_pSunsBitmapInfoHeader[iIndex].biHeight ;
	}
}


/* Add a new planet or sun. */
void CPlanetSystem::addNewPlanetOrSun(int iIndex, BOOLEAN bPlanet)
{
	int    iCount, iRandNr ;
	double dMassa ;
	double pdKoordExakt[3], pdHast[3], pdAcc[3] ;


	// Planet
	if (bPlanet)
	{
		/* Creating speed, coordinates, mass and radius */
		for (iCount=0; iCount < 3; iCount++) 
		{
			pdKoordExakt[iCount] = slumptal(0.0,(double) m_plMax_Koord[iCount]);
			pdHast[iCount] = 2.0 * slumptal(-20,20) ;
			pdAcc[iCount] = 0.0 ;
		}
		dMassa = slumptal(m_dMinPlan, m_dMaxPlan) ;

		if (m_planeter[iIndex] != NULL)
		{
			delete m_planeter[iIndex] ;
		}

		m_planeter[iIndex] = new CPlanet(pdKoordExakt, pdHast, pdAcc, dMassa) ;
		++m_antPlan ;

		m_planeter[iIndex]->setRadius(1.0 * pow(dMassa, (double) 1.0/3.0)) ;
		m_planeter[iIndex]->setInfoIfPlanetOrSun(TRUE) ;  // It's a planet

		// Rotation of the planet.
		for (iCount = 0 ; iCount < 3; ++iCount)
		{
			// Creates rotation
			m_planeter[iIndex]->m_dW[iCount] = (double) 0.0 ;
			m_planeter[iIndex]->m_dDeltaW[iCount] = 1.0 * slumptal(-0.45, 0.45) ;			
		}

		// Set the planet texture.
		iRandNr = (int) round(slumptal(0.0, (double) (NROFPLANETTEXTURES - 1))) ;
		setPlanetOrSunTextureIndex(iIndex, iRandNr, TRUE) ;
		m_planeter[iIndex]->setTexture((char*) m_szPlanetTextures[iRandNr]) ;
		setPlanetOrSunTextureID(iIndex, getTextureIDFromDataBas(iRandNr, TRUE), TRUE) ;  // Planet
	}
	// Sun
	else
	{	
		/* Creating speed, coordinates, mass and radius */
		for (iCount = 0 ; iCount < 3; ++iCount) 
		{
			// Only one sun - set it at rest.
			if (m_antSol == 0)
			{
				pdKoordExakt[iCount] = (double) (m_plMax_Koord[iCount]/2) ;

				pdHast[iCount] = 0.0 ;
			}
			// More than one sun.
			else
			{
				pdKoordExakt[iCount] = slumptal((double) (m_plMax_Koord[iCount] * 0.25),
										 	    (double) (m_plMax_Koord[iCount] * 0.75)) ;

				pdHast[iCount] = 0.5 * slumptal(-1.0, 1.0) ;
			}
			pdAcc[iCount] = 0.0 ;
		}
		dMassa = slumptal(m_dMinSol, m_dMaxSol) ;

		if (m_solar[iIndex] != NULL)
		{
			delete m_solar[iIndex] ;
		}

		m_solar[iIndex] = new CPlanet(pdKoordExakt, pdHast, pdAcc, dMassa) ;
		++m_antSol ;

		m_solar[iIndex]->setRadius(1.0 * pow(dMassa, (double) 1.0/3.0)) ;
		m_solar[iIndex]->setInfoIfPlanetOrSun(FALSE) ;  // It's a sun

		// Rotation of the sun.
		for (iCount = 0 ; iCount < 3; ++iCount)
		{
			// Creates rotation
			m_solar[iIndex]->m_dW[iCount] = (double) 0.0 ;
			m_solar[iIndex]->m_dDeltaW[iCount] = 1.0 * slumptal(-0.05, 0.05) ;			
		}			

		// Set the sun texture.
		iRandNr = (int) round(slumptal(0.0, (double) (NROFSUNTEXTURES - 1))) ;
		setPlanetOrSunTextureIndex(iIndex, iRandNr, FALSE) ;
		m_solar[iIndex]->setTexture((char*) m_szSunTextures[iRandNr]) ;
		setPlanetOrSunTextureID(iIndex, getTextureIDFromDataBas(iRandNr, FALSE), FALSE) ;   // Sun
	}
}

/* Add planets or suns. */
BOOLEAN CPlanetSystem::addPlanetsOrSuns(int iNrOfPlanOrSun, BOOLEAN bPlanet)
{
	int       antPlanOrSun, iCount ;
	int       iNrOfPlan, iNrOfSun ;


	// Number of planets.
	if (bPlanet)
	{
		antPlanOrSun = (int) get_antalPlaneter() ;
	}
	// Number of suns.
	else
	{
		antPlanOrSun = (int) get_antalSolar() ;
	}

	// If no planets or suns are to be removed -> return.
	if (iNrOfPlanOrSun <= 0)
	{
		return TRUE ;
	}
	
	// If sun or planet list is empty - create a new one.
	if (antPlanOrSun == 0)
	{
		// Planets
		if (bPlanet)
		{
			m_planeter = NULL ;
			m_planeter = (CPlanet **) malloc(sizeof(CPlanet*) * iNrOfPlanOrSun) ;

			// The planets could not be created - return FALSE.
			if (m_planeter == NULL)
			{
				return FALSE ;
			}
		}
		// Suns
		else
		{
			m_solar = NULL ;
			m_solar = (CPlanet **) malloc(sizeof(CPlanet*) * iNrOfPlanOrSun) ;

			// The planets could not be created - return FALSE.
			if (m_solar == NULL)
			{
				return FALSE ;
			}
		}
	}
	// A list exists -> expand it.
	else
	{
		// Planets
		if (bPlanet)
		{
			m_planeter = (CPlanet **) realloc( (CPlanet **) m_planeter, sizeof(CPlanet*) * (m_antPlan + iNrOfPlanOrSun) ) ;
			// Was not possible to expand solar system - return FALSE.
			if (m_planeter == NULL)
			{
				return FALSE ;
			}

		}
		// Suns
		else
		{
			m_solar = (CPlanet **) realloc( (CPlanet **) m_solar, sizeof(CPlanet*) * (m_antSol + iNrOfPlanOrSun) ) ;
			// Was not possible to expand solar system - return FALSE.
			if (m_solar == NULL)
			{
				return FALSE ;
			}
		}
	}

	iNrOfPlan = m_antPlan ;
	iNrOfSun = m_antSol ;

	// Planets will be added.
	if (bPlanet)
	{
		for (iCount = iNrOfPlan ; iCount < (iNrOfPlan + iNrOfPlanOrSun) ; ++iCount)
		{
			m_planeter[iCount] = NULL ;
			addNewPlanetOrSun(iCount, TRUE) ;
		}
	}
	// Suns will be added.
	else
	{
		for (iCount = iNrOfSun ; iCount < (iNrOfSun + iNrOfPlanOrSun) ; ++iCount)
		{
			m_solar[iCount] = NULL ;
			addNewPlanetOrSun(iCount, FALSE) ;
		}
	}
	return TRUE ;
}

/* Delete one planet or sun. */
void CPlanetSystem::deletePlanetOrSun(int iPlanOrSunNr, BOOLEAN bPlanet)
{
	int antPlanOrSun, iCount ;

	// Number of planets.
	if (bPlanet)
	{
		antPlanOrSun = get_antalPlaneter() ;
	}
	// Number of suns.
	else
	{
		antPlanOrSun = get_antalSolar() ;
	}

	// If the list is empty or index wrong -> return.
	if (antPlanOrSun == 0 || !(iPlanOrSunNr >= 0 && iPlanOrSunNr < antPlanOrSun))
	{
		return ;
	}

	// A planet will be removed.
	if (bPlanet)
	{
		// If not the last planet, reshape the list.
		if (iPlanOrSunNr != (antPlanOrSun - 1))
		{
			for (iCount = iPlanOrSunNr; iCount < antPlanOrSun - 1; ++iCount)
			{
				*(m_planeter[iCount]) = *(m_planeter[iCount + 1]) ;
			}
		}

		// The last planet is removed.
		delete m_planeter[antPlanOrSun - 1] ;
		m_planeter[antPlanOrSun - 1] = NULL ;
		--m_antPlan ;
	}
	// A sun will be removed.
	else
	{
		// If not the last sun, reshape the list.
		if (iPlanOrSunNr != (antPlanOrSun - 1))
		{
			for (iCount = iPlanOrSunNr; iCount < antPlanOrSun - 1; ++iCount)
			{
				*(m_solar[iCount]) = *(m_solar[iCount + 1]) ;
			}
		}

		// The last sun is removed.
		delete m_solar[antPlanOrSun - 1] ;
		m_solar[antPlanOrSun - 1] = NULL ;
		--m_antSol ;
	}
}

/* Get number of collisions between planets or suns and other planets or suns. */
int CPlanetSystem::getNumberOfCollisions(BOOLEAN bPlanet1, BOOLEAN bPlanet2)
{
	int iCount1, iCount2, iNrOfCollision = 0 ;
	double iDist ;
	BOOLEAN bCollision = TRUE ;


	// Planet collisions (with other planets).
	if (bPlanet1 == TRUE && bPlanet2 == TRUE)
	{
		for (iCount1 = 0; iCount1 < get_antalPlaneter() - 1; iCount1++)
		{
			for (iCount2 = iCount1 + 1; iCount2 < get_antalPlaneter(); iCount2++)
			{
				iDist = distanceBetweenTwoPlanetsOrSuns(m_planeter[iCount1], 
														m_planeter[iCount2], FALSE) ;
				bCollision = (BOOLEAN) (iDist < (m_planeter[iCount1]->getRadius() + 
												 m_planeter[iCount2]->getRadius())) ;
				
				// A collision between two planets occurred.
				if (bCollision)
				{
					++iNrOfCollision ;
				}
			}
		}
	}

	// Sun collisions (with other suns).
	if (bPlanet1 == FALSE && bPlanet2 == FALSE)
	{
		for (iCount1 = 0; iCount1 < get_antalSolar() - 1; iCount1++)
		{
			for (iCount2 = iCount1 + 1; iCount2 < get_antalSolar(); iCount2++)
			{
				iDist = distanceBetweenTwoPlanetsOrSuns(m_solar[iCount1], 
														m_solar[iCount2], FALSE) ;
				bCollision = (BOOLEAN) (iDist < (m_solar[iCount1]->getRadius() + 
												 m_solar[iCount2]->getRadius())) ;
				
				// A collision between two suns occurred.
				if (bCollision)
				{
					++iNrOfCollision ;
				}
			}
		}
	}

	// Sun collisions (with other planets).
	if (bPlanet1 != bPlanet2)
	{
		for (iCount1 = 0; iCount1 < get_antalSolar(); iCount1++)
		{
			for (iCount2 = 0; iCount2 < get_antalPlaneter(); iCount2++)
			{
				iDist = distanceBetweenTwoPlanetsOrSuns(m_solar[iCount1], 
														m_planeter[iCount2], FALSE) ;
				bCollision = (BOOLEAN) (iDist < (m_solar[iCount1]->getRadius() + 
												 m_planeter[iCount2]->getRadius())) ;
				
				// A collision between a planet and a sun occurred.
				if (bCollision)
				{
					++iNrOfCollision ;
				}
			}
		}
	}
	return iNrOfCollision ;
}

/* Check for collisions between planets and suns. */
BOOLEAN CPlanetSystem::checkIfMoreCollisionsAndUpdate(BOOLEAN bPlanet1, BOOLEAN bPlanet2)
{
	int iCount1, iCount2 ;
	double iDist, dMass1, dMass2 ;
	BOOLEAN bCollision = TRUE, bCollisionFound = FALSE ;
	double dVect_Hast1[3] ;
	double dVect_Hast2[3] ;


	// All collisions has been taken care of.
	if (( (get_antalPlaneter() < (long) 2) && get_antalSolar()    == (long) 0 ) ||
		( (get_antalSolar()    < (long) 2) && get_antalPlaneter() == (long) 0 ) )
	{
		return FALSE ;
	}

	// Planet collisions (with other planets).
	if (bPlanet1 == TRUE && bPlanet2 == TRUE)
	{
		// Check that at least two planets are available.
		if (get_antalPlaneter() < 2)
			return FALSE ;

		for (iCount1 = 0; iCount1 < get_antalPlaneter() - 1; ++iCount1)
		{
			for (iCount2 = iCount1 + 1; iCount2 < get_antalPlaneter(); ++iCount2)
			{
				iDist = distanceBetweenTwoPlanetsOrSuns(m_planeter[iCount1], 
														m_planeter[iCount2], FALSE) ;
				bCollision = (BOOLEAN) (iDist < (m_planeter[iCount1]->getRadius() + 
												 m_planeter[iCount2]->getRadius())) ;
						
				// A collision between two planets occurred.
				if (bCollision)
				{
					dMass1 = m_planeter[iCount1]->getMassa() ;
					dMass2 = m_planeter[iCount2]->getMassa() ;

					m_planeter[iCount1]->getVelocity(dVect_Hast1) ;
					m_planeter[iCount2]->getVelocity(dVect_Hast2) ;

					dVect_Hast1[0] = dMass1/(dMass1 + dMass2) * dVect_Hast1[0] +
									  dMass2/(dMass1 + dMass2) * dVect_Hast2[0] ;

					dVect_Hast1[1] = dMass1/(dMass1 + dMass2) * dVect_Hast1[1] +
									  dMass2/(dMass1 + dMass2) * dVect_Hast2[1] ;

					dVect_Hast1[2] = dMass1/(dMass1 + dMass2) * dVect_Hast1[2] +
									  dMass2/(dMass1 + dMass2) * dVect_Hast2[2] ;

					
					// The first planet is heaviest (delete the second planet).
					if (dMass1 >= dMass2)
					{
						m_planeter[iCount1]->changeMassa(dMass1 + dMass2) ;

						m_planeter[iCount1]->setRadius(1.0 * pow(dMass1 + dMass2, 
													   (double) 1.0/3.0)) ;

						m_planeter[iCount1]->changeHast(dVect_Hast1) ;
						
						deletePlanetOrSun(iCount2, TRUE) ;
						
					}
					// The second planet is heaviest (delete the first planet).
					else if (dMass1 < dMass2)
					{

						m_planeter[iCount2]->changeMassa(dMass1 + dMass2) ;

						m_planeter[iCount2]->setRadius(1.0 * pow(dMass1 + dMass2, 
													   (double) 1.0/3.0)) ;

						m_planeter[iCount2]->changeHast(dVect_Hast1) ;

						deletePlanetOrSun(iCount1, TRUE) ;
						
					}
					return TRUE ;
				}
			}
		}
		return FALSE ;
	}

	// Sun collisions (with other suns).
	if (bPlanet1 == FALSE && bPlanet2 == FALSE)
	{
		// Check that at least two suns are available.
		if (get_antalSolar() < 2)
			return FALSE ;

		for (iCount1 = 0; iCount1 < get_antalSolar() - 1; ++iCount1)
		{
			for (iCount2 = iCount1 + 1; iCount2 < get_antalSolar(); ++iCount2)
			{
				iDist = distanceBetweenTwoPlanetsOrSuns(m_solar[iCount1], 
														m_solar[iCount2], FALSE) ;
				bCollision = (BOOLEAN) (iDist < (m_solar[iCount1]->getRadius() + 
												 m_solar[iCount2]->getRadius())) ;
				
				// A collision between two suns occurred.
				if (bCollision)
				{
					dMass1 = m_solar[iCount1]->getMassa() ;
					dMass2 = m_solar[iCount2]->getMassa() ;

					m_solar[iCount1]->getVelocity(dVect_Hast1) ;
					m_solar[iCount2]->getVelocity(dVect_Hast2) ;

					dVect_Hast1[0] = dMass1/(dMass1 + dMass2) * dVect_Hast1[0] +
									  dMass2/(dMass1 + dMass2) * dVect_Hast2[0] ;

					dVect_Hast1[1] = dMass1/(dMass1 + dMass2) * dVect_Hast1[1] +
									  dMass2/(dMass1 + dMass2) * dVect_Hast2[1] ;

					dVect_Hast1[2] = dMass1/(dMass1 + dMass2) * dVect_Hast1[2] +
									  dMass2/(dMass1 + dMass2) * dVect_Hast2[2] ;

					// The first sun is heaviest (delete the second sun).
					if (dMass1 >= dMass2)
					{
						m_solar[iCount1]->changeMassa(dMass1 + dMass2) ;

						m_solar[iCount1]->setRadius(1.0 * pow(dMass1 + dMass2, 
													(double) 1.0/3.0)) ;

						m_solar[iCount1]->changeHast(dVect_Hast1) ;

						deletePlanetOrSun(iCount2, FALSE) ;
						return TRUE ;
					}
					// The second sun is heaviest (delete the first sun).
					else if (dMass1 < dMass2)
					{
						m_solar[iCount2]->changeMassa(dMass1 + dMass2) ;

						m_solar[iCount2]->setRadius(1.0 * pow(dMass1 + dMass2, 
													   (double) 1.0/3.0)) ;

						m_solar[iCount2]->changeHast(dVect_Hast1) ;

						deletePlanetOrSun(iCount1, FALSE) ;
						return TRUE ;
					}
				}
			}
		}
		return FALSE ;
	}

	// Sun collisions (with other planets).
	if (bPlanet1 != bPlanet2)
	{
		// Check that at least one sun and one planet exist.
		if (!((get_antalPlaneter() >= (long) 1) && (get_antalSolar() >= (long) 1)))
				return FALSE ;

		for (iCount1 = 0; iCount1 < get_antalSolar(); ++iCount1)
		{
			for (iCount2 = 0; iCount2 < get_antalPlaneter(); ++iCount2)
			{
				iDist = distanceBetweenTwoPlanetsOrSuns(m_solar[iCount1], 
														m_planeter[iCount2], FALSE) ;
				bCollision = (BOOLEAN) (iDist < (m_solar[iCount1]->getRadius() + 
												 m_planeter[iCount2]->getRadius())) ;

				// A collision between a sun and a planet occurred.
				if (bCollision)
				{
					dMass1 = m_solar[iCount1]->getMassa() ;
					dMass2 = m_planeter[iCount2]->getMassa() ;

					m_solar[iCount1]->getVelocity(dVect_Hast1) ;
					m_planeter[iCount2]->getVelocity(dVect_Hast2) ;

					dVect_Hast1[0] = dMass1/(dMass1 + dMass2) * dVect_Hast1[0] +
									  dMass2/(dMass1 + dMass2) * dVect_Hast2[0] ;

					dVect_Hast1[1] = dMass1/(dMass1 + dMass2) * dVect_Hast1[1] +
									  dMass2/(dMass1 + dMass2) * dVect_Hast2[1] ;

					dVect_Hast1[2] = dMass1/(dMass1 + dMass2) * dVect_Hast1[2] +
									  dMass2/(dMass1 + dMass2) * dVect_Hast2[2] ;

					// The sun is heaviest (delete the planet).
					if (dMass1 >= dMass2)
					{

						m_solar[iCount1]->changeMassa(dMass1 + dMass2) ;

						m_solar[iCount1]->setRadius(1.0 * pow(dMass1 + dMass2, 
													(double) 1.0/3.0)) ;
						
						m_solar[iCount1]->changeHast(dVect_Hast1) ;

						deletePlanetOrSun(iCount2, TRUE) ;
						return TRUE ;
					}
					// The planet is heaviest (delete the sun).
					else if (dMass1 < dMass2)
					{
						m_planeter[iCount2]->changeMassa(dMass1 + dMass2) ;

						m_planeter[iCount2]->setRadius(1.0 * pow(dMass1 + dMass2, 
													   (double) 1.0/3.0)) ;

						m_planeter[iCount2]->changeHast(dVect_Hast1) ;
						
						deletePlanetOrSun(iCount1, FALSE) ;
						return TRUE ;
					}
				}
			}
		}
		return FALSE ;
	}
	return FALSE ;
}

/* Check for collisions between planets and suns. */
void CPlanetSystem::checkForCollisionAndUpdate()
{
	// Planet - planet collisions.
	while (checkIfMoreCollisionsAndUpdate(TRUE, TRUE) == TRUE) ;

	// Sun - sun collisions.
	while (checkIfMoreCollisionsAndUpdate(FALSE, FALSE) == TRUE) ;

	// Sun - planet collisions.
	while (checkIfMoreCollisionsAndUpdate(FALSE, TRUE) == TRUE) ;
}


/* Calculate the distance between a sun or planet with another sun or planet. */
/* bMinDist controlls if mindist should be returned, if planets are close, or the real distance. */
double CPlanetSystem::distanceBetweenTwoPlanetsOrSuns(CPlanet* plan1, CPlanet* plan2, BOOLEAN bMinDist)
{
	if (bMinDist)
	{
		return (double) (plan1->distance(plan2, m_mindist)) ;
	}
	else
	{
		return (double) (plan1->distance(plan2, (double) 0.0)) ;
	}
}
