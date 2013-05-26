#include "functions.h"

static LPCTSTR lpszAppName = (LPCTSTR) "GL Clock";

HPALETTE hPalette = NULL;
HWND    hWnd_glob;

void winTitle(char *addon) {
	char buf[40];
	wsprintf((LPWSTR)buf, TEXT("%s%s"), lpszAppName, addon);
	SetWindowText(hWnd_glob, (LPCWSTR)buf);
}

void SetDCPixelFormat(HDC hDC)
{
    int nPixelFormat;

    static PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        24,
        0,0,0,0,0,0,
        0,0,
        0,0,0,0,0,
        32,
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0,0,0 };
    nPixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, nPixelFormat, &pfd);
} // SetDCPixelFormat

HPALETTE GetOpenGLPalette(HDC hDC)
{
	HPALETTE hRetPal = NULL;
	PIXELFORMATDESCRIPTOR pfd;
	LOGPALETTE *pPal;
	int nPixelFormat;
	int nColors;
	int i;
	BYTE RedRange,GreenRange,BlueRange;
	nPixelFormat = GetPixelFormat(hDC);
	DescribePixelFormat(hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	if(!(pfd.dwFlags & PFD_NEED_PALETTE))
		return NULL;
	nColors = 1 << pfd.cColorBits;
	pPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) +nColors*sizeof(PALETTEENTRY));
	pPal->palVersion = 0x300;
	pPal->palNumEntries = nColors;
	RedRange = (1 << pfd.cRedBits) -1;
	GreenRange = (1 << pfd.cGreenBits) - 1;
	BlueRange = (1 << pfd.cBlueBits) -1;
	for(i = 0; i < nColors; i++)
	{
		pPal->palPalEntry[i].peRed = (i >> pfd.cRedShift) & RedRange;
		pPal->palPalEntry[i].peRed = (unsigned char)(
			(double) pPal->palPalEntry[i].peRed * 255.0 / RedRange);
		pPal->palPalEntry[i].peGreen = (i >> pfd.cGreenShift) & GreenRange;
		pPal->palPalEntry[i].peGreen = (unsigned char)(
			(double)pPal->palPalEntry[i].peGreen * 255.0 / GreenRange);
		pPal->palPalEntry[i].peBlue = (i >> pfd.cBlueShift) & BlueRange;
		pPal->palPalEntry[i].peBlue = (unsigned char)(
			(double)pPal->palPalEntry[i].peBlue * 255.0 / BlueRange);
		pPal->palPalEntry[i].peFlags = (unsigned char) NULL;
	}
	hRetPal = CreatePalette(pPal);
	SelectPalette(hDC,hRetPal,FALSE);
	RealizePalette(hDC);
	free(pPal);
	return hRetPal;
} // GetOpenGLPalette

/* do przechwytywania komunikatów */
LRESULT CALLBACK WndProc(   HWND    hWnd,
                            UINT    message,
                            WPARAM  wParam,
                            LPARAM  lParam)
{	
    static HGLRC hRC;       // Sta-y kontekst renderowania
    static HDC hDC;         // Prywatny kontekst urzŚdzenia GDI
	hWnd_glob=hWnd;

	switch (message) {
	case WM_CREATE:
		hDC = GetDC(hWnd);
		SetDCPixelFormat(hDC);
		hPalette = GetOpenGLPalette(hDC);
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
		SetTimer(hWnd, 101, 1, NULL);
		init();
		break;
	case WM_DESTROY:
		KillTimer(hWnd,101);
		wglMakeCurrent(hDC,NULL);
		wglDeleteContext(hRC);
		if(hPalette != NULL)
			DeleteObject(hPalette);
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		reshape(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_TIMER:
		idle();
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_PAINT:
		draw();
		SwapBuffers(hDC);
		ValidateRect(hWnd, NULL);
		break;
	case WM_QUERYNEWPALETTE:
		if (hPalette) {
			int nRet;
			SelectPalette(hDC, hPalette, FALSE);
			nRet = RealizePalette(hDC);
			InvalidateRect(hWnd,NULL,FALSE);
			return nRet;
		}
	case WM_PALETTECHANGED:
		if ((hPalette != NULL) && ((HWND)wParam != hWnd)) {
			SelectPalette(hDC,hPalette,FALSE);
			RealizePalette(hDC);
			UpdateColors(hDC);
			return 0;
		}
		break;
	case WM_KEYDOWN:
		if (key((int)wParam))
			InvalidateRect(hWnd,NULL,FALSE);
		break;
	default:
		return (DefWindowProc(hWnd, message, wParam, lParam));
	}
	return (0L);
}

/* główna funckyjka */
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG             msg;            // Struktura komunikatu
    WNDCLASS        wc;             // Struktura klasy okna
    HWND            hWnd;           // Uchwyt g-Ąwnego okna
		//klasa okna
    wc.style            = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = (WNDPROC) WndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = hInstance;
    wc.hIcon            = NULL;
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
		//nie trzeba pędzla tła
    wc.hbrBackground    = NULL;
	
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = lpszAppName;

    // Zarejestrowanie klasy okna
    if(RegisterClass(&wc) == 0)
        return FALSE;

	hWnd = CreateWindow(
		lpszAppName,
		lpszAppName,
		// OpenGL wymaga WS_CLIPCHILDREN i WS_CLIPSIBLINGS
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		// Po-oŹenie i wymiary okna
		100, 100,
        width, height,
		NULL,
		NULL,
		hInstance,
		NULL);

	if(hWnd == NULL)
		return FALSE;

    ShowWindow(hWnd,SW_SHOW);
    UpdateWindow(hWnd);

    while( GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

	return msg.wParam;
}

