//******** PRACTICA VISUALITZACIÓ 3D (Facultat de Ciències / Escola d'Enginyeria - UAB)
//******** Entorn bàsic VS2019 MULTIFINESTRA amb OpenGL, interfície MFC i Status Bar
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (Setembre 2021)
// EntornV3DView.cpp: implementación de la clase CEntornV3DView
// FUNCIONS:		- Control del bucle principal (OnPaint)
//					- Control teclat (OnKeyDown)
//					- Control mouse interactiu i botons mouse 
//							(OnLButtomDown, OnRButtomDown, OnMouseMove)
//					- Control opcions de menú (On*, OnUpdate*)
//					- Control de color de fons per teclat (FONS)
//					- Transformacions Geomètriques Interactives via mouse
//
//    Versió 2.0:	- Canvi de color de l'objecte per teclat (OBJ)
//					- Fixar Transformacions Geomètriques desactivant opcions Transforma (INSERT dins opcions Transforma)
//					- Esborrar Transformacions Geomètriques fixades (DELETE dins opcions Transforma)
//					- Fixar Traslació pantalla fora de l'opció Vista->Pan? (INSERT dins opció Vista->Pan?)
//					- Esborrar Traslació pantalla fixada (DELETE dins opció Vista->Pan?)
//
//	  Versió 2.2:	- Opció VISTA --> Satèl.lit: Punt de Vista en moviment segons moviment mouse
//
//	  Versió 2.5:	- Afegits objectes cubRGB i Tie (nau Star Wars fet per alumnes)
//					- Opció VISTA --> FullScreen?: Opció FullScreen per menú i per PopUp menu enmig pantalla. Té un bug al restaurar pantalla 
//													normal (inclos menú IDR_MENU_WINDOW)
//

#include "stdafx.h"

// Se pueden definir SHARED_HANDLERS en un proyecto ATL implementando controladores de vista previa, miniatura
// y filtro de búsqueda, y permiten compartir código de documentos con ese proyecto.
#ifndef SHARED_HANDLERS
#include "EntornV3D.h"
#endif

#include "EntornV3DDoc.h"
#include "EntornV3DView.h"
#include "visualitzacio.h"	// Include funcions de projeció i il.luminació
#include "escena.h"			// Include funcions d'objectes OpenGL
#include "trajectories.h"	// EntornV3D. ESCENA: Include funcions trajectòries BSpline i càlcul del Triedre de Frenet
GLfloat croata=1;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////
// CEntornV3DView

IMPLEMENT_DYNCREATE(CEntornV3DView, CView)

BEGIN_MESSAGE_MAP(CEntornV3DView, CView)
	// Comandos de impresión estándar
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEntornV3DView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(ID_ARXIU_OBRIR_FRACTAL, &CEntornV3DView::OnArxiuObrirFractal)
	ON_COMMAND(ID_ARXIU_OBRIR_FITXER_OBJ, &CEntornV3DView::OnArxiuObrirFitxerObj)
	ON_COMMAND(ID_ARXIU_OBRIR_FITXER_3DS, &CEntornV3DView::OnArxiuObrirFitxer3ds)
	ON_COMMAND(ID_VISTA_Mobil, &CEntornV3DView::OnVistaMobil)
	ON_UPDATE_COMMAND_UI(ID_VISTA_Mobil, &CEntornV3DView::OnUpdateVistaMobil)
	ON_COMMAND(ID_VISTA_ZOOM, &CEntornV3DView::OnVistaZoom)
	ON_UPDATE_COMMAND_UI(ID_VISTA_ZOOM, &CEntornV3DView::OnUpdateVistaZoom)
	ON_COMMAND(ID_VISTA_POLARS_X, &CEntornV3DView::OnVistaPolarsX)
	ON_UPDATE_COMMAND_UI(ID_VISTA_POLARS_X, &CEntornV3DView::OnUpdateVistaPolarsX)
	ON_COMMAND(ID_VISTA_POLARS_Y, &CEntornV3DView::OnVistaPolarsY)
	ON_UPDATE_COMMAND_UI(ID_VISTA_POLARS_Y, &CEntornV3DView::OnUpdateVistaPolarsY)
	ON_COMMAND(ID_VISTA_POLARS_Z, &CEntornV3DView::OnVistaPolarsZ)
	ON_UPDATE_COMMAND_UI(ID_VISTA_POLARS_Z, &CEntornV3DView::OnUpdateVistaPolarsZ)
	ON_COMMAND(ID_VISTA_PAN, &CEntornV3DView::OnVistaPan)
	ON_UPDATE_COMMAND_UI(ID_VISTA_PAN, &CEntornV3DView::OnUpdateVistaPan)
	ON_COMMAND(ID_VISTA_ORIGENPAN, &CEntornV3DView::OnVistaOrigenpan)
	ON_COMMAND(ID_VISTA_NAVEGA, &CEntornV3DView::OnVistaNavega)
	ON_UPDATE_COMMAND_UI(ID_VISTA_NAVEGA, &CEntornV3DView::OnUpdateVistaNavega)
	ON_COMMAND(ID_VISTA_ORIGENNAVEGA, &CEntornV3DView::OnVistaOrigennavega)
	ON_COMMAND(ID_VISTA_EIXOS, &CEntornV3DView::OnVistaEixos)
	ON_UPDATE_COMMAND_UI(ID_VISTA_EIXOS, &CEntornV3DView::OnUpdateVistaEixos)
	ON_COMMAND(ID_PROJECCIO_PERSPECTIVA, &CEntornV3DView::OnProjeccioPerspectiva)
	ON_UPDATE_COMMAND_UI(ID_PROJECCIO_PERSPECTIVA, &CEntornV3DView::OnUpdateProjeccioPerspectiva)
	ON_COMMAND(ID_OBJECTE_CUB, &CEntornV3DView::OnObjecteCub)
	ON_UPDATE_COMMAND_UI(ID_OBJECTE_CUB, &CEntornV3DView::OnUpdateObjecteCub)
	ON_COMMAND(ID_OBJECTE_ESFERA, &CEntornV3DView::OnObjecteEsfera)
	ON_UPDATE_COMMAND_UI(ID_OBJECTE_ESFERA, &CEntornV3DView::OnUpdateObjecteEsfera)
	ON_COMMAND(ID_OBJECTE_TETERA, &CEntornV3DView::OnObjecteTetera)
	ON_UPDATE_COMMAND_UI(ID_OBJECTE_TETERA, &CEntornV3DView::OnUpdateObjecteTetera)
	ON_COMMAND(ID_OBJECTE_TRUCK, &CEntornV3DView::OnObjecteTruck)
	ON_UPDATE_COMMAND_UI(ID_OBJECTE_TRUCK, &CEntornV3DView::OnUpdateObjecteTruck)
	ON_COMMAND(ID_TRANSFORMA_TRASLACIO, &CEntornV3DView::OnTransformaTraslacio)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORMA_TRASLACIO, &CEntornV3DView::OnUpdateTransformaTraslacio)
	ON_COMMAND(ID_TRANSFORMA_ORIGENTRASLACIO, &CEntornV3DView::OnTransformaOrigentraslacio)
	ON_COMMAND(ID_TRANSFORMA_ROTACIO, &CEntornV3DView::OnTransformaRotacio)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORMA_ROTACIO, &CEntornV3DView::OnUpdateTransformaRotacio)
	ON_COMMAND(ID_TRANSFORMA_ORIGENROTACIO, &CEntornV3DView::OnTransformaOrigenrotacio)
	ON_COMMAND(ID_TRANSFORMA_ESCALAT, &CEntornV3DView::OnTransformaEscalat)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORMA_ESCALAT, &CEntornV3DView::OnUpdateTransformaEscalat)
	ON_COMMAND(ID_TRANSFORMA_ORIGENESCALAT, &CEntornV3DView::OnTransformaOrigenescalat)
	ON_COMMAND(ID_TRANSFORMA_MOBILX, &CEntornV3DView::OnTransformaMobilx)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORMA_MOBILX, &CEntornV3DView::OnUpdateTransformaMobilx)
	ON_COMMAND(ID_TRANSFORMA_MOBILY, &CEntornV3DView::OnTransformaMobily)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORMA_MOBILY, &CEntornV3DView::OnUpdateTransformaMobily)
	ON_COMMAND(ID_TRANSFORMA_MOBILZ, &CEntornV3DView::OnTransformaMobilz)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORMA_MOBILZ, &CEntornV3DView::OnUpdateTransformaMobilz)
	ON_COMMAND(ID_OCULTACIONS_TESTVIS, &CEntornV3DView::OnOcultacionsTestvis)
	ON_UPDATE_COMMAND_UI(ID_OCULTACIONS_TESTVIS, &CEntornV3DView::OnUpdateOcultacionsTestvis)
	ON_COMMAND(ID_OCULTACIONS_ZBUFFER, &CEntornV3DView::OnOcultacionsZbuffer)
	ON_UPDATE_COMMAND_UI(ID_OCULTACIONS_ZBUFFER, &CEntornV3DView::OnUpdateOcultacionsZbuffer)
	ON_COMMAND(ID_OCULTACIONS_BACKLINE, &CEntornV3DView::OnOcultacionsBackline)
	ON_UPDATE_COMMAND_UI(ID_OCULTACIONS_BACKLINE, &CEntornV3DView::OnUpdateOcultacionsBackline)
	ON_COMMAND(ID_ILUMINACIO_LLUMFIXE, &CEntornV3DView::OnIluminacioLlumfixe)
	ON_UPDATE_COMMAND_UI(ID_ILUMINACIO_LLUMFIXE, &CEntornV3DView::OnUpdateIluminacioLlumfixe)
	ON_COMMAND(ID_ILUMINACIO_FILFERROS, &CEntornV3DView::OnIluminacioFilferros)
	ON_UPDATE_COMMAND_UI(ID_ILUMINACIO_FILFERROS, &CEntornV3DView::OnUpdateIluminacioFilferros)
	ON_COMMAND(ID_ILUMINACIO_PLANA, &CEntornV3DView::OnIluminacioPlana)
	ON_UPDATE_COMMAND_UI(ID_ILUMINACIO_PLANA, &CEntornV3DView::OnUpdateIluminacioPlana)
	ON_COMMAND(ID_ILUMINACIO_GOURAUD, &CEntornV3DView::OnIluminacioGouraud)
	ON_UPDATE_COMMAND_UI(ID_ILUMINACIO_GOURAUD, &CEntornV3DView::OnUpdateIluminacioGouraud)
	ON_COMMAND(ID_MATERIAL_EMISSIO, &CEntornV3DView::OnMaterialEmissio)
	ON_UPDATE_COMMAND_UI(ID_MATERIAL_EMISSIO, &CEntornV3DView::OnUpdateMaterialEmissio)
	ON_COMMAND(ID_MATERIAL_AMBIENT, &CEntornV3DView::OnMaterialAmbient)
	ON_UPDATE_COMMAND_UI(ID_MATERIAL_AMBIENT, &CEntornV3DView::OnUpdateMaterialAmbient)
	ON_COMMAND(ID_MATERIAL_DIFUSA, &CEntornV3DView::OnMaterialDifusa)
	ON_UPDATE_COMMAND_UI(ID_MATERIAL_DIFUSA, &CEntornV3DView::OnUpdateMaterialDifusa)
	ON_COMMAND(ID_MATERIAL_ESPECULAR, &CEntornV3DView::OnMaterialEspecular)
	ON_UPDATE_COMMAND_UI(ID_MATERIAL_ESPECULAR, &CEntornV3DView::OnUpdateMaterialEspecular)
	ON_COMMAND(ID_ILUMINACIO_TEXTURES, &CEntornV3DView::OnIluminacioTextures)
	ON_UPDATE_COMMAND_UI(ID_ILUMINACIO_TEXTURES, &CEntornV3DView::OnUpdateIluminacioTextures)
	ON_COMMAND(ID_ILUMINACIO_TEXTURA_FITXERIMATGE, &CEntornV3DView::OnIluminacioTexturaFitxerimatge)
	ON_UPDATE_COMMAND_UI(ID_ILUMINACIO_TEXTURA_FITXERIMATGE, &CEntornV3DView::OnUpdateIluminacioTexturaFitxerimatge)
	ON_COMMAND(ID_LLUMS_LLUMAMBIENT, &CEntornV3DView::OnLlumsLlumambient)
	ON_UPDATE_COMMAND_UI(ID_LLUMS_LLUMAMBIENT, &CEntornV3DView::OnUpdateLlumsLlumambient)
	ON_COMMAND(ID_LLUMS_LLUM0, &CEntornV3DView::OnLlumsLlum0)
	ON_UPDATE_COMMAND_UI(ID_LLUMS_LLUM0, &CEntornV3DView::OnUpdateLlumsLlum0)
	ON_COMMAND(ID_SHADERS_SENSE, &CEntornV3DView::OnShadersSense)
	ON_UPDATE_COMMAND_UI(ID_SHADERS_SENSE, &CEntornV3DView::OnUpdateShadersSense)
	ON_COMMAND(ID_SHADERS_GOURAUD, &CEntornV3DView::OnShadersGouraud)
	ON_UPDATE_COMMAND_UI(ID_SHADERS_GOURAUD, &CEntornV3DView::OnUpdateShadersGouraud)
	ON_COMMAND(ID_SHADERS_PHONG, &CEntornV3DView::OnShadersPhong)
	ON_UPDATE_COMMAND_UI(ID_SHADERS_PHONG, &CEntornV3DView::OnUpdateShadersPhong)
	ON_COMMAND(ID_VISTA_SATELIT, &CEntornV3DView::OnVistaSatelit)
	ON_UPDATE_COMMAND_UI(ID_VISTA_SATELIT, &CEntornV3DView::OnUpdateVistaSatelit)
	ON_COMMAND(ID_VISTA_FULLSCREEN, &CEntornV3DView::OnVistaFullscreen)
	ON_UPDATE_COMMAND_UI(ID_VISTA_FULLSCREEN, &CEntornV3DView::OnUpdateVistaFullscreen)
	ON_COMMAND(ID_OBJECTE_CUB_RGB, &CEntornV3DView::OnObjecteCubRGB)
	ON_UPDATE_COMMAND_UI(ID_OBJECTE_CUB_RGB, &CEntornV3DView::OnUpdateObjecteCubRGB)
	ON_COMMAND(ID_OBJECTE_TIE, &CEntornV3DView::OnObjecteTie)
	ON_UPDATE_COMMAND_UI(ID_OBJECTE_TIE, &CEntornV3DView::OnUpdateObjecteTie)
	ON_COMMAND(ID_ILUMINACIO_PUNTS, &CEntornV3DView::OnIluminacioPunts)
	ON_UPDATE_COMMAND_UI(ID_ILUMINACIO_PUNTS, &CEntornV3DView::OnUpdateIluminacioPunts)
	ON_COMMAND(ID_SHADER_LOAD_FILES, &CEntornV3DView::OnShaderLoadFiles)
	ON_UPDATE_COMMAND_UI(ID_SHADER_LOAD_FILES, &CEntornV3DView::OnUpdateShaderLoadFiles)
	ON_COMMAND(ID_OCULTACIONS_FRONTFACES, &CEntornV3DView::OnOcultacionsFrontFaces)
	ON_UPDATE_COMMAND_UI(ID_OCULTACIONS_FRONTFACES, &CEntornV3DView::OnUpdateOcultacionsFrontFaces)
	ON_COMMAND(ID_LLUMS_LLUM1, &CEntornV3DView::OnLlumsLlum1)
	ON_UPDATE_COMMAND_UI(ID_LLUMS_LLUM1, &CEntornV3DView::OnUpdateLlumsLlum1)
	ON_COMMAND(ID_LLUMS_LLUM2, &CEntornV3DView::OnLlumsLlum2)
	ON_UPDATE_COMMAND_UI(ID_LLUMS_LLUM2, &CEntornV3DView::OnUpdateLlumsLlum2)
	ON_COMMAND(ID_LLUMS_LLUM3, &CEntornV3DView::OnLlumsLlum3)
	ON_UPDATE_COMMAND_UI(ID_LLUMS_LLUM3, &CEntornV3DView::OnUpdateLlumsLlum3)
	ON_COMMAND(ID_LLUMS_LLUM4, &CEntornV3DView::OnLlumsLlum4)
	ON_UPDATE_COMMAND_UI(ID_LLUMS_LLUM4, &CEntornV3DView::OnUpdateLlumsLlum4)
	ON_COMMAND(ID_OBJETE_CORBA_BEZIER, &CEntornV3DView::OnObjeteCorbaBezier)
	ON_UPDATE_COMMAND_UI(ID_OBJETE_CORBA_BEZIER, &CEntornV3DView::OnUpdateObjeteCorbaBezier)
	ON_COMMAND(ID_OBJECTE_CORBA_LEMNISCATA, &CEntornV3DView::OnObjecteCorbaLemniscata)
	ON_UPDATE_COMMAND_UI(ID_OBJECTE_CORBA_LEMNISCATA, &CEntornV3DView::OnUpdateObjecteCorbaLemniscata)
	ON_COMMAND(ID_OBJECTE_CORBA_BSPLINE, &CEntornV3DView::OnObjecteCorbaBSpline)
	ON_UPDATE_COMMAND_UI(ID_OBJECTE_CORBA_BSPLINE, &CEntornV3DView::OnUpdateObjecteCorbaBSpline)
	ON_COMMAND(ID_OBJECTE_PUNTS_CONTROL, &CEntornV3DView::OnObjectePuntsControl)
	ON_UPDATE_COMMAND_UI(ID_OBJECTE_PUNTS_CONTROL, &CEntornV3DView::OnUpdateObjectePuntsControl)
	ON_COMMAND(ID_VISTA_GRIDXY, &CEntornV3DView::OnVistaGridXY)
	ON_UPDATE_COMMAND_UI(ID_VISTA_GRIDXY, &CEntornV3DView::OnUpdateVistaGridXY)
	ON_COMMAND(ID_VISTA_GRIDXZ, &CEntornV3DView::OnVistaGridXZ)
	ON_UPDATE_COMMAND_UI(ID_VISTA_GRIDXZ, &CEntornV3DView::OnUpdateVistaGridXZ)
	ON_COMMAND(ID_VISTA_GRIDYZ, &CEntornV3DView::OnVistaGridYZ)
	ON_UPDATE_COMMAND_UI(ID_VISTA_GRIDYZ, &CEntornV3DView::OnUpdateVistaGridYZ)
	ON_COMMAND(ID_VISTA_GRIDXYZ, &CEntornV3DView::OnVistaGridXYZ)
	ON_UPDATE_COMMAND_UI(ID_VISTA_GRIDXYZ, &CEntornV3DView::OnUpdateVistaGridXYZ)
	ON_COMMAND(ID_ILUMINACIO2SIDES, &CEntornV3DView::OnIluminacio2Sides)
	ON_UPDATE_COMMAND_UI(ID_ILUMINACIO2SIDES, &CEntornV3DView::OnUpdateIluminacio2Sides)
		//ON_COMMAND(ID_OBJECTE_ESCENA_TFRENET, &CEntornV3DView::OnObjecteEscenaTfrenet)
		//ON_UPDATE_COMMAND_UI(ID_OBJECTE_ESCENA_TFRENET, &CEntornV3DView::OnUpdateObjecteEscenaTFrenet)
		//ON_COMMAND(ID_OBJECTE_ESCENA, &CEntornV3DView::OnObjecteEscena)
		//ON_UPDATE_COMMAND_UI(ID_OBJECTE_ESCENA, &CEntornV3DView::OnUpdateObjecteEscena)
		ON_COMMAND(ID_CAMERA_ESFERICA, &CEntornV3DView::OnCameraEsferica)
		ON_UPDATE_COMMAND_UI(ID_CAMERA_ESFERICA, &CEntornV3DView::OnUpdateCameraEsferica)
		ON_COMMAND(ID_OBJECTE_ESCENA_LOXODROMA, &CEntornV3DView::OnObjecteEscenaLoxodroma)
		ON_UPDATE_COMMAND_UI(ID_OBJECTE_ESCENA_LOXODROMA, &CEntornV3DView::OnUpdateObjecteEscenaLoxodroma)
		ON_COMMAND(ID_CORBES_TRIEDREFRENET, &CEntornV3DView::OnCorbesTriedreFrenet)
		ON_UPDATE_COMMAND_UI(ID_CORBES_TRIEDREFRENET, &CEntornV3DView::OnUpdateCorbesTriedreFrenet)
		ON_COMMAND(ID_PROJECCIO_ORTOGRAFICA, &CEntornV3DView::OnProjeccioOrtografica)
		ON_UPDATE_COMMAND_UI(ID_PROJECCIO_ORTOGRAFICA, &CEntornV3DView::OnUpdateProjeccioOrtografica)
		ON_COMMAND(ID_PROJECCIO_AXONOMETRICA, &CEntornV3DView::OnProjeccioAxonometrica)
		ON_UPDATE_COMMAND_UI(ID_PROJECCIO_AXONOMETRICA, &CEntornV3DView::OnUpdateProjeccioAxonometrica)
		ON_COMMAND(ID_TRANSFORMA_QUATERNIONS, &CEntornV3DView::OnTransforma_Quaternions)
		ON_COMMAND(ID_OBJECTE_CAMI32875, &CEntornV3DView::OnObjecteCamion)
		ON_UPDATE_COMMAND_UI(ID_OBJECTE_CAMI32875, &CEntornV3DView::OnUpdateObjecteCamion)
		ON_UPDATE_COMMAND_UI(ID_TRANSFORMA_QUATERNIONS, &CEntornV3DView::OnUpdateTransformaQuaternions)
		ON_COMMAND(ID_TRANSFORMA_LERP, &CEntornV3DView::OnTransformaLerp)
		ON_UPDATE_COMMAND_UI(ID_TRANSFORMA_LERP, &CEntornV3DView::OnUpdateTransformaLerp)
		ON_COMMAND(ID_TRANSFORMA_SLERP, &CEntornV3DView::OnTransformaSlerp)
		ON_UPDATE_COMMAND_UI(ID_TRANSFORMA_SLERP, &CEntornV3DView::OnUpdateTransformaSlerp)
		ON_COMMAND(ID_OBJECTE_PATO, &CEntornV3DView::OnObjectePato)
		ON_UPDATE_COMMAND_UI(ID_OBJECTE_PATO, &CEntornV3DView::OnUpdateObjectePato)
		ON_COMMAND(ID_LOXODROMA_LOXODROMA32891, &CEntornV3DView::OnLoxodromaLoxodroma32891)
		ON_UPDATE_COMMAND_UI(ID_LOXODROMA_LOXODROMA32891, &CEntornV3DView::OnUpdateLoxodromaLoxodroma32891)
		ON_COMMAND(ID_LOXODROMA_LOXODROMA32892, &CEntornV3DView::OnLoxodromaLoxodroma32892)
		ON_UPDATE_COMMAND_UI(ID_LOXODROMA_LOXODROMA32892, &CEntornV3DView::OnUpdateLoxodromaLoxodroma32892)
		ON_COMMAND(ID_LOXODROMA_LOXODROMA32893, &CEntornV3DView::OnLoxodromaLoxodroma32893)
		ON_UPDATE_COMMAND_UI(ID_LOXODROMA_LOXODROMA32893, &CEntornV3DView::OnUpdateLoxodromaLoxodroma32893)
		ON_COMMAND(ID_LOXODROMA_LOXODROMA32894, &CEntornV3DView::OnLoxodromaLoxodroma32894)
		ON_UPDATE_COMMAND_UI(ID_LOXODROMA_LOXODROMA32894, &CEntornV3DView::OnUpdateLoxodromaLoxodroma32894)
		ON_COMMAND(ID_LOXODROMA_LOXODROMA32895, &CEntornV3DView::OnLoxodromaLoxodroma32895)
		ON_UPDATE_COMMAND_UI(ID_LOXODROMA_LOXODROMA32895, &CEntornV3DView::OnUpdateLoxodromaLoxodroma32895)
		ON_COMMAND(ID_CORBES_TRIEDREDEDARBOUX, &CEntornV3DView::OnCorbesTriedrededarboux)
		ON_UPDATE_COMMAND_UI(ID_CORBES_TRIEDREDEDARBOUX, &CEntornV3DView::OnUpdateCorbesTriedrededarboux)
		ON_COMMAND(ID_C32897, &CEntornV3DView::OnC32897)
		ON_UPDATE_COMMAND_UI(ID_C32897, &CEntornV3DView::OnUpdateC32897)
		ON_COMMAND(ID_C32898, &CEntornV3DView::OnC32898)
		ON_UPDATE_COMMAND_UI(ID_C32898, &CEntornV3DView::OnUpdateC32898)
		END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Construcción o destrucción de CEntornV3DView

