/*-----------------------------------------------------------------------------
   File: PlanetSystemFuncLib.cpp
   Implementation of help functions to the planet system simulation program
-----------------------------------------------------------------------------*/



#include "PlanetSystemFuncLib.h"

// MAXFILEPATHLENGTH, NROFPLANETTEXTURES and NROFSUNTEXTURES
// defined in the file: CPlanetSystem.h.

// Creates a vector of size NROFPLANETTEXTURES  and stores the 
// planets and suns .bmp files in this vector.
void SaveTextureNamesToStruct(char szPlanetTextures[NROFPLANETTEXTURES][MAXTEXTUREFILELENGTH],
						      char szSunTextures[NROFSUNTEXTURES][MAXTEXTUREFILELENGTH])
{
	int strLen ;

	// Suns
	strLen = strlen("YellowSun.bmp") ;
	memset(szSunTextures[0], 0, strLen) ;
	strcpy(szSunTextures[0],    "YellowSun.bmp") ;
	szSunTextures[0][strLen] = '\0' ;

	strLen = strlen("RedSun.bmp") ;
	memset(szSunTextures[1], 0, strLen) ;
	strcpy(szSunTextures[1],    "RedSun.bmp") ;
	szSunTextures[1][strLen] = '\0' ;

	// Planets
	strLen = strlen("Earth.bmp") ;
	memset(szPlanetTextures[0], 0, strLen) ;
	strcpy(szPlanetTextures[0], "Earth.bmp") ;
	szPlanetTextures[0][strLen] = '\0' ;

	strLen = strlen("Mars1.bmp") ;
	memset(szPlanetTextures[1], 0, strLen) ;
	strcpy(szPlanetTextures[1], "Mars1.bmp") ;
	szPlanetTextures[1][strLen] = '\0' ;

	strLen = strlen("Mars2.bmp") ;
	memset(szPlanetTextures[2], 0, strLen) ;
	strcpy(szPlanetTextures[2], "Mars2.bmp") ;
	szPlanetTextures[2][strLen] = '\0' ;

	strLen = strlen("Jupiter.bmp") ;
	memset(szPlanetTextures[3], 0, strLen) ;
	strcpy(szPlanetTextures[3], "Jupiter.bmp") ;
	szPlanetTextures[3][strLen] = '\0' ;

	strLen = strlen("Mercury.bmp") ;
	memset(szPlanetTextures[4], 0, strLen) ;
	strcpy(szPlanetTextures[4], "Mercury.bmp") ;
	szPlanetTextures[4][strLen] = '\0' ;

	strLen = strlen("Neptune.bmp") ;
	memset(szPlanetTextures[5], 0, strLen) ;
	strcpy(szPlanetTextures[5], "Neptune.bmp") ;
	szPlanetTextures[5][strLen] = '\0' ;
}

// Uses a vector of size NROFPLANETTEXTURES and of size NROFSUNTEXTURES.
// These two structures store the planets and suns .bmp files in the CPlanetSystem class. 
// It's important to call CPlanetSystem.initiera(...) before this function. Both constants: 
// NROFPLANETTEXTURES and NROFSUNTEXTURES are defined in PlanetSystemFuncLib.h.
void SavePlanetAndSunTextureNames(char szPlanetTextures[NROFPLANETTEXTURES][MAXTEXTUREFILELENGTH],
						          char szSunTextures[NROFSUNTEXTURES][MAXTEXTUREFILELENGTH],
								  CPlanetSystem* PlanetSystem)
{
	int NrOfPlanets = 0 ;
	int NrOfSuns = 0 ;
	int count, randNr ;
	int iTextureID ;

	NrOfPlanets = (int) PlanetSystem->get_antalPlaneter() ;
	NrOfSuns    = (int) PlanetSystem->get_antalSolar() ;


	// Set the planets textures.
	for (count = 0 ; count < NrOfPlanets ; count++)
	{
		randNr = (int) round(slumptal(0.0, (double) (NROFPLANETTEXTURES - 1))) ;
		PlanetSystem->setPlanetTexture(count, szPlanetTextures[randNr]) ;

		iTextureID = LoadBitmap(szPlanetTextures[randNr]) ;

		PlanetSystem->getPlanet(count).setTextureID(iTextureID) ;
		glBindTexture(GL_TEXTURE_2D, iTextureID) ; // We set the active texture 

	}

	// Set the suns textures first.
	for (count = 0 ; count < NrOfSuns ; count++)
	{
		randNr = (int) round(slumptal(0.0, (double) (NROFSUNTEXTURES - 1))) ;
		PlanetSystem->setSunTexture(count, szSunTextures[randNr]) ;

		iTextureID = LoadBitmap(szSunTextures[randNr]) ;

		PlanetSystem->getSun(count).setTextureID(iTextureID) ;
		glBindTexture(GL_TEXTURE_2D, iTextureID) ; // We set the active texture 
	}
}

