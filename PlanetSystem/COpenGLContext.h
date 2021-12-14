
#include <windows.h>
#include <gl/gl.h>  // For OpenGL calls.
#include <gl/glu.h> // Only needed for extra functions to OpenGL.



class COpenGLContext
{
private:
    HWND  m_hWnd;
    HDC   m_hDC;
    HGLRC m_hRC;

public:
    COpenGLContext() ;
    ~COpenGLContext() ;
    void init(HWND hWnd) ;
    void purge() ;

private:
    void reset() ;
} ;

