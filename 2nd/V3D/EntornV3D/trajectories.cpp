//******** PRACTICA VISUALITZACIÓ 3D (Facultat de Ciències / Escola d'Enginyeria - UAB)
//******** Entorn bàsic VS2019 MULTIFINESTRA amb OpenGL, interfície MFC i Status Bar
//******** Carme Julià, Débora Gil, Enric Martí (Octubre 2020)
// trajectories.cpp : Definició de les trajectories per splines

#include "stdafx.h"
#include "constants.h"
#include "trajectories.h"

#include <math.h>

//---------------------------- CORBA BSPLINE

// P_CBSpline2D: Calcul del punt del spline en coordenades 2D (CPoint) segons el 
//               paràmetre i i els punts de control ctr 
CPoint P_CBSpline2D(float t, CPoint * ctr)
{
	CPoint p(0,0);
	float coef[4];
	int i,j;
  
// Polinomis que multipliquen els punts de control del patch
	for(i=0;i<4;i++)
	{	coef[i]=0;
		for(j=0;j<4;j++)
		coef[i]=coef[i]*t+A[i][j];
	}

// Càlcul de la Posició
	for(i=0;i<4;i++)
	{
		p.x+=coef[i]*ctr[i].x;
		p.y+=coef[i]*ctr[i].y;
	}
	return p;
   
}


// D_CBspline2D: Calcul de la derivada del spline en coordenades 2D (CPoint) segons el 
//               paràmetre i i els punts de control ctr 
CPoint D_CBspline2D(float t, CPoint * ctr)
{
	CPoint dp(0,0);
	float coef[4];
	int i,j;

// Polinomis que multipliquen els punts de control del patch
	for(i=0;i<4;i++)
	{	coef[i]=0;
		for(j=0;j<3;j++)
		 coef[i]=coef[i]*t+(3-j)*A[i][j];
	}

//Càlcul de la Derivada
	for(i=0;i<4;i++)
	{	dp.x+=coef[i]*ctr[i].x;
		dp.y+=coef[i]*ctr[i].y;
	}
	return dp;
}   


// P_CBSpline3D: Calcul del punt del spline en coordenades 3D (CPunt3D) segons el 
//				 paràmetre i i els punts de control ctr 
CPunt3D P_CBSpline3D(float t, CPunt3D * ctr)
{
	CPunt3D p = { (0.0f, 0.0f, 0.0f, 1.0f) };
	float coef[4];
	int i,j;
  
// Polinomis que multipliquen els punts de control del patch
	for(i=0;i<4;i++)
	{	coef[i]=0;
		for(j=0;j<4;j++)
		 coef[i]=coef[i]*t+A[i][j];
	}

// Càlcul de la Posició
	for(i=0;i<4;i++)
	{	p.x+=coef[i]*ctr[i].x;
		p.y+=coef[i]*ctr[i].y;
		p.z+=coef[i]*ctr[i].z;
	}
	return p;
}


// D_CBSpline3D: Calcul de la derivada del spline en coordenades 3D (CPunt3D) segons el 
//				 paràmetre i i els punts de control ctr
CPunt3D D_CBSpline3D(float t, CPunt3D * ctr)
{
	CPunt3D dp = { 0.0f, 0.0f, 0.0f, 1.0f };
	float coef[4];
	int i,j;

// Polinomis que multipliquen els punts de control del patch
	for(i=0;i<4;i++)
	{	coef[i]=0;
		for(j=0;j<3;j++)
		 coef[i]=coef[i]*t+(3-j)*A[i][j];
	}

// Càlcul de la Primera Derivada
	for(i=0;i<4;i++)
	{	dp.x+=coef[i]*ctr[i].x;
		dp.y+=coef[i]*ctr[i].y;
		dp.z+=coef[i]*ctr[i].z;
	}
	return dp;
}


// D2_CBSpline3D: Calcul de la segona derivada del spline en coordenades 3D (CPunt3D) segons el 
//				  paràmetre i i els punts de control ctr
CPunt3D D2_CBSpline3D(float t, CPunt3D * ctr)
{
	CPunt3D dp = { 0.0f, 0.0f, 0.0f, 1.0f };
	float coef[4];
	int i,j;
	
// Polinomis que multipliquen els punts de control del patch
	for(i=0;i<4;i++)
	{	coef[i]=0;
		for(j=0;j<2;j++)
		 coef[i]=coef[i]*t+(3-j)*(2-j)*A[i][j];
	}

// Càlcul de la Segona Derivada
	for(i=0;i<4;i++)
	{	dp.x+=coef[i]*ctr[i].x;
		dp.y+=coef[i]*ctr[i].y;
		dp.z+=coef[i]*ctr[i].z;
	}
	return dp;
}