CEntornV3DView::CEntornV3DView()
{
// TODO: agregar aquí el código de construcción
//	int i = 0;

//------ Entorn V3D: Inicialització de les variables globals de CEntornV3DView
	int i;

// Entorn V3D: Variables de control per Menú Càmera: Esfèrica, Navega, Mòbil, Zoom, Satelit, Polars... 
	camera = CAM_ESFERICA;
	mobil = true;	zzoom = true;	zzoomO = false;	 satelit = false;
	//navega = false;

// Entorn V3D: Variables de control per Menú Vista: Pantalla Completa, Pan, dibuixar eixos i grids
	fullscreen = false;
	pan = false;	eixos = true;
	sw_grid = false;
	grid.x = false;	grid.y = false; grid.z = false;		grid.w = false;
	hgrid.x = 0.0;	hgrid.y = 0.0;	hgrid.z = 0.0;		hgrid.w = 0.0;

// Entorn V3D: Variables opció Vista->Pan
	fact_pan = 1;
	tr_cpv.x = 0;	tr_cpv.y = 0;	tr_cpv.z = 0;		tr_cpvF.x = 0;	tr_cpvF.y = 0;	tr_cpvF.z = 0;

// Entorn V3D: Variables de control de l'opció Càmera->Navega?
	n[0] = 0.0;		n[1] = 0.0;		n[2] = 0.0;
	opvN.x = 10.0;	opvN.y = 0.0;		opvN.z = 0.0;
	angleZ = 0.0;

// Entorn V3D: Variables de control de l'opció Càmera->Loxodroma_T*?
	cam_t = 0.0;
	nCam.x = 0.0;		nCam.y = 0.0;		nCam.z = 0.0;
	vCam.x = 10.0;		vCam.y = 0.0;		vCam.z = 1.0;
	animaCamLxd = false;					palante = false;

// Entorn V3D: Variables de control per les opcions de menú Projecció, Objecte
	projeccio = CAP;			objecte = CAP;

// Entorn V3D: Variables de control del menú Transforma
	transf = false;		trasl = false;		rota = false;		escal = false;
	fact_Tras = 1;		fact_Rota = 90;
	TG.VTras.x = 0.0;	TG.VTras.y = 0.0;	TG.VTras.z = 0;	TGF.VTras.x = 0.0;	TGF.VTras.y = 0.0;	TGF.VTras.z = 0;
	TG.VRota.x = 0;		TG.VRota.y = 0;		TG.VRota.z = 0;	TGF.VRota.x = 0;	TGF.VRota.y = 0;	TGF.VRota.z = 0;
	TG.VScal.x = 1;		TG.VScal.y = 1;		TG.VScal.z = 1;	TGF.VScal.x = 1;	TGF.VScal.y = 1;	TGF.VScal.z = 1;

	transX = false;	transY = false;	transZ = false;

// Entorn V3D: Variables de control per les opcions de menú Ocultacions
	front_faces = true;	test_vis = false;	oculta = false;		back_line = false;

// Entorn V3D: Variables de control del menú Iluminació		
	ilumina = FILFERROS;			ifixe = false;					ilum2sides = false;
// Reflexions actives: Ambient [1], Difusa [2] i Especular [3]. No actives: Emission [0]. 
	sw_material[0] = false;			sw_material[1] = true;			sw_material[2] = true;			sw_material[3] = true;
	sw_material_old[0] = false;		sw_material_old[1] = true;		sw_material_old[2] = true;		sw_material_old[3] = true;
	textura = false;				t_textura = CAP;
	for (i = 0; i < NUM_MAX_TEXTURES; i++) texturesID[i] = -1;

// Entorn V3D: Variables de control del menú Llums
// Entorn V3D: Inicialització variables Llums
	llum_ambient = true;
	for (i = 1; i<NUM_MAX_LLUMS; i++) llumGL[i].encesa = false;
	for (i = 0; i<NUM_MAX_LLUMS; i++) {
		llumGL[i].encesa = false;
		llumGL[i].difusa[0] = 1.0f;	llumGL[i].difusa[1] = 1.0f;	llumGL[i].difusa[2] = 1.0f;	llumGL[i].difusa[3] = 1.0f;
		llumGL[i].especular[0] = 1.0f; llumGL[i].especular[1] = 1.0f; llumGL[i].especular[2] = 1.0f; llumGL[i].especular[3] = 1.0f;
	}

// LLum 0: Átenuació constant (c=1), sobre l'eix Z, no restringida.
	llumGL[0].encesa = true;
	llumGL[0].difusa[0] = 1.0f;			llumGL[0].difusa[1] = 1.0f;			llumGL[0].difusa[2] = 1.0f;		llumGL[0].difusa[3] = 1.0f;
	llumGL[0].especular[0] = 1.0f;		llumGL[0].especular[1] = 1.0f;		llumGL[0].especular[2] = 1.0f;	llumGL[0].especular[3] = 1.0f;

	llumGL[0].posicio.R = 75.0;			llumGL[0].posicio.alfa = 90.0;		llumGL[0].posicio.beta = 0.0;		// Posició llum (x,y,z)=(0,0,75)
	llumGL[0].atenuacio.a = 0.0;		llumGL[0].atenuacio.b = 0.0;		llumGL[0].atenuacio.c = 1.0;		// Llum sense atenuació per distància (a,b,c)=(0,0,1)
	llumGL[0].restringida = false;
	llumGL[0].spotdirection[0] = 0.0;	llumGL[0].spotdirection[1] = 0.0;	llumGL[0].spotdirection[2] = 0.0;
	llumGL[0].cutoff = 0.0;				llumGL[0].exponent = 0.0;

// LLum 1: Atenuació constant (c=1), sobre l'eix X, no restringida.
	llumGL[1].encesa = false;
	llumGL[1].difusa[0] = 1.0f;			llumGL[1].difusa[1] = 1.0f;			llumGL[1].difusa[2] = 1.0f;		llumGL[1].difusa[3] = 1.0f;
	llumGL[1].especular[0] = 1.0f;		llumGL[1].especular[1] = 1.0f;		llumGL[1].especular[2] = 1.0f;	llumGL[1].especular[3] = 1;

	llumGL[1].posicio.R = 75.0;			llumGL[1].posicio.alfa = 0.0;		llumGL[1].posicio.beta = 0.0;// (x,y,z)=(75,0,0)
	llumGL[1].atenuacio.a = 0.0;		llumGL[1].atenuacio.b = 0.0;		llumGL[1].atenuacio.c = 1.0;
	llumGL[1].restringida = false;
	llumGL[1].spotdirection[0] = 0.0;	llumGL[1].spotdirection[1] = 0.0;	llumGL[1].spotdirection[2] = 0.0;
	llumGL[1].cutoff = 0.0;				llumGL[1].exponent = 0.0;

// LLum 2: Atenuació constant (c=1), sobre l'eix Y, no restringida.
	llumGL[2].encesa = false;
	llumGL[2].difusa[0] = 1.0f;			llumGL[2].difusa[1] = 1.0f;			llumGL[2].difusa[2] = 1.0f;		llumGL[2].difusa[3] = 1.0f;
	llumGL[2].especular[0] = 1.0f;		llumGL[2].especular[1] = 1.0f;		llumGL[2].especular[2] = 1.0f;	llumGL[2].especular[3] = 1;

	llumGL[2].posicio.R = 75.0;			llumGL[2].posicio.alfa = 0.0;		llumGL[2].posicio.beta = 90.0;	// (x,y,z)=(0,75,0)
	llumGL[2].atenuacio.a = 0.0;		llumGL[2].atenuacio.b = 0.0;		llumGL[2].atenuacio.c = 1.0;
	llumGL[2].restringida = false;
	llumGL[2].spotdirection[0] = 0.0;	llumGL[2].spotdirection[1] = 0.0;	llumGL[2].spotdirection[2] = 0.0;
	llumGL[2].cutoff = 0.0;				llumGL[2].exponent = 0.0;

// LLum 3: Atenuació constant (c=1), sobre l'eix Y=X, no restringida.
	llumGL[3].encesa = false;
	llumGL[3].difusa[0] = 1.0f;			llumGL[2].difusa[1] = 1.0f;			llumGL[3].difusa[2] = 1.0f;		llumGL[3].difusa[3] = 1.0f;
	llumGL[3].especular[0] = 1.0f;		llumGL[2].especular[1] = 1.0f;		llumGL[3].especular[2] = 1.0f;	llumGL[3].especular[3] = 1;

	llumGL[3].posicio.R = 75.0;			llumGL[3].posicio.alfa = 45.0;		llumGL[3].posicio.beta = 45.0;// (x,y,z)=(75,75,75)
	llumGL[3].atenuacio.a = 0.0;		llumGL[3].atenuacio.b = 0.0;		llumGL[3].atenuacio.c = 1.0;
	llumGL[3].restringida = false;
	llumGL[3].spotdirection[0] = 0.0;	llumGL[3].spotdirection[1] = 0.0;	llumGL[3].spotdirection[2] = 0.0;
	llumGL[3].cutoff = 0.0;				llumGL[3].exponent = 0.0;

// LLum 4: Atenuació constant (c=1), sobre l'eix -Z, no restringida.
	llumGL[4].encesa = false;
	llumGL[4].difusa[0] = 1.0f;			llumGL[4].difusa[1] = 1.0f;			llumGL[4].difusa[2] = 1.0f;		llumGL[4].difusa[3] = 1.0f;
	llumGL[4].especular[0] = 1.0f;		llumGL[4].especular[1] = 1.0f;		llumGL[4].especular[2] = 1.0f;	llumGL[4].especular[3] = 1;

	llumGL[4].posicio.R = 75.0;			llumGL[4].posicio.alfa = -90.0;		llumGL[4].posicio.beta = 0.0;// (x,y,z)=(0,0,-75)
	llumGL[4].atenuacio.a = 0.0;		llumGL[4].atenuacio.b = 0.0;		llumGL[4].atenuacio.c = 1.0;
	llumGL[4].restringida = false;
	llumGL[4].spotdirection[0] = 0.0;	llumGL[4].spotdirection[1] = 0.0;	llumGL[4].spotdirection[2] = 0.0;
	llumGL[4].cutoff = 0.0;				llumGL[4].exponent = 0.0;

// LLum #5:
	llumGL[5].encesa = false;
	llumGL[5].difusa[0] = 1.0f;			llumGL[5].difusa[1] = 1.0f;			llumGL[5].difusa[2] = 1.0f;		llumGL[5].difusa[3] = 1.0f;
	llumGL[5].especular[0] = 1.0f;		llumGL[5].especular[1] = 1.0f;		llumGL[5].especular[2] = 1.0f;	llumGL[5].especular[3] = 1;

	llumGL[5].posicio.R = 0.0;			llumGL[5].posicio.alfa = 0.0;		llumGL[5].posicio.beta = 0.0; // Cap posició definida
	llumGL[5].atenuacio.a = 0.0;		llumGL[5].atenuacio.b = 0.0;		llumGL[5].atenuacio.c = 1.0;
	llumGL[5].restringida = false;
	llumGL[5].spotdirection[0] = 0.0;	llumGL[5].spotdirection[1] = 0.0;	llumGL[5].spotdirection[2] = 0.0;
	llumGL[5].cutoff = 0.0;				llumGL[5].exponent = 0.0;

// LLum #6: Llum Vaixell, configurada a la funció vaixell() en escena.cpp.
	llumGL[6].encesa = false;
	llumGL[6].difusa[0] = 1.0f;			llumGL[6].difusa[1] = 1.0f;			llumGL[6].difusa[2] = 1.0f;		llumGL[6].difusa[3] = 1.0f;
	llumGL[6].especular[0] = 1.0f;		llumGL[6].especular[1] = 1.0f;		llumGL[6].especular[2] = 1.0f;	llumGL[6].especular[3] = 1;

	llumGL[6].posicio.R = 0.0;			llumGL[6].posicio.alfa = 0.0;		llumGL[6].posicio.beta = 0.0; // Cap posició definida, definida en funció vaixell() en escena.cpp
	llumGL[6].atenuacio.a = 0.0;		llumGL[6].atenuacio.b = 0.0;		llumGL[6].atenuacio.c = 1.0;
	llumGL[6].restringida = false;
	llumGL[6].spotdirection[0] = 0.0;	llumGL[6].spotdirection[1] = 0.0;	llumGL[6].spotdirection[2] = 0.0;
	llumGL[6].cutoff = 0.0;				llumGL[6].exponent = 0.0;

// LLum #7: Llum Far, configurada a la funció faro() en escena.cpp.
	llumGL[7].encesa = false;
	llumGL[7].difusa[0] = 1.0f;			llumGL[7].difusa[1] = 1.0f;			llumGL[7].difusa[2] = 1.0f;		llumGL[7].difusa[3] = 1.0f;
	llumGL[7].especular[0] = 1.0f;		llumGL[7].especular[1] = 1.0f;		llumGL[7].especular[2] = 1.0f;	llumGL[7].especular[3] = 1;

	llumGL[7].posicio.R = 0.0;			llumGL[7].posicio.alfa = 0.0;		llumGL[7].posicio.beta = 0.0; // Cap posició definida, definida en funció faro() en escena.cpp
	llumGL[7].atenuacio.a = 0.0;		llumGL[7].atenuacio.b = 0.0;		llumGL[7].atenuacio.c = 1.0;
	llumGL[7].restringida = false;
	llumGL[7].spotdirection[0] = 0.0;	llumGL[7].spotdirection[1] = 0.0;	llumGL[7].spotdirection[2] = 0.0;
	llumGL[7].cutoff = 0.0;				llumGL[7].exponent = 0.0;

// Entorn V3D: Variables de control del menú Shaders
	sw_shader = false;				shader_menu = CAP;				shader_program = 0;

// Entorn V3D: Variables de control dels botons de mouse
	m_PosEAvall = (0, 0);		m_PosDAvall = (0, 0);
	m_ButoEAvall = false;		m_ButoDAvall = false;
	m_EsfeEAvall.R = 0.0;		m_EsfeEAvall.alfa = 0.0;	m_EsfeEAvall.beta = 0.0;
	m_EsfeIncEAvall.R = 0.0;	m_EsfeIncEAvall.alfa = 0.0;	m_EsfeIncEAvall.beta = 0.0;

// Entorn V3D: Variables que controlen paràmetres visualització: Mides finestra Windows i PV
	w = 0;				h = 0;								// Mides finestra
	w_old = 0;			h_old = 0;							// Copia mides finestre per a FullScreen
	OPV.R = 15.0;		OPV.alfa = 0.0;		OPV.beta = 0.0;	// Origen PV en esfèriques
	Vis_Polar = POLARZ;

// Entorn V3D: Color de fons i de l'objecte
	fonsR = true;		fonsG = true;		fonsB = true;
	c_fons.r = 0.0;		c_fons.g = 0.0;		c_fons.b = 0.0;
	sw_color = false;
	col_obj.r = 1.0;	col_obj.g = 1.0;	col_obj.b = 1.0;		col_obj.a = 1.0;

// Entorn V3D: Objecte 3DS
	Ob3DS = NULL;

// Entorn V3D: Objecte OBJ
	ObOBJ = NULL;

// V3D: OBJECTE --> Corba B-Spline i Bezier
	npts_T = 0;
	for (i = 0; i < MAX_PATCH_CORBA; i = i++)
	{	PC_t[i].x = 0.0;
		PC_t[i].y = 0.0;
		PC_t[i].z = 0.0;
	}

	pas_CS = PAS_BSPLINE;
	sw_Punts_Control = false;

// TRIEDRE DE FRENET / DARBOUX: VT: vector Tangent, VNP: Vector Normal Principal, VBN: vector BiNormal
	dibuixa_TriedreFrenet = false;		dibuixa_TriedreDarboux = false;
	VT = { 0.0, 0.0, 1.0 };		VNP = { 1.0, 0.0, 0.0 };	VBN = { 0.0, 1.0, 0.0 };

// Entorn V3D: Variables de l'objecte FRACTAL
	t_fractal = CAP;	soroll = 'C';
	pas = 64;			pas_ini = 64;
	sw_il = true;		palcolFractal = false;

// Entorn V3D: Variables del Timer


// Entorn V3D. QUATERNIONS:  Animació de cossos rígids per QUATERNIONS. Inicialització de variables
	rotaQ = false;		animaQ = false;		np_InQ = 0;	
	for (i = 0; i < MAX_KEYFRAMES_Q; i++)
		{	keyf_Rota[i].x = 0;	keyf_Rota[i].y = 0;	keyf_Rota[i].z = 0;
			keyf_Tras[i].x = 0;	keyf_Tras[i].y = 0;	keyf_Tras[i].z = 0;
			keyf_Quat[i].w = 0.0f;	keyf_Quat[i].y = 0.0f;	keyf_Quat[i].y = 0.0f; keyf_Quat[i].z = 0.0f;
		}
	int_Tras.x = 0.0f;	int_Tras.y = 0;			int_Tras.z = 0.0f;
	qI.w = 1;			qI.x = 0;				qI.y = 0;				qI.z = 0;
	qF.w = 1;			qF.x = 0;				qF.y = 0;				qF.z = 0;
	qT.w = 1;			qT.x = 0;				qT.y = 0;				qT.z = 0;
	for (i = 0; i < 16; i++) QMatrix[i] = 0;	QMatrix[0] = QMatrix[5] = QMatrix[10] = QMatrix[15] = 1.0;
//	keyf_Tras[0].x = 10.0f;		keyf_Tras[0].y = -20.0f;	keyf_Tras[0].z = 30.0f;
//	keyf_Tras[1].x = 0.0f;		keyf_Tras[1].y = 0.0f;		keyf_Tras[1].z = 0.0f;
//	keyf_Tras[2].x = -10.0f;	keyf_Tras[2].y = 20.0f;		keyf_Tras[2].z = 30.0f;

/* Entorn V3D.ESCENA: Inicialització variables animació ESCENA
	t = 0;					delta = 0.0f;		animaE = false;
	for (i = 0; i < 3; i++) 
		{	ctr[i].x = 0.0;		ctr[i].y = 0.0f;	ctr[i].z = 0.0f;	ctr[i].w = 0.0f;
		}
	patch = 0;
	VT.x = 0.0f;	VT.y = 0.0f;	VT.z = 0.0f;	VT.w = 0.0f;	// Vector Tangent del Triedre de Frenet
	VBN.x = 0.0f;	VBN.y = 0.0f;	VBN.z = 0.0f;	VBN.w = 0.0f;	// Vector Binormal del Triedre de Frenet
	VNP.x = 0.0f;	VNP.y = 0.0f;	VNP.z = 0.0f;	VNP.w = 0.0f;	// Vector Normal del Triedre de Frenet
	pos_traject.x = 0.0f;	pos_traject.y = 0.0f;	pos_traject.z = 0.0f;	pos_traject.w = 1.0f;	// Posició (x,y,z) de la trajectòria
	angle_traject.x = 0.0f;	angle_traject.y = 0.0f;	angle_traject.z = 0.0f;	angle_traject.w = 1.0f;	// Orientació objecte en Angles d'Euler
*/

// Entorn V3D: Altres variables
	mida = 1.0;			nom = "";		buffer = "";

// Entorn V3D: Inicialització de les llibreries DevIL per a la càrrega de textures i fitxers .3DS
	ilInit();					// Inicialitzar llibreria IL
	iluInit();					// Inicialitzar llibreria ILU
	ilutRenderer(ILUT_OPENGL);	// Inicialitzar llibreria ILUT per a OpenGL

// Entorn V3D: Definir desplegable per a Full Screen
	ContextMenu = new CMenu();
	if (!ContextMenu->LoadMenu(IDR_MENU_WINDOW))	AfxMessageBox(_T("Fail to create context menu"));
}

CEntornV3DView::~CEntornV3DView()
{
}

BOOL CEntornV3DView::PreCreateWindow(CREATESTRUCT& cs)
{
// TODO: modificar aquí la clase Window o los estilos cambiando
//  CREATESTRUCT cs

// An OpenGL window must be created with the following flags and must not
// include CS_PARENTDC for the class style.
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
// Impresión de CEntornV3DView

void CEntornV3DView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CEntornV3DView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Preparación predeterminada
	return DoPreparePrinting(pInfo);
}

void CEntornV3DView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
// TODO: agregar inicialización adicional antes de imprimir
}

void CEntornV3DView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
// TODO: agregar limpieza después de imprimir
}

void CEntornV3DView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// Diagnósticos de CEntornV3DView

#ifdef _DEBUG
void CEntornV3DView::AssertValid() const
{
	CView::AssertValid();
}

void CEntornV3DView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEntornV3DDoc* CEntornV3DView::GetDocument() const // La versión de no depuración está alineada
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEntornV3DDoc)));
	return (CEntornV3DDoc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// Controladores de mensaje de CEntornV3DView

int CEntornV3DView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

// TODO:  Agregue aquí su código de creación especializado

	//CDC* pDC = GetDC();
	//m_glRenderer.CreateGLContext(pDC);
	m_pDC = new CClientDC(this);

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

// Entorn V3D: Format del pixel que millor s'ajusti al descrit en pfd
	//int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	int nPixelFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);

	if (!nPixelFormat)
	{	::MessageBoxW(NULL, L"Error en SetPixelFormat", L"Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);		// This sends a message telling the program to quit
		return false;
	}

// Activació format pixel per al contexte dispositiu
	//BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);
	BOOL bResult = SetPixelFormat(m_pDC->GetSafeHdc(), nPixelFormat, &pfd);

	if (!bResult) 
	{	::MessageBoxW(NULL, L"Error en SetPixelFormat", L"Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);		// This sends a message telling the program to quit
		return false;
	}

// Entorn V3D: Creació contexte generació OpenGL
	// m_hRC=wglCreateContext(m_hDC);
	m_hRC = wglCreateContext(m_pDC->GetSafeHdc());
	if (!m_hRC)
	{	::MessageBoxW(NULL, L"Error en GL Rendering Context", L"Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);		// This sends a message telling the program to quit
		return false;
	}

// OPENGL 2.0 - OPENGL 3.0 **********************************************************
	// --- OpenGL 3.x ---
	HGLRC tempContext = wglCreateContext(m_pDC->GetSafeHdc());
	wglMakeCurrent(m_pDC->GetSafeHdc(), tempContext);

//Get access to modern OpenGL functionality from this old style context.
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		AfxMessageBox(_T("GLEW could not be initialized!"));
		return FALSE;
	}

//InitAPI();

//	if (glewIsExtensionSupported("GLEW_ARB_vertex_shader"))
//	AfxMessageBox(_T("INFO: GLEW_ARB_vertex_shader supported, proceeding\n"));
//	else AfxMessageBox(_T("INFO: GLEW_ARB_vertex_shader NOT supported, proceeding\n"));
//
//	if (glewIsExtensionSupported("GLEW_ARB_fragment_shader"))
//	AfxMessageBox(_T("INFO: GLEW_ARB_fragment_shader supported, proceeding\n"));
//	else AfxMessageBox(_T("INFO: GLEW_ARB_fragment_shader NOT supported, proceeding\n"));
//
	if (glewIsSupported("GL_VERSION_2_0")) //(GLEW_VERSION_2_0)
	{	} //AfxMessageBox(_T("INFO: OpenGL 2.0 supported!. Proceed\n"));
	else
	{
		AfxMessageBox(_T("INFO: OpenGL 2.0 not supported!. Exit\n"));
		//return EXIT_FAILURE;
	}

	
	int major, minor;
	GetGLVersion(&major, &minor);

	if (major < 3 || (major == 3 && minor < 2))
		AfxMessageBox(_T("OpenGL 3.2 is not supported!. Exit\n"));
//	else 
//		AfxMessageBox(_T("OpenGL 3.2 is supported!. Proceed"));

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, major,
		WGL_CONTEXT_MINOR_VERSION_ARB, minor,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,// | WGL_CONTEXT_DEBUG_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB, //0x9126 , 0x00000001,
		0
	};

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	if (wglCreateContextAttribsARB != NULL)
	{
		m_hrc = wglCreateContextAttribsARB(m_pDC->GetSafeHdc(), 0, attribs);
	}

	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);

// Entorn V3D: Inicialització de funcions de shaders per a OpenGL 2.0
	InitAPI();

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(tempContext);

	if (!m_hrc)
	{
		AfxMessageBox(_T("OpenGL 3.x RC was not created!"));
		return false;
	}

// Permet la coexistencia d'altres contextes de generació
	wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);

	return true;
}