// Transforms the text in the dialog input data to
// a number.
int TransformTextToNumber(TCHAR* szText)
{
	int iStrLength = 0, iCount = 0,  iNbr = 0 ;

	if (szText == NULL)
		return 0 ;

	iStrLength = lstrlen(szText) ;
	for (iCount = 0 ; iCount < iStrLength ; iCount++)
	{
		iNbr = iNbr * 10 + (szText[iCount] - (TCHAR) '0') ;
	}
	return iNbr ;
}


// Prints out team data to the window given by hwnd, starting
// at position (x, y). Background color is black and text white.
BOOL PrintOutPlanetSystemData(HWND hwnd, PLANETSYSTEMDATA PlanetSystemData, int x, int y)
{
	HDC         hdc ;
	TEXTMETRIC  tm ;
	int         cxChar, cyChar ;
	TCHAR       szTextBuffer[120] ;
	PAINTSTRUCT ps ;


	hdc = BeginPaint(hwnd, &ps) ;

	SelectObject(hdc, GetStockObject(SYSTEM_FONT)) ;
	GetTextMetrics(hdc, &tm) ;
	cxChar = max(3 * tm.tmAveCharWidth, 2 * tm.tmMaxCharWidth) ;
	cyChar = tm.tmHeight + 3 ;
	SetTextColor(hdc, RGB(255,255,255)) ;  // Text color is set to white.
	SetBkColor(hdc, RGB(0,0,0)) ;          // Background is set to black.


	// At least one troop from each team need to be selected to make it meaningful
	// to print the result. If this is not the case return FALSE.
	if (PlanetSystemData.iNrOfSuns <= 0 || PlanetSystemData.iNrOfPlanets <= 0)
	{
		TextOut(hdc, x, y + cyChar, szTextBuffer, 
				wsprintf(szTextBuffer, 
				TEXT("At least one sun and one planet need to be selected for a planet system simulation."))) ;

		// Clean up.
		DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT))) ;		
		EndPaint(hwnd, &ps) ;
		return FALSE ;
	}

	// Write out the planet system data.
	TextOut(hdc, x, y + cyChar, szTextBuffer, 
			wsprintf(szTextBuffer, TEXT("Number of suns: %d"), 
			PlanetSystemData.iNrOfSuns)) ;

	TextOut(hdc, x, y + 2 * cyChar, szTextBuffer, 
			wsprintf(szTextBuffer, TEXT("Number of planets: %d"), 
			PlanetSystemData.iNrOfPlanets)) ;

	TextOut(hdc, x, y + 3 * cyChar, szTextBuffer, 
			wsprintf(szTextBuffer, TEXT("Maximum number of moons circling around a planet: %d"), 
			PlanetSystemData.iMaxNrOfMoons)) ;

	// Clean up.
	DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT))) ;
	
	EndPaint(hwnd, &ps) ;


	return TRUE ;
}


// Save the planet system values gotten from the dialog input.
// If the planetsystem should have zero planets or suns, then the indata
// isn't correct and FALSE is returned.
BOOLEAN SavePlanetSystemDataToStruct(HWND hDlg, HWND hCtrlEditBox, PLANETSYSTEMDATA* pPlanetSystemData)
{
	int   iCount1, iNbr ;
	TCHAR szIntBufferText[10] ;

	// Save the team values gotten from the dialog.
	for (iCount1 = IDC_SUNS ; iCount1 <= IDC_MAX_MOONS ; iCount1++)
	{
		// Planet system:				
		hCtrlEditBox = GetDlgItem(hDlg, iCount1) ;
		GetDlgItemText(hDlg, iCount1, szIntBufferText, 4) ;

		iNbr = TransformTextToNumber(szIntBufferText) ;

		switch (iCount1)
		{
		case IDC_SUNS:   // Number of suns
			pPlanetSystemData->iNrOfSuns = iNbr ;
			break ;

		case IDC_PLANETS:   // Number of moons
			pPlanetSystemData->iNrOfPlanets = iNbr ;
			break ;

		case IDC_MAX_MOONS:    // Maximum number of moons circling around a planet
			pPlanetSystemData->iMaxNrOfMoons = iNbr ;
			break ;
		}
	}

	// At least one sun and one planet need to be selected to make it meaningful
	// data. If this is not the case return FALSE.
	if (pPlanetSystemData->iNrOfSuns >= 1 && pPlanetSystemData->iNrOfPlanets >= 1)
	{
		return TRUE ;
	}
	else
	{
		return FALSE ;
	}
}



