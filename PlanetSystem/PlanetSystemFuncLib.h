/*-----------------------------------------------------------------------------
   File: BattleFieldFuncLib1.h
   Header to the help functions to the battle simulation program.
-----------------------------------------------------------------------------*/

#include <windows.h>
#include <math.h>

// MAXFILEPATHLENGTH, NROFPLANETTEXTURES and NROFSUNTEXTURES
// defined in the file: CPlanetSystem.h.

// Resource defined here.
#ifndef ___RESOURCE_H
#define ___RESOURCE_H

// To access the constants {IDC_SUNS, IDC_PLANETS and IDC_MAX_MOONS}
#include "resource.h"

#endif

// OpenGL defined here.
#ifndef ___OPENGL
#define ___OPENGL

#include <gl/gl.h>  // For OpenGL calls.
#include <gl/glu.h> // Only needed for extra functions to OpenGL.

#endif

// Struct teamdata defined here.
#ifndef ___PLANETSYSTEMDATA
#define ___PLANETSYSTEMDATA

typedef struct _PLANETSYSTEMDATA
{
	int iNrOfSuns ;
	int iNrOfPlanets ;
	int iMaxNrOfMoons ;
} PLANETSYSTEMDATA ;




#include "CPlanetSystem.h"
#include "Round.h"
#include "Slumptal.h"

#endif



// The C3DSLoader defined here.
#ifndef ___C3DSLOADER
#define ___C3DSLOADER

#include "COpenGLTypeDef.h"
#include "COpenGLTexture.h"
#include "C3DSLoader.h"

#endif

#include <assert.h>


// Creates a PLANETTEXTURENAME vector of size NROFPLANETTEXTURES  and stores the 
// planets and suns .bmp files in this vector.
void SaveTextureNamesToStruct(char szPlanetTextures[NROFPLANETTEXTURES][MAXTEXTUREFILELENGTH],
						      char szSunTextures[NROFSUNTEXTURES][MAXTEXTUREFILELENGTH]) ;

// Uses a vector of size NROFPLANETTEXTURES and of size NROFSUNTEXTURES.
// These two structures store the planets and suns .bmp files in the CPlanetSystem class. 
// It's important to call CPlanetSystem.initiera(...) before this function. Both constants: 
// MAXFILEPATHLENGTH, NROFPLANETTEXTURES and NROFSUNTEXTURES are defined in PlanetSystemFuncLib.h.
void SavePlanetAndSunTextureNames(char szPlanetTextures[NROFPLANETTEXTURES][MAXTEXTUREFILELENGTH],
						          char szSunTextures[NROFSUNTEXTURES][MAXTEXTUREFILELENGTH],
								  CPlanetSystem* PlanetSystem) ;

// Transforms the text in the dialog input data to
// a number.
int TransformTextToNumber(TCHAR* szText) ;

// Prints out team data to the window given by hwnd, starting
// at position (x, y).
BOOL PrintOutPlanetSystemData(HWND hwnd, PLANETSYSTEMDATA TeamData, int x, int y) ;


// Save the team values gotten from the dialog input for both teams.
// If any of the teams should have zero troops, then the indata
// isn't correct and FALSE is returned and the teamdata is zero everywhere.
BOOLEAN SavePlanetSystemDataToStruct(HWND hDlg, HWND hCtrlEditBox, PLANETSYSTEMDATA* pPlanetSystemData) ;

// Updates the main window so that the changes can be seen.
void UpdateWindowForNewData(HWND hwnd) ;

// Startup OpenGL for writing.
void StartUpOpenGLForWriting(int screen_width, int screen_height) ;

// Start painting spheres.
void DisplaySphereStart(void) ;

// Displays a sphere.
void DisplaySphere(HWND hwnd, int iPlanetNr, BOOLEAN bPlanet, CPlanetSystem* PlanetSystem, 
				   double iScale) ;

// Stop painting spheres.
void DisplaySphereEnd(HWND hwnd) ;

// Resizes the object views based on window size, 
// but does not paint anything to screen.
void ResizeWindowForImagePainting(int piScreenSize[2]) ;


// Clears all textures from memory (before the program ends).
void ClearAllTextures(CPlanetSystem* pPlanetSystem) ;

// Deletes planets and suns from the planetary system.
BOOLEAN DeletePlanetsOrSuns(HWND hDlg, HWND hCtrlEditBox, CPlanetSystem* pPlanetSystem, 
							int* pNrOfPlanets, int* pNrOfSuns) ;

// Adds planets and suns from the planetary system.
void AddPlanetsOrSuns(HWND hDlg, HWND hCtrlEditBox, CPlanetSystem* pPlanetSystem, 
	 				  int* pNrOfPlanets, int* pNrOfSuns) ;


// The image fil: "spaceship.3ds" and the texture file: "spaceshiptexture.bmp" are used.
// The texture file: "earthmap.bmp" are used.

