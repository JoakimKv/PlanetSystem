
/*-----------------------------------------------------------------------------
   File: PlanetSystem.cpp
   A planet system simulation program
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------

   Project Type = WIN32.
   
-----------------------------------------------------------------------------*/

/////////////////////////////////////////////////////////////////////////
//
// opengl32.lib and glu32.lib (only needed for certain extra functions).
//
/////////////////////////////////////////////////////////////////////////

#include <windows.h>

// OpenGL defined here.
#ifndef ___OPENGL
#define ___OPENGL

#include <gl/gl.h>  // For OpenGL calls.
#include <gl/glu.h> // Only needed for extra functions to OpenGL.

#endif

#include "COpenGLContext.h"
#include "resource.h"
#include "PlanetSystemFuncLib.h"



LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM) ;
BOOL CALLBACK NewPlanetSystemProc(HWND, UINT, WPARAM, LPARAM) ;
BOOL CALLBACK DeletePlanetOrSunProc(HWND, UINT, WPARAM, LPARAM) ;
BOOL CALLBACK AddPlanetOrSunProc(HWND, UINT, WPARAM, LPARAM) ;



// MAXFILEPATHLENGTH, NROFPLANETTEXTURES and NROFSUNTEXTURES
// defined in the file: CPlanetSystem.h.

static int			 g_iPartitioning = 3 ;
static double        g_iScale = 1.0 ;
static int           g_iScreenWidth, g_iScreenHeight ;
static int           g_iFillingMode = 1 ;  // Polygons are filled.
static double        g_pdTranslation_xyz[3] = { 0.0, 0.0, -1200.0 } ;
static BOOLEAN       g_bUpdateWindowNeeded = TRUE ;
TCHAR                szAppName[] = TEXT("PlanetSystem") ;
HWND                 g_hDlgModelessPlanetSystem, g_hDlgModelessDeletePlanetOrSun, g_hwndMain ;
HWND                 g_hDlgModelessAddPlanetOrSun ;
static int           g_iNrOfPlanetTextures = (int) NROFPLANETTEXTURES ; // Constant defined in PlanetSystemFuncLib.h.
static int           g_iNrOfSunTextures = (int) NROFSUNTEXTURES ; // Constant defined in PlanetSystemFuncLib.h.

PLANETSYSTEMDATA     g_PlanetSystemData = {0,0,0} ;

COpenGLContext       g_COpenGLCon ;
CPlanetSystem*       g_pCPlanetSystem ;
long                 g_plMax_Coord[3] ;
char                 g_szPlanetTextures[NROFPLANETTEXTURES][MAXTEXTUREFILELENGTH] ;
char                 g_szSunTextures[NROFSUNTEXTURES][MAXTEXTUREFILELENGTH] ;