// VT_CBSpline: Calcul del Vector Tangent (primera derivada) del B-Spline en coordenades 3D (CPunt3D) segons el 
//             paràmetre t i els punts de control ctr. Normalitzat.
CPunt3D VT_CBSpline(float t, CPunt3D * ctr)
{
	CPunt3D vt = { 0.0f, 0.0f, 0.0f, 1.0f };
	// CAL FER: Implementar la funció.

	return vt;
}


// VNP_CSpline: Calcul del Vector Normal Principal (segona derivada) del B-Spline en coordenades 3D (CPunt3D) segons el 
//             paràmetre t i els punts de control ctr. Normalitzat.
CPunt3D VNP_CBSpline(float t, CPunt3D * ctr)
{
	CPunt3D vnp = { 0.0f, 0.0f, 0.0f, 1.0f };
	// CAL FER: Implementar la funció.


	return vnp;
}
//---------------------------- FI CORBA BSPLINE

//----------------------------  CORBA LEMNISCATA

// Punt_Lemniscata2D: Calcul de la posició (x,y,z) segons lemniscata 2D (CPoint3D)
CPunt3D Punt_Lemniscata2D(float t)
{
	CPunt3D p = { (0, 0, 0) };
	//	float angle;
	const double a = 1.0*FMAX*(0.6); // paràmetre lemniscata

	float bet = sqrtf(2.0) / 2;
	//float aeli=0.95*512/2;
	//float beli=0.8*512/2;
	float x1 = a * cos(2 * t*pi / 180)*cos(t*pi / 180);
	float y1 = a * cos(2 * t*pi / 180)*sin(t*pi / 180);

// Càlcul de la Posició
	p.x = bet * x1 + bet * y1;
	p.y = -bet * x1 + bet * y1;
	p.z = 0;
	return p;

}

CPunt3D D_Lemniscata2D(float t)
{
	const double a = 1.0*FMAX*(0.6); // paràmetre lemniscata

	CPunt3D dp = { 0, 0, 0 };

	float bet = sqrtf(2.0) / 2;

	float dx1 = -2 * a*sin(2 * t*pi / 180)*cos(t*pi / 180) - a * cos(2 * t*pi / 180)*sin(t*pi / 180);
	float dy1 = -2 * a*sin(2 * t*pi / 180)*sin(t*pi / 180) + a * cos(2 * t*pi / 180)*cos(t*pi / 180);

	// Càlcul de la Derivada
	dp.x = bet * dx1 + bet * dy1;
	dp.y = -bet * dx1 + bet * dy1;
	dp.z = 0;

	return dp;
}


// Punt_Lemniscata: Calcul de la posició (x,y,z) segons lemniscata 3D (CPunt3D)
CPunt3D Punt_Lemniscata(float t)
{
	CPunt3D p = { (0.0f, 0.0f, 0.0f, 1.0f) };
	const double a = 1.0*FMAX*(0.6); // paràmetre lemniscata

	float bet = sqrtf(2.0) / 2;
	//float aeli=0.95*512/2;
	//float beli=0.8*512/2;
	float x1 = a * cos(2 * t*pi / 180)*cos(t*pi / 180);
	float y1 = a * cos(2 * t*pi / 180)*sin(t*pi / 180);
	float z1 = a * (sin(t*pi / 180) + cos(t*pi / 180));	// Trajectòria 1
	//float z1=a*sin(t*pi/180);					// Trajectòria 2

// Càlcul de la Posició
	p.x = bet * x1 + bet * y1;
	p.y = -bet * x1 + bet * y1;

	//	p.z=bet*z1+256;		// Elevació 1 (la més alta)
	//	p.z=(bet/2)*z1+158;	// Elevació 2 
	p.z = (bet / 4)*z1 + 82;	// Elevació 3
//	p.z=(bet/8)*z1+43;	// Elevació 4
	return p;
}