// Updates the main window so that the changes can be seen.
void UpdateWindowForNewData(HWND hwnd)
{
	HDC         hdc ;
	RECT        rect ;
	PAINTSTRUCT ps ;

	// Updates the main window so that the changes can be seen.
	hdc = BeginPaint(hwnd, &ps) ;

	GetClientRect(hwnd, &rect) ;
	InvalidateRect(hwnd, &rect, TRUE) ;

	EndPaint(hwnd, &ps) ;
}

// Startup OpenGL for writing.
void StartUpOpenGLForWriting(int screen_width, int screen_height)
{
    glClearColor(0.0, 0.0, 0.0, 0.0); // This clear the background color to black
    glShadeModel(GL_SMOOTH); // Type of shading for the polygons
   	
    // Viewport transformation
    glViewport(0, 0, screen_width, screen_height);  

    // Projection transformation
    glMatrixMode(GL_PROJECTION); // Specifies which matrix stack is the target for matrix operations 
    glLoadIdentity(); // We initialize the projection matrix as identity
    gluPerspective(45.0f,(GLfloat)screen_width/(GLfloat)screen_height,10.0f,10000.0f); // We define the "viewing volume"
   
    glEnable(GL_DEPTH_TEST); // We enable the depth test (also called z buffer)
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
    
    glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping
}

// Resizes the object views based on window size, 
// but does not paint anything to screen.
void ResizeWindowForImagePainting(int piScreenSize[2])
{

	// Resizing object (ship) after the window size.
    // Viewport transformation
    glViewport(0, 0, piScreenSize[0], piScreenSize[1]);  

    // Projection transformation
    glMatrixMode(GL_PROJECTION); // Specifies which matrix stack is the target for matrix operations 
    glLoadIdentity(); // We initialize the projection matrix as identity
    gluPerspective(45.0f,(GLfloat)piScreenSize[0]/(GLfloat)piScreenSize[0],10.0f,10000.0f); // We define the "viewing volume"
}

// Start painting spheres.
void DisplaySphereStart(void)
{
	// Clear screen and set right matrix mode.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // This clear the background color to dark blue
    glMatrixMode(GL_MODELVIEW); // Modeling transformation
}

// Displays a sphere.
void DisplaySphere(HWND hwnd, int iPlanetNr, BOOLEAN bPlanet, CPlanetSystem* PlanetSystem, double iScale)
{
	GLUquadricObj* QuadrObj ;



	glEnable(GL_TEXTURE_2D) ;

    glLoadIdentity(); // Initialize the model matrix as identity

	if (bPlanet)  // Planet
	{

		glTranslatef((float) PlanetSystem->getPlanet(iPlanetNr).getKoord_Exakt()[0], 
			         (float) PlanetSystem->getPlanet(iPlanetNr).getKoord_Exakt()[1], 
					 (float) PlanetSystem->getPlanet(iPlanetNr).getKoord_Exakt()[2]) ; // We move the object forward (the model matrix is multiplied by the translation matrix)

    }
	else   // Sun
	{

		glTranslatef((float) PlanetSystem->getSun(iPlanetNr).getKoord_Exakt()[0], 
			         (float) PlanetSystem->getSun(iPlanetNr).getKoord_Exakt()[1], 
					 (float) PlanetSystem->getPlanet(iPlanetNr).getKoord_Exakt()[2]) ; // We move the object forward (the model matrix is multiplied by the translation matrix)

    }

	// Sale the image in (x,y,z) with iScale.
	glScaled(iScale, iScale, iScale) ;

	// Rotate the image.
	if (bPlanet)  // Planet
	{
		glRotatef((float) (PlanetSystem->getPlanet(iPlanetNr).m_dW[0]),1.0f,0.0f,0.0f) ; // Rotations of the object (the model matrix is multiplied by the rotation matrices)
		glRotatef((float) (PlanetSystem->getPlanet(iPlanetNr).m_dW[1]),0.0f,1.0f,0.0f) ;
		glRotatef((float) (PlanetSystem->getPlanet(iPlanetNr).m_dW[2]),0.0f,0.0f,1.0f) ;
	}
	else          // Sun
	{
		glRotatef((float) (PlanetSystem->getSun(iPlanetNr).m_dW[0]),1.0f,0.0f,0.0f) ; // Rotations of the object (the model matrix is multiplied by the rotation matrices)
		glRotatef((float) (PlanetSystem->getSun(iPlanetNr).m_dW[1]),0.0f,1.0f,0.0f) ;
		glRotatef((float) (PlanetSystem->getSun(iPlanetNr).m_dW[2]),0.0f,0.0f,1.0f) ;
	}

	glBindTexture(GL_TEXTURE_2D, PlanetSystem->getPlanetOrSunTextureID(iPlanetNr, bPlanet)) ; // We set the active texture 

	// Spheres
	QuadrObj = gluNewQuadric() ;
	gluQuadricNormals(QuadrObj, GLU_SMOOTH) ;
    gluQuadricOrientation(QuadrObj, GLU_OUTSIDE) ;
    gluQuadricDrawStyle(QuadrObj, GLU_FILL) ;
	gluQuadricTexture(QuadrObj, GLU_TRUE) ;


	if (bPlanet)  // Planet
	{
		gluSphere(QuadrObj, (float) PlanetSystem->getPlanet(iPlanetNr).getRadius(), 40, 40) ;
	}
	else   // Sun
	{
		gluSphere(QuadrObj, (float) PlanetSystem->getSun(iPlanetNr).getRadius(), 40, 40) ;
	}

	glFlush() ; 
    gluDeleteQuadric(QuadrObj) ;
}


