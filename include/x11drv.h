/*
 * X11 driver definitions
 */

#ifndef __WINE_X11DRV_H
#define __WINE_X11DRV_H

#include "config.h"

#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#ifdef HAVE_LIBXXSHM
# include <X11/extensions/XShm.h>
#endif /* defined(HAVE_LIBXXSHM) */

#include "windef.h"
#include "winbase.h"
#include "gdi.h"
#include "user.h"

#define MAX_PIXELFORMATS 8

struct tagBITMAPOBJ;
struct tagCLASS;
struct tagCREATESTRUCTA;
struct tagCURSORICONINFO;
struct tagDC;
struct tagDeviceCaps;
struct tagPALETTEOBJ;
struct tagWND;
struct tagWINDOWPOS;
struct tagKEYBOARD_CONFIG;
struct DIDEVICEOBJECTDATA;

  /* X physical pen */
typedef struct
{
    int          style;
    int          endcap;
    int          linejoin;
    int          pixel;
    int          width;
    char *       dashes;
    int          dash_len;
    int          type;          /* GEOMETRIC || COSMETIC */
} X_PHYSPEN;

  /* X physical brush */
typedef struct
{
    int          style;
    int          fillStyle;
    int          pixel;
    Pixmap       pixmap;
} X_PHYSBRUSH;

  /* X physical font */
typedef UINT	 X_PHYSFONT;

  /* X physical device */
typedef struct
{
    GC            gc;          /* X Window GC */
    Drawable      drawable;
    X_PHYSFONT    font;
    X_PHYSPEN     pen;
    X_PHYSBRUSH   brush;
    int           backgroundPixel;
    int           textPixel;
    XVisualInfo  *visuals[MAX_PIXELFORMATS];
    int           used_visuals;
    int           current_pf;
} X11DRV_PDEVICE;


  /* GCs used for B&W and color bitmap operations */
extern GC BITMAP_monoGC, BITMAP_colorGC;

#define BITMAP_GC(bmp) \
  (((bmp)->bitmap.bmBitsPixel == 1) ? BITMAP_monoGC : BITMAP_colorGC)

extern DeviceCaps X11DRV_DevCaps;

extern unsigned int X11DRV_server_startticks;

/* Wine driver X11 functions */

extern const DC_FUNCTIONS X11DRV_DC_Funcs;

extern BOOL X11DRV_BitBlt( struct tagDC *dcDst, INT xDst, INT yDst,
                             INT width, INT height, struct tagDC *dcSrc,
                             INT xSrc, INT ySrc, DWORD rop );
extern BOOL X11DRV_EnumDeviceFonts( HDC hdc, LPLOGFONTW plf,
				    DEVICEFONTENUMPROC dfeproc, LPARAM lp );
extern BOOL X11DRV_GetCharWidth( struct tagDC *dc, UINT firstChar,
                                   UINT lastChar, LPINT buffer );
extern BOOL X11DRV_GetDCOrgEx( struct tagDC *dc, LPPOINT lpp );
extern BOOL X11DRV_GetTextExtentPoint( struct tagDC *dc, LPCWSTR str,
                                         INT count, LPSIZE size );
extern BOOL X11DRV_GetTextMetrics(struct tagDC *dc, TEXTMETRICW *metrics);
extern BOOL X11DRV_PatBlt( struct tagDC *dc, INT left, INT top,
                             INT width, INT height, DWORD rop );
extern VOID   X11DRV_SetDeviceClipping(struct tagDC *dc);
extern BOOL X11DRV_StretchBlt( struct tagDC *dcDst, INT xDst, INT yDst,
                                 INT widthDst, INT heightDst,
                                 struct tagDC *dcSrc, INT xSrc, INT ySrc,
                                 INT widthSrc, INT heightSrc, DWORD rop );
extern BOOL X11DRV_LineTo( struct tagDC *dc, INT x, INT y);
extern BOOL X11DRV_Arc( struct tagDC *dc, INT left, INT top, INT right,
			  INT bottom, INT xstart, INT ystart, INT xend,
			  INT yend );
extern BOOL X11DRV_Pie( struct tagDC *dc, INT left, INT top, INT right,
			  INT bottom, INT xstart, INT ystart, INT xend,
			  INT yend );
extern BOOL X11DRV_Chord( struct tagDC *dc, INT left, INT top,
			    INT right, INT bottom, INT xstart,
			    INT ystart, INT xend, INT yend );
extern BOOL X11DRV_Ellipse( struct tagDC *dc, INT left, INT top,
			      INT right, INT bottom );
extern BOOL X11DRV_Rectangle(struct tagDC *dc, INT left, INT top,
			      INT right, INT bottom);
extern BOOL X11DRV_RoundRect( struct tagDC *dc, INT left, INT top,
				INT right, INT bottom, INT ell_width,
				INT ell_height );
extern COLORREF X11DRV_SetPixel( struct tagDC *dc, INT x, INT y,
				 COLORREF color );
extern COLORREF X11DRV_GetPixel( struct tagDC *dc, INT x, INT y);
extern BOOL X11DRV_PaintRgn( struct tagDC *dc, HRGN hrgn );
extern BOOL X11DRV_Polyline( struct tagDC *dc,const POINT* pt,INT count);
extern BOOL X11DRV_Polygon( struct tagDC *dc, const POINT* pt, INT count );
extern BOOL X11DRV_PolyPolygon( struct tagDC *dc, const POINT* pt, 
				  const INT* counts, UINT polygons);
extern BOOL X11DRV_PolyPolyline( struct tagDC *dc, const POINT* pt, 
				  const DWORD* counts, DWORD polylines);

extern HGDIOBJ X11DRV_SelectObject( struct tagDC *dc, HGDIOBJ handle );

extern COLORREF X11DRV_SetBkColor( struct tagDC *dc, COLORREF color );
extern COLORREF X11DRV_SetTextColor( struct tagDC *dc, COLORREF color );
extern BOOL X11DRV_ExtFloodFill( struct tagDC *dc, INT x, INT y,
				   COLORREF color, UINT fillType );
extern BOOL X11DRV_ExtTextOut( struct tagDC *dc, INT x, INT y,
				 UINT flags, const RECT *lprect,
				 LPCWSTR str, UINT count, const INT *lpDx );
extern BOOL X11DRV_CreateBitmap( HBITMAP hbitmap );
extern BOOL X11DRV_DeleteObject( HGDIOBJ handle );
extern LONG X11DRV_BitmapBits( HBITMAP hbitmap, void *bits, LONG count,
			       WORD flags );
extern INT X11DRV_SetDIBitsToDevice( struct tagDC *dc, INT xDest,
				       INT yDest, DWORD cx, DWORD cy,
				       INT xSrc, INT ySrc,
				       UINT startscan, UINT lines,
				       LPCVOID bits, const BITMAPINFO *info,
				       UINT coloruse );
extern INT X11DRV_DeviceBitmapBits( struct tagDC *dc, HBITMAP hbitmap,
				      WORD fGet, UINT startscan,
				      UINT lines, LPSTR bits,
				      LPBITMAPINFO info, UINT coloruse );
extern BOOL X11DRV_GetDeviceGammaRamp( struct tagDC *dc, LPVOID ramp );
extern BOOL X11DRV_SetDeviceGammaRamp( struct tagDC *dc, LPVOID ramp );

/* OpenGL / X11 driver functions */
extern int X11DRV_ChoosePixelFormat(DC *dc, const PIXELFORMATDESCRIPTOR *pppfd) ;
extern int X11DRV_DescribePixelFormat(DC *dc, int iPixelFormat, UINT nBytes, PIXELFORMATDESCRIPTOR *ppfd) ;
extern int X11DRV_GetPixelFormat(DC *dc) ;
extern BOOL X11DRV_SetPixelFormat(DC *dc, int iPixelFormat, const PIXELFORMATDESCRIPTOR *ppfd) ;
extern BOOL X11DRV_SwapBuffers(DC *dc) ;

/* X11 driver internal functions */

extern BOOL X11DRV_BITMAP_Init(void);
extern BOOL X11DRV_BRUSH_Init(void);
extern BOOL X11DRV_DIB_Init(void);
extern BOOL X11DRV_FONT_Init( struct tagDeviceCaps* );
extern BOOL X11DRV_OBM_Init(void);

struct tagBITMAPOBJ;
extern XImage *X11DRV_BITMAP_GetXImage( const struct tagBITMAPOBJ *bmp );
extern int X11DRV_DIB_GetXImageWidthBytes( int width, int depth );
extern HBITMAP X11DRV_BITMAP_CreateBitmapHeaderFromPixmap(Pixmap pixmap);
extern HGLOBAL X11DRV_DIB_CreateDIBFromPixmap(Pixmap pixmap, HDC hdc, BOOL bDeletePixmap);
extern HBITMAP X11DRV_BITMAP_CreateBitmapFromPixmap(Pixmap pixmap, BOOL bDeletePixmap);
extern Pixmap X11DRV_DIB_CreatePixmapFromDIB( HGLOBAL hPackedDIB, HDC hdc );
extern Pixmap X11DRV_BITMAP_CreatePixmapFromBitmap( HBITMAP hBmp, HDC hdc );

extern BOOL X11DRV_SetupGCForPatBlt( struct tagDC *dc, GC gc,
				       BOOL fMapColors );
extern BOOL X11DRV_SetupGCForBrush( struct tagDC *dc );
extern BOOL X11DRV_SetupGCForPen( struct tagDC *dc );
extern BOOL X11DRV_SetupGCForText( struct tagDC *dc );

extern const int X11DRV_XROPfunction[];

extern void _XInitImageFuncPtrs(XImage *);

#define XCREATEIMAGE(image,width,height,bpp) \
{ \
    int width_bytes = X11DRV_DIB_GetXImageWidthBytes( (width), (bpp) ); \
    (image) = TSXCreateImage(display, X11DRV_GetVisual(), \
                           (bpp), ZPixmap, 0, calloc( (height), width_bytes ),\
                           (width), (height), 32, width_bytes ); \
}

/* exported dib functions for now */

/* Additional info for DIB section objects */
typedef struct
{
    /* Windows DIB section */
    DIBSECTION  dibSection;

    /* Mapping status */
    int         status, p_status;

    /* Color map info */
    int         nColorMap;
    int        *colorMap;

    /* Cached XImage */
    XImage     *image;

    /* Selector for 16-bit access to bits */
    WORD selector;

#ifdef HAVE_LIBXXSHM
    /* Shared memory segment info */
    XShmSegmentInfo shminfo;
#endif

    /* Aux buffer access function */
    void (*copy_aux)(void*ctx, int req);
    void *aux_ctx;

    /* GDI access lock */
    CRITICAL_SECTION lock;

} X11DRV_DIBSECTION;

extern int *X11DRV_DIB_BuildColorMap( struct tagDC *dc, WORD coloruse,
				      WORD depth, const BITMAPINFO *info,
				      int *nColors );
extern INT X11DRV_CoerceDIBSection(struct tagDC *dc,INT,BOOL);
extern INT X11DRV_LockDIBSection(struct tagDC *dc,INT,BOOL);
extern void X11DRV_UnlockDIBSection(struct tagDC *dc,BOOL);
extern INT X11DRV_CoerceDIBSection2(HBITMAP bmp,INT,BOOL);
extern INT X11DRV_LockDIBSection2(HBITMAP bmp,INT,BOOL);
extern void X11DRV_UnlockDIBSection2(HBITMAP bmp,BOOL);

extern HBITMAP X11DRV_DIB_CreateDIBSection(struct tagDC *dc, BITMAPINFO *bmi, UINT usage,
					   LPVOID *bits, HANDLE section, DWORD offset, DWORD ovr_pitch);
extern HBITMAP16 X11DRV_DIB_CreateDIBSection16(struct tagDC *dc, BITMAPINFO *bmi, UINT16 usage,
					       SEGPTR *bits, HANDLE section, DWORD offset, DWORD ovr_pitch);

extern struct tagBITMAP_DRIVER X11DRV_BITMAP_Driver;

extern INT X11DRV_DIB_SetDIBits(struct tagBITMAPOBJ *bmp, struct tagDC *dc, UINT startscan,
				UINT lines, LPCVOID bits, const BITMAPINFO *info,
				UINT coloruse, HBITMAP hbitmap);
extern INT X11DRV_DIB_GetDIBits(struct tagBITMAPOBJ *bmp, struct tagDC *dc, UINT startscan,
				UINT lines, LPVOID bits, BITMAPINFO *info,
				UINT coloruse, HBITMAP hbitmap);
extern void X11DRV_DIB_DeleteDIBSection(struct tagBITMAPOBJ *bmp);
extern UINT X11DRV_DIB_SetDIBColorTable(struct tagBITMAPOBJ *,struct tagDC*,UINT,UINT,const RGBQUAD *);
extern UINT X11DRV_DIB_GetDIBColorTable(struct tagBITMAPOBJ *,struct tagDC*,UINT,UINT,RGBQUAD *);
extern INT X11DRV_DIB_Coerce(struct tagBITMAPOBJ *,INT,BOOL);
extern INT X11DRV_DIB_Lock(struct tagBITMAPOBJ *,INT,BOOL);
extern void X11DRV_DIB_Unlock(struct tagBITMAPOBJ *,BOOL);
void X11DRV_DIB_CopyDIBSection(DC *dcSrc, DC *dcDst,     
			       DWORD xSrc, DWORD ySrc,   
			       DWORD xDest, DWORD yDest, 
			       DWORD width, DWORD height);

extern INT X11DRV_DCICommand(INT cbInput, LPVOID lpInData, LPVOID lpOutData);

/**************************************************************************
 * X11 GDI driver
 */

BOOL X11DRV_GDI_Initialize(void);
void X11DRV_GDI_Finalize(void);

/* X11 GDI palette driver */

#define X11DRV_PALETTE_FIXED    0x0001 /* read-only colormap - have to use XAllocColor (if not virtual) */
#define X11DRV_PALETTE_VIRTUAL  0x0002 /* no mapping needed - pixel == pixel color */

#define X11DRV_PALETTE_PRIVATE  0x1000 /* private colormap, identity mapping */
#define X11DRV_PALETTE_WHITESET 0x2000

extern Colormap X11DRV_PALETTE_PaletteXColormap;
extern UINT16 X11DRV_PALETTE_PaletteFlags;

extern int *X11DRV_PALETTE_PaletteToXPixel;
extern int *X11DRV_PALETTE_XPixelToPalette;

extern int X11DRV_PALETTE_mapEGAPixel[16];

extern BOOL X11DRV_PALETTE_Init(void);
extern void X11DRV_PALETTE_Cleanup(void);

extern COLORREF X11DRV_PALETTE_ToLogical(int pixel);
extern int X11DRV_PALETTE_ToPhysical(struct tagDC *dc, COLORREF color);

extern struct tagPALETTE_DRIVER X11DRV_PALETTE_Driver;

extern int X11DRV_PALETTE_SetMapping(struct tagPALETTEOBJ *palPtr, UINT uStart, UINT uNum, BOOL mapOnly);
extern int X11DRV_PALETTE_UpdateMapping(struct tagPALETTEOBJ *palPtr);
extern BOOL X11DRV_PALETTE_IsDark(int pixel);

/**************************************************************************
 * X11 USER driver
 */

extern Display *display;
extern Screen *screen;
extern Visual *visual;
extern Window root_window;
extern unsigned int screen_width;
extern unsigned int screen_height;
extern unsigned int screen_depth;

static inline Screen *X11DRV_GetXScreen(void)    { return screen; }
static inline Visual *X11DRV_GetVisual(void)     { return visual; }
static inline Window X11DRV_GetXRootWindow(void) { return root_window; }
static inline unsigned int X11DRV_GetDepth(void) { return screen_depth; }

/* X11 clipboard driver */

extern void X11DRV_CLIPBOARD_FreeResources( Atom property );
extern BOOL X11DRV_CLIPBOARD_RegisterPixmapResource( Atom property, Pixmap pixmap );
extern BOOL X11DRV_CLIPBOARD_IsNativeProperty(Atom prop);
extern UINT X11DRV_CLIPBOARD_MapPropertyToFormat(char *itemFmtName);
extern Atom X11DRV_CLIPBOARD_MapFormatToProperty(UINT id);
extern void X11DRV_CLIPBOARD_ReleaseSelection(Atom selType, Window w, HWND hwnd);
extern BOOL X11DRV_IsSelectionOwner(void);
extern BOOL X11DRV_GetClipboardData(UINT wFormat);

/* X11 event driver */

extern WORD X11DRV_EVENT_XStateToKeyState( int state ) ;

extern void X11DRV_EVENT_Init(void);
extern void X11DRV_EVENT_Cleanup(void);
extern void X11DRV_Synchronize( void );

typedef enum {
  X11DRV_INPUT_RELATIVE,
  X11DRV_INPUT_ABSOLUTE
} INPUT_TYPE;
extern INPUT_TYPE X11DRV_EVENT_SetInputMethod(INPUT_TYPE type);

#ifdef HAVE_LIBXXF86DGA2
void X11DRV_EVENT_SetDGAStatus(HWND hwnd, int event_base) ;
#endif


/* X11 keyboard driver */

extern void X11DRV_InitKeyboard(void);
extern WORD X11DRV_VkKeyScan(CHAR cChar);
extern UINT16 X11DRV_MapVirtualKey(UINT16 wCode, UINT16 wMapType);
extern INT16 X11DRV_GetKeyNameText(LONG lParam, LPSTR lpBuffer, INT16 nSize);
extern BOOL X11DRV_GetDIState(DWORD len, LPVOID ptr);
extern BOOL X11DRV_GetDIData(BYTE *keystate, DWORD dodsize, struct DIDEVICEOBJECTDATA *dod, LPDWORD entries, DWORD flags);
extern void X11DRV_GetKeyboardConfig(struct tagKEYBOARD_CONFIG *cfg);
extern void X11DRV_SetKeyboardConfig(struct tagKEYBOARD_CONFIG *cfg, DWORD mask);

extern void X11DRV_HandleEvent(struct tagWND *pWnd, XKeyEvent *event);

/* X11 mouse driver */

extern void X11DRV_InitMouse(LPMOUSE_EVENT_PROC);
extern void X11DRV_SetCursor(struct tagCURSORICONINFO *lpCursor);
extern void X11DRV_MoveCursor(WORD wAbsX, WORD wAbsY);
extern void X11DRV_SendEvent( DWORD mouseStatus, DWORD posX, DWORD posY,
                                    DWORD keyState, DWORD time, HWND hWnd );

/* X11 windows driver */

extern struct tagWND_DRIVER X11DRV_WND_Driver;

typedef struct _X11DRV_WND_DATA {
  Window window;
  HBITMAP hWMIconBitmap;
  HBITMAP hWMIconMask;
  int bit_gravity;
} X11DRV_WND_DATA;

extern Window X11DRV_WND_GetXWindow(struct tagWND *wndPtr);
extern Window X11DRV_WND_FindXWindow(struct tagWND *wndPtr);

extern void X11DRV_WND_Initialize(struct tagWND *wndPtr);
extern void X11DRV_WND_Finalize(struct tagWND *wndPtr);
extern BOOL X11DRV_WND_CreateDesktopWindow(struct tagWND *wndPtr);
extern BOOL X11DRV_WND_CreateWindow(struct tagWND *wndPtr, struct tagCREATESTRUCTA *cs, BOOL bUnicode);
extern BOOL X11DRV_WND_DestroyWindow(struct tagWND *pWnd);
extern struct tagWND *X11DRV_WND_SetParent(struct tagWND *wndPtr, struct tagWND *pWndParent);
extern void X11DRV_WND_ForceWindowRaise(struct tagWND *pWnd);
extern void X11DRV_WND_SetWindowPos(struct tagWND *wndPtr, const struct tagWINDOWPOS *winpos, BOOL bSMC_SETXPOS);
extern void X11DRV_WND_SetText(struct tagWND *wndPtr, LPCWSTR text);
extern void X11DRV_WND_SetFocus(struct tagWND *wndPtr);
extern void X11DRV_WND_PreSizeMove(struct tagWND *wndPtr);
extern void X11DRV_WND_PostSizeMove(struct tagWND *wndPtr);
extern void X11DRV_WND_SurfaceCopy(struct tagWND *wndPtr, HDC hdc, INT dx, INT dy, const RECT *clipRect, BOOL bUpdate);
extern void X11DRV_WND_SetDrawable(struct tagWND *wndPtr, HDC hdc, WORD flags, BOOL bSetClipOrigin);
extern BOOL X11DRV_WND_SetHostAttr(struct tagWND *wndPtr, INT haKey, INT value);
extern BOOL X11DRV_WND_IsSelfClipping(struct tagWND *wndPtr);
extern void X11DRV_WND_DockWindow(struct tagWND *wndPtr);
extern void X11DRV_WND_SetWindowRgn(struct tagWND *wndPtr, HRGN hrgnWnd); 

extern int X11DRV_EVENT_PrepareShmCompletion( Drawable dw );
extern void X11DRV_EVENT_WaitShmCompletion( int compl );
extern void X11DRV_EVENT_WaitShmCompletions( Drawable dw );
extern void X11DRV_EVENT_WaitReplaceShmCompletion( int *compl, Drawable dw );

#endif  /* __WINE_X11DRV_H */