void CEntornV3DView::InitAPI()
{
/*	Vendor, Renderer, Version, Shading Laguage Version i Extensions suportades per la placa gràfica gravades en fitxer extensions.txt
	CString nomf;
	nomf = "extensions.txt";
	char *nomfitxer = CString2Char(nomf);
	char* str = (char*)glGetString(GL_VENDOR);
	FILE* f = fopen(nomfitxer, "w");
	if(f)	{	fprintf(f,"VENDOR: %s\n",str);
				str = (char*)glGetString(GL_RENDERER);
				fprintf(f, "RENDERER: %s\n", str);
				str = (char*)glGetString(GL_VERSION);
				fprintf(f, "VERSION: %s\n", str);
				str = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
				fprintf(f, "SHADING_LANGUAGE_VERSION: %s\n", str);
				str = (char*)glGetString(GL_EXTENSIONS);
				fprintf(f, "EXTENSIONS: %s\n", str);
				fclose(f);
			}
	*/

// Program
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	glUniform1iv = (PFNGLUNIFORM1IVPROC)wglGetProcAddress("glUniform1iv");
	glUniform2iv = (PFNGLUNIFORM2IVPROC)wglGetProcAddress("glUniform2iv");
	glUniform3iv = (PFNGLUNIFORM3IVPROC)wglGetProcAddress("glUniform3iv");
	glUniform4iv = (PFNGLUNIFORM4IVPROC)wglGetProcAddress("glUniform4iv");
	glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
	glUniform1fv = (PFNGLUNIFORM1FVPROC)wglGetProcAddress("glUniform1fv");
	glUniform2fv = (PFNGLUNIFORM2FVPROC)wglGetProcAddress("glUniform2fv");
	glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
	glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
	glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)wglGetProcAddress("glVertexAttrib1f");
	glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC)wglGetProcAddress("glVertexAttrib1fv");
	glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC)wglGetProcAddress("glVertexAttrib2fv");
	glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC)wglGetProcAddress("glVertexAttrib3fv");
	glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)wglGetProcAddress("glVertexAttrib4fv");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
	glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)wglGetProcAddress("glGetActiveUniform");

	// Shader
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");

	// VBO
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
}

void CEntornV3DView::GetGLVersion(int* major, int* minor)
{
	// for all versions
	char* ver = (char*)glGetString(GL_VERSION); // ver = "3.2.0"

	*major = ver[0] - '0';
	if (*major >= 3)
	{
		// for GL 3.x
		glGetIntegerv(GL_MAJOR_VERSION, major);		// major = 3
		glGetIntegerv(GL_MINOR_VERSION, minor);		// minor = 2
	}
	else
	{
		*minor = ver[2] - '0';
	}

	// GLSL
	ver = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);	// 1.50 NVIDIA via Cg compiler
}


void CEntornV3DView::OnDestroy()
{
	CView::OnDestroy();

// TODO: Agregue aquí su código de controlador de mensajes
	CDC* pDC = GetDC();
	ReleaseDC(pDC);
}


void CEntornV3DView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

// TODO: Agregue aquí su código de controlador de mensajes

// A resize event occured; cx and cy are the window's new width and height.
// Find the OpenGL change size function given in the Lab 1 notes and call it here

// Entorn V3D: MODIFICACIÓ ->Establim les mides de la finestra actual
	w = cx;
	h = cy;

}


void CEntornV3DView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	//m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}


/////////////////////////////////////////////////////////////////////////////
// Dibujo de CEntornV3DView

void CEntornV3DView::OnDraw(CDC* /*pDC*/)
{

// TODO: agregar aquí el código de dibujo para datos nativos

	CEntornV3DDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}


void CEntornV3DView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
// TODO: Agregue aquí su código de controlador de mensajes
	GLdouble vpv[3] = { 0.0, 0.0, 1.0 };

// Entorn V3D: Activació el contexte OpenGL
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

// Cridem a les funcions de l'escena i la projecció segons s'hagi 
// seleccionat una projecció o un altra
	switch (projeccio)
	{
	case AXONOM:
// PROJECCIÓ AXONOMÈTRICA
// Activació del retall de pantalla
		glEnable(GL_SCISSOR_TEST);

// Retall
		glScissor(0, 0, w, h);
		glViewport(0, 0, w, h);

		// Fons condicionat al color de fons
		if ((c_fons.r < 0.5) || (c_fons.g < 0.5) || (c_fons.b < 0.5))
			FonsB();
		else
			FonsN();

// Aquí farem les crides per a definir Viewport, Projecció i Càmara: INICI -------------------------

		Projeccio_Orto(0, 0, w, h, OPV.R);
		Vista_Esferica(OPV, Vis_Polar, pan, tr_cpv, tr_cpvF, c_fons, col_obj, objecte, mida, pas,
			front_faces, oculta, test_vis, back_line,
			ilumina, llum_ambient, llumGL, ifixe, ilum2sides, eixos, grid, hgrid);



// Aquí farem les cridesper a definir Viewport, Projecció i Càmara:: FI -------------------------
		// Dibuixar Model (escena)
		glPushMatrix();
			configura_Escena();     // Aplicar Transformacions Geometriques segons persiana Transformacio i configurar objectes
			//glScalef();			// Escalat d'objectes, per adequar-los a les vistes ortogràfiques
			dibuixa_Escena();		// Dibuix geometria de l'escena amb comandes GL.
		glPopMatrix();

// Intercanvia l'escena al front de la pantalla
		SwapBuffers(m_pDC->GetSafeHdc());
		break;

	case ORTO:
// PROJECCIÓ ORTOGRÀFICA
// Activació del retall de pantalla
		glEnable(GL_SCISSOR_TEST);

// Retall
		glScissor(0, 0, w, h);
		glViewport(0, 0, w, h);
		

// Fons condicionat al color de fons
		if ((c_fons.r < 0.5) || (c_fons.g < 0.5) || (c_fons.b<0.5))
			FonsB();
		else
			FonsN();

// Aquí farem les quatre crides a ProjeccioOrto i Ortografica per obtenir 
// les quatre vistes ortogràfiques
// ---------- Entorn V3D: DESCOMENTAR QUAN S'IMPLEMENTI PROJECCIO ORTOGRÀFICA

// PLANTA (Inferior Esquerra)
		

		// Definició de Viewport, Projecció i Càmara
		Projeccio_Orto(0, 0, w/2, h/2, OPV.R);
		

		Vista_Ortografica(0, OPV.R, c_fons, col_obj, objecte, mida, pas, front_faces, oculta,
			test_vis, back_line, ilumina, llum_ambient, llumGL, ifixe, ilum2sides, eixos, grid, hgrid);
		// Dibuix de l'Objecte o l'Escena
		glPushMatrix();
			configura_Escena();     // Aplicar Transformacions Geometriques segons persiana Transformacio i configurar objectes
			// glScalef();			// Escalat d'objectes, per adequar-los a les vistes ortogràfiques (Pràctica 2)
			dibuixa_Escena();		// Dibuix geometria de l'escena amb comandes GL.
		glPopMatrix();

// ISOMÈTRICA (Inferior Dreta)
		// Definició de Viewport, Projecció i Càmara
		Projeccio_Orto(w/2+1, 0, w/2, h/2, OPV.R);
		Vista_Ortografica(3, OPV.R, c_fons, col_obj, objecte, mida, pas, front_faces, oculta,
			test_vis, back_line, ilumina, llum_ambient, llumGL, ifixe, ilum2sides,eixos, grid, hgrid);
		// Dibuix de l'Objecte o l'Escena
		glPushMatrix();
			configura_Escena();     // Aplicar Transformacions Geometriques segons persiana Transformacio i configurar objectes
			// glScalef();			// Escalat d'objectes, per adequar-los a les vistes ortogràfiques (Pràctica 2)
			dibuixa_Escena();		// Dibuix geometria de l'escena amb comandes GL.
		glPopMatrix();

// ALÇAT (Superior Esquerra)
		// Definició de Viewport, Projecció i Càmara
		
		Projeccio_Orto(0, h/2+1, w/2, h/2, OPV.R);
		Vista_Ortografica(1, OPV.R, c_fons, col_obj, objecte, mida, pas, front_faces, oculta,
			test_vis, back_line, ilumina, llum_ambient, llumGL, ifixe, ilum2sides, eixos, grid, hgrid);
		// Dibuix de l'Objecte o l'Escena
		glPushMatrix();
			configura_Escena();     // Aplicar Transformacions Geom?triques segons persiana Transformacio i configurar objectes
			//glScalef();			// Escalat d'objectes, per adequar-los a les vistes ortogràfiques (Pràctica 2)
			dibuixa_Escena();		// Dibuix geometria de l'escena amb comandes GL.
		glPopMatrix();

// PERFIL (Superior Dreta)
		// Definició de Viewport, Projecció i Càmara
		Projeccio_Orto(w/2+1, h/2+1, w/2, h/2, OPV.R);
		Vista_Ortografica(2, OPV.R, c_fons, col_obj, objecte, mida, pas, front_faces, oculta,
			test_vis, back_line, ilumina, llum_ambient, llumGL, ifixe, ilum2sides, eixos, grid, hgrid);
		// Dibuix de l'Objecte o l'Escena
		glPushMatrix();
			configura_Escena();     // Aplicar Transformacions Geom?triques segons persiana Transformacio i configurar objectes
 			// glScalef();			// Escalat d'objectes, per adequar-los a les vistes ortogràfiques (Pràctica 2)
			dibuixa_Escena();		// Dibuix geometria de l'escena amb comandes GL.
		glPopMatrix();
		
// Intercanvia l'escena al front de la pantalla
		SwapBuffers(m_pDC->GetSafeHdc());
		break;

	case PERSPECT:
// PROJECCIÓ PERSPECTIVA
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Set Perspective Calculations To Most Accurate
		glDisable(GL_SCISSOR_TEST);		// Desactivació del retall de pantalla

		// Definició de Viewport, Projecció i Càmara
		Projeccio_Perspectiva(0, 0, w, h, OPV.R);
		if (camera==CAM_NAVEGA)	{	Vista_Navega(opvN, n, vpv, pan, tr_cpv, tr_cpvF, c_fons, col_obj, objecte, true, pas,
											front_faces, oculta, test_vis, back_line, 
											ilumina, llum_ambient, llumGL, ifixe, ilum2sides, eixos, grid, hgrid);
					}
		else if (camera == CAM_LOXODROMA_TDARBOUX || camera == CAM_LOXODROMA_TFRENET) {
			// Càmera LOXODROMA: Definició de la càmera segons el Triedre definit
			GLdouble otramanera[3];
			otramanera[0] = vCam.x;
			otramanera[1] = vCam.y;
			otramanera[2] = vCam.z;

			GLdouble igual[3] = {0,0,0};
			
			Vista_Navega(opvN, igual, otramanera, pan, tr_cpv, tr_cpvF, c_fons, col_obj, objecte, true, pas,
				front_faces, oculta, test_vis, back_line,
				ilumina, llum_ambient, llumGL, ifixe, ilum2sides, eixos, grid, hgrid);

		}
		else {	n[0] = 0;		n[1] = 0;		n[2] = 0;
				Vista_Esferica(OPV, Vis_Polar, pan, tr_cpv, tr_cpvF, c_fons, col_obj, objecte, mida, pas,
					front_faces, oculta, test_vis, back_line, 
					ilumina, llum_ambient, llumGL, ifixe, ilum2sides, eixos, grid, hgrid);
			}

		// Dibuix de l'Objecte o l'Escena
		glPushMatrix();
			configura_Escena();     // Aplicar Transformacions Geometriques segons persiana Transformacio i configurar objectes.
			dibuixa_Escena();		// Dibuix geometria de l'escena amb comandes GL.
		glPopMatrix();

// Intercanvia l'escena al front de la pantalla
		SwapBuffers(m_pDC->GetSafeHdc());
		break;

/* Entorn V3D.ESCENA: PASSEJADA VIRTUAL Per a l'objecte ESCENA
	case PASSEIG_V:
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Set Perspective Calculations To Most Accurate
		// Activació del retall de pantalla
		glEnable(GL_SCISSOR_TEST);

		// Retall
		glScissor(0, 0, w, h);
		glViewport(0, 0, w, h);

	// Fons condicionat al color de fons
		if ((c_fons.r < 0.5) || (c_fons.g < 0.5) || (c_fons.b < 0.5))	FonsB();
		else FonsN();

	// Definició de Viewport, Projecció i Càmara i geometria VIEWPORT ESQUERRA
		Projeccio_Perspectiva(0, 0, w / 2, h, OPV.R);
		n[0] = 0;		n[1] = 0;		n[2] = 0;
		Vista_Esferica(OPV, Vis_Polar, pan, tr_cpv, tr_cpvF, c_fons, col_obj, objecte, mida, pas,
			front_faces, oculta, test_vis, back_line, ilumina, llum_ambient, llumGL,
			ifixe, ilum2sides, eixos, grid, hgrid);

	// Dibuix de l'Objecte o l'Escena
		glPushMatrix();
		configura_Escena();     // Aplicar Transformacions Geometriques segons persiana Transformacio i configurar objectes.
		dibuixa_Escena();		// Dibuix geometria de l'escena amb comandes GL.
		glPopMatrix();

	// Definició de Viewport, Projecció i Càmara i geometria VIEWPORT DRET
		Projeccio_Perspectiva(w / 2 + 1, 0, w / 2, h, OPV.R);
// Entorn V3D. ESCENA: Crida a la funció de càmera amb els tres primers paràmetres corresponents als paràmetres de gluLookAt().
//						CAL FER: Definir els tres primers paràmetres: <ORIGEN CAMERA>, <PUNT ON MIRA>, <VECTOR V>.
//		Vista_Navega(<ORIGEN CAMERA>, <PUNT ON MIRA>, <VECTOR V>, pan, tr_cpv, tr_cpvF, c_fons, col_obj, objecte, true, pas,
//			front_faces, oculta, test_vis, back_line, ilumina, llum_ambient, llumGL,
//			ifixe, ilum2sides, eixos, grid, hgrid);

// Dibuix de l'Objecte o l'Escena
		glPushMatrix();
		configura_Escena();     // Aplicar Transformacions Geometriques segons persiana Transformacio i configurar objectes.
		// Dibuix de l'escena amb el l'objecte semitransparent (darrer paràmetre a true).
		dibuixa_EscenaGL(objecte, col_obj, true, sw_material, textura, texturesID,
			npts_T, PC_t, pas_CS, sw_Punts_Control, true, VT, VNP, VBN, pos_traject, angle_traject, QMatrix);
		glPopMatrix();

// Intercanvia l'escena al front de la pantalla
		SwapBuffers(m_pDC->GetSafeHdc());
		break;

*/

	default:
// Entorn V3D: Creació de la llista que dibuixarà els eixos Coordenades Món. Funció on està codi per dibuixar eixos	
		glNewList(EIXOS, GL_COMPILE);
		  // Dibuix dels eixos sense il.luminació
		  glDisable(GL_LIGHTING);
		  // Dibuixar eixos sense textures
		  glDisable(GL_TEXTURE_2D);
		  // Desactivar boira
		  glDisable(GL_FOG);
		  deixos();						// Funció que dibuixa els Eixos Coordenades Món
		  if (ilumina != FILFERROS) glEnable(GL_LIGHTING);
		  if (textura) glEnable(GL_TEXTURE_2D);
		glEndList();

// Crida a la funció Fons Blanc
		FonsB();

// Intercanvia l'escena al front de la pantalla
		SwapBuffers(m_pDC->GetSafeHdc());
		break;
}

// Entorn V3D: Activació el contexte OpenGL. Permet la coexistencia d'altres contextes de generació
	wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);

//  Actualitzar la barra d'estat de l'aplicació amb els valors R,A,B,PVx,PVy,PVz
	Barra_Estat();
}


// configura_Escena: Funcio que configura els parametres de Model i dibuixa les
//                   primitives OpenGL dins classe Model
void CEntornV3DView::configura_Escena() {

// Aplicar Transformacions Geometriques segons persiana Transformacio i Quaternions
	instancia(transf, TG, TGF, rotaQ, animaQ, QMatrix, int_Tras);
}

// dibuixa_Escena: Funcio que crida al dibuix dels diferents elements de l'escana
void CEntornV3DView::dibuixa_Escena() {

//	Dibuix geometria de l'escena amb comandes GL.
	dibuixa_EscenaGL(objecte, col_obj, true, sw_material, textura, texturesID,
		npts_T, PC_t, pas_CS, sw_Punts_Control, QMatrix, 
		dibuixa_TriedreFrenet, dibuixa_TriedreDarboux, croata);
}

// Barra_Estat: Actualitza la barra d'estat (Status Bar) de l'aplicació amb els
//      valors R,A,B,PVx,PVy,PVz en Visualització Interactiva.
void CEntornV3DView::Barra_Estat()
{
	CString sss;
	CEsfe3D OPVAux;
	float PVx, PVy, PVz;
	//GLfloat color;

// Status Bar fitxer fractal
	if (nom != "") GetStatusBar().SetPaneText(0, nom);

// Càlcul dels valors per l'opció Vista->Navega
	if (projeccio != CAP && projeccio != ORTO) {
		if (camera == CAM_ESFERICA)
		{
			OPVAux.R = sqrt(opvN.x*opvN.x + opvN.y*opvN.y + opvN.z*opvN.z);
			OPVAux.alfa = (asin(opvN.z / OPVAux.R) * 180) / pi;
			OPVAux.beta = (atan(opvN.y / opvN.x)) * 180 / pi;
		}
		else {
			OPVAux.R = OPV.R; OPVAux.alfa = OPV.alfa; OPVAux.beta = OPV.beta; //Raux=R; angv=anglev; angh=angleh; 
		}
	}
	else {
		OPVAux.R = OPV.R; OPVAux.alfa = OPV.alfa; OPVAux.beta = OPV.beta; //Raux=R; angv=anglev; angh=angleh; 
	}

// Status Bar R Origen Punt de Vista
	if (projeccio == CAP) buffer = "       ";	
		else if (projeccio==ORTO) buffer=" ORTO   ";
			else if (camera == CAM_NAVEGA) buffer = " NAV   ";
			else buffer.Format(_T("%.1f"), OPVAux.R);
	sss = _T("R=") + buffer;
// Refrescar posició R Status Bar
	GetStatusBar().SetPaneText(1, sss);

// Status Bar angle alfa Origen Punt de Vista
	if (projeccio == CAP) buffer = "       ";
		else if (projeccio==ORTO) buffer="ORTO   ";
			else if (camera == CAM_NAVEGA) buffer = " NAV   ";
				else buffer.Format(_T("%.1f"), OPVAux.alfa);
	sss = _T("a=") + buffer;
// Refrescar posició angleh Status Bar
	GetStatusBar().SetPaneText(2, sss);

// Status Bar angle beta Origen Punt de Vista
	if (projeccio == CAP) buffer = "       ";
		else if (projeccio==ORTO) buffer="ORTO   ";
			else if (camera == CAM_NAVEGA) buffer = " NAV   ";
				else buffer.Format(_T("%.1f"), OPVAux.beta);
	sss = _T("ß=") + buffer;
// Refrescar posició anglev Status Bar
	GetStatusBar().SetPaneText(3, sss);

// Transformació PV de Coord. esfèriques (R,anglev,angleh) --> Coord. cartesianes (PVx,PVy,PVz)
	if (camera == CAM_NAVEGA) { PVx = opvN.x; PVy = opvN.y; PVz = opvN.z; }
	else {
		if (Vis_Polar == POLARZ) {
			PVx = OPVAux.R*cos(OPVAux.beta*pi / 180)*cos(OPVAux.alfa*pi / 180);
			PVy = OPVAux.R*sin(OPVAux.beta*pi / 180)*cos(OPVAux.alfa*pi / 180);
			PVz = OPVAux.R*sin(OPVAux.alfa*pi / 180);
		}
		else if (Vis_Polar == POLARY) {
			PVx = OPVAux.R*sin(OPVAux.beta*pi / 180)*cos(OPVAux.alfa*pi / 180);
			PVy = OPVAux.R*sin(OPVAux.alfa*pi / 180);
			PVz = OPVAux.R*cos(OPVAux.beta*pi / 180)*cos(OPVAux.alfa*pi / 180);
		}
		else {
			PVx = OPVAux.R*sin(OPVAux.alfa*pi / 180);
			PVy = OPVAux.R*cos(OPVAux.beta*pi / 180)*cos(OPVAux.alfa*pi / 180);
			PVz = OPVAux.R*sin(OPVAux.beta*pi / 180)*cos(OPVAux.alfa*pi / 180);
		}
	}

// Status Bar PVx
	if (projeccio == CAP) buffer = "       ";
		else if (pan) float2CString(tr_cpv.x);
			else float2CString(PVx);
	sss = _T("PVx=") + buffer;
// Refrescar posició PVx Status Bar
	GetStatusBar().SetPaneText(4, sss);

// Status Bar PVy
	if (projeccio == CAP) buffer = "       ";
		else if (pan) float2CString(tr_cpv.y);
			else float2CString(PVy);
	sss = _T("PVy=") + buffer;
// Refrescar posició PVy Status Bar
	GetStatusBar().SetPaneText(5, sss);

// Status Bar PVz
	if (projeccio == CAP) buffer = "       ";
		else if (pan) float2CString(tr_cpv.z);
			else float2CString(PVz);
	sss = _T("PVz=") + buffer;
// Refrescar posició PVz Status Bar
	GetStatusBar().SetPaneText(6, sss);

// Status Bar per indicar el modus de canvi de color (FONS o OBJECTE)
	sss = " ";
	if (sw_grid) sss = "GRID";
		else if (pan) sss = " PAN";
			else if (sw_color) sss = " OBJ";
					// EntornV3D. QUATERNIONS: Displaiar el missatge 'QTN' en Status Bar
					else if (transf) {	if (rota) { if (rotaQ) sss = "QTN"; else sss = "ROT"; } //sss = "ROT";
											else if (trasl) sss = "TRA";
												else if (escal) sss = "ESC";
									}
							else sss = "FONS";

// Refrescar posició Transformacions en Status Bar
	GetStatusBar().SetPaneText(7, sss);

// Status Bar per indicar tipus de Transformació (TRAS, ROT, ESC)
	sss = " ";
	if (transf) {	// EntornV3D. QUATERNIONS: Displaiar el valor w del Quaternió en Status Bar
					if (rotaQ) {	buffer.Format(_T("%.2f"), qT.w);
									sss = buffer + _T("  ");
								}
				}
	else {
		// Components d'intensitat de fons que varien per teclat
		if ((fonsR) && (fonsG) && (fonsB)) sss = " RGB";
		else if ((fonsR) && (fonsG)) sss = " RG ";
		else if ((fonsR) && (fonsB)) sss = " R   B";
		else if ((fonsG) && (fonsB)) sss = "   GB";
		else if (fonsR) sss = " R  ";
		else if (fonsG) sss = "   G ";
		else if (fonsB) sss = "      B";
	}
// Refrescar posició Transformacions en Status Bar
	GetStatusBar().SetPaneText(8, sss);

// Status Bar dels paràmetres de Transformació, Color i posicions de Robot i Cama
	sss = " ";
	if (transf)
	{	if (rota)
		{	if (rotaQ) // Entorn V3D. QUATERNIONS: Visualitzar el quaternió (x,y,z) o angles d'Euler equivalents
				{	buffer.Format(_T("%.2f"), qT.x);
					sss = buffer + _T("  ");

					buffer.Format(_T("%.2f"), qT.y);
					sss = sss + buffer + _T("  ");

					buffer.Format(_T("%.2f"), qT.z);
					sss = sss + buffer;
				}
				else {	buffer.Format(_T("%.2f"), TG.VRota.x);
						sss = buffer + _T("  ");

						buffer.Format(_T("%.2f"), TG.VRota.y);
						sss = sss + buffer + _T("  ");

						buffer.Format(_T("%.2f"), TG.VRota.z);
						sss = sss + buffer;
					}
		}
		else if (trasl)
		{	buffer.Format(_T("%.2f"), TG.VTras.x);
			sss = _T("  ") + buffer + _T("   ");

			buffer.Format(_T("%.2f"), TG.VTras.y);
			sss = sss + buffer + _T("   ");

			buffer.Format(_T("%.2f"), TG.VTras.z);
			sss = sss + buffer;
		}
		else if (escal)
		{	buffer.Format(_T("%.2f"), TG.VScal.x);
			sss = _T(" ") + buffer + _T("  ");

			buffer.Format(_T("%.2f"), TG.VScal.y);
			sss = sss + buffer + _T("  ");

			buffer.Format(_T("%.2f"), TG.VScal.x);
			sss = sss + buffer;
		}
	}
	else {	// Color fons
		if (!sw_color)
		{	buffer.Format(_T("%.3f"), c_fons.r);
			sss = _T(" ") + buffer + _T("  ");

			buffer.Format(_T("%.3f"), c_fons.g);
			sss = sss + buffer + _T("  ");

			buffer.Format(_T("%.3f"), c_fons.b);
			sss = sss + buffer;
		}
		else
		{	// Color objecte
			buffer.Format(_T("%.3f"), col_obj.r);
			sss = _T(" ") + buffer + _T("  ");

			buffer.Format(_T("%.3f"), col_obj.g);
			sss = sss + buffer + _T("  ");

			buffer.Format(_T("%.3f"), col_obj.b);
			sss = sss + buffer;
		}
	}

// Refrescar posició PVz Status Bar
	GetStatusBar().SetPaneText(9, sss);

// Status Bar per indicar el pas del Fractal
	if (objecte == O_FRACTAL)
		{	buffer.Format(_T("%.1d"), pas);
			sss = _T("Pas=") + buffer;
		}
// Entorn V3D. QUATERNIONS: Status Bar pels Keyframes de l'animació d'objectes rígids per QUATERNIONS
		else if (rotaQ) {	if (np_InQ == 1) sss = " KEYFQ 1";
								else if (np_InQ == 2) sss = " KEYFQ 2";
									else if (np_InQ == 3) sss = " KEYFQ 3";
										else sss = "       ";
						}
				else { sss = "          "; }

// Refrescar posició Transformacions en Status Bar
	GetStatusBar().SetPaneText(10, sss);
}

void CEntornV3DView::float2CString(float varf)
{
	float vfloat;
	vfloat = varf;
	if (abs(varf)<1.0) buffer.Format(_T("%.5f"), varf);
	else if (abs(varf)<99.0) buffer.Format(_T("%.4f"), varf);
	else if (abs(varf)<999.0) buffer.Format(_T("%.3f"), varf);
	else if (abs(varf)<9999.0) buffer.Format(_T("%.2f"), varf);
	else if (abs(varf)<99999.0) buffer.Format(_T("%.1f"), varf);
	else buffer.Format(_T("%.0f"), varf);

}


/* ------------------------------------------------------------------------- */
/*                                  SHADERS                                  */
/* ------------------------------------------------------------------------- */

/*****************************************************************************
* initializeShaders()
*      Loads custom shader by specifying filename (expects frag/vert pair)
*****************************************************************************/
GLuint CEntornV3DView::initializeShaders(CString filename)
{
	GLuint v, f, p;
	char *vs = NULL, *fs = NULL;
	CString file_Vert, file_Frag;
	int info_log_length = 0, compile_result = 0;
	FILE *fd;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

// Llegir Vertex Shader <filename>.vert
	file_Vert = filename + _T(".vert");
	char* fitxer_Vert = CString2Char(file_Vert);
	vs = textFileRead(fitxer_Vert);

// Llegir Fragment Shader <filename>.vert
	file_Frag = filename + _T(".frag");
	char* fitxer_Frag = CString2Char(file_Frag);
	fs = textFileRead(fitxer_Frag);

// Si s'han trobats els fitxers, procedir.
	if ((vs != NULL) && (fs != NULL))
	{
		const char * vv = vs;
		const char * ff = fs;

		glShaderSource(v, 1, &vv, NULL);
		glShaderSource(f, 1, &ff, NULL);

		free(vs);
		free(fs);
		glCompileShader(v);
		glGetShaderiv(v, GL_COMPILE_STATUS, &compile_result);
// Llista error de compilació del Vertex Shader
		if (compile_result == GL_FALSE){
			GLint maxLength = 0;

			glGetShaderiv(v, GL_INFO_LOG_LENGTH, &maxLength);
			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(v, maxLength, &maxLength, &errorLog[0]);

			AfxMessageBox(_T("Error compilació Vertex Shader"));

			// Volcar missatges error a fitxer Compile_Error.VERT
			if ((fd = fopen("GLSL_Error.VERT", "w")) == NULL)
			{
				AfxMessageBox(_T("GLSL_Error.VERT was not opened"));
				return false;
			}
			for (int i = 0; i <= maxLength; i = i++) fprintf(fd, "%c", errorLog[i]);
			fclose(fd);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(v); // Don't leak the shader.
			return 0;
		}

// Compilació Fragment Shader
		glCompileShader(f);
		glGetShaderiv(v, GL_COMPILE_STATUS, &compile_result);

// Llista error de compilació del Vertex Shader
		if (compile_result == GL_FALSE){
			GLint maxLength = 0;

			glGetShaderiv(v, GL_INFO_LOG_LENGTH, &maxLength);
			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(v, maxLength, &maxLength, &errorLog[0]);

			AfxMessageBox(_T("Error compilació Fragment Shader"));

			// Volcar missatges error a fitxer Compile_Error.FRAG
			if ((fd = fopen("GLSL_Error.FRAG", "w")) == NULL)
			{
				AfxMessageBox(_T("GLSL_Error.FRAG was not opened"));
				return false;
			}
			for (int i = 0; i <= maxLength; i = i++) fprintf(fd, "%c", errorLog[i]);
			fclose(fd);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(v); // Don't leak the shader.
			return 0;
		}

// Creació del Program que inclou Vertex i Fragment Shader
		p = glCreateProgram();
		glAttachShader(p, v);	// Relaciona Vertex Shader al Program
		glAttachShader(p, f);	// relaciona Fragment Shader al Program

// Linkedició del Program
		glLinkProgram(p);

// Activació del Programa
		glUseProgram(p);

// Retorna referència al Program creat
		return p;
	}
	else return 0;	// retornar 0 si no s'ha trobat el fitxer.
}


/*****************************************************************************
* loadFileShaders()
*      Loads custom shader by specifying filename (expects frag/vert pair)
*****************************************************************************/
GLuint CEntornV3DView::loadFileShaders(CString file_Vert, CString file_Frag)
{

	GLuint v, f, p;
	char *vs = NULL, *fs = NULL;
	//	CString file_Vert, file_Frag;
	int info_log_length = 0, compile_result = 0;
	FILE *fd;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

//	file_Vert = filename + _T(".vert");
	char* fitxer_Vert = CString2Char(file_Vert);

	vs = textFileRead(fitxer_Vert);
	//vs = textFileRead(filename + ".vert");

//	file_Frag = filename + _T(".frag");
	char* fitxer_Frag = CString2Char(file_Frag);

	fs = textFileRead(fitxer_Frag);
//fs = textFileRead(filename + ".frag");

	const char * vv = vs;
	const char * ff = fs;

	glShaderSource(v, 1, &vv, NULL);
	glShaderSource(f, 1, &ff, NULL);

	free(vs);
	free(fs);
	glCompileShader(v);
	glGetShaderiv(v, GL_COMPILE_STATUS, &compile_result);
	// Llista error de compilació
	if (compile_result == GL_FALSE){
		GLint maxLength = 0;

		glGetShaderiv(v, GL_INFO_LOG_LENGTH, &maxLength);
		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(v, maxLength, &maxLength, &errorLog[0]);

		AfxMessageBox(_T("Error compilació Vertex Shader"));

		// Volcar missatges error a fitxer Compile_Error.VERT
		if ((fd = fopen("GLSL_Error.VERT", "w")) == NULL)
		{
			AfxMessageBox(_T("GLSL_Error.VERT was not opened"));
			return false;
		}
		for (int i = 0; i <= maxLength; i = i++) fprintf(fd, "%c", errorLog[i]);
		fclose(fd);

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(v); // Don't leak the shader.
		return 0;
	}

	glCompileShader(f);
	glGetShaderiv(f, GL_COMPILE_STATUS, &compile_result);
	// Llista error de compilació
	if (compile_result == GL_FALSE){
		GLint maxLength = 0;

		glGetShaderiv(f, GL_INFO_LOG_LENGTH, &maxLength);
		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(f, maxLength, &maxLength, &errorLog[0]);

		AfxMessageBox(_T("Error compilació Fragment Shader"));

		// Volcar missatges error a fitxer Compile_Error.FRAG
		if ((fd = fopen("GLSL_Error.FRAG", "w")) == NULL)
		{
			AfxMessageBox(_T("GLSL_Error.FRAG was not opened"));
			return false;
		}
		for (int i = 0; i <= maxLength; i = i++) fprintf(fd, "%c", errorLog[i]);
		fclose(fd);

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(f); // Don't leak the shader.
		return 0;
	}

	p = glCreateProgram();
	glAttachShader(p, v);
	glAttachShader(p, f);

	glLinkProgram(p);
	glUseProgram(p);

	return p;
}


char *CEntornV3DView::textFileRead(char *fn) {
	FILE *fp;
	char *content = NULL;
	int count = 0;
	if (fn != NULL) {
		fp = fopen(fn, "rt");
		if (fp != NULL) {
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);
			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count + 1));
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
		else {	AfxMessageBox(_T("Vertex or Fragment Program not found"));
				return NULL;
			}
	}
	return content;
}

/*****************************************************************************
* releaseAllShaders()
*      Unload shaders, return to OpenGL fixed function.
*****************************************************************************/
void CEntornV3DView::releaseAllShaders()
{
// Entorn V3D: Activació del contexte OpenGL
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	glUseProgram(0);

// Entorn V3D: Desactivació del contexte OpenGL. Permet la coexistencia d'altres contextes de generació
	wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
}


/* ------------------------------------------------------------------------- */
/*                           CONTROL DEL TECLAT                              */
/* ------------------------------------------------------------------------- */

// OnKeyDown: Funció de tractament de teclat (funció que es crida quan es prem una tecla)
//   PARÀMETRES:
//    - nChar: Codi del caracter seleccionat
//    - nRepCnt: Nombre de vegades que s'ha apretat la tecla (acceleració)
//    - nFlags: Flags d'interrupció activats.
void CEntornV3DView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
	const float incr = 0.025f;
	float modul = 0;
	GLfloat vdir[3] = { 0, 0, 0 };

	if (nChar == 'F') this->OnVistaFullscreen();	// Activació-Desactivació Full Screen
	if (camera == CAM_LOXODROMA_TDARBOUX || camera == CAM_LOXODROMA_TFRENET) Teclat_Camera_Loxodroma(nChar, nRepCnt);
	else if (objecte == ESCENA_LOXODROMA || objecte == C_LOXODROMA || objecte == C_BEZIER || 
			 objecte == C_BSPLINE || objecte == C_LEMNISCATA) Teclat_PasCorbes(nChar, nRepCnt);
		else if ((sw_grid) && ((grid.x) || (grid.y) || (grid.z))) Teclat_Grid(nChar, nRepCnt);
			else if (((nChar == 'G') || (nChar == 'g')) && ((grid.x) || (grid.y) || (grid.z))) sw_grid = !sw_grid;
				else if ((!pan) && (!transf) && (camera != CAM_NAVEGA))
						{	if (!sw_color) Teclat_ColorFons(nChar, nRepCnt);
								else Teclat_ColorObjecte(nChar, nRepCnt);
						}
						else {	if (transf)
									{	if (rota) Teclat_TransRota(nChar, nRepCnt);
											 else if (trasl) Teclat_TransTraslada(nChar, nRepCnt);
												else if (escal) Teclat_TransEscala(nChar, nRepCnt);
									}
								if (pan) Teclat_Pan(nChar, nRepCnt);
									else if (camera == CAM_NAVEGA) Teclat_Navega(nChar, nRepCnt);
							}

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CEntornV3DView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}


// Teclat_Camera_Loxodroma: Teclat per incrementar-Decrementar el pas de dibuix de les corbes (pas_CS).
void CEntornV3DView::Teclat_Camera_Loxodroma(UINT nChar, UINT nRepCnt)
{
	switch (nChar)
	{
	// Tecla cursor esquerra: Retrocedir posició de la càmera segons la corba Loxodroma
	case VK_LEFT:
		cam_t -= pas_CS;
		if (cam_t < -15.0) cam_t = -15.0;

		opvN = Punt_Loxodroma(OPV.R, 0.125, cam_t);
		switch (camera) {
			case CAM_LOXODROMA_TDARBOUX:
				vCam = Prod_Vectorial(VT_TDLoxodroma(OPV.R, 0.125, cam_t), VNP_TDLoxodroma(OPV.R, 0.125, cam_t));
				break;

			case CAM_LOXODROMA_TFRENET:
				vCam = VBN_TFLoxodroma(OPV.R, 0.125, cam_t);
				break;
		}
		break;

		// Tecla cursor esquerra: Avançar la posició de la càmera segons la corba Loxodroma
	case VK_RIGHT:
		cam_t += pas_CS;
		if (cam_t > 15.0) cam_t = 15.0;

		opvN = Punt_Loxodroma(OPV.R, 0.125, cam_t);
		switch (camera) {
		case CAM_LOXODROMA_TDARBOUX:
			vCam = Prod_Vectorial(VT_TDLoxodroma(OPV.R, 0.125, cam_t), VNP_TDLoxodroma(OPV.R, 0.125, cam_t));
			break;

		case CAM_LOXODROMA_TFRENET:
			vCam = VBN_TFLoxodroma(OPV.R, 0.125, cam_t);
			break;
		}
		break;

	// Tecla '+' (incrementar pas)
	case 107:
		pas_CS = pas_CS * 2.0;
		if (pas_CS > 0.5) pas_CS = 0.5;
		break;

	// Tecla '-' (decrementar pas)
	case 109:
		pas_CS = pas_CS / 2.0;
		if (pas_CS < 0.0125) pas_CS = 0.00625;
		break;

	// Tecla PgUp ('9') (incrementar pas)
	case VK_PRIOR:
		pas_CS = pas_CS * 2.0;
		if (pas_CS > 0.5)  pas_CS = 0.5;
		break;

	// Tecla PgDown ('3') (decrementar pas)
	case VK_NEXT:
		pas_CS = pas_CS / 2.0;
		if (pas_CS < 0.0125) pas_CS = 0.00625;
		break;

	// Tecla F5: Activar animació Càmera loxodroma
	case VK_F5:
		animaCamLxd = true;
		//palante = true;
		//cam_t = 0;
		SetTimer(WM_TIMER, 10, NULL);
		break;
		
	// Tecla END: Desactivar animació Càmera loxodroma
	case VK_END:

		animaCamLxd = false;

		break;

		// Tecla PAUSE: Pausar / reactivar animació Càmera loxodroma
	case VK_PAUSE:
		animaCamLxd = !animaCamLxd;
		break;

	default:
		if (transf)
		{	if (rota) Teclat_TransRota(nChar, nRepCnt);
				else if (trasl) Teclat_TransTraslada(nChar, nRepCnt);
				else if (escal) Teclat_TransEscala(nChar, nRepCnt);
		}
		if (pan) Teclat_Pan(nChar, nRepCnt);
				else if (camera == CAM_NAVEGA) Teclat_Navega(nChar, nRepCnt);
					else Teclat_ColorFons(nChar, nRepCnt);
		break;
	}
}

// Teclat_PasCorbes: Teclat per incrementar-Decrementar el pas de dibuix de les corbes (pas_CS).
void CEntornV3DView::Teclat_PasCorbes(UINT nChar, UINT nRepCnt)
{
	switch (nChar)
	{
	// Tecla '+' (incrementar pas)
	case 107:
		pas_CS = pas_CS * 2.0;
		if (pas_CS > 0.5) pas_CS = 0.5;
		break;

	// Tecla '-' (decrementar pas)
	case 109:
		pas_CS = pas_CS / 2;
		if (pas_CS < 0.0125) pas_CS = 0.00625;
		break;

	// Tecla PgUp ('9') (incrementar pas)
	case VK_PRIOR:
		pas_CS = pas_CS * 2.0;
		if (pas_CS > 0.5) pas_CS = 0.5;
		break;

	// Tecla PgDown ('3') (decrementar pas)
	case VK_NEXT:
		pas_CS = pas_CS / 2;
		if (pas_CS < 0.0125) pas_CS = 0.00625;
		break;

	default:
		if (transf)
		{
			if (rota) Teclat_TransRota(nChar, nRepCnt);
			else if (trasl) Teclat_TransTraslada(nChar, nRepCnt);
			else if (escal) Teclat_TransEscala(nChar, nRepCnt);
		}
		if (pan) Teclat_Pan(nChar, nRepCnt);
			else if (camera == CAM_NAVEGA) Teclat_Navega(nChar, nRepCnt);
				else if (!sw_color) Teclat_ColorFons(nChar, nRepCnt);
					else Teclat_ColorObjecte(nChar, nRepCnt);
		break;
	}
}


// Teclat_ColorObjecte: Teclat pels canvis de color de l'objecte per teclat.
void CEntornV3DView::Teclat_ColorObjecte(UINT nChar, UINT nRepCnt)
{
	const float incr = 0.025f;

// FRACTAL: Canvi resolució del fractal pe tecles '+' i'-'
	if (objecte == O_FRACTAL)
	{
		if (nChar == 109) // Caràcter '-' 
		{	pas = pas * 2;
			if (pas>64) pas = 64;
			sw_il = true;
		}
		 else if (nChar == 107) // Caràcter '+' 
		 {	pas = pas / 2;
			if (pas<1) pas = 1;
			sw_il = true;
		 }
	}
	//	else 
	if (nChar == VK_DOWN) 
	{	if (fonsR) {	col_obj.r -= nRepCnt*incr;
						if (col_obj.r<0.0) col_obj.r = 0.0;
					}
		if (fonsG) {	col_obj.g -= nRepCnt*incr;
						if (col_obj.g<0.0) col_obj.g = 0.0;
					}
		if (fonsB) {	col_obj.b -= nRepCnt*incr;
						if (col_obj.b<0.0) col_obj.b = 0.0;
					}
	}
	else if (nChar == VK_UP) 
		{	if (fonsR) {	col_obj.r += nRepCnt*incr;
							if (col_obj.r>1.0) col_obj.r = 1.0;
						}
			if (fonsG) {	col_obj.g += nRepCnt*incr;
							if (col_obj.g>1.0) col_obj.g = 1.0;
						}
			if (fonsB) {	col_obj.b += nRepCnt*incr;
							if (col_obj.b>1.0) col_obj.b = 1.0;
						}
		}
		else if (nChar == VK_SPACE) 
			{	if ((fonsR) && (fonsG) && (fonsB)) {	fonsG = false;
														fonsB = false;
													}
				  else if ((fonsR) && (fonsG)) {	fonsG = false;
													fonsB = true;
												}
					else if ((fonsR) && (fonsB)) {	fonsR = false;
													fonsG = true;
												}
						else if ((fonsG) && (fonsB)) fonsR = true;
							else if (fonsR) {	fonsR = false;
												fonsG = true;
											}
								else if (fonsG) {	fonsG = false;
													fonsB = true;
												}
									else if (fonsB) {	fonsR = true;
														fonsG = true;
														fonsB = false;
													}
			}
				else if ((nChar == 'o') || (nChar == 'O')) sw_color = true;
					else if ((nChar == 'b') || (nChar == 'B')) sw_color = false;

}


// Teclat_ColorFons: Teclat pels canvis del color de fons.
void CEntornV3DView::Teclat_ColorFons(UINT nChar, UINT nRepCnt)
{
	const float incr = 0.025f;

// FRACTAL: Canvi resolució del fractal pe tecles '+' i'-'
	if (objecte == O_FRACTAL)
	{	if (nChar == 109) // Caràcter '-' 
		{	pas = pas * 2;
			if (pas>64) pas = 64;
			sw_il = true;
		}
		else if (nChar == 107) // Caràcter '+' 
		{	pas = pas / 2;
			if (pas<1) pas = 1;
			sw_il = true;
		}
	}
	//	else 
	if (nChar == VK_DOWN) {
		if (fonsR) {	c_fons.r -= nRepCnt*incr;
						if (c_fons.r<0.0) c_fons.r = 0.0;
					}
		if (fonsG) {	c_fons.g -= nRepCnt*incr;
						if (c_fons.g<0.0) c_fons.g = 0.0;
					}
		if (fonsB) {	c_fons.b -= nRepCnt*incr;
						if (c_fons.b<0.0) c_fons.b = 0.0;
					}
		}
	else if (nChar == VK_UP) {
		if (fonsR) {	c_fons.r += nRepCnt*incr;
						if (c_fons.r>1.0) c_fons.r = 1.0;
					}
		if (fonsG) {	c_fons.g += nRepCnt*incr;
						if (c_fons.g>1.0) c_fons.g = 1.0;
					}
		if (fonsB) {	c_fons.b += nRepCnt*incr;
						if (c_fons.b>1.0) c_fons.b = 1.0;
					}
		}
	else if (nChar == VK_SPACE) {
		if ((fonsR) && (fonsG) && (fonsB)) {	fonsG = false;
												fonsB = false;
											}
			else if ((fonsR) && (fonsG)) {		fonsG = false;
												fonsB = true;
										}
				else if ((fonsR) && (fonsB)) {	fonsR = false;
												fonsG = true;
											}
					else if ((fonsG) && (fonsB)) fonsR = true;
						else if (fonsR) {	fonsR = false;
											fonsG = true;
										}
							else if (fonsG) {	fonsG = false;
												fonsB = true;
											}
								else if (fonsB) {	fonsR = true;
													fonsG = true;
													fonsB = false;
												}
		}
		else if ((nChar == 'o') || (nChar == 'O')) sw_color = true;
			else if ((nChar == 'b') || (nChar == 'B')) sw_color = false;
}

// Teclat_Navega: Teclat pels moviments de navegació.
void CEntornV3DView::Teclat_Navega(UINT nChar, UINT nRepCnt)
{
	GLfloat vdir[3] = { 0, 0, 0 };
	float modul = 0;

// Entorn V3D: Controls de moviment de navegació
	vdir[0] = n[0] - opvN.x;
	vdir[1] = n[1] - opvN.y;
	vdir[2] = n[2] - opvN.z;
	modul = sqrt(vdir[0] * vdir[0] + vdir[1] * vdir[1] + vdir[2] * vdir[2]);
	vdir[0] = vdir[0] / modul;
	vdir[1] = vdir[1] / modul;
	vdir[2] = vdir[2] / modul;
	switch (nChar)
	{	
	// Tecla cursor amunt
	case VK_UP:
		opvN.x += nRepCnt*fact_pan*vdir[0];
		opvN.y += nRepCnt*fact_pan*vdir[1];
		n[0] += nRepCnt*fact_pan*vdir[0];
		n[1] += nRepCnt*fact_pan*vdir[1];
		break;

	// Tecla cursor avall
	case VK_DOWN:
		opvN.x -= nRepCnt*fact_pan*vdir[0];
		opvN.y -= nRepCnt*fact_pan*vdir[1];
		n[0] -= nRepCnt*fact_pan*vdir[0];
		n[1] -= nRepCnt*fact_pan*vdir[1];
		break;

	// Tecla cursor esquerra
	case VK_LEFT:
		angleZ = +nRepCnt*fact_pan;
		n[0] = n[0] - opvN.x;
		n[1] = n[1] - opvN.y;
		n[0] = n[0] * cos(angleZ*pi / 180) - n[1] * sin(angleZ*pi / 180);
		n[1] = n[0] * sin(angleZ*pi / 180) + n[1] * cos(angleZ*pi / 180);
		n[0] = n[0] + opvN.x;
		n[1] = n[1] + opvN.y;
		break;

	// Tecla cursor dret
	case VK_RIGHT:
		angleZ = 360 - nRepCnt*fact_pan;
		n[0] = n[0] - opvN.x;
		n[1] = n[1] - opvN.y;
		n[0] = n[0] * cos(angleZ*pi / 180) - n[1] * sin(angleZ*pi / 180);
		n[1] = n[0] * sin(angleZ*pi / 180) + n[1] * cos(angleZ*pi / 180);
		n[0] = n[0] + opvN.x;
		n[1] = n[1] + opvN.y;
		break;

	// Tecla Inicio
	case VK_HOME:
		opvN.z += nRepCnt*fact_pan;
		n[2] += nRepCnt*fact_pan;
		break;

	// Tecla Fin
	case VK_END:
		opvN.z -= nRepCnt*fact_pan;
		n[2] -= nRepCnt*fact_pan;
		break;

	// Tecla PgUp
	case VK_PRIOR:
		fact_pan /= 2;
		if (fact_pan<1) fact_pan = 1;
		break;

	// Tecla PgDown
	case VK_NEXT:
		fact_pan *= 2;
		if (fact_pan>2048) fact_pan = 2048;
		break;

	default:
		break;
	}
}