// Stop painting spheres.
void DisplaySphereEnd(HWND hwnd)
{
	HDC hdc ;

	hdc = GetDC(hwnd) ;

    glFlush() ; // This force the execution of OpenGL commands
    SwapBuffers(hdc) ; // In double buffered mode we invert the positions of the visible buffer and the writing buffer

	ReleaseDC(hwnd, hdc) ;
}


// Clears all textures from memory (before the program ends).
void ClearAllTextures(CPlanetSystem* pPlanetSystem)
{
	if (pPlanetSystem != NULL)
	{
		int iCount, iTextureIndex ;
		unsigned int piTextureIDPlanet[NROFPLANETTEXTURES] ;
		unsigned int piTextureIDSun[NROFSUNTEXTURES] ;


		// Remove planet textures
		for (iCount = 0; iCount < NROFPLANETTEXTURES; ++iCount)
		{
			iTextureIndex = pPlanetSystem->getTextureIDFromDataBas(iCount, TRUE) ;
			piTextureIDPlanet[iCount] = iTextureIndex ;

		}
		glDeleteTextures(NROFPLANETTEXTURES, piTextureIDPlanet) ;
		
		// Remove sun textures
		for (iCount = 0; iCount < NROFSUNTEXTURES; ++iCount)
		{
			iTextureIndex = pPlanetSystem->getTextureIDFromDataBas(iCount, FALSE) ;
			piTextureIDSun[iCount] = iTextureIndex ;
		}
		glDeleteTextures(NROFSUNTEXTURES, piTextureIDSun) ;
	}

}