// Common planet system data
double               g_ddt = 0.01 ;
double               g_dG = 1.0 ;
double               g_dMinDist = 60.0 ;
double               g_dMinPlanetWeight = 100.0 ;
double               g_dMaxPlanetWeight = 200.0 ;
double               g_dMinSunWeight = 200000.0 ;
double               g_dMaxSunWeight = 800000.0 ;




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine,
				   int iCmdShow)
{
	HACCEL        hAccel ;
    HWND          hwnd ;
	MSG           msg ;
	WNDCLASS      wndclass ;
	static HMENU  hMenu ;
	BOOLEAN       bQuit = FALSE ;
	BOOLEAN       bLoadOK = FALSE ;
	char*         szImageFileName = "spaceship.3ds" ;
	char*         szTextureFileName = "spaceshiptexture.bmp" ;
	int           iCount, iNrOfPlanets, iNrOfSuns ; 
	double*       pdCoord ;
	char*         szTexture = "Earth.bmp" ;



	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = WndProc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = DLGWINDOWEXTRA ;
	wndclass.hInstance     = hInstance ;
	wndclass.hIcon         = LoadIcon(hInstance, szAppName) ;
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH) ;
	wndclass.lpszMenuName  = szAppName ;
	wndclass.lpszClassName = szAppName ;

	if ( !RegisterClass(&wndclass) )
    {
		MessageBox(NULL, TEXT("This program requires Windows NT!"),
			       szAppName, MB_ICONERROR) ;
        return 0 ;
	}

	hwnd = CreateWindow(szAppName,                              // window class name
		                szAppName,                              // window caption
						WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,  // window style
						CW_USEDEFAULT,                          // initial x position
						CW_USEDEFAULT,                          // initial y position
						CW_USEDEFAULT,                          // initial x size
						CW_USEDEFAULT,                          // initial y size
						NULL,                                   // parent window handle
						NULL,                                   // window menu handle
						hInstance,                              // program instance handle
						NULL);                                  // creation parameters

	g_hwndMain = hwnd ;  // Copy the handle for the main window for easy access.

	g_COpenGLCon.init(g_hwndMain) ;

	ShowWindow(hwnd, iCmdShow) ;
	UpdateWindow(hwnd) ;

	hAccel = LoadAccelerators(hInstance, szAppName) ;

	// Get window size.
    g_iScreenWidth  = GetSystemMetrics(SM_CXSCREEN) ;
    g_iScreenHeight = GetSystemMetrics(SM_CYSCREEN) ;

	g_plMax_Coord[0] = (long) g_iScreenWidth ;
	g_plMax_Coord[1] = (long) g_iScreenHeight ;
	g_plMax_Coord[2] = (long) max(g_iScreenWidth, g_iScreenHeight) ;

	g_pCPlanetSystem = new CPlanetSystem(g_ddt/((double) g_iPartitioning), g_dG, 1, 10, g_dMinDist) ;
	g_pCPlanetSystem->initiera(g_dMinPlanetWeight, g_dMaxPlanetWeight, 
		                       g_dMinSunWeight, g_dMaxSunWeight, g_plMax_Coord) ;

	// Save the texture file names to the two structs.
	SaveTextureNamesToStruct(g_szPlanetTextures, g_szSunTextures) ;

	SavePlanetAndSunTextureNames(g_szPlanetTextures, g_szSunTextures,
							     g_pCPlanetSystem) ;

	g_pCPlanetSystem->setAllTextures(g_szPlanetTextures, g_szSunTextures) ;
	g_pCPlanetSystem->setRotationScalingPlanetsOrSuns(g_iPartitioning, TRUE) ;
	g_pCPlanetSystem->setRotationScalingPlanetsOrSuns(g_iPartitioning, FALSE) ;

	StartUpOpenGLForWriting(g_iScreenWidth, g_iScreenHeight) ;

	pdCoord = (double*) g_pCPlanetSystem->getSun(0).getKoord_Exakt() ;
	gluLookAt(0.0, 0.0, 0.0, pdCoord[0], pdCoord[1], pdCoord[2], 0.0, 1.0, 0.0) ;

	// Program's main loop.
	while ( !bQuit )
	{		
		// Check for messages.
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE )  )
		{
			
			// Handle or dispatch messages.
			if ( msg.message == WM_QUIT ) 
			{
				bQuit = TRUE;
			} 
			else
			{
				if (g_hDlgModelessPlanetSystem == 0 || 
				!IsDialogMessage(g_hDlgModelessPlanetSystem, &msg))
				{
					if (!TranslateAccelerator(hwnd, hAccel, &msg))
					{
						TranslateMessage(&msg) ;
						DispatchMessage(&msg) ;
					}
				}
			}
		} 
		else 
		{
				for (iCount = 0 ; iCount < g_iPartitioning ; ++iCount)
				{
					g_pCPlanetSystem->enForflyttning() ;
				}
				// g_pCPlanetSystem->checkForCollisionAndUpdate() ;
				// g_pCPlanetSystem->deletePlanetOrSun(1,TRUE) ;

				DisplaySphereStart() ;

				iCount = 0 ;

				// Planets.
				iNrOfPlanets = g_pCPlanetSystem->get_antalPlaneter() ;

				for (iCount = 0; iCount < iNrOfPlanets; iCount++)
				{
					// Displays a sphere.

					DisplaySphere(g_hwndMain, iCount, TRUE, g_pCPlanetSystem, g_iScale) ;
				}

				// Suns.
				iNrOfSuns = g_pCPlanetSystem->get_antalSolar() ;

				for (iCount = 0; iCount < iNrOfSuns; iCount++)
				{
					// Displays a sphere.

					DisplaySphere(g_hwndMain, iCount, FALSE, g_pCPlanetSystem, g_iScale) ;
				}					
				DisplaySphereEnd(g_hwndMain) ;

				g_bUpdateWindowNeeded = FALSE ;  // Update is done, no update needed anymore.
		}
	}
	return msg.wParam ;	
}



int AskConfirmation(HWND hwnd)
{
	return MessageBox(hwnd, TEXT("Do you really want to close PlanetSystem?"),
		              szAppName, MB_YESNO | MB_ICONQUESTION) ;
}



LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HMENU     hMenu ;
	static HINSTANCE hInst ;
	int              piScreenSize[2] ;
	HDC              hdc ;
	PAINTSTRUCT		 ps ;




    switch (message)
	{
	case WM_CREATE:
		hInst = (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE) ;
		hMenu = LoadMenu(hInst, szAppName) ;
		SetMenu(hwnd, hMenu) ;

		return 0 ;

	case WM_SETFOCUS:
		SetFocus(hwnd) ;
		return 0 ;

	case WM_SIZE:
        piScreenSize[0] = g_iScreenWidth = LOWORD(lParam) ;
		piScreenSize[1] = g_iScreenHeight = HIWORD(lParam) ;
		g_bUpdateWindowNeeded = TRUE ;
		return 0 ;

	case WM_PAINT:

		hdc = BeginPaint(g_hwndMain, &ps) ;

		EndPaint(g_hwndMain, &ps) ;

		return 0 ;

	case WM_INITMENUPOPUP:
		break ;

	case WM_COMMAND:
        switch (LOWORD(wParam))
		{
		case IDM_FILE_NEW:
            g_hDlgModelessPlanetSystem = CreateDialog(hInst, TEXT("PlanetSystemNewDlg"),
		                                              hwnd, NewPlanetSystemProc) ;
			return 0 ;

		case IDM_APP_EXIT:
			SendMessage(hwnd, WM_CLOSE, 0, 0) ;
			return 0 ;

		case IDM_EDIT_DELETEPLANETORSUN:
            g_hDlgModelessDeletePlanetOrSun = CreateDialog(hInst, TEXT("PlanetSystemDeleteDlg"),
		                                                   hwnd, DeletePlanetOrSunProc) ;
			return 0 ;

		case IDM_EDIT_ADDPLANETORSUN:
            g_hDlgModelessAddPlanetOrSun = CreateDialog(hInst, TEXT("PlanetSystemNewPlanetOrSunDlg"),
		                                                hwnd, AddPlanetOrSunProc) ;
			return 0 ;

		case IDM_APP_HELP:
			MessageBox(hwnd, TEXT("Help not yet implemented!"), 
				       szAppName, MB_ICONEXCLAMATION | MB_OK) ;
			g_bUpdateWindowNeeded = TRUE ;
			return 0 ;

		case IDM_APP_ABOUT:
			MessageBox(hwnd, TEXT("PlanetSystem by Joakim Kvistholm"),
				       szAppName, MB_ICONINFORMATION | MB_OK) ;
			g_bUpdateWindowNeeded = TRUE ;
			return 0 ;
		}
		break ;

	case WM_CLOSE:
		if (IDYES == AskConfirmation(hwnd))
		{
			DestroyWindow(hwnd) ;
		}
		return 0 ;

	case WM_QUERYENDSESSION:
		if (IDYES == AskConfirmation(hwnd))
			return 1 ;
		else
		    return 0 ;

	case WM_DESTROY:
		 if (g_pCPlanetSystem != NULL)
		 {
			ClearAllTextures(g_pCPlanetSystem) ;
			delete g_pCPlanetSystem ;
		 }
		 g_COpenGLCon.purge() ;
         PostQuitMessage(0) ;
		 return 0 ;
	}
    return DefWindowProc(hwnd, message, wParam, lParam) ; 
}




BOOL CALLBACK NewPlanetSystemProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND    hCtrlEdit ;
	static BOOLEAN bCorrectPlanetSystemData = FALSE ;
	int            x, y ;
	double*        pdCoord ;

	switch (message)
	{
	case WM_INITDIALOG:
        // The order is given by {IDC_SUN = 10, IDC_PLANET = 11 and 
		// IDC_MOON = 12},
		return FALSE ;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			// Save the new planet system data from the dialog box.
			bCorrectPlanetSystemData = SavePlanetSystemDataToStruct(hDlg, hCtrlEdit, &g_PlanetSystemData) ;

			// Update the main window for new information.
            UpdateWindowForNewData(g_hwndMain) ;

			// End the dialog and update.
			EndDialog(hDlg, TRUE) ;

			// Print out result to screen.
			x = 10 ;
			y = 150 ;
			PrintOutPlanetSystemData(g_hwndMain, g_PlanetSystemData, x, y) ;
			Sleep(1000) ;

			// Restart a new planet system if data OK.

			// Get window size.
			g_iScreenWidth  = GetSystemMetrics(SM_CXSCREEN) ;
			g_iScreenHeight = GetSystemMetrics(SM_CYSCREEN) ;

			// Set positional area of planets and suns.
			g_plMax_Coord[0] = (long) g_iScreenWidth ;
			g_plMax_Coord[1] = (long) g_iScreenHeight ;
			g_plMax_Coord[2] = (long) max(g_iScreenWidth, g_iScreenHeight) ;

			// Create a new planet system
			// with indata (dt, G, NrOfSuns, NrOfPlanets, Minimal distance between objects)
			if (g_PlanetSystemData.iNrOfSuns > 0 && 
				g_PlanetSystemData.iNrOfPlanets > 0)
			{
				// Remove old planet system.
				delete g_pCPlanetSystem ;
				g_pCPlanetSystem = NULL ;

				// Create a new Planet system.
				g_pCPlanetSystem = new CPlanetSystem(g_ddt/((double) g_iPartitioning), g_dG, g_PlanetSystemData.iNrOfSuns, 
													 g_PlanetSystemData.iNrOfPlanets, g_dMinDist) ;

				// Data to planet system (min mass planet, max mass planet, min mass sun, 
				// max mass sun, coordinates to place are planets and suns) 
				g_pCPlanetSystem->initiera(g_dMinPlanetWeight, g_dMaxPlanetWeight, 
										   g_dMinSunWeight, g_dMaxSunWeight, g_plMax_Coord) ;

				SavePlanetAndSunTextureNames(g_szPlanetTextures, g_szSunTextures,
											 g_pCPlanetSystem) ;

				g_pCPlanetSystem->setAllTextures(g_szPlanetTextures, g_szSunTextures) ;

				pdCoord = (double*) g_pCPlanetSystem->getSun(0).getKoord_Exakt() ;
				gluLookAt(0.0, 0.0, 0.0, pdCoord[0], pdCoord[1], pdCoord[2], 0.0, 1.0, 0.0) ;
			}

			g_bUpdateWindowNeeded = TRUE ;

			return TRUE ;

		case IDCANCEL:
			// End the dialog and no updates.
			EndDialog(hDlg, FALSE) ;

			// Print out result to screen.
			x = 10 ;
			y = 150 ;
			PrintOutPlanetSystemData(g_hwndMain, g_PlanetSystemData, x, y) ;
			Sleep(1000) ;
			g_bUpdateWindowNeeded = TRUE ;

			return TRUE ;
		}
		break ;
	}
	return FALSE ;
}


// Window that reads how many planets and suns that will be deleted.
BOOL CALLBACK DeletePlanetOrSunProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int    iNrOfPlanets = 0, iNrOfSuns = 0;
	static HWND    hCtrlEdit ;
	static BOOLEAN bCorrectPlanetOrSunDeletion = FALSE ;



	switch (message)
	{
	case WM_INITDIALOG:
		return FALSE ;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:

			// Deletes planets and suns from the solar system.
            bCorrectPlanetOrSunDeletion = DeletePlanetsOrSuns(hDlg, hCtrlEdit, g_pCPlanetSystem, 
							                                  &iNrOfPlanets, &iNrOfSuns) ;

			// Update the main window for new information.
            UpdateWindowForNewData(g_hwndMain) ;

			// End the dialog and update.
			EndDialog(hDlg, TRUE) ;

			g_bUpdateWindowNeeded = TRUE ;

			return TRUE ;

		case IDCANCEL:
			// End the dialog and no updates.
			EndDialog(hDlg, FALSE) ;

			g_bUpdateWindowNeeded = TRUE ;

			return TRUE ;
		}
		break ;
	}
	return FALSE ;
}

// Window that reads how many planets and suns that will be added.
BOOL CALLBACK AddPlanetOrSunProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int    iNrOfPlanets = 0, iNrOfSuns = 0;
	static HWND    hCtrlEdit ;



	switch (message)
	{
	case WM_INITDIALOG:
		return FALSE ;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:

			// Add planets and suns to the solar system.
            AddPlanetsOrSuns(hDlg, hCtrlEdit, g_pCPlanetSystem, 
			                 &iNrOfPlanets, &iNrOfSuns) ;

			// Update the main window for new information.
            UpdateWindowForNewData(g_hwndMain) ;

			// End the dialog and update.
			EndDialog(hDlg, TRUE) ;

			g_bUpdateWindowNeeded = TRUE ;

			return TRUE ;

		case IDCANCEL:
			// End the dialog and no updates.
			EndDialog(hDlg, FALSE) ;

			g_bUpdateWindowNeeded = TRUE ;

			return TRUE ;
		}
		break ;
	}
	return FALSE ;
}