// D_Lemniscata: Calcul de la posició (x,y,z) de la tangent de lemniscata 3D (CPoint3D)
CPunt3D D_Lemniscata(float t)
{
	const double a = 1.0*FMAX*(0.6); // paràmetre lemniscata

	CPunt3D dp = { 0, 0, 0 };

	float bet = sqrtf(2.0) / 2;
	//float x1=a*cos(2*t*pi/180)*cos(t*pi/180);
	//float y1=a*cos(2*t*pi/180)*sin(t*pi/180);
	//	float z1=a*(sin(t*pi/180)+cos(t*pi/180));

	float dx1 = -2 * a*sin(2 * t*pi / 180)*cos(t*pi / 180) - a * cos(2 * t*pi / 180)*sin(t*pi / 180);
	float dy1 = -2 * a*sin(2 * t*pi / 180)*sin(t*pi / 180) + a * cos(2 * t*pi / 180)*cos(t*pi / 180);
	float dz1 = a * (cos(t*pi / 180) - sin(t*pi / 180));	// Trajectòria 1
	//float dz1=a*cos(t*pi/180);						// Trajectòria 2

// Càlcul de la Derivada
	dp.x = bet * dx1 + bet * dy1;
	dp.y = -bet * dx1 + bet * dy1;
	dp.z = bet * dz1;

	return dp;
}

// D2_CLemniscata: Calcul de la segona derivada de lemniscata 3D (CPoint3D)
CPunt3D D2_Lemniscata(float t)
{
	const double a = 1.0*FMAX*(0.6); // paràmetre lemniscata

	CPunt3D dp = { 0.0f, 0.0f, 0.0f, 1.0f };

	float bet = sqrtf(2.0) / 2;

	float dx1 = -4 * a*cos(2 * t*pi / 180)*cos(t*pi / 180) + 2 * a*sin(2 * t*pi / 180)*sin(t*pi / 180) + 2 * a*sin(2 * t*pi / 180)*sin(t*pi / 180) - a * cos(2 * t*pi / 180)*cos(t*pi / 180);
	float dy1 = -4 * a*cos(2 * t*pi / 180)*sin(t*pi / 180) - 2 * a*sin(2 * t*pi / 180)*cos(t*pi / 180) - 2 * a*sin(2 * t*pi / 180)*cos(t*pi / 180) - a * cos(2 * t*pi / 180)*sin(t*pi / 180);
	float dz1 = -a * sin(t*pi / 180) - cos(t*pi / 180);	// Trajectòria 1

// Càlcul de la Segona Derivada
	dp.x = bet * dx1 + bet * dy1;
	dp.y = -bet * dx1 + bet * dy1;
	dp.z = bet * dz1;

	return dp;
}

//-- TRIEDRE DE FRENET PER A CORBES LEMNISCATA 3D

// VT_Lemniscata3D: Calcul del Vector Tangent (primera derivada) de la corba lemniscata 3D en coordenades 3D (CPunt3D) segons el 
//             paràmetre i l'escala scale. Normalitzat.
CPunt3D VT_Lemniscata(float t)
{
	CPunt3D vt = { 0.0f, 0.0f, 0.0f, 1.0f };
	float longitut = 0;

	vt = D_Lemniscata(t);

// Convertim el vector en vector unitat (normalització)
	longitut = sqrt(vt.x * vt.x + vt.y * vt.y + vt.z * vt.z);
	vt.x = vt.x / longitut;
	vt.y = vt.y / longitut;
	vt.z = vt.z / longitut;

	return vt;
}


// VNP_Lemniscata3D: Calcul del Vector Normal Principal (segona derivada) de la corba lemniscata 3D en coordenades 3D (CPunt3D) segons el 
//             paràmetre t i l'escala scale. Normalitzat.
CPunt3D VNP_Lemniscata(float t)
{
	CPunt3D vnp = { 0.0f, 0.0f, 0.0f, 1.0f };
	float longitut = 0;

	vnp = D2_Lemniscata(t);

// Convertim el vector en vector unitat (normalització)
	longitut = sqrt(vnp.x * vnp.x + vnp.y * vnp.y + vnp.z * vnp.z);
	vnp.x = vnp.x / longitut;
	vnp.y = vnp.y / longitut;
	vnp.z = vnp.z / longitut;

	return vnp;
}
//-------------------------------- FI LEMNISCATA


//------------------------------- CÀLCUL D'ANGLES ENTRE VECTORS 3D --------------------------------
//Càlcul de l'angle de rotació Z entre dos vectors en el pla X-Y
float ComputeAngleZ(CPunt3D eix,CPunt3D tangent)
{  
	float sinus,cosinus,normeix,normtg,angle;

	sinus=eix.x*tangent.y-eix.y*tangent.x;
	normeix=sqrt(eix.x*eix.x+eix.y*eix.y);
	normtg=sqrt(tangent.x*tangent.x+tangent.y*tangent.y);
	cosinus=(eix.x*tangent.x+eix.y*tangent.y)/(normeix*normtg);

	if(sinus>=0) angle=180.0*acos(cosinus)/pi;
	if(sinus<0) angle=-180.0*acos(cosinus)/pi;

	return angle;
}

//Càlcul de l'angle de rotació X entre dos vectors en el pla Y-Z
float ComputeAngleX(CPunt3D eix,CPunt3D tangent)
{  
	float sinus,cosinus,normeix,normtg,angle;

	if (tangent.y>0) tangent.y=-tangent.y;
	sinus=eix.y*tangent.z-eix.z*tangent.y;
	normeix=sqrt(eix.y*eix.y+eix.z*eix.z);
	normtg=sqrt(tangent.y*tangent.y+tangent.z*tangent.z);
	cosinus=(eix.y*tangent.y+eix.z*tangent.z)/(normeix*normtg);

	if(sinus>=0) angle=180.0*acos(cosinus)/pi;
	if(sinus<0) angle=-180.0*acos(cosinus)/pi;

	return angle;
}

//Càlcul de l'angle de rotació Y entre dos vectors en el pla X-Z
float ComputeAngleY(CPunt3D eix,CPunt3D tangent)
{  
	float sinus,cosinus,normeix,normtg,angle;

	//if (tangent.y>0) tangent.y=-tangent.y;
	sinus=eix.z*tangent.x-eix.x*tangent.z;

	normeix=sqrt(eix.x*eix.x+eix.z*eix.z);
	normtg=sqrt(tangent.x*tangent.x+tangent.z*tangent.z);
	cosinus=(eix.x*tangent.x+eix.z*tangent.z)/(normeix*normtg);

	if(sinus>=0) angle=180.0*acos(cosinus)/pi;
	if(sinus<0) angle=-180.0*acos(cosinus)/pi;

	return angle;
}

//------------------------------- TRIEDRE DE FRENET --------------------------------

// VNormal: Càlcul del producte vectorial normalitzat de dos vectors v1,v2.
//			Resultat a la variable n.
CPunt3D Vector_Normal(CPunt3D v1, CPunt3D v2)
{
	float longitud;
	CPunt3D vn;

	vn.x = v1.y * v2.z - v2.y * v1.z;	//	n[0] = v1[1] * v2[2] - v2[1] * v1[2];
	vn.y = v1.z * v2.x - v2.z * v1.x;	//	n[1] = v1[2] * v2[0] - v2[2] * v1[0];
	vn.z = v1.x * v2.y - v1.y * v2.x;	//	n[2] = v1[0] * v2[1] - v1[1] * v2[0];

// Convertim el vector en vector unitat (normalització)
	longitud = sqrt(vn.x * vn.x + vn.y * vn.y + vn.z * vn.z);
	vn.x = vn.x / longitud;
	vn.y = vn.y / longitud;
	vn.z = vn.z / longitud;

	return vn;
}


// VBiNormal: Calcul del Binormal (segona derivada) a partir dels vector Tangent (VT) i el vector Binormal (VNP) com a 
//            producte vectorial dels dos. Normalitzat.
CPunt3D VBiNormal(CPunt3D VT, CPunt3D VNP)
{
	CPunt3D vbn = { 0.0f, 0.0f, 0.0f, 1.0f };
	// CAL FER: Implementar la funció.

	return vbn;
}


// dibuixa_TriedreFrenet: Dibuix del triedre de Frenet segons: VNP N(t)-(vermell), VBN B(T)-(verd) i VT T(t)-(blau)
void dibuixa_TriedreFrenet(CPunt3D vertex, CPunt3D VT, CPunt3D VNP, CPunt3D VBN)
{
	const int incr = 50;

// Doblar el gruix de la linia dels eixos.
	glLineWidth(2.0);

// CAL FER: Implementar la funció de dibuix dels tres eixos del Triedre de Frenet.

// Eix VNP N(t) (vermell)


// Eix VBN B(T)(verd)


// Eix VT T(t) (blau) 

// Restaurar el gruix de la linia dels eixos
	glLineWidth(1.0);
}
