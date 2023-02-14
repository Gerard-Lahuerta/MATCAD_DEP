// EntornV3D.h: archivo de encabezado principal para la aplicación EntornV3D
//
#pragma once

#ifndef __AFXWIN_H__
	#error "incluir 'stdafx.h' antes de incluir este archivo para PCH"
#endif

#include "resource.h"       // Símbolos principales
#include "quatern.h"


// CEntornV3DApp:
// Consulte la sección EntornV3D.cpp para obtener información sobre la implementación de esta clase
//

class CEntornV3DApp : public CWinAppEx
{
public:
	CEntornV3DApp();


// Reemplazos
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementación
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CEntornV3DApp theApp;