// Teclat_Pan: Teclat pels moviments de Pan.
void CEntornV3DView::Teclat_Pan(UINT nChar, UINT nRepCnt)
{
	switch (nChar)
	{
	// Tecla cursor amunt
	case VK_UP:
		tr_cpv.y -= nRepCnt*fact_pan;
		if (tr_cpv.y<-100000) tr_cpv.y = 100000;
		break;

	// Tecla cursor avall
	case VK_DOWN:
		tr_cpv.y += nRepCnt*fact_pan;
		if (tr_cpv.y>100000) tr_cpv.y = 100000;
		break;

	// Tecla cursor esquerra
	case VK_LEFT:
		tr_cpv.x += nRepCnt*fact_pan;
		if (tr_cpv.x>100000) tr_cpv.x = 100000;
		break;

	// Tecla cursor dret
	case VK_RIGHT:
		tr_cpv.x -= nRepCnt*fact_pan;
		if (tr_cpv.x<-100000) tr_cpv.x = 100000;
		break;

	// Tecla PgUp
	case VK_PRIOR:
		fact_pan /= 2;
		if (fact_pan<0.125) fact_pan = 0.125;
		break;

	// Tecla PgDown
	case VK_NEXT:
		fact_pan *= 2;
		if (fact_pan>2048) fact_pan = 2048;
		break;

	// Tecla Insert: Fixar el desplaçament de pantalla (pan)
	case VK_INSERT:
		// Acumular desplaçaments de pan (tr_cpv) en variables fixes (tr_cpvF).
		tr_cpvF.x += tr_cpv.x;		tr_cpv.x = 0.0;
		if (tr_cpvF.x>100000) tr_cpvF.y = 100000;
		tr_cpvF.y += tr_cpv.y;		tr_cpv.y = 0.0;
		if (tr_cpvF.y>100000) tr_cpvF.y = 100000;
		tr_cpvF.z += tr_cpv.z;		tr_cpv.z = 0.0;
		if (tr_cpvF.z>100000) tr_cpvF.z = 100000;
		break;

	// Tecla Delete: Inicialitzar el desplaçament de pantalla (pan)
	case VK_DELETE:
		// Inicialitzar els valors de pan tant de la variable tr_cpv com de la tr_cpvF.
		tr_cpv.x = 0.0;			tr_cpv.y = 0.0;			tr_cpv.z = 0.0;
		tr_cpvF.x = 0.0;		tr_cpvF.y = 0.0;		tr_cpvF.z = 0.0;
		break;

	default:
		break;
	}
}

// Teclat_TransEscala: Teclat pels canvis del valor d'escala per X,Y,Z.
void CEntornV3DView::Teclat_TransEscala(UINT nChar, UINT nRepCnt)
{
	switch (nChar)
	{
// Modificar vector d'Escalatge per teclat (actiu amb Escalat únicament)
	// Tecla '+' (augmentar tot l'escalat)
	case 107:
		TG.VScal.x = TG.VScal.x * 2;
		if (TG.VScal.x>8192) TG.VScal.x = 8192;
		TG.VScal.y = TG.VScal.y * 2;
		if (TG.VScal.y>8192) TG.VScal.y = 8192;
		TG.VScal.z = TG.VScal.z * 2;
		if (TG.VScal.z>8192) TG.VScal.z = 8192;
		break;

	// Tecla '-' (disminuir tot l'escalat)
	case 109:
		TG.VScal.x = TG.VScal.x / 2;
		if (TG.VScal.x<0.25) TG.VScal.x = 0.25;
		TG.VScal.y = TG.VScal.y / 2;
		if (TG.VScal.y<0.25) TG.VScal.y = 0.25;
		TG.VScal.z = TG.VScal.z / 2;
		if (TG.VScal.z<0.25) TG.VScal.z = 0.25;
		break;

	// Tecla cursor amunt ('8')
	case 104:
		TG.VScal.x = TG.VScal.x * 2;
		if (TG.VScal.x>8192) TG.VScal.x = 8192;
		break;

	// Tecla cursor avall ('2')
	case 98:
		TG.VScal.x = TG.VScal.x / 2;
		if (TG.VScal.x<0.25) TG.VScal.x = 0.25;
		break;

	// Tecla cursor esquerra ('4')
	case 100:
		TG.VScal.y = TG.VScal.y / 2;
		if (TG.VScal.y<0.25) TG.VScal.y = 0.25;
		break;

	// Tecla cursor dret ('6')
	case 102:
		TG.VScal.y = TG.VScal.y * 2;
		if (TG.VScal.y>8192) TG.VScal.y = 8192;
		break;

	// Tecla HOME ('7')
	case 103:
		TG.VScal.z = TG.VScal.z * 2;
		if (TG.VScal.z>8192) TG.VScal.z = 8192;
		break;

	// Tecla END ('1')
	case 97:
		TG.VScal.z = TG.VScal.z / 2;
		if (TG.VScal.z<0.25) TG.VScal.z = 0.25;
		break;

	// Tecla INSERT
	case VK_INSERT:
		// Acumular transformacions Geomètriques (variable TG) i de pan en variables fixes (variable TGF)
		TGF.VScal.x *= TG.VScal.x;	TGF.VScal.y *= TG.VScal.y; TGF.VScal.z *= TG.VScal.z;
		if (TGF.VScal.x>8192)		TGF.VScal.x = 8192;
		if (TGF.VScal.y>8192)		TGF.VScal.y = 8192;
		if (TGF.VScal.z>8192)		TGF.VScal.z = 8192;
		TG.VScal.x = 1.0;				TG.VScal.y = 1.0;			TG.VScal.z = 1.0;
		TGF.VRota.x += TG.VRota.x;	TGF.VRota.y += TG.VRota.y; TGF.VRota.z += TG.VRota.z;
		if (TGF.VRota.x >= 360)		TGF.VRota.x -= 360; 		if (TGF.VRota.x<0) TGF.VRota.x += 360;
		if (TGF.VRota.y >= 360)		TGF.VRota.y -= 360;		if (TGF.VRota.y<0) TGF.VRota.y += 360;
		if (TGF.VRota.z >= 360)		TGF.VRota.z -= 360;		if (TGF.VRota.z<0) TGF.VRota.z += 360;
		TG.VRota.x = 0.0;				TG.VRota.y = 0.0;					TG.VRota.z = 0.0;
		TGF.VTras.x += TG.VTras.x;	TGF.VTras.y += TG.VTras.y; TGF.VTras.z += TG.VTras.z;
		if (TGF.VTras.x<-100000)		TGF.VTras.x = 100000;		if (TGF.VTras.x>10000) TGF.VTras.x = 100000;
		if (TGF.VTras.y<-100000)		TGF.VTras.y = 100000;		if (TGF.VTras.y>10000) TGF.VTras.y = 100000;
		if (TGF.VTras.z<-100000)		TGF.VTras.z = 100000;		if (TGF.VTras.z>10000) TGF.VTras.z = 100000;
		TG.VTras.x = 0.0;		TG.VTras.y = 0.0;		TG.VTras.z = 0.0;
		break;

	// Tecla Delete: Esborrar les Transformacions Geomètriques Calculades
	case VK_DELETE:
		// Inicialitzar els valors de transformacions Geomètriques i de pan en variables fixes.
		TGF.VScal.x = 1.0;		TGF.VScal.y = 1.0;;		TGF.VScal.z = 1.0;
		TG.VScal.x = 1.0;		TG.VScal.y = 1.0;		TG.VScal.z = 1.0;
		TGF.VRota.x = 0.0;		TGF.VRota.y = 0.0;		TGF.VRota.z = 0.0;
		TG.VRota.x = 0.0;		TG.VRota.y = 0.0;		TG.VRota.z = 0.0;
		TGF.VTras.x = 0.0;		TGF.VTras.y = 0.0;		TGF.VTras.z = 0.0;
		TG.VTras.x = 0.0;		TG.VTras.y = 0.0;		TG.VTras.z = 0.0;
		break;

	default:
		break;
	}
}

// Teclat_TransRota: Teclat pels canvis del valor del vector de l'angle de rotació per X,Y,Z.
void CEntornV3DView::Teclat_TransRota(UINT nChar, UINT nRepCnt)
{	int i = 0;
	double angle = 0.0;
	GL_Quat angleQ;

// Inicialitzar quaternio angleQ.
	angleQ.w = 1.0;	 angleQ.x = 0.0;	angleQ.y = 0.0, angleQ.z = 0.0;

	switch (nChar)
	{
// Tecla cursor amunt ('8')
	case 104:
		TG.VRota.x += nRepCnt * fact_Rota;
		if (TG.VRota.x >= 360.0f) TG.VRota.x -= 360;
// Entorn V3D. QUATERNIONS: Definir nou angle rotació X com a quaternió (angleQ) i multiplicar-lo per qT --> qT=angleQ*qT. 
		if (rotaQ) {	
			angle += nRepCnt * fact_Rota;
			if (angle >= 360.0f) while(angle >=360.0f) angle -= 360;
			EixAngleToQuat(1, 0, 0, angle, angleQ);
			QuatMul(angleQ, qT, qT);
			QuatToMatrix(qT, QMatrix);
					}
		break;

// Tecla cursor avall ('2')
	case 98:
		TG.VRota.x -= nRepCnt * fact_Rota;
		if (TG.VRota.x < 0.0f) TG.VRota.x += 360;
// Entorn V3D. QUATERNIONS: Definir nou angle rotació X com a quaternió (angleQ) i multiplicar-lo per qT --> qT=angleQ*qT. 
		if (rotaQ) {	
			angle -= nRepCnt * fact_Rota;
			if (angle < 0.0f) while (angle < 0.0f) angle += 360;
			EixAngleToQuat(1, 0, 0, angle, angleQ);
			QuatMul(angleQ, qT, qT);
			QuatToMatrix(qT, QMatrix);
					}
		break;

// Tecla cursor esquerra ('4')
	case 100:
		TG.VRota.y -= nRepCnt * fact_Rota;
		if (TG.VRota.y < 0.0) TG.VRota.y += 360.0;
// Entorn V3D. QUATERNIONS: Definir nou angle rotació Y com a quaternió (angleQ) i multiplicar-lo per qT --> qT=angleQ*qT. 
		if (rotaQ) {	
			angle += nRepCnt * fact_Rota;
			if (angle >= 360.0f) while (angle >= 360.0f) angle -= 360;
			EixAngleToQuat(0, 1, 0, angle, angleQ);
			QuatMul(angleQ, qT, qT);
			QuatToMatrix(qT, QMatrix);
					}
		break;

// Tecla cursor dret ('6')
	case 102:
		TG.VRota.y += nRepCnt * fact_Rota;
		if (TG.VRota.y >= 360.0) TG.VRota.y -= 360.0;
// Entorn V3D. QUATERNIONS: Definir nou angle rotació Y com a quaternió (angleQ) i multiplicar-lo per qT --> qT=angleQ*qT. 
		if (rotaQ) {	
			angle -= nRepCnt * fact_Rota;
			if (angle < 0.0f) while (angle < 0.0f) angle += 360;
			EixAngleToQuat(0, 1, 0, angle, angleQ);
			QuatMul(angleQ, qT, qT);
			QuatToMatrix(qT, QMatrix);
					}
		break;

// Tecla HOME ('7')
	case 103:
		TG.VRota.z += nRepCnt * fact_Rota;
		if (TG.VRota.z >= 360.0) TG.VRota.z -= 360.0;
// Entorn V3D. QUATERNIONS: Definir nou angle rotació Z com a quaternió (angleQ) i multiplicar-lo per qT --> qT=angleQ*qT. 
		if (rotaQ) {	
			angle += nRepCnt * fact_Rota;
			if (angle >= 360.0f) while (angle >= 360.0f) angle -= 360;
			EixAngleToQuat(0, 0, 1, angle, angleQ);
			QuatMul(angleQ, qT, qT);
			QuatToMatrix(qT, QMatrix);
					}
		break;

// Tecla END ('1')
	case 97:
		TG.VRota.z -= nRepCnt * fact_Rota;
		if (TG.VRota.z < 0.0) TG.VRota.z += 360.0;
		// Entorn V3D. QUATERNIONS: Definir nou angle rotació Z com a quaternió (angleQ) i multiplicar-lo per qT --> qT=angleQ*qT. 
		if (rotaQ) {	
			angle -= nRepCnt * fact_Rota;
			if (angle < 0.0f) while (angle < 0.0f) angle += 360;
			EixAngleToQuat(0, 0, 1, angle, angleQ);
			QuatMul(angleQ, qT, qT);
			QuatToMatrix(qT, QMatrix);
					}
		break;

// Tecla PgUp ('9')
	case 105:
		fact_Rota /= 2.0;
		if (fact_Rota < 1.0) fact_Rota = 1.0;
		break;

// Tecla PgDown ('3')
	case 99:
		fact_Rota *= 2.0;
		if (fact_Rota > 90.0) fact_Rota = 90.0;
		break;

// Entorn V3D. QUATERNIONS: Tecla F5 (Activar animació cossos rígids per QUATERNIONS) - PENDENT DE FER
	case VK_F5:
		if (np_InQ > 1 && rotaQ) {
			//activar timer
			//calcular quat associat a la interpolacio esferica entre keyf(0) i keyf(1) amb t=0
			animaQ = true;
				
			index = 0;
			adelante = true;
			qI = keyf_Quat[index];
			qF = keyf_Quat[index + 1];
			index++;
			t = 0;
			SetTimer(WM_TIMER, 10, NULL);
		}
		break;

// Entorn V3D. QUATERNIONS: Tecla Pausa (Activar/parar animació cossos rígids per QUATERNIONS) - PENDENT DE FER
	case VK_PAUSE:
		if (rotaQ && np_InQ > 1) {
			animaQ = !animaQ;
					}
		break;

// Entorn V3D. QUATERNIONS: Tecla End (Para animació i inicialitzar vector Keyframes - PENDENT DE FER
	case VK_END:
		if (rotaQ) {

			index = 0;
			adelante = true;
			qF = keyf_Quat[index];
			qI = keyf_Quat[index+1];
			t = 1;

			animaQ = false;
			QuatSlerp(qI, qF, t, qT);

			QuatToMatrix(qT, QMatrix);


			// Crida a OnPaint() per redibuixar l'escena
			InvalidateRect(NULL, false);
			
		}
		break;

// Entorn V3D. QUATERNIONS:  Tecla F3 (Accelerar animació cossos rígids per QUATERNIONS) - PENDENT DE FER (OPCIONAL)
	case VK_F3:
		if (rotaQ) {
			temps*=1.5f;
					}
		break;

// Entorn V3D. QUATERNIONS: Tecla F4 (Desaccelerar animació cossos rígids per QUATERNIONS) - PENDENT DE FER (OPCIONAL)
	case VK_F4:
		if (rotaQ) {
			temps *=0.75f;
					}
		break;

// Modificar vector d'Escalatge per teclat (actiu amb Rotació)
// Tecla '+' (augmentar escalat)
	case 107:
		TG.VScal.x = TG.VScal.x * 2;
		if (TG.VScal.x > 8192) TG.VScal.x = 8192;
		TG.VScal.y = TG.VScal.y * 2;
		if (TG.VScal.y > 8192) TG.VScal.y = 8192;
		TG.VScal.z = TG.VScal.z * 2;
		if (TG.VScal.z > 8192) TG.VScal.z = 8192;
		break;

// Tecla '-' (disminuir escalat)
	case 109:
		TG.VScal.x = TG.VScal.x / 2;
		if (TG.VScal.x < 0.25) TG.VScal.x = 0.25;
		TG.VScal.y = TG.VScal.y / 2;
		if (TG.VScal.y < 0.25) TG.VScal.y = 0.25;
		TG.VScal.z = TG.VScal.z / 2;
		if (TG.VScal.z < 0.25) TG.VScal.z = 0.25;
		break;

// Tecla Insert: Acumular transformacions Geomètriques (variable TG) i de pan en variables fixes (variable TGF)
	case VK_INSERT:
		// Entorn V3D. QUATERNIONS: Fixar els Quaternions en els keyframes (animació cossos rígids per QUATERNIONS)
		if (rotaQ) {	if (np_InQ < 3)
							{	// KeyFrame Quaternió qT guardat a keyf_Quat
								QuatCopy(qT, keyf_Quat[np_InQ]); // keyf_Quat[np_InQ] = qT;
								np_InQ = np_InQ + 1;
							}
							else {	// Introduir keyframe Rotació a la primera posició
									// KeyFrame Quaternió qT guardat a keyf_Quat
									QuatCopy(qT, keyf_Quat[0]); // keyf_Quat[0] = qT;
									// Inicialitzar resta de posicions
									for (i = 1; i < 3; i++) {	keyf_Quat[i].w = 1.0;	keyf_Quat[i].x = 0.0;
																keyf_Quat[i].y = 0.0;	keyf_Quat[i].z = 0.0;
															}
									np_InQ = 1;
								}
					}
		else {	TGF.VScal.x *= TG.VScal.x;	TGF.VScal.y *= TG.VScal.y; TGF.VScal.z *= TG.VScal.z;
				if (TGF.VScal.x > 8192)		TGF.VScal.x = 8192;
				if (TGF.VScal.y > 8192)		TGF.VScal.y = 8192;
				if (TGF.VScal.z > 8192)		TGF.VScal.z = 8192;
				TG.VScal.x = 1.0;			TG.VScal.y = 1.0;			TG.VScal.z = 1.0;
				TGF.VRota.x += TG.VRota.x;	TGF.VRota.y += TG.VRota.y; TGF.VRota.z += TG.VRota.z;
				if (TGF.VRota.x >= 360)		TGF.VRota.x -= 360; 		if (TGF.VRota.x < 0) TGF.VRota.x += 360;
				if (TGF.VRota.y >= 360)		TGF.VRota.y -= 360;			if (TGF.VRota.y < 0) TGF.VRota.y += 360;
				if (TGF.VRota.z >= 360)		TGF.VRota.z -= 360;			if (TGF.VRota.z < 0) TGF.VRota.z += 360;
				TG.VRota.x = 0.0;			TG.VRota.y = 0.0;			TG.VRota.z = 0.0;
				TGF.VTras.x += TG.VTras.x;	TGF.VTras.y += TG.VTras.y; TGF.VTras.z += TG.VTras.z;
				if (TGF.VTras.x < -100000)	TGF.VTras.x = 100000;		if (TGF.VTras.x > 10000) TGF.VTras.x = 100000;
				if (TGF.VTras.y < -100000)	TGF.VTras.y = 100000;		if (TGF.VTras.y > 10000) TGF.VTras.y = 100000;
				if (TGF.VTras.z < -100000)	TGF.VTras.z = 100000;		if (TGF.VTras.z > 10000) TGF.VTras.z = 100000;
				TG.VTras.x = 0.0;			TG.VTras.y = 0.0;			TG.VTras.z = 0.0;
			}
		break;

// Tecla Delete: Esborrar les Transformacions Geomètriques Calculades
	case VK_DELETE:
		// Inicialitzar els valors de transformacions Geomètriques i de pan en variables fixes.
		TGF.VScal.x = 1.0;	TGF.VScal.y = 1.0;;	TGF.VScal.z = 1.0;
		TG.VScal.x = 1.0;	TG.VScal.y = 1.0;	TG.VScal.z = 1.0;
		TGF.VRota.x = 0.0;	TGF.VRota.y = 0.0;	TGF.VRota.z = 0.0;
		TG.VRota.x = 0.0;	TG.VRota.y = 0.0;	TG.VRota.z = 0.0;
		TGF.VTras.x = 0.0;	TGF.VTras.y = 0.0;	TGF.VTras.z = 0.0;
		TG.VTras.x = 0.0;	TG.VTras.y = 0.0;	TG.VTras.z = 0.0;
		break;

// Tecla Espaiador
	case VK_SPACE:
		rota = !rota;
		trasl = !trasl;
		break;

	default:
		break;
	}
}


// Teclat_TransTraslada: Teclat pels canvis del valor de traslació per X,Y,Z.
void CEntornV3DView::Teclat_TransTraslada(UINT nChar, UINT nRepCnt)
{
	GLfloat vdir[3] = { 0, 0, 0 };
	float modul = 0;

	switch (nChar)
	{
	// Tecla cursor amunt ('8')
	case 104:
		TG.VTras.x -= nRepCnt*fact_Tras;
		if (TG.VTras.x<-100000) TG.VTras.x = 100000;
		break;

	// Tecla cursor avall ('2')
	case 98:
		TG.VTras.x += nRepCnt*fact_Tras;
		if (TG.VTras.x>10000) TG.VTras.x = 100000;
		break;

	// Tecla cursor esquerra ('4')
	case 100:
		TG.VTras.y -= nRepCnt*fact_Tras;
		if (TG.VTras.y<-100000) TG.VTras.y = -100000;
		break;

	// Tecla cursor dret ('6')
	case 102:
		TG.VTras.y += nRepCnt*fact_Tras;
		if (TG.VTras.y>100000) TG.VTras.y = 100000;
		break;

	// Tecla HOME ('7')
	case 103:
		TG.VTras.z += nRepCnt*fact_Tras;
		if (TG.VTras.z>100000) TG.VTras.z = 100000;
		break;

	// Tecla END ('1')
	case 97:
		TG.VTras.z -= nRepCnt*fact_Tras;
		if (TG.VTras.z<-100000) TG.VTras.z = -100000;
		break;

	// Tecla PgUp ('9')
	case 105:
		fact_Tras /= 2;
		if (fact_Tras<1) fact_Tras = 1;
		break;

	// Tecla PgDown ('3')
	case 99:
		fact_Tras *= 2;
		if (fact_Tras>100000) fact_Tras = 100000;
		break;

// Modificar vector d'Escalatge per teclat (actiu amb Traslació)
	// Tecla '+' (augmentar escalat)
	case 107:
		TG.VScal.x = TG.VScal.x * 2;
		if (TG.VScal.x>8192) TG.VScal.x = 8192;
		TG.VScal.y = TG.VScal.y * 2;
		if (TG.VScal.y>8192) TG.VScal.y = 8192;
		TG.VScal.z = TG.VScal.z * 2;
		if (TG.VScal.z>8192) TG.VScal.z = 8192;
		break;

	// Tecla '-' (disminuir escalat)
	case 109:
		TG.VScal.x = TG.VScal.x / 2;
		if (TG.VScal.x<0.25) TG.VScal.x = 0.25;
		TG.VScal.y = TG.VScal.y / 2;
		if (TG.VScal.y<0.25) TG.VScal.y = 0.25;
		TG.VScal.z = TG.VScal.z / 2;
		if (TG.VScal.z<0.25) TG.VScal.z = 0.25;
		break;

	// Tecla INSERT
	case VK_INSERT:
		// Acumular transformacions Geomètriques (variable TG) i de pan en variables fixes (variable TGF)
		TGF.VScal.x *= TG.VScal.x;	TGF.VScal.y *= TG.VScal.y; TGF.VScal.z *= TG.VScal.z;
		if (TGF.VScal.x>8192)		TGF.VScal.x = 8192;
		if (TGF.VScal.y>8192)		TGF.VScal.y = 8192;
		if (TGF.VScal.z>8192)		TGF.VScal.z = 8192;
		TG.VScal.x = 1.0;				TG.VScal.y = 1.0;			TG.VScal.z = 1.0;
		TGF.VRota.x += TG.VRota.x;	TGF.VRota.y += TG.VRota.y; TGF.VRota.z += TG.VRota.z;
		if (TGF.VRota.x >= 360)		TGF.VRota.x -= 360; 		if (TGF.VRota.x<0) TGF.VRota.x += 360;
		if (TGF.VRota.y >= 360)		TGF.VRota.y -= 360;		if (TGF.VRota.y<0) TGF.VRota.y += 360;
		if (TGF.VRota.z >= 360)		TGF.VRota.z -= 360;		if (TGF.VRota.z<0) TGF.VRota.z += 360;
		TG.VRota.x = 0.0;				TG.VRota.y = 0.0;					TG.VRota.z = 0.0;
		TGF.VTras.x += TG.VTras.x;	TGF.VTras.y += TG.VTras.y; TGF.VTras.z += TG.VTras.z;
		if (TGF.VTras.x<-100000)		TGF.VTras.x = 100000;		if (TGF.VTras.x>10000) TGF.VTras.x = 100000;
		if (TGF.VTras.y<-100000)		TGF.VTras.y = 100000;		if (TGF.VTras.y>10000) TGF.VTras.y = 100000;
		if (TGF.VTras.z<-100000)		TGF.VTras.z = 100000;		if (TGF.VTras.z>10000) TGF.VTras.z = 100000;
		TG.VTras.x = 0.0;		TG.VTras.y = 0.0;		TG.VTras.z = 0.0;
		break;

	// Tecla Delete: Esborrar les Transformacions Geomètriques Calculades
	case VK_DELETE:
		// Inicialitzar els valors de transformacions Geomètriques i de pan en variables fixes.
		TGF.VScal.x = 1.0;		TGF.VScal.y = 1.0;;		TGF.VScal.z = 1.0;
		TG.VScal.x = 1.0;		TG.VScal.y = 1.0;		TG.VScal.z = 1.0;
		TGF.VRota.x = 0.0;		TGF.VRota.y = 0.0;		TGF.VRota.z = 0.0;
		TG.VRota.x = 0.0;		TG.VRota.y = 0.0;		TG.VRota.z = 0.0;
		TGF.VTras.x = 0.0;		TGF.VTras.y = 0.0;		TGF.VTras.z = 0.0;
		TG.VTras.x = 0.0;		TG.VTras.y = 0.0;		TG.VTras.z = 0.0;
		break;

	// Tecla Espaiador
	case VK_SPACE:
		rota = !rota;
		trasl = !trasl;
		break;

	default:
		break;
	}
}


