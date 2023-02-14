//******** PRACTICA VISUALITZACIÓ 3D (Facultat de Ciències / Escola d'Enginyeria - UAB)
//******** Entorn bàsic VS2019 MULTIFINESTRA amb OpenGL, interfície MFC i Status Bar
//******** Carme Julià, Débora Gil, Enric Martí (Octubre 2021)
// trajectories.h : Definició de les trajectories per a BSplines

#ifndef TRAJECT_H
#define TRAJECT_H

// Numero Pts control
const int npts = 9;

const int H = -50;
const int S = 2;

// Constant per a corbes Lemniscata
const int FMAX = 512;

// --------------- V3D. Nombre de frames de l'animació Escena
#define NFRAMES_E 120

// Matriu dels BSplines
const double A[4][4] =
{
 {-1.0 / 6.0,   0.5,  -0.5,  1.0 / 6.0 },
 {   0.5, -1.0,   0.0,  4.0 / 6.0 },
 {  -0.5,  0.5,   0.5,  1.0 / 6.0 },
 { 1.0 / 6.0,   0.0,   0.0,    0.0   }
};

// Vector dels punts de control
const CPoint Pts[] = { CPoint(256 + 16,256),CPoint(-256,256),CPoint(-256,0),CPoint(-128 - 16,0),
CPoint(-16,64 + 32 + 32),CPoint(256 - 32,64 - 32),CPoint(256 + 16,256),CPoint(-256,256),CPoint(-256,0) };

// Vector dels punts de control Vaixell
const CPunt3D PtsV[] = { {256 + 16,256,3},{-256,256,3},{-256,0,3},{-128 - 16,0,3},
{-16,64 + 32 + 32,3},{256 - 32,64 - 32,3},{256 + 16,256,3},{-256,256,3},{-256,0,3} };

// Vector dels punts de control Tie
const CPunt3D PtsT[] = { {S*(256 + 16),S*256,S*(30+H)},{-256*S,256*S,S*(-10+H)},{-256*S,0,S*(30+H)},{S*(-128 - 16),0,S*(75+H)},
{-16*S,S*(64 + 32 + 32),S*(150+H)},{S*(256 - 32),S*(64 - 32),S*(75+H)},{S*(256 + 16),256*S,S*(30+H)},{-256*S,256*S,S*(-10+H)},{-256*S,0,S*(30+H)} };

// Eix principal de l'objecte Tie (direcció): 
const CPunt3D eixT = { 0.0, 1.0, 0.0 };


//---------------------------- DECLARACIÓ DE FUNCIONS ---------------------------------
//---------------------------- FUNCIONS CORBA SPLINE
CPoint P_CBSpline2D(float t, CPoint * ctr);
CPoint D_CBSpline2D(float t, CPoint * ctr);

// spline: Funció per a calcular la posició de la corba spline segons:
//     * t: paràmetre entre (0,1).
//     * ctr: Punts de control.
CPunt3D P_CBSpline3D(float t, CPunt3D * ctr);

// Dspline: Funció per a calcular la primera derivada de la corba spline segons:
//     * t: paràmetre entre (0,1).
//     * ctr: Punts de control.
CPunt3D D_CBSpline3D(float t, CPunt3D * ctr);

// D2spline: Funció per a calcular la segona derivada de la corba spline segons:
//     * t: paràmetre entre (0,1).
//     * ctr: Punts de control.
CPunt3D D2_CBSpline3D(float t, CPunt3D * ctr);

// VT_CBSpline: Calcul del Vector Tangent (primera derivada) del B-Spline en coordenades 3D (CPunt3D) segons el 
//             paràmetre t i els punts de control ctr. Normalitzat.
CPunt3D VT_CBSpline(float t, CPunt3D * ctr);

// VNP_CSpline: Calcul del Vector Normal Principal (segona derivada) del B-Spline en coordenades 3D (CPunt3D) segons el 
//             paràmetre t i els punts de control ctr. Normalitzat.
CPunt3D VNP_CBSpline(float t, CPunt3D * ctr);

//---------------------------- FI CORBA SPLINE

//----------------------------  CORBA LEMNISCATA
CPunt3D Punt_Lemniscata2D(float t);
CPunt3D D_Lemniscata2D(float t);
CPunt3D Punt_Lemniscata(float t);
CPunt3D D_Lemniscata(float t);
CPunt3D D2_Lemniscata(float t);
CPunt3D VT_Lemniscata(float t);
CPunt3D VNP_Lemniscata(float t);
//----------------------------  FI CORBA LEMNISCATA

//------------------------------- CÀLCUL D'ANGLES ENTRE VECTORS 3D --------------------------------
// ComputeAngleZ:	Càlcul de l'angle de rotació Z entre els vectors
//					eix i tangent projectats sobre el pla X-Y,
float ComputeAngleZ(CPunt3D eix,CPunt3D tangent);

// ComputeAngleX:	Càlcul de l'angle de rotació X entre els vectors
//					eix i tangent projectats sobre el pla Y-Z.
float ComputeAngleX(CPunt3D eix,CPunt3D tangent);

// ComputeAngleY:	Càlcul de l'angle de rotació Y entre els vectors
//					eix i tangent projectats sobre el pla X-Z
float ComputeAngleY(CPunt3D eix,CPunt3D tangent);

//------------------------------- FI CÀLCUL D'ANGLES ENTRE VECTORS 3D --------------------------------

//------------------------------- FUNCIONS GENÈRIQUES PEL CÀLCUL TRIEDRE DE FRENET  --------------------------------

// VNormal: Càlcul del producte vectorial normalitzat de dos vectors v1,v2.
//			Resultat a la variable n.
CPunt3D Vector_Normal(CPunt3D v1, CPunt3D v2);

// VBiNormal: Calcul del Binormal (segona derivada) a partir dels vector Tangent (VT) i el vector Binormal (VNP) com a 
//            producte vectorial dels dos. Normalitzat.
CPunt3D VBiNormal(CPunt3D VT, CPunt3D VNP);

// dibuixa_TriedreFrenet: Dibuix del triedre de Frenet segons: VBN B(T)-(vermell), VNP N(t)-(verd) i VT T(t)-(blau)
void dibuixa_TriedreFrenet(CPunt3D vertex, CPunt3D VT, CPunt3D VNP, CPunt3D VBN);
#endif