

#include "COpenGLContext.h"



COpenGLContext::COpenGLContext()
{
    reset();
}

COpenGLContext::~COpenGLContext()
{
    purge();
}

void COpenGLContext::init(HWND hWnd)
{
    // Remember the window handle (HWND)
    m_hWnd = hWnd;

    // Get the device context (DC)
    m_hDC = GetDC( m_hWnd );

    // Set the pixel format for the DC
    PIXELFORMATDESCRIPTOR pfd;
    ZeroMemory( &pfd, sizeof( pfd ) );
    pfd.nSize = sizeof( pfd );
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
                  PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    int format = ChoosePixelFormat( m_hDC, &pfd );
    SetPixelFormat( m_hDC, format, &pfd );

    // Create the render context (RC)
    m_hRC = wglCreateContext( m_hDC );

    // Make it the current render context
    wglMakeCurrent( m_hDC, m_hRC );
}

void COpenGLContext::purge()
{
    if ( m_hRC )
    {
        wglMakeCurrent( NULL, NULL );
        wglDeleteContext( m_hRC );
    }
    if ( m_hWnd && m_hDC )
    {
        ReleaseDC( m_hWnd, m_hDC );
    }
    reset();
}

void COpenGLContext::reset()
{
    m_hWnd = NULL;
    m_hDC = NULL;
    m_hRC = NULL;
}