// Teclat_Grid: Teclat pels desplaçaments dels gridXY, gridXZ i gridYZ.
void CEntornV3DView::Teclat_Grid(UINT nChar, UINT nRepCnt)
{
	switch (nChar)
	{
		// Key Up cursor ('8')
	case 104:
		hgrid.x -= nRepCnt*PAS_GRID;
		break;

		// Key Down cursor ('2')
	case 98:
		hgrid.x += nRepCnt*PAS_GRID;
		break;

		// Key Left cursor ('4')
	case 100:
		hgrid.y -= nRepCnt*PAS_GRID;
		break;

		// Key Right cursor ('6')
	case 102:
		hgrid.y += nRepCnt*PAS_GRID;
		break;

		// Key HOME ('7')
	case 103:
		hgrid.z += nRepCnt*PAS_GRID;
		break;

		// Key END ('1')
	case 97:
		hgrid.z -= nRepCnt*PAS_GRID;
		break;

		// Key grid ('G')
	case 'G':
		sw_grid = !sw_grid;
		break;

		/*
		// Key grid ('g')
		case 'g':
		sw_grid = !sw_grid;
		break;
		*/

	default:
		break;
	}
}


/* ------------------------------------------------------------------------- */
/*                           CONTROL DEL RATOLI                              */
/* ------------------------------------------------------------------------- */

// OnLButtonDown: Funció que es crida quan s'apreta el botó esquerra del mouse.
void CEntornV3DView::OnLButtonDown(UINT nFlags, CPoint point)
{
// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado

// Entorn V3D: Detectem en quina posició s'ha apretat el botó esquerra del
//				mouse i ho guardem a la variable m_PosEAvall i activem flag m_ButoEAvall
	m_ButoEAvall = true;
	m_PosEAvall = point;
	m_EsfeEAvall = OPV;

	CView::OnLButtonDown(nFlags, point);
}


// OnLButtonDown: Funció que es crida quan deixem d'apretar el botó esquerra del mouse.
void CEntornV3DView::OnLButtonUp(UINT nFlags, CPoint point)
{
// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
// Entorn V3D: Desactivem flag m_ButoEAvall quan deixem d'apretar botó esquerra del mouse.
	m_ButoEAvall = false;

// OPCIÓ VISTA-->SATÈLIT: Càlcul increment desplaçament del Punt de Vista
	if ((satelit) && (projeccio != ORTO))
	{	//m_EsfeIncEAvall.R = m_EsfeEAvall.R - OPV.R;
		m_EsfeIncEAvall.alfa = 0.01f * (OPV.alfa - m_EsfeEAvall.alfa); //if (abs(m_EsfeIncEAvall.alfa)<0.01) { if ((m_EsfeIncEAvall.alfa)>0.0) m_EsfeIncEAvall.alfa = 0.01 else m_EsfeIncEAvall.alfa=0.01}
		m_EsfeIncEAvall.beta = 0.01f * (OPV.beta - m_EsfeEAvall.beta);
		if (abs(m_EsfeIncEAvall.beta) < 0.01) {
			if ((m_EsfeIncEAvall.beta) > 0.0) m_EsfeIncEAvall.beta = 0.01;
			else m_EsfeIncEAvall.beta = 0.01;
		}
		if ((m_EsfeIncEAvall.R == 0.0) && (m_EsfeIncEAvall.alfa == 0.0) && (m_EsfeIncEAvall.beta == 0.0)) KillTimer(WM_TIMER);
		else SetTimer(WM_TIMER, 10, NULL);
	}

	CView::OnLButtonUp(nFlags, point);
}


// OnLButtonDown: Funció que es crida quan s'apreta el botó dret del mouse.
void CEntornV3DView::OnRButtonDown(UINT nFlags, CPoint point)
{
// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
// Entorn V3D: Detectem en quina posició s'ha apretat el botó esquerra del
//				mouse i ho guardem a la variable m_PosEAvall i activem flag m_ButoDAvall
	m_ButoDAvall = true;
	m_PosDAvall = point;

	CView::OnRButtonDown(nFlags, point);
}


// OnLButtonDown: Funció que es crida quan deixem d'apretar el botó dret del mouse.
void CEntornV3DView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
// Entorn V3D: Desactivem flag m_ButoEAvall quan deixem d'apretar botó esquerra del mouse.
	m_ButoDAvall = false;

//	ClientToScreen(&point);
//	OnContextMenu(this, point);

// Si fullscreen afegir menú per a restaurar Pantalla Normal en posició del cursor.
	if (fullscreen) {
		ClientToScreen(&point);
		if (ContextMenu->GetSubMenu(0)){
			CMenu *pSubMenu = ContextMenu->GetSubMenu(0);

			if (fullscreen)	pSubMenu->CheckMenuItem(ID_VISTA_FULLSCREEN, MF_CHECKED);
			else pSubMenu->CheckMenuItem(ID_VISTA_FULLSCREEN, MF_UNCHECKED);
			pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
				point.x, point.y, this);
		}
	}
}


// OnMouseMove: Funció que es crida quan es mou el mouse. La utilitzem per la 
//				  Visualització Interactiva amb les tecles del mouse apretades per 
//				  modificar els paràmetres de P.V. (R,angleh,anglev) segons els 
//				  moviments del mouse.
//      PARAMETRES: - nFlags: Flags que controlen si el botó es apretat o no.
//					- point: Estructura (x,y) que dóna la posició del mouse 
//							 (coord. pantalla) quan el botó s'ha apretat.
void CEntornV3DView::OnMouseMove(UINT nFlags, CPoint point)
{
// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
	float modul = 0;
	GLfloat vdir[3] = { 0, 0, 0 };

// TODO: Add your message handler code here and/or call default
	if (m_ButoEAvall && mobil && projeccio != CAP)
	{	
// Entorn V3D: Determinació dels angles (en graus) segons l'increment
//				horitzontal i vertical de la posició del mouse.
		CSize gir = m_PosEAvall - point;
		m_PosEAvall = point;
		OPV.beta = OPV.beta - gir.cx / 2;
		OPV.alfa = OPV.alfa + gir.cy / 2;

// Entorn V3D: Control per evitar el creixement desmesurat dels angles.
		if (OPV.alfa >= 360)	OPV.alfa = OPV.alfa - 360;
		if (OPV.alfa<0)		OPV.alfa = OPV.alfa + 360;
		if (OPV.beta >= 360)	OPV.beta = OPV.beta - 360;
		if (OPV.beta<0)		OPV.beta = OPV.beta + 360;
		InvalidateRect(NULL, false);
	}
	else if (m_ButoEAvall && camera == CAM_NAVEGA && (projeccio != CAP && projeccio != ORTO)) // Opció Navegació
	{
// Entorn V3D: Canviar orientació en opció de Navegació
		CSize girn = m_PosEAvall - point;
		angleZ = girn.cx / 2.0;
// Entorn V3D: Control per evitar el creixement desmesurat dels angles.
		if (angleZ >= 360) angleZ = angleZ - 360;
		if (angleZ<0)	angleZ = angleZ + 360;

		n[0] = n[0] - opvN.x;
		n[1] = n[1] - opvN.y;
		n[0] = n[0] * cos(angleZ*pi / 180) - n[1] * sin(angleZ*pi / 180);
		n[1] = n[0] * sin(angleZ*pi / 180) + n[1] * cos(angleZ*pi / 180);
		n[0] = n[0] + opvN.x;
		n[1] = n[1] + opvN.y;

		m_PosEAvall = point;
		InvalidateRect(NULL, false);
	}

// Entorn V3D: Transformació Geomètrica interactiva pels eixos X,Y boto esquerra del mouse.
	else {
		bool transE = transX || transY;
		if (m_ButoEAvall && transE && transf)
		{
			// Calcular increment
			CSize girT = m_PosEAvall - point;
			if (transX)
			{	long int incrT = girT.cx;
				if (trasl)
				{	TG.VTras.x += incrT*fact_Tras;
					if (TG.VTras.x<-100000) TG.VTras.x = 100000;
					if (TG.VTras.x>100000) TG.VTras.x = 100000;
				}
				else if (rota)
				{	TG.VRota.x += incrT*fact_Rota;
					while (TG.VRota.x >= 360) TG.VRota.x -= 360;
					while (TG.VRota.x<0) TG.VRota.x += 360;
				}
				else if (escal)
				{	if (incrT<0) incrT = -1 / incrT;
					TG.VScal.x = TG.VScal.x*incrT;
					if (TG.VScal.x<0.25) TG.VScal.x = 0.25;
					if (TG.VScal.x>8192) TG.VScal.x = 8192;
				}
			}
			if (transY)
			{	long int incrT = girT.cy;
				if (trasl)
				{	TG.VTras.y += incrT*fact_Tras;
					if (TG.VTras.y<-100000) TG.VTras.y = 100000;
					if (TG.VTras.y>100000) TG.VTras.y = 100000;
				}
				else if (rota)
				{	TG.VRota.y += incrT*fact_Rota;
					while (TG.VRota.y >= 360) TG.VRota.y -= 360;
					while (TG.VRota.y<0) TG.VRota.y += 360;
				}
				else if (escal)
				{	if (incrT <= 0) {	if (incrT >= -2) incrT = -2;
										incrT = 1 / Log2(-incrT);
									}
					  else incrT = Log2(incrT);
					TG.VScal.y = TG.VScal.y*incrT;
					if (TG.VScal.y<0.25) TG.VScal.y = 0.25;
					if (TG.VScal.y>8192) TG.VScal.y = 8192;
				}
			}
			m_PosEAvall = point;
			InvalidateRect(NULL, false);
		}
	}

// Entorn V3D: Determinació del desplaçament del pan segons l'increment
//				vertical de la posició del mouse (tecla dreta apretada).
	if (m_ButoDAvall && pan && (projeccio != CAP && projeccio != ORTO))
	{
		CSize zoomincr = m_PosDAvall - point;
		long int incrx = zoomincr.cx;
		long int incry = zoomincr.cy;

// Desplaçament pan vertical
		tr_cpv.y -= incry*fact_pan;
		if (tr_cpv.y>100000) tr_cpv.y = 100000;
		  else if (tr_cpv.y<-100000) tr_cpv.y = -100000;

// Desplaçament pan horitzontal
		tr_cpv.x += incrx*fact_pan;
		if (tr_cpv.x>100000) tr_cpv.x = 100000;
		  else if (tr_cpv.x<-100000) tr_cpv.x = -100000;

		m_PosDAvall = point;
		InvalidateRect(NULL, false);
	}
// Determinació del paràmetre R segons l'increment
//   vertical de la posició del mouse (tecla dreta apretada)
	else if (m_ButoDAvall && (zzoom || zzoomO) && (projeccio != CAP))
	{	CSize zoomincr = m_PosDAvall - point;
		long int incr = zoomincr.cy / 1.0;

		OPV.R = OPV.R + incr;
		if (OPV.R < 0.125) OPV.R = 0.125;
		m_PosDAvall = point;
		if (camera == CAM_LOXODROMA_TDARBOUX || camera == CAM_LOXODROMA_TFRENET) 
						opvN = Punt_Loxodroma(OPV.R, a_CamLoxo, cam_t);	// Posició càmera Loxodroma segons esfera radi OPV.R.
		InvalidateRect(NULL, false);
	}
	else if (m_ButoDAvall && camera == CAM_NAVEGA && (projeccio != CAP && projeccio != ORTO))
	{	// Avançar en opció de Navegació
		if (m_PosDAvall != point)
		{	CSize zoomincr = m_PosDAvall - point;

			float incr = zoomincr.cy / 2;
//			long int incr=zoomincr.cy/2.0;  // Causa assertion en "afx.inl" lin 169
			vdir[0] = n[0] - opvN.x;
			vdir[1] = n[1] - opvN.y;
			vdir[2] = n[2] - opvN.z;
			modul = sqrt(vdir[0] * vdir[0] + vdir[1] * vdir[1] + vdir[2] * vdir[2]);
			vdir[0] = vdir[0] / modul;
			vdir[1] = vdir[1] / modul;
			vdir[2] = vdir[2] / modul;
			opvN.x += incr*vdir[0];
			opvN.y += incr*vdir[1];
			n[0] += incr*vdir[0];
			n[1] += incr*vdir[1];
			m_PosDAvall = point;
			InvalidateRect(NULL, false);
		}
	}

// Entorn V3D: Transformació Geomètrica interactiva per l'eix Z amb boto dret del mouse.
	else if (m_ButoDAvall && transZ && transf)
	{
		// Calcular increment
		CSize girT = m_PosDAvall - point;
		long int incrT = girT.cy;
		if (trasl)
		{	TG.VTras.z += incrT*fact_Tras;
			if (TG.VTras.z<-100000) TG.VTras.z = 100000;
			if (TG.VTras.z>100000) TG.VTras.z = 100000;
		}
		else if (rota)
		{	incrT = girT.cx;
			TG.VRota.z += incrT*fact_Rota;
			while (TG.VRota.z >= 360) TG.VRota.z -= 360;
			while (TG.VRota.z<0) TG.VRota.z += 360;
		}
		else if (escal)
		{	if (incrT <= 0) {	if (incrT >= -2) incrT = -2;
								incrT = 1 / Log2(-incrT);
							}
			  else incrT = Log2(incrT);
			TG.VScal.z = TG.VScal.z*incrT;
			if (TG.VScal.z<0.25) TG.VScal.z = 0.25;
			if (TG.VScal.z>8192) TG.VScal.z = 8192;
		}

		m_PosDAvall = point;
		InvalidateRect(NULL, false);
	}

// Do not call CView::OnPaint() for painting messages
	CView::OnMouseMove(nFlags, point);
}

// OnMouseWheel: Funció que es crida quan es mou el rodet del mouse. La utilitzem per la 
//				  Visualització Interactiva per modificar el paràmetre R de P.V. (R,angleh,anglev) 
//				  segons el moviment del rodet del mouse.
//      PARAMETRES: - nFlags: Flags que controlen si el botó es apretat o no.
//					- zDelta: Unitats de desplaçament del rodet del mouse.
//					- pt: Estructura (x,y) que dóna la posició del mouse 
//							 (coord. pantalla) quan el botó s'ha apretat.
BOOL CEntornV3DView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
	float modul = 0;
	GLfloat vdir[3] = { 0, 0, 0 };

	// Funció de zoom quan està activada la funció pan o les T. Geomètriques
	if ((zzoomO) || (zzoom) || (transX) || (transY) || (transZ))
	{
		OPV.R = OPV.R + zDelta / 4;
		if (OPV.R < 0.125) OPV.R = 0.125;
		if (camera == CAM_LOXODROMA_TFRENET || CAM_LOXODROMA_TDARBOUX)
					opvN = Punt_Loxodroma(OPV.R, a_CamLoxo, cam_t);	// Posició càmera Loxodroma segons esfera radi OPV.R.
		InvalidateRect(NULL, false);
	}
	else if (camera == CAM_NAVEGA)
	{	vdir[0] = n[0] - opvN.x;
		vdir[1] = n[1] - opvN.y;
		vdir[2] = n[2] - opvN.z;
		modul = sqrt(vdir[0] * vdir[0] + vdir[1] * vdir[1] + vdir[2] * vdir[2]);
		vdir[0] = vdir[0] / modul;
		vdir[1] = vdir[1] / modul;
		vdir[2] = vdir[2] / modul;
		opvN.x += (zDelta / 4)*vdir[0];
		opvN.y += (zDelta / 4)*vdir[1];
		n[0] += (zDelta / 4)*vdir[0];
		n[1] += (zDelta / 4)*vdir[1];
		InvalidateRect(NULL, false);
	}
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


/* ------------------------------------------------------------------------- */
/*					     TIMER (ANIMACIÓ)									 */
/* ------------------------------------------------------------------------- */
void CEntornV3DView::OnTimer(UINT_PTR nIDEvent)
{
// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
// EntornV3D. QUATERNIONS: Animació Objecte Rígid per Quaternions
	if (satelit)	{	// OPCIÓ SATÈLIT: Increment OPV segons moviments mouse.
							OPV.alfa = OPV.alfa + m_EsfeIncEAvall.alfa;
							while (OPV.alfa > 360) OPV.alfa = OPV.alfa - 360;	while (OPV.alfa < 0) OPV.alfa = OPV.alfa + 360;
							OPV.beta = OPV.beta + m_EsfeIncEAvall.beta;
							while (OPV.beta > 360) OPV.beta = OPV.beta - 360;	while (OPV.beta < 0) OPV.beta = OPV.beta + 360;

							// Crida a OnPaint() per redibuixar l'escena
							InvalidateRect(NULL, false);
					}
 // EntornV3D. QUATERNIONS: Animació Objecte Rígid per Quaternions
	else if (rotaQ && animaQ && np_InQ > 1) {	// CAL FER: Càlcul del nou quaternió per a l'orientació de l'objecte.
			if (t > 1) {

				if (adelante && index < np_InQ - 1) {
					qI = keyf_Quat[index];
					qF = keyf_Quat[index + 1];
					index++;
					t = 0;
				}
				else if (!adelante && index > 0) {
					qI = keyf_Quat[index];
					qF = keyf_Quat[index - 1];
					index--;
					t = 0;
				}

				if (index >= np_InQ - 1) {
					adelante = false;
					index = np_InQ - 1;
				}
				else if (index <= 0) {
					adelante = true;
					index = 0;
				}

			}
			
			switch (interpolacio) {
				case ESFERICA:
					QuatSlerp(qI, qF, t, qT);
					break;

				case LINEAL:
					QuatLerp(qI, qF, t, qT);
					break;
			}

			t += temps;

			QuatToMatrix(qT, QMatrix);

			// Crida a OnPaint() per redibuixar l'escena
			InvalidateRect(NULL, false);
	}
 // EntornV3D. LOXODROMA: Animació de les càmeres associades als Triedres de Darboux i Frenet

	else if ((camera == CAM_LOXODROMA_TDARBOUX) && animaCamLxd)
	{//PASSOS:
		//1) Actualitzar paràmetre t de la corba (cam_t) i verificar límits per a que animació sigui sense fi.
		if (cam_t > 15.0) {
			palante = false;
			cam_t = 15.0;
		}
		else if (cam_t < -15.0) {
			palante = true;
			cam_t = -15.0;
		}


		opvN = Punt_Loxodroma(OPV.R, 0.125, cam_t);
		vCam = Prod_Vectorial(VT_TDLoxodroma(OPV.R, 0.125, cam_t), VNP_TDLoxodroma(OPV.R, 0.125, cam_t));


		gluLookAt(opvN.x, opvN.y, opvN.z, 0, 0, 0, vCam.x, vCam.y, vCam.z);


		if (palante) {
			cam_t += pas_CS;
		}
		else {
			cam_t -= pas_CS;
		}

		// Crida a OnPaint() per redibuixar l'escena
		InvalidateRect(NULL, false);
	}
	else if ((camera == CAM_LOXODROMA_TFRENET) && animaCamLxd)
	{//PASSOS:
		//1) Actualitzar paràmetre t de la corba (cam_t) i verificar límits per a que animació sigui sense fi.
		//2) Calcular posició de la càmera (opvN) segons corba Loxodroma amb paràmetre t (cam_t)
		//3) Calcular vectors n (nCam) i v (vCam) de la càmera segons el Triedre de Frenet.

		// Crida a OnPaint() per redibuixar l'escena
		if (cam_t > 15.0) {
			palante = false;
			cam_t = 15.0;
		}
		else if (cam_t < -15.0) {
			palante = true;
			cam_t = -15.0;
		}


		opvN = Punt_Loxodroma(OPV.R, 0.125, cam_t);
		//vCam = Prod_Vectorial(VT_TFLoxodroma(OPV.R, 0.125, cam_t), VBN_TFLoxodroma(OPV.R, 0.125, cam_t));
		vCam = VBN_TFLoxodroma(OPV.R, 0.125, cam_t);
		gluLookAt(opvN.x, opvN.y, opvN.z, 0, 0, 0, vCam.x, vCam.y, vCam.z);


		if (palante) {
			cam_t += pas_CS;
		}
		else {
			cam_t -= pas_CS;
		}
		InvalidateRect(NULL, false);
	}

	CView::OnTimer(nIDEvent);
}


/* ------------------------------------------------------------------------- */
/*   RECURSOS DE MENU (persianes) DE L'APLICACIO:                            */
/*					1. ARXIUS												 */
/*					4. CÀMERA: Esfèrica (Mobil, Zoom, ZoomO, Satelit), Navega*/
/*					5. VISTA: Pantalla Completa, Pan i Eixos	             */
/*					6. PROJECCIÓ                                             */
/*					7. OBJECTE					                             */
/*					8. TRANSFORMA											 */
/*					9. OCULTACIONS											 */
/*				   10. IL.LUMINACIÓ											 */
/*				   11. LLUMS												 */
/*				   12. SHADERS												 */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/*					1. ARXIUS 												 */
/* ------------------------------------------------------------------------- */

// Obrir fitxer Fractal
void CEntornV3DView::OnArxiuObrirFractal()
{
// TODO: Agregue aquí su código de controlador de comandos
// Entorn V3D: Obrir diàleg de lectura de fitxer (fitxers (*.MNT)
	CFileDialog openMunt(TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("MNT Files(*.mnt)|*.mnt|Error Files (*.err)|*err|All Files (*.*)|*.*||"));;

	if (openMunt.DoModal() != IDOK)
		return;                 // stay with old data file
	else nom = openMunt.GetPathName();

// Entorn V3D: Variable de tipus CString 'nom' conté el nom del fitxer seleccionat

// Entorn V3D: Conversió de la variable CString nom a la variable char *nomfitx, 
//		compatible amb  les funcions de càrrega de fitxers fractals
	char *nomfitx = CString2Char(nom);

// Entorn V3D: Variable de tipus char *nomfitx conté el nom del fitxer seleccionat


// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


// OnArchivoObrirFitxerObj: Obrir fitxer en format gràfic OBJ
void CEntornV3DView::OnArxiuObrirFitxerObj()
{
// TODO: Agregue aquí su código de controlador de comandos
//if (ObOBJ != NULL) delete ObOBJ;

	objecte = OBJOBJ;	textura = true;

// Entorn V3D: Obrir diàleg de lectura de fitxer
	CFileDialog openOBJ(TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("OBJ Files(*.obj)|*.obj|Error Files (*.err)|*err|All Files (*.*)|*.*||"));;

	if (openOBJ.DoModal() != IDOK)	return;  // stay with old data file
	else nom = openOBJ.GetPathName();

// Entorn V3D: Variable de tipus CString 'nom' conté el nom del fitxer seleccionat

// Entorn V3D: Conversió de la variable CString nom a la variable char *nomfitx, 
//		compatible amb  les funcions de càrrega de fitxers fractals
	char *nomfitx = CString2Char(nom);

// i carreguem
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);	// Activem contexte OpenGL

	if (ObOBJ == NULL) ObOBJ = new COBJModel;
	ObOBJ->LoadModel(nomfitx, OBJECTEOBJ,false);	// Carregar objecte OBJ SENSE textura
	ObOBJ->LoadModel(nomfitx, OBJECTEOBJT,true);	// Carregar objecte OBJ AMB textura

	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);	// Desactivem contexte OpenGL

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);

}

// Obrir fitxer en format gràfic 3DS
void CEntornV3DView::OnArxiuObrirFitxer3ds()
{
// TODO: Agregue aquí su código de controlador de comandos
	//if(Ob3DS!=NULL) delete Ob3DS;

	objecte = OBJ3DS;	textura = true;

// Entorn V3D: Obrir diàleg de lectura de fitxer
	CFileDialog open3DS(TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("3DS Files(*.3ds)|*.3ds|Error Files (*.err)|*err|All Files (*.*)|*.*||"));;

	if (open3DS.DoModal() != IDOK)	return;  // stay with old data file
	else nom = open3DS.GetPathName();

// Entorn V3D: Variable de tipus CString 'nom' conté el nom del fitxer seleccionat

// Entorn V3D: Conversió de la variable CString nom a la variable char *nomfitx, 
//		compatible amb  les funcions de càrrega de fitxers fractals
	char *nomfitx = CString2Char(nom);

// i carreguem
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC); // Activem contexte OpenGL

	if (Ob3DS == NULL) Ob3DS = new Obj_3DS;
	Ob3DS->EliminarMemoria();
	Ob3DS->Inicialitzar();
	Ob3DS->Carregar3DS(nomfitx);

// Precompilació de dos objectes nous: OBJECTE3DS: Objecte 3DS sense textures i OBJECTE3DST amb textures,
	Ob3DS->Dibuixa3DS(false, OBJECTE3DS, false);
	Ob3DS->Dibuixa3DS(false, OBJECTE3DST, true);

	wglMakeCurrent(m_pDC->GetSafeHdc(), NULL); // Desactivem contexte OpenGL

//  Entorn V3D: Modificar R per centrar Vista amb mida de l'objecte
	mida = sqrtf(3.0) * 10;
	OPV.R = 0.5*mida / sin(30 * pi / 180) + p_near;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);

}

/* ------------------------------------------------------------------------- */
/*					4. CÀMERA: Esfèrica (Mobil, Zoom, ZoomO, Satelit), Navega*/
/* ------------------------------------------------------------------------- */
// CÀMERA: Mode Esfèrica (Càmera esfèrica en polars-opció booleana)
void CEntornV3DView::OnCameraEsferica()
{
// TODO: Agregue aquí su código de controlador de comandos
	camera = CAM_ESFERICA;

// Inicialitzar paràmetres Càmera Esfèrica
	OPV.R = 15.0;		OPV.alfa = 0.0;		OPV.beta = 0.0;				// Origen PV en esfèriques
	mobil = true;	zzoom = true;	zzoomO = false;		satelit = false;
	Vis_Polar = POLARZ;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateCameraEsferica(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (camera == CAM_ESFERICA) 	pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


// VISTA: Mobil. Punt de Vista Interactiu (opció booleana)
void CEntornV3DView::OnVistaMobil()
{
// TODO: Agregue aquí su código de controlador de comandos
	if ((projeccio != ORTO) || (projeccio != CAP) && (camera == CAM_ESFERICA)) mobil = !mobil;
// Desactivació de Transformacions Geomètriques via mouse 
//		si Visualització Interactiva activada.	
	if (mobil) {
		transX = false;	transY = false; transZ = false;
	}

	// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

// CÀMERA--> ESFERICA: Mobil. Punt de Vista Interactiu (opció booleana)
void CEntornV3DView::OnUpdateVistaMobil(CCmdUI* pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if ((camera == CAM_ESFERICA) && (mobil)) 	pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

// CÀMERA--> ESFERICA: Zoom. Zoom Interactiu (opció booleana)
void CEntornV3DView::OnVistaZoom()
{
// TODO: Agregue aquí su código de controlador de comandos
	if ((projeccio != ORTO) || (projeccio != CAP) && (camera == CAM_ESFERICA)) zzoom = !zzoom;
// Desactivació de Transformacions Geomètriques via mouse 
//		si Zoom activat.
	if (zzoom) {	zzoomO = false; 
					transX = false;	transY = false;	transZ = false;
				}

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateVistaZoom(CCmdUI* pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if ((camera == CAM_ESFERICA) && (zzoom)) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


// CÀMERA--> ESFERICA: Satèlit. Vista interactiva i animada en que increment de movimen és activat per mouse (opció booleana)
void CEntornV3DView::OnVistaSatelit()
{
	// TODO: Agregue aquí su código de controlador de comandos
	if ((projeccio == AXONOM || projeccio == PERSPECT) && (camera == CAM_ESFERICA)) satelit = !satelit;
	if (satelit) mobil = true;
	bool testA = animaCamLxd;									// Testejar si hi ha alguna animació activa apart de Satèlit.
	if ((!satelit) && (!testA)) KillTimer(WM_TIMER);	// Si es desactiva Satèlit i no hi ha cap animació activa es desactiva el Timer.

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateVistaSatelit(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if ((camera == CAM_ESFERICA) && (satelit)) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


// CÀMERA--> ESFERICA: Polars Eix X cap amunt per a Visualització Interactiva
void CEntornV3DView::OnVistaPolarsX()
{
	// TODO: Agregue aquí su código de controlador de comandos
	if ((projeccio != CAP) && (camera == CAM_ESFERICA)) Vis_Polar = POLARX;

	// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateVistaPolarsX(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if ((camera == CAM_ESFERICA) && (Vis_Polar == POLARX))  pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


// CÀMERA--> ESFERICA: Polars Eix Y cap amunt per a Visualització Interactiva
void CEntornV3DView::OnVistaPolarsY()
{
// TODO: Agregue aquí su código de controlador de comandos
	if ((projeccio != CAP) && (camera == CAM_ESFERICA)) Vis_Polar = POLARY;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateVistaPolarsY(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if ((camera == CAM_ESFERICA) && (Vis_Polar == POLARY))  pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


// CÀMERA--> ESFERICA: Polars Eix Z cap amunt per a Visualització Interactiva
void CEntornV3DView::OnVistaPolarsZ()
{
// TODO: Agregue aquí su código de controlador de comandos
	if ((projeccio != CAP) && (camera == CAM_ESFERICA)) Vis_Polar = POLARZ;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateVistaPolarsZ(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if ((camera == CAM_ESFERICA) && (Vis_Polar == POLARZ))  pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


// CÀMERA: Mode NAVEGA. Navegació sobre un pla amb botons de teclat o de mouse (nav) (opció booleana)
void CEntornV3DView::OnVistaNavega()
{
// TODO: Agregue aquí su código de controlador de comandos
	if (projeccio == PERSPECT || projeccio == AXONOM) camera = CAM_NAVEGA;

// Desactivació de zoom, mobil, Transformacions Geomètriques via mouse i pan 
//		si navega activat
	mobil = false;	zzoom = false;
	transX = false;	transY = false;	transZ = false;
	//pan = false;
	tr_cpv.x = 0.0;		tr_cpv.y = 0.0;		tr_cpv.z = 0.0;	// Inicialitzar a 0 desplaçament de pantalla
	tr_cpvF.x = 0.0;	tr_cpvF.y = 0.0;	tr_cpvF.x = 0.0; // Inicialitzar a 0 desplaçament de pantalla

// Incialitzar variables Navega
	n[0] = 0.0;		n[1] = 0.0;		n[2] = 0.0;
	opvN.x = 10.0;	opvN.y = 0.0;		opvN.z = 0.0;
	angleZ = 0.0;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateVistaNavega(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (camera == CAM_NAVEGA) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

// Tornar a lloc d'origen
void CEntornV3DView::OnVistaOrigennavega()
{
	// TODO: Agregue aquí su código de controlador de comandos
	if (camera == CAM_NAVEGA) {	n[0] = 0.0;		n[1] = 0.0;		n[2] = 0.0;
								opvN.x = 10.0;	opvN.y = 0.0;		opvN.z = 0.0;
								angleZ = 0.0;
	}

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


/* -------------------------------------------------------------------------------- */
/*					5. VISTA: Pantalla Completa, Pan i Eixos	                    */
/* -------------------------------------------------------------------------------- */
// VISTA: FullScreen (Pantalla Completa-opció booleana)
void CEntornV3DView::OnVistaFullscreen()
{
// TODO: Agregue aquí su código de controlador de comandos

	if (!fullscreen)
		{	// I note that I go to full-screen mode
			fullscreen = true;
			// Remembers the address of the window in which the view was placed (probably a frame)
			saveParent = this->GetParent();
			// Assigns a view to a new parent - desktop
			this->SetParent(GetDesktopWindow());
			CRect rect; // It's about the dimensions of the desktop-desktop
			GetDesktopWindow()->GetWindowRect(&rect);
			// I set the window on the desktop
			MoveWindow(rect);
		}
		else {	// Switching off the full-screen mode
				fullscreen = false;
				// Assigns an old parent view
				this->SetParent(saveParent);
				CRect rect; // It's about the dimensions of the desktop-desktop
				// Get client screen dimensions
				saveParent->GetClientRect(&rect);
				// Changes the position and dimensions of the specified window.
				MoveWindow(rect,FALSE);
			}

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateVistaFullscreen(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (fullscreen) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// VISTA: Mode de Desplaçament horitzontal i vertical per pantalla del Punt de Vista (pan) (opció booleana)
void CEntornV3DView::OnVistaPan()
{
// TODO: Agregue aquí su código de controlador de comandos
	if ((projeccio != ORTO) || (projeccio != CAP)) pan = !pan;
// Desactivació de Transformacions Geomètriques via mouse i navega si pan activat
	if (pan)	{	mobil = true;		zzoom = true;
					transX = false;	transY = false;	transZ = false;
					//navega = false;
				}

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateVistaPan(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (pan) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}

// Tornar a lloc d'origen
void CEntornV3DView::OnVistaOrigenpan()
{
// TODO: Agregue aquí su código de controlador de comandos
	if (pan) {	fact_pan = 1;
				tr_cpv.x = 0;	tr_cpv.y = 0;	tr_cpv.z = 0;
			}

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}



// VISTA: Visualitzar eixos coordenades món (opció booleana)
void CEntornV3DView::OnVistaEixos()
{
// TODO: Agregue aquí su código de controlador de comandos
	eixos = !eixos;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateVistaEixos(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (eixos) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}



void CEntornV3DView::OnVistaGridXY()
{
// TODO: Agregue aquí su código de controlador de comandos
	grid.x = !grid.x;	hgrid.x = 0.0;
	if (grid.x) grid.w = false;
	
// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateVistaGridXY(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (grid.x) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


void CEntornV3DView::OnVistaGridXZ()
{
// TODO: Agregue aquí su código de controlador de comandos
	grid.y = !grid.y;	hgrid.y = 0.0;
	if (grid.y) grid.w = false;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateVistaGridXZ(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (grid.y) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


void CEntornV3DView::OnVistaGridYZ()
{
// TODO: Agregue aquí su código de controlador de comandos
	grid.z = !grid.z;	hgrid.z = 0.0;
	if (grid.z) grid.w = false;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateVistaGridYZ(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (grid.z) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


void CEntornV3DView::OnVistaGridXYZ()
{
// TODO: Agregue aquí su código de controlador de comandos
	grid.w = !grid.w;	hgrid.w = 0.0;
	if (grid.w)
		{	grid.x = false;	grid.y = false;		grid.z = false;
		}


// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateVistaGridXYZ(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (grid.w) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);

}


/* ------------------------------------------------------------------------- */
/*					6. PROJECCIÓ                                             */
/* ------------------------------------------------------------------------- */

// PROJECCIÓ: Perspectiva
void CEntornV3DView::OnProjeccioPerspectiva()
{
// TODO: Agregue aquí su código de controlador de comandos
	projeccio = PERSPECT;
	mobil = true;			zzoom = true;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);

}

void CEntornV3DView::OnUpdateProjeccioPerspectiva(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (projeccio == PERSPECT) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


/* ------------------------------------------------------------------------- */
/*					7. OBJECTE					                             */
/* ------------------------------------------------------------------------- */

// OBJECTE: Cub
void CEntornV3DView::OnObjecteCub()
{
// TODO: Agregue aquí su código de controlador de comandos

	objecte = CUB;

//  Modificar R per centrar la Vista a la mida de l'objecte (Perspectiva)
//	Canviar l'escala per a centrar la vista (Ortogràfica)


// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateObjecteCub(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == CUB) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// OBJECTE: Cub RGB
void CEntornV3DView::OnObjecteCubRGB()
{
// TODO: Agregue aquí su código de controlador de comandos
	objecte = CUB_RGB;

//  Modificar R per centrar la Vista a la mida de l'objecte (Perspectiva)
//	Canviar l'escala per a centrar la vista (Ortogràfica)

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateObjecteCubRGB(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == CUB_RGB) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


// OBJECTE Esfera
void CEntornV3DView::OnObjecteEsfera()
{
// TODO: Agregue aquí su código de controlador de comandos
	objecte = ESFERA;

//  Modificar R per centrar la Vista a la mida de l'objecte (Perspectiva)
//	Canviar l'escala per a centrar la vista (Ortogràfica)

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateObjecteEsfera(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == ESFERA) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}

// OBJECTE Tetera
void CEntornV3DView::OnObjecteTetera()
{
// TODO: Agregue aquí su código de controlador de comandos
	objecte = TETERA;

//  Modificar R per centrar la Vista a la mida de l'objecte (Perspectiva)
//	Canviar l'escala per a centrar la vista (Ortogràfica)

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateObjecteTetera(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == TETERA) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// OBJECTE Truck
void CEntornV3DView::OnObjecteTruck()
{
// TODO: Agregue aquí su código de controlador de comandos
	objecte = TRUCK;		textura = true;

	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);	// Activem contexte OpenGL
// Inicialitza vector imatges textura pel Truck.
	texturesID[1] = loadIMA_ILUT("./textures/vent.bmp");

	texturesID[2] = loadIMA_ILUT("./textures/plat.bmp");

	texturesID[3] = loadIMA_ILUT("./textures/reixeta.bmp");

	texturesID[4] = loadIMA_ILUT("./textures/fars.bmp");

	texturesID[5] = loadIMA_ILUT("./textures/txapa.bmp");

	texturesID[6] = loadIMA_ILUT("./textures/metall.bmp");
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);	// Desactivem contexte OpenGL

//  Modificar R per centrar la Vista a la mida de l'objecte (Perspectiva)
//	Canviar l'escala per a centrar la vista (Ortogràfica)

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateObjecteTruck(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == TRUCK) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// OBJECTE Tie
void CEntornV3DView::OnObjecteTie()
{
// TODO: Agregue aquí su código de controlador de comandos
	objecte = TIE;

//  Modificar R per centrar la Vista a la mida de l'objecte (Perspectiva)
//	Canviar l'escala per a centrar la vista (Ortogràfica)

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateObjecteTie(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == TIE) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}

// ----------------- OBJECTES CORBES BEZIER, LEMNISCATA i B-SPLINE
// OBJECTE Corba Bezier
void CEntornV3DView::OnObjeteCorbaBezier()
{
// TODO: Agregue aquí su código de controlador de comandos
	nom = "";
	objecte = C_BEZIER;

// Entorn V3D: Obrir diàleg de lectura de fitxer (fitxers (*.crv)
	CFileDialog openSpline(TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("SPL Files(*.crv)|*.crv|All Files (*.*)|*.*||"));;

	if (openSpline.DoModal() != IDOK)
		return;                 // stay with old data file
	else nom = openSpline.GetPathName();

// Entorn V3D: Variable de tipus CString 'nom' conté el nom del fitxer seleccionat

// Entorn V3D: Conversió de la variable CString nom a la variable char *nomfitx, 
//		compatible amb  les funcions de càrrega de fitxers fractals
	char *nomfitx = CString2Char(nom);

	npts_T = llegir_ptsC(nomfitx);

// 	---- Entorn V3D: Modificar R per centrar Vista amb mida de l'objecte

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateObjeteCorbaBezier(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == C_BEZIER) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// OBJECTE Corba Lemniscata 3D
void CEntornV3DView::OnObjecteCorbaLemniscata()
{
// TODO: Agregue aquí su código de controlador de comandos
	objecte = C_LEMNISCATA;

// 	---- Entorn V3D: Modificar R per centrar Vista amb mida de l'objecte

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateObjecteCorbaLemniscata(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == C_LEMNISCATA) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// OBJECTE Corba B-Spline
void CEntornV3DView::OnObjecteCorbaBSpline()
{
// TODO: Agregue aquí su código de controlador de comandos
	nom = "";
	objecte = C_BSPLINE;

// Entorn V3D: Obrir diàleg de lectura de fitxer (fitxers (*.crv)
	CFileDialog openSpline(TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("SPL Files(*.crv)|*.crv|All Files (*.*)|*.*||"));;

	if (openSpline.DoModal() != IDOK)
		return;                 // stay with old data file
	else nom = openSpline.GetPathName();

// Entorn V3D: Variable de tipus CString 'nom' conté el nom del fitxer seleccionat

// Entorn V3D: Conversió de la variable CString nom a la variable char *nomfitx, 
//		compatible amb  les funcions de càrrega de fitxers fractals
	char *nomfitx = CString2Char(nom);

	npts_T = llegir_ptsC(nomfitx);

// 	---- Entorn V3D: Modificar R per centrar Vista amb mida de l'objecte

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateObjecteCorbaBSpline(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == C_BSPLINE) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}

// OBJECTE --> CORBES: Activar o desactivar visualització Punts de control de les corbes Bezier i BSpline
void CEntornV3DView::OnObjectePuntsControl()
{
// TODO: Agregue aquí su código de controlador de comandos
	sw_Punts_Control = !sw_Punts_Control;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateObjectePuntsControl(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (sw_Punts_Control) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


// OBJECTE --> CORBES: Activar o desactivar visualització Triedre de Frenet Punts de control de les corbes 
//					Lemniscata, Loxodroma, Bezier i BSpline
void CEntornV3DView::OnCorbesTriedreFrenet()
{
// TODO: Agregue aquí su código de controlador de comandos
	dibuixa_TriedreFrenet = !dibuixa_TriedreFrenet;
	if (dibuixa_TriedreFrenet) dibuixa_TriedreDarboux = false;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateCorbesTriedreFrenet(CCmdUI* pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (dibuixa_TriedreFrenet) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}



// // OBJECTE Escena Loxodroma on es dibuixa un objecte OBJ al centre, una esfera i la corba Loxodroma
void CEntornV3DView::OnObjecteEscenaLoxodroma()
{
// TODO: Agregue aquí su código de controlador de comandos
	objecte = ESCENA_LOXODROMA;

// Entorn V3D: Obrir diàleg de lectura de fitxer
	CFileDialog openOBJ(TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("OBJ Files(*.obj)|*.obj|Error Files (*.err)|*err|All Files (*.*)|*.*||"));;

	if (openOBJ.DoModal() != IDOK)	return;  // stay with old data file
	else nom = openOBJ.GetPathName();
	// Entorn V3D: Variable de tipus CString 'nom' conté el nom del fitxer seleccionat

	// Entorn V3D: Conversió de la variable CString nom a la variable char *nomfitx, 
	//		compatible amb  les funcions de càrrega de fitxers fractals
	char* nomfitx = CString2Char(nom);

	// i carreguem
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);	// Activem contexte OpenGL

	if (ObOBJ == NULL) ObOBJ = new COBJModel;
	ObOBJ->LoadModel(nomfitx, OBJECTEOBJ, false);	// Carregar objecte OBJ SENSE textura
	ObOBJ->LoadModel(nomfitx, OBJECTEOBJT, true);	// Carregar objecte OBJ AMB textura

	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);	// Desactivem contexte OpenGL

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateObjecteEscenaLoxodroma(CCmdUI* pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte==ESCENA_LOXODROMA) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


/* ------------------------------------------------------------------------- */
/*					8. TRANSFORMA											 */
/* ------------------------------------------------------------------------- */

// TRANSFORMA: TRASLACIÓ
void CEntornV3DView::OnTransformaTraslacio()
{
// TODO: Agregue aquí su código de controlador de comandos
	trasl = !trasl;
	rota = false;
	if (trasl) escal = true;
	transf = trasl || rota || escal;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateTransformaTraslacio(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (trasl) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


void CEntornV3DView::OnTransformaOrigentraslacio()
{
// TODO: Agregue aquí su código de controlador de comandos
	if (trasl)
	{	fact_Tras = 1;
		TG.VTras.x = 0.0;	TG.VTras.y = 0.0;	TG.VTras.z = 0;
	}

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


// TRANSFORMA: ROTACIÓ
void CEntornV3DView::OnTransformaRotacio()
{
// TODO: Agregue aquí su código de controlador de comandos
	rota = !rota;
	trasl = false;
	if (rota) escal = true;
	transf = trasl || rota || escal;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateTransformaRotacio(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (rota) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


void CEntornV3DView::OnTransformaOrigenrotacio()
{
// TODO: Agregue aquí su código de controlador de comandos
	if (rota)
	{	fact_Rota = 90;
		TG.VRota.x = 0;		TG.VRota.y = 0;		TG.VRota.z = 0;
		qT.w = 1.0;			qT.x = 1.0;			qT.y = 0.0;		qT.z = 0.0;	// Inicialitzar Quaternió i matriu.
		QuatToMatrix(qT, QMatrix);
	}

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


// TRANSFORMA: ESCALAT
void CEntornV3DView::OnTransformaEscalat()
{
// TODO: Agregue aquí su código de controlador de comandos
	if ((!rota) && (!trasl)) escal = !escal;
	transf = trasl || rota || escal;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateTransformaEscalat(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (escal) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


void CEntornV3DView::OnTransformaOrigenescalat()
{
// TODO: Agregue aquí su código de controlador de comandos
	if (escal) { TG.VScal.x = 1;	TG.VScal.y = 1;	TG.VScal.z = 1; }

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


// TRANSFOMA: Mòbil Eix X? (opció booleana).
void CEntornV3DView::OnTransformaMobilx()
{
// TODO: Agregue aquí su código de controlador de comandos
	if (transf)
	{	transX = !transX;
		if (transX) {	mobil = false;	zzoom = false;
						pan = false;	//navega = false;
					}
			else if ((!transY) && (!transZ)){	mobil = true;
												zzoom = true;
											}
	}

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateTransformaMobilx(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (transX) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// TRANSFOMA: Mòbil Eix Y? (opció booleana).
void CEntornV3DView::OnTransformaMobily()
{
// TODO: Agregue aquí su código de controlador de comandos
	if (transf)
	{	transY = !transY;
		if (transY) {	mobil = false;	zzoom = false;
						pan = false;	//navega = false;
					}
			else if ((!transX) && (!transZ)){	mobil = true;
												zzoom = true;
											}
	}

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateTransformaMobily(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (transY) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// TRANSFOMA: Mòbil Eix Z? (opció booleana).
void CEntornV3DView::OnTransformaMobilz()
{
// TODO: Agregue aquí su código de controlador de comandos
	if (transf)
	{	transZ = !transZ;
		if (transZ) {	mobil = false;	zzoom = false;
						pan = false;	//navega = false;
					}
		else if ((!transX) && (!transY)) {	mobil = true;
											zzoom = true;
										}
	}

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateTransformaMobilz(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (transZ) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


/* ------------------------------------------------------------------------- */
/*					9. OCULTACIONS											 */
/* ------------------------------------------------------------------------- */
void CEntornV3DView::OnOcultacionsFrontFaces()
{
// TODO: Agregue aquí su código de controlador de comandos
	front_faces = !front_faces;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateOcultacionsFrontFaces(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (front_faces) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// OCULTACIONS: Test de Visibilitat? (opció booleana).
void CEntornV3DView::OnOcultacionsTestvis()
{
// TODO: Agregue aquí su código de controlador de comandos
	test_vis = !test_vis;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateOcultacionsTestvis(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (test_vis) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// OCULTACIONS: Z-Buffer? (opció booleana).
void CEntornV3DView::OnOcultacionsZbuffer()
{
// TODO: Agregue aquí su código de controlador de comandos
	oculta = !oculta;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateOcultacionsZbuffer(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (oculta) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// OCULTACIONS: Back Line? (opció booleana).
void CEntornV3DView::OnOcultacionsBackline()
{
// TODO: Agregue aquí su código de controlador de comandos
	back_line = !back_line;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateOcultacionsBackline(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (back_line) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


/* ------------------------------------------------------------------------- */
/*					10. IL.LUMINACIÓ										 */
/* ------------------------------------------------------------------------- */

// IL.LUMINACIÓ Font de llum fixe? (opció booleana).
void CEntornV3DView::OnIluminacioLlumfixe()
{
// TODO: Agregue aquí su código de controlador de comandos
	ifixe = !ifixe;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateIluminacioLlumfixe(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (ifixe) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// IL.LUMINACIÓ: Mantenir iluminades les Cares Front i Back
void CEntornV3DView::OnIluminacio2Sides()
{
// TODO: Agregue aquí su código de controlador de comandos
	ilum2sides = !ilum2sides;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateIluminacio2Sides(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (ilum2sides) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}



// ILUMINACIÓ PUNTS
void CEntornV3DView::OnIluminacioPunts()
{
// TODO: Agregue aquí su código de controlador de comandos
	ilumina = PUNTS;
	test_vis = false;		oculta = false;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateIluminacioPunts(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (ilumina == PUNTS) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


// ILUMINACIÓ FILFERROS
void CEntornV3DView::OnIluminacioFilferros()
{
// TODO: Agregue aquí su código de controlador de comandos
	ilumina = FILFERROS;
	test_vis = false;		oculta = false;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateIluminacioFilferros(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (ilumina == FILFERROS) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// ILUMINACIÓ PLANA
void CEntornV3DView::OnIluminacioPlana()
{
// TODO: Agregue aquí su código de controlador de comandos
	ilumina = PLANA;
	test_vis = false;		oculta = true;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);

}

void CEntornV3DView::OnUpdateIluminacioPlana(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (ilumina == PLANA) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// ILUMINACIÓ GOURAUD
void CEntornV3DView::OnIluminacioGouraud()
{
// TODO: Agregue aquí su código de controlador de comandos
	ilumina = GOURAUD;
	test_vis = false;		oculta = true;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);

}

void CEntornV3DView::OnUpdateIluminacioGouraud(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (ilumina == GOURAUD) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// ILUMINACIÓ->REFLECTIVITAT MATERIAL EMISSIÓ: Activació i desactivació de la reflectivitat pròpia del material.
void CEntornV3DView::OnMaterialEmissio()
{
// TODO: Agregue aquí su código de controlador de comandos
	sw_material[0] = !sw_material[0];

	if (shader_menu != CAP_SHADER) {
		// Entorn V3D: Activació del contexte OpenGL
		wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
		// Pas màscara llums
		glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[0]"), sw_material[0]);
		// Entorn V3D: Desactivació del contexte OpenGL. Permet la coexistencia d'altres contextes de generació
		wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
	}

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateMaterialEmissio(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (sw_material[0])	pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// ILUMINACIÓ->REFLECTIVITAT MATERIAL AMBIENT: Activació i desactivació de la reflectivitat ambient del material.
void CEntornV3DView::OnMaterialAmbient()
{
// TODO: Agregue aquí su código de controlador de comandos
	sw_material[1] = !sw_material[1];

	if (shader_menu != CAP_SHADER) {
		// Entorn V3D: Activació del contexte OpenGL
		wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
		// Pas màscara llums
		glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[1]"), sw_material[1]);
		// Entorn V3D: Desactivació del contexte OpenGL. Permet la coexistencia d'altres contextes de generació
		wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
	}

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateMaterialAmbient(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (sw_material[1])	pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// ILUMINACIÓ->REFLECTIVITAT MATERIAL DIFUSA: Activació i desactivació de la reflectivitat difusa del materials.
void CEntornV3DView::OnMaterialDifusa()
{
// TODO: Agregue aquí su código de controlador de comandos
	sw_material[2] = !sw_material[2];

	if (shader_menu != CAP_SHADER) {
		// Entorn V3D: Activació del contexte OpenGL
		wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
		// Pas màscara llums
		glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[2]"), sw_material[2]);
		// Entorn V3D: Desactivació del contexte OpenGL. Permet la coexistencia d'altres contextes de generació
		wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
	}

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateMaterialDifusa(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (sw_material[2])	pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// ILUMINACIÓ->REFLECTIVITAT MATERIAL ESPECULAR: Activació i desactivació de la reflectivitat especular del material.
void CEntornV3DView::OnMaterialEspecular()
{
// TODO: Agregue aquí su código de controlador de comandos
	sw_material[3] = !sw_material[3];

	if (shader_menu != CAP_SHADER) {
		// Entorn V3D: Activació del contexte OpenGL
		wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
		// Pas màscara llums
		glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[3]"), sw_material[3]);
		// Entorn V3D: Desactivació del contexte OpenGL. Permet la coexistencia d'altres contextes de generació
		wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
	}

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateMaterialEspecular(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (sw_material[3])	pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// ILUMINACIÓ: Textures?: Activació (TRUE) o desactivació (FALSE) de textures.
void CEntornV3DView::OnIluminacioTextures()
{
// TODO: Agregue aquí su código de controlador de comandos
	textura = !textura;

// Entorn V3D: Activació del contexte OpenGL
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

//	Pas de textura al shader
	if (shader_menu != CAP_SHADER) glUniform1i(glGetUniformLocation(shader_program, "texture"), textura);

// Entorn V3D: Desactivació del contexte OpenGL. Permet la coexistencia d'altres contextes de generació
	wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateIluminacioTextures(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (textura) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// IL.LUMINACIÓ->TEXTURA TEXTURES PREDEFINIDES
// TEXTURA Fusta
// TEXTURA Marbre
// TEXTURA Metall


void CEntornV3DView::OnIluminacioTexturaFitxerimatge()
{
// TODO: Agregue aquí su código de controlador de comandos
	CString nomf;
	t_textura = FITXERIMA;

// Obrir diàleg de lectura de fitxer
	CFileDialog openTextur(TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("JPG Files(*.jpg)|*.jpg|Error Files (*.err)|*err|All Files (*.*)|*.*||"));;

	if (openTextur.DoModal() != IDOK)	return;                 // stay with old data file
	else nomf = openTextur.GetPathName();

// Conversió de la variable CString nom a la variable char *nomfitx, 
//		compatible amb  les funcions de càrrega de fitxers textura
	char *nomfitx = CString2Char(nomf);

// Entorn V3D: Activació el contexte OpenGL
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	texturesID[0] = loadIMA_SOIL(nomfitx);

//	Pas de textura al shader
	if (shader_menu != CAP_SHADER) glUniform1i(glGetUniformLocation(shader_program, "texture0"), GLint(0));

// Desactivació contexte OpenGL: Permet la coexistencia d'altres contextes de generació
	wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateIluminacioTexturaFitxerimatge(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (t_textura == FITXERIMA) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


/* ------------------------------------------------------------------------- */
/*					11. LLUMS												 */
/* ------------------------------------------------------------------------- */

// LLUMS: Activació / Desactivació llum ambient 
void CEntornV3DView::OnLlumsLlumambient()
{
// TODO: Agregue aquí su código de controlador de comandos
	llum_ambient = !llum_ambient;

	if (shader_menu != CAP_SHADER) {
// Entorn V3D: Activació del contexte OpenGL
		wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
// Pas màscara llums
		glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[1]"), (llum_ambient && sw_material[1]));
// Entorn V3D: Desactivació del contexte OpenGL. Permet la coexistencia d'altres contextes de generació
		wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
	}

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateLlumsLlumambient(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (llum_ambient) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// LLUMS: Activació /Desactivació llum 0 (GL_LIGHT0)
void CEntornV3DView::OnLlumsLlum0()
{
// TODO: Agregue aquí su código de controlador de comandos
	llumGL[0].encesa = !llumGL[0].encesa;

	if (shader_menu != CAP_SHADER) {
// Entorn V3D: Activació del contexte OpenGL
		wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
// Pas màscara llums
		glUniform1i(glGetUniformLocation(shader_program, "sw_lights[0]"), llumGL[0].encesa);
// Entorn V3D: Desactivació del contexte OpenGL. Permet la coexistencia d'altres contextes de generació
		wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
	}

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateLlumsLlum0(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (llumGL[0].encesa) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// LLUMS-->ON/OFF: Activació /Desactivació llum 1 (GL_LIGHT1)
void CEntornV3DView::OnLlumsLlum1()
{
// TODO: Agregue aquí su código de controlador de comandos
	llumGL[1].encesa = !llumGL[1].encesa;

	if (shader_menu != CAP_SHADER) {
		// Entorn V3D: Activació del contexte OpenGL
		wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
		// Pas màscara llums
		glUniform1i(glGetUniformLocation(shader_program, "sw_lights[1]"), llumGL[1].encesa);
		// Entorn V3D: Desactivació del contexte OpenGL. Permet la coexistencia d'altres contextes de generació
		wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
	}

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateLlumsLlum1(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (llumGL[1].encesa) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


// LLUMS-->ON/OFF: Activació /Desactivació llum 2 (GL_LIGHT2)
void CEntornV3DView::OnLlumsLlum2()
{
// TODO: Agregue aquí su código de controlador de comandos
	llumGL[2].encesa = !llumGL[2].encesa;

	if (shader_menu != CAP_SHADER) {
		// Entorn V3D: Activació del contexte OpenGL
		wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
		// Pas màscara llums
		glUniform1i(glGetUniformLocation(shader_program, "sw_lights[2]"), llumGL[2].encesa);
		// Entorn V3D: Desactivació del contexte OpenGL. Permet la coexistencia d'altres contextes de generació
		wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
	}

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateLlumsLlum2(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (llumGL[2].encesa) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


// LLUMS-->ON/OFF: Activació /Desactivació llum 3 (GL_LIGHT3)
void CEntornV3DView::OnLlumsLlum3()
{
// TODO: Agregue aquí su código de controlador de comandos
	llumGL[3].encesa = !llumGL[3].encesa;
	sw_il = true;

	if (shader_menu != CAP_SHADER) {
		// Entorn V3D: Activació del contexte OpenGL
		wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
		// Pas màscara llums
		glUniform1i(glGetUniformLocation(shader_program, "sw_lights[3]"), llumGL[3].encesa);
		// Entorn V3D: Desactivació del contexte OpenGL. Permet la coexistencia d'altres contextes de generació
		wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
	}

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateLlumsLlum3(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (llumGL[3].encesa) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


// LLUMS-->ON/OFF: Activació /Desactivació llum 4 (GL_LIGHT4)
void CEntornV3DView::OnLlumsLlum4()
{
// TODO: Agregue aquí su código de controlador de comandos
	llumGL[4].encesa = !llumGL[4].encesa;
	sw_il = true;

	if (shader_menu != CAP_SHADER) {
		// Entorn V3D: Activació del contexte OpenGL
		wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
		// Pas màscara llums
		glUniform1i(glGetUniformLocation(shader_program, "sw_lights[4]"), llumGL[4].encesa);
		// Entorn V3D: Desactivació del contexte OpenGL. Permet la coexistencia d'altres contextes de generació
		wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
	}

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateLlumsLlum4(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (llumGL[4].encesa) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


/* ------------------------------------------------------------------------- */
/*					12. SHADERS												 */
/* ------------------------------------------------------------------------- */

// SHADERS: Desactivació de tots els shaders
void CEntornV3DView::OnShadersSense()
{
// TODO: Agregue aquí su código de controlador de comandos
	shader_menu = CAP_SHADER;
	releaseAllShaders();

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornV3DView::OnUpdateShadersSense(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (shader_menu == CAP) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// SHADERS: Iluminació Gouraud amb Shaders
void CEntornV3DView::OnShadersGouraud()
{
// TODO: Agregue aquí su código de controlador de comandos
	bool apagat = false;
	if (sw_shader) releaseAllShaders();
	else sw_shader = true;
	shader_menu = GOURAUD_SHADER;

// Entorn V3D: Activació del contexte OpenGL
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	shader_program = initializeShaders(_T("./shaders/gouraud_shdrML"));

	glUniform1i(glGetUniformLocation(shader_program, "texture"), textura);
	glUniform1i(glGetUniformLocation(shader_program, "texture0"), GLint(0));
// Pas màscara reflectivitat
	glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[0]"), sw_material[0]);
	glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[1]"), sw_material[1]);
	glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[2]"), sw_material[2]);
	glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[3]"), sw_material[3]);
//Pas màscara llums
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[0]"), llumGL[0].encesa);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[1]"), llumGL[1].encesa);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[2]"), llumGL[2].encesa);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[3]"), llumGL[3].encesa);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[4]"), llumGL[4].encesa);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[5]"), llumGL[5].encesa);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[6]"), llumGL[6].encesa);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[7]"), llumGL[7].encesa);

// Entorn V3D: Desactivació del contexte OpenGL. Permet la coexistencia d'altres contextes de generació
	wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateShadersGouraud(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (shader_menu == GOURAUD_SHADER) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// SHADERS: Iluminació Phong amb Shaders
void CEntornV3DView::OnShadersPhong()
{
// TODO: Agregue aquí su código de controlador de comandos
	bool apagat=false;
	if (sw_shader) releaseAllShaders();		// Eliminar Programs previs
		else sw_shader = true;
	shader_menu = PHONG_SHADER;

// Entorn V3D: Activació del contexte OpenGL
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

// Carregar shaders phong_shdrML.vert i phong_shdrML.frag en Program shader_program
	shader_program = initializeShaders(_T("./shaders/phong_shdrML"));	

// Linkar uniform variables amb variables C++
	glUniform1i(glGetUniformLocation(shader_program, "texture"), textura);
	glUniform1i(glGetUniformLocation(shader_program, "texture0"), GLint(0));
// Pas màscara reflectivitat
	glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[0]"), sw_material[0]);
	glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[1]"), sw_material[1]);
	glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[2]"), sw_material[2]);
	glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[3]"), sw_material[3]);
//Pas màscara llums
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[0]"), llumGL[0].encesa);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[1]"), llumGL[1].encesa);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[2]"), llumGL[2].encesa);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[3]"), llumGL[3].encesa);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[4]"), llumGL[4].encesa);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[5]"), llumGL[5].encesa);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[6]"), llumGL[6].encesa);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[7]"), llumGL[7].encesa);

// Entorn V3D: Desactivació del contexte OpenGL. Permet la coexistencia d'altres contextes de generació
	wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateShadersPhong(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (shader_menu == PHONG_SHADER) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// SHADERS: Càrrega Fitxers Shader
void CEntornV3DView::OnShaderLoadFiles()
{
// TODO: Agregue aquí su código de controlador de comandos
	CString nomVert, nomFrag;

	if (sw_shader) releaseAllShaders();
		else sw_shader = true;
	shader_menu = FILE_SHADER;

// Càrrega fitxer VERT
// Entorn V3D: Obrir diàleg de lectura de fitxer (fitxers (*.VERT)
	CFileDialog openVert(TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("VERT Files(*.vert)|*.vert|All Files (*.*)|*.*||"));;

	if (openVert.DoModal() != IDOK)
		return;                 // stay with old data file
	else nomVert = openVert.GetPathName();
// Entorn V3D: Variable de tipus CString 'nomVert' conté el nom del fitxer seleccionat

// Entorn V3D: Conversió de la variable CString nom a la variable char *nomfitx, 
//		compatible amb  les funcions de càrrega de fitxers fractals
//	char *nomfitxV = CString2Char(nomVert);

// Càrrega fitxer FRAG
// Entorn V3D: Obrir diàleg de lectura de fitxer (fitxers (*.VERT)
	CFileDialog openFrag(TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("FRAG Files(*.frag)|*.frag|All Files (*.*)|*.*||"));;

	if (openFrag.DoModal() != IDOK)
		return;                 // stay with old data file
	else nomFrag = openFrag.GetPathName();
	// Entorn V3D: Variable de tipus CString 'nomFrag' conté el nom del fitxer seleccionat

// Entorn V3D: Conversió de la variable CString nom a la variable char *nomfitx, 
//		compatible amb  les funcions de càrrega de fitxers fractals
//	char *nomfitxF = CString2Char(nomFrag);

// Entorn V3D: Activació del contexte OpenGL
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	shader_program = loadFileShaders(nomVert, nomFrag);

//	shader_program = initializeShaders(_T("./shaders/gouraud_shdrML"));

	glUniform1i(glGetUniformLocation(shader_program, "texture"), textura);
	glUniform1i(glGetUniformLocation(shader_program, "texture0"), GLint(0));

// Entorn V3D: Desactivació del contexte OpenGL. Permet la coexistencia d'altres contextes de generació
	wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);

}


void CEntornV3DView::OnUpdateShaderLoadFiles(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (shader_menu == FILE_SHADER) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}



/* ------------------------------------------------------------------------- */
/*   RECURSOS DE BOTONS (ToolBar) DE L'APLICACIO                             */
/* ------------------------------------------------------------------------- */
// Incloure funcions de tractament de botons


// ---------------- Entorn V3D: Funcions locals a EntornV3DView.cpp

// Log2: Càlcul del log base 2 de num
int CEntornV3DView::Log2(int num)
{
	int tlog;

	if (num >= 8192) tlog = 13;
	else if (num >= 4096) tlog = 12;
	else if (num >= 2048) tlog = 11;
	else if (num >= 1024) tlog = 10;
	else if (num >= 512) tlog = 9;
	else if (num >= 256) tlog = 8;
	else if (num >= 128) tlog = 7;
	else if (num >= 64) tlog = 6;
	else if (num >= 32) tlog = 5;
	else if (num >= 16) tlog = 4;
	else if (num >= 8) tlog = 3;
	else if (num >= 4) tlog = 2;
	else if (num >= 2) tlog = 1;
	else tlog = 0;

	return tlog;
}


// CString2char: Funció de conversió de variable CString a char * per a noms de fitxers 
char * CEntornV3DView::CString2Char(CString entrada)
{
//char * par_sortida=" ";
// Variable de tipus CString 'entrada' conté un string tipus CString
//-------------------------------------------------------------------------------------
// Conversió de la variable CString entrada a la variable char *sortida, 
//	compatible amb la funció Carregar3DS, de càrrega de fitxers 3DS
//	char * nomfitx = (char *)(LPCTSTR)nom;

// Conversió variable w_char --> char *
//	char *nomf="  ";
//	wcstombs(nomf,strFileName.GetBuffer(3),90);
//	char *nomf = reinterpret_cast<char *> (nom.GetBuffer(3));

	size_t origsize = wcslen(entrada.GetBuffer(3)) + 1;
	size_t convertedChars = 0;

// Use a multibyte string to append the type of string
// to the new string before displaying the result.
	char strConcat[] = " (char *)";
	size_t strConcatsize = (strlen(strConcat) + 1) * 2;

// Allocate two bytes in the multibyte output string for every wide
// character in the input string (including a wide character
// null). Because a multibyte character can be one or two bytes,
// you should allot two bytes for each character. Having extra
// space for the new string is not an error, but having
// insufficient space is a potential security problem.
	const size_t newsize = origsize * 2;
// The new string will contain a converted copy of the original
// string plus the type of string appended to it.
//	char *nomfitx = new char[newsize + strConcatsize];
	char *par_sortida = new char[newsize + strConcatsize];

// Put a copy of the converted string into nstring
	wcstombs_s(&convertedChars, par_sortida, newsize, entrada.GetBuffer(3), _TRUNCATE);
// append the type of string to the new string.
//----------------------------------------------------------------------------------

// Variable de tipus char *nomfitx conté el nom del fitxer seleccionat
	return par_sortida;
}


// Refl_MaterialOff: Desactivar Reflexió de Material
void CEntornV3DView::Refl_MaterialOff()
{
	sw_material_old[0] = sw_material[0];	sw_material[0] = false;
	sw_material_old[1] = sw_material[1];	sw_material[1] = false;
	sw_material_old[2] = sw_material[2];	sw_material[2] = false;
	sw_material_old[3] = sw_material[3];	sw_material[3] = false;
}

// Refl_MaterialOn: Activar Reflexió de Material
void CEntornV3DView::Refl_MaterialOn()
{
	sw_material[0] = sw_material_old[0];
	sw_material[1] = sw_material_old[1];
	sw_material[2] = sw_material_old[2];
	sw_material[3] = sw_material_old[3];
}


// -------------------- FUNCIONS CORBES SPLINE i BEZIER

// llegir_ptsC: Llegir punts de control de corba (spline o Bezier) d'un fitxer .crv. 
//				Retorna el nombre de punts llegits en el fitxer.
//int llegir_pts(CString nomf)
int CEntornV3DView::llegir_ptsC(char *nomf)
{
	int i, j;
	FILE *fd;

	// Inicialitzar vector punts de control de la corba spline
	for (i = 0; i < MAX_PATCH_CORBA; i = i++)
	{
		PC_t[i].x = 0.0;
		PC_t[i].y = 0.0;
		PC_t[i].z = 0.0;
	}

	//	ifstream f("altinicials.dat",ios::in);
	//    f>>i; f>>j;
	if ((fd = fopen(nomf, "rt")) == NULL)
	{
		LPCWSTR texte1 = reinterpret_cast<LPCWSTR> ("ERROR:");
		LPCWSTR texte2 = reinterpret_cast<LPCWSTR> ("File .crv was not opened");
		//MessageBox(NULL, texte1, texte2, MB_OK);
		MessageBox(texte1, texte2, MB_OK);
		//AfxMessageBox("file was not opened");
		return false;
	}
	fscanf(fd, "%d \n", &i);
	if (i == 0) return false;
	else {
		for (j = 0; j < i; j = j++)
		{	//fscanf(fd, "%f", &corbaSpline[j].x);
			//fscanf(fd, "%f", &corbaSpline[j].y);
			//fscanf(fd, "%f \n", &corbaSpline[j].z);
			fscanf(fd, "%lf %lf %lf \n", &PC_t[j].x, &PC_t[j].y, &PC_t[j].z);

		}
	}
	fclose(fd);

	return i;
}



void CEntornV3DView::OnProjeccioOrtografica()
{
	// TODO: Agregue aquí su código de controlador de comandos

	projeccio = ORTO;
	eixos = true; 
	mobil = false;
	zzoom = false;
	// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
	
}


void CEntornV3DView::OnUpdateProjeccioOrtografica(CCmdUI* pCmdUI)
{
	if (projeccio == ORTO) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);  

	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
}


void CEntornV3DView::OnProjeccioAxonometrica()
{
	// TODO: Agregue aquí su código de controlador de comandos
	projeccio = AXONOM;
	eixos = true;
	mobil = true;
	zzoom = true;
	// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateProjeccioAxonometrica(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (projeccio == AXONOM) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


void CEntornV3DView::OnTransforma_Quaternions()
{
	// TODO: Agregue aquí su código de controlador de comandos
	rotaQ = !rotaQ;
}


void CEntornV3DView::OnObjecteCamion()
{
	// TODO: Agregue aquí su código de controlador de comandos
	objecte = CAMIO;
	InvalidateRect(NULL, false);	
}


void CEntornV3DView::OnUpdateObjecteCamion(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == CAMIO) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


void CEntornV3DView::OnUpdateTransformaQuaternions(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (rotaQ) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}




void CEntornV3DView::OnTransformaLerp()
{
	interpolacio = LINEAL;
	// TODO: Agregue aquí su código de controlador de comandos
}


void CEntornV3DView::OnUpdateTransformaLerp(CCmdUI* pCmdUI)
{
	if (interpolacio == LINEAL) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
}


void CEntornV3DView::OnTransformaSlerp()
{
	// TODO: Agregue aquí su código de controlador de comandos
	interpolacio = ESFERICA;
}


void CEntornV3DView::OnUpdateTransformaSlerp(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (interpolacio == ESFERICA) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


void CEntornV3DView::OnObjectePato()
{
	// TODO: Agregue aquí su código de controlador de comandos
	objecte = PATO;
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateObjectePato(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == PATO) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


void CEntornV3DView::OnLoxodromaLoxodroma32891()
{
	// TODO: Agregue aquí su código de controlador de comandos
	croata = 0.25;
	objecte = C_LOXODROMA;
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateLoxodromaLoxodroma32891(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == C_LOXODROMA && croata == 0.25) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


void CEntornV3DView::OnLoxodromaLoxodroma32892()
{
	// TODO: Agregue aquí su código de controlador de comandos
	croata = 0.5;
	objecte = C_LOXODROMA;
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateLoxodromaLoxodroma32892(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == C_LOXODROMA && croata == 0.5) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


void CEntornV3DView::OnLoxodromaLoxodroma32893()
{
	// TODO: Agregue aquí su código de controlador de comandos
	croata = 1;
	objecte = C_LOXODROMA;
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateLoxodromaLoxodroma32893(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == C_LOXODROMA && croata == 1) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


void CEntornV3DView::OnLoxodromaLoxodroma32894()
{
	// TODO: Agregue aquí su código de controlador de comandos
	croata = 2;
	objecte = C_LOXODROMA;
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateLoxodromaLoxodroma32894(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == C_LOXODROMA && croata == 2) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


void CEntornV3DView::OnLoxodromaLoxodroma32895()
{
	// TODO: Agregue aquí su código de controlador de comandos
	croata = 4;
	objecte = C_LOXODROMA;
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateLoxodromaLoxodroma32895(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == C_LOXODROMA && croata == 4) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


void CEntornV3DView::OnCorbesTriedrededarboux()
{
	// TODO: Agregue aquí su código de controlador de comandos
	dibuixa_TriedreDarboux = !dibuixa_TriedreDarboux;
	if (dibuixa_TriedreDarboux) dibuixa_TriedreFrenet = false;

	// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateCorbesTriedrededarboux(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (dibuixa_TriedreDarboux) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


void CEntornV3DView::OnC32897()
{
	// TODO: Agregue aquí su código de controlador de comandos
	camera = CAM_LOXODROMA_TDARBOUX;
	cam_t = 0.0;
	OPV.R = 10;

	opvN = Punt_Loxodroma(OPV.R, 0.125, cam_t);
	vCam = Prod_Vectorial(VT_TDLoxodroma(OPV.R, 0.125, cam_t), VNP_TDLoxodroma(OPV.R, 0.125, cam_t));

	gluLookAt(opvN.x, opvN.y, opvN.z, 0, 0, 0, vCam.x, vCam.y, vCam.z);

	//eixos = true;
	//mobil = false;
	// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateC32897(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (camera == CAM_LOXODROMA_TDARBOUX) 	pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


void CEntornV3DView::OnC32898()
{
	// TODO: Agregue aquí su código de controlador de comandos
	camera = CAM_LOXODROMA_TFRENET;
	cam_t = 0.0;
	OPV.R = 10;

	opvN = Punt_Loxodroma(OPV.R, 0.125, cam_t);
	//vCam = Prod_Vectorial(VT_TFLoxodroma(OPV.R, 0.125, cam_t), VBN_TFLoxodroma(OPV.R, 0.125, cam_t));
	vCam = VBN_TFLoxodroma(OPV.R, 0.125, cam_t);
	gluLookAt(opvN.x, opvN.y, opvN.z, 0, 0, 0, vCam.x, vCam.y, vCam.z);

	InvalidateRect(NULL, false);
}


void CEntornV3DView::OnUpdateC32898(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (camera == CAM_LOXODROMA_TFRENET) 	pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}