BOOLEAN DeletePlanetsOrSuns(HWND hDlg, HWND hCtrlEditBox, CPlanetSystem* pPlanetSystem, 
							int* pNrOfPlanets, int* pNrOfSuns)
{
			int iCountPlanet = IDC_DELETED_PLANETS ;
			int iCountSun    = IDC_DELETED_SUNS ;
			int iCount ;
			int iNrOfPlanets = 0 ;
			int iNrOfSuns    = 0 ;
			BOOLEAN          bDataOK = TRUE ;
			TCHAR            szText[80] ;
			TCHAR			 szIntBufferText[10] ;


			// Get number of planets to delete.				
			hCtrlEditBox = GetDlgItem(hDlg, iCountPlanet) ;
			GetDlgItemText(hDlg, iCountPlanet, szIntBufferText, 4) ;

			*pNrOfPlanets = TransformTextToNumber(szIntBufferText) ;
			iNrOfPlanets = pPlanetSystem->get_antalPlaneter() ;

			// Too many planets are deleted (can't delete more planets than we have
			// in the planet system). 
			if (*pNrOfPlanets > iNrOfPlanets)
			{
				sprintf(szText, 
					    "Too many planets where deleted (it only contains %d planets).", 
						iNrOfPlanets) ;

				MessageBox(hDlg, szText, "Wrong user data", MB_ICONINFORMATION | MB_OK) ;
				bDataOK = FALSE ;
				return bDataOK ;
			}
            // No planets are deleted.
			else if (*pNrOfPlanets == 0 || iNrOfPlanets == 0)
			{

			}
			// Too many planets are deleted (at least one planet must remain in the
			// planet system). 
			else if (*pNrOfPlanets == iNrOfPlanets)
			{
				sprintf(szText, 
					    "At least one planet must remain (it only contains %d planets).",
						iNrOfPlanets) ;

				MessageBox(hDlg, szText, "Wrong user data", MB_ICONINFORMATION | MB_OK) ;
				bDataOK = FALSE ;
				return bDataOK ;
			}
			// Delete the number of planets the user has specified.
			else
			{
				for (iCount = iNrOfPlanets - 1 ; iCount > iNrOfPlanets - (*pNrOfPlanets) - 1 ; --iCount)
				{
					pPlanetSystem->deletePlanetOrSun(iCount, TRUE) ;	
				}
				bDataOK = TRUE ;
			}


			// Get number of suns to delete.				
			hCtrlEditBox = GetDlgItem(hDlg, iCountSun) ;
			GetDlgItemText(hDlg, iCountSun, szIntBufferText, 4) ;

			*pNrOfSuns = TransformTextToNumber(szIntBufferText) ;
			iNrOfSuns = pPlanetSystem->get_antalSolar() ;

			// Too many suns are deleted. 
			if (*pNrOfSuns > iNrOfSuns)
			{
				sprintf(szText, 
					    "Too many suns where deleted (it only contains %d suns).", 
						iNrOfSuns) ;

				MessageBox(hDlg, szText, "Wrong user data", MB_ICONINFORMATION | MB_OK) ;
				bDataOK = FALSE ;
				return bDataOK ;
			}
			// No suns are deleted.
			else if (*pNrOfSuns == 0 || iNrOfSuns == 0)
			{

			}
			// Too many suns are deleted (at least one sun must remain in the
			// planet system). 
			else if (*pNrOfSuns == iNrOfSuns)
			{
				sprintf(szText, 
					    "At least one sun must remain (it only contains %d suns).",
						iNrOfSuns) ;

				MessageBox(hDlg, szText, "Wrong user data", MB_ICONINFORMATION | MB_OK) ;
				bDataOK = FALSE ;
				return bDataOK ;
			}
			// Delete the number of suns the user has specified.
			else
			{
				for (iCount = iNrOfSuns - 1 ; iCount > iNrOfSuns - (*pNrOfSuns) - 1 ; --iCount)
				{
					pPlanetSystem->deletePlanetOrSun(iCount, FALSE) ;	
				}
				bDataOK = TRUE ;
			}

			return bDataOK ;

}





void AddPlanetsOrSuns(HWND hDlg, HWND hCtrlEditBox, CPlanetSystem* pPlanetSystem, 
					  int* pNrOfPlanets, int* pNrOfSuns)
{
			int iCountPlanet = IDC_NEW_PLANETS ;
			int iCountSun    = IDC_NEW_SUNS ;
			int iNrOfPlanets = 0 ;
			int iNrOfSuns    = 0 ;
			TCHAR            szText[140] ;
			TCHAR			 szIntBufferText[10] ;



			// Get number of planets to add.				
			hCtrlEditBox = GetDlgItem(hDlg, iCountPlanet) ;
			GetDlgItemText(hDlg, iCountPlanet, szIntBufferText, 4) ;

			*pNrOfPlanets = TransformTextToNumber(szIntBufferText) ;

			// Get number of suns to add.				
			hCtrlEditBox = GetDlgItem(hDlg, iCountSun) ;
			GetDlgItemText(hDlg, iCountSun, szIntBufferText, 4) ;

			*pNrOfSuns = TransformTextToNumber(szIntBufferText) ;


			// Planets are added.
			if ((*pNrOfPlanets) > 0)
			{
				pPlanetSystem->addPlanetsOrSuns((int) (*pNrOfPlanets), TRUE) ;  
			}

			iNrOfPlanets = pPlanetSystem->get_antalPlaneter() ;

			// Suns are added.
			if ((*pNrOfSuns) > 0)
			{
				pPlanetSystem->addPlanetsOrSuns((int) (*pNrOfSuns), FALSE) ;  
			}

			iNrOfSuns = pPlanetSystem->get_antalSolar() ;

			if ( ((iNrOfSuns > 0) || (iNrOfPlanets > 0)) && 
				 (((*pNrOfPlanets) > 0) || ((*pNrOfSuns) > 0)))
			{
				// Write the number of suns and planets in the new solar system. 
				sprintf(szText, 
						"The planetary system will now contain %d suns and %d planets.", 
						iNrOfSuns, iNrOfPlanets) ;
				MessageBox(hDlg, szText, "Planetary update", MB_ICONINFORMATION | MB_OK) ;
			}
}
