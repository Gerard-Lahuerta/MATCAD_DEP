//******** PRACTICA VISUALITZACI� 3D (Facultat de Ci�ncies / Escola d'Enginyeria - UAB)
//******** Entorn b�sic VS2019 MULTIFINESTRA amb OpenGL, interf�cie MFC i Status Bar
//******** Ferran Poveda, Marc Vivet, Carme Juli�, D�bora Gil, Enric Mart� (Setembre 2021)
// visualitzacio.h : Interface de visualitzacio.cpp
//

#ifndef GLVIS_H
#define GLVIS_H

/* ------------------------------------------------------------------------- */
/*                                Funcions                                   */
/* ------------------------------------------------------------------------- */

// Entorn V3D: Il.luminaci�
void Iluminacio(char ilumin, bool ifix, bool ilu2sides, bool ll_amb, LLUM* lumin, bool textur, char obj, bool frnt_fcs, bool bc_lin, int step);

// Entorn V3D: Projecci� Ortografica;
void Projeccio_Orto(int minx, int miny, GLsizei w, GLsizei h, float zoom);
void Vista_Ortografica(int prj,GLfloat Raux,CColor col_fons,CColor col_object,char objecte,GLfloat mida,int step,
				bool frnt_fcs, bool oculta, bool testv, bool bck_ln, char iluminacio, bool llum_amb, LLUM* lumin,
				bool ifix, bool il2sides, bool eix, CMask3D reixa, CPunt3D hreixa);

// Entorn V3D: Projecci� Perspectiva
void Projeccio_Perspectiva(int,int,GLsizei,GLsizei,float);
void Vista_Esferica(CEsfe3D opv,char VPol,bool pant,CPunt3D tr,CPunt3D trF,
				 CColor col_fons,CColor col_object,char objecte,double mida,int step,
				 bool frnt_fcs, bool oculta, bool testv, bool bck_ln, char iluminacio, bool llum_amb, LLUM* lumin, 
				 bool ifix, bool il2sides, bool eix, CMask3D reixa, CPunt3D hreixa);

void Vista_Navega(CPunt3D pv,GLdouble n[3],GLdouble v[3], bool pant,CPunt3D tr,CPunt3D trF,
				  CColor col_fons,CColor col_object,char objecte,bool color, int step, 
				  bool frnt_fcs, bool oculta, bool testv,bool bck_ln, char iluminacio, bool llum_amb, LLUM* lumin, 
				  bool ifix, bool il2sides, bool eix, CMask3D reixa, CPunt3D hreixa);

// Entorn V3D: C�rrega TG
void instancia(bool TR, INSTANCIA tg, INSTANCIA tgF, bool rotQ, bool animQ, GLdouble Qmat[16], CPunt3D intTr);

// Entorn V3D: Dibuix dels eixos de coordenades
void deixos(void);

// Entorn V3D: GRID (gridXY, gridYZ, gridXZ, gridXYZ) -------------------------
void draw_Grid(CMask3D quadricula, CPunt3D hquadricula);
void gridXY(int size, GLdouble h);
void gridXZ(int size, GLdouble h);
void gridYZ(int size, GLdouble h);
void gridXYZ(int size);

// Entorn V3D: Esborrats de pantalla a fons variable (Fons), negre (Fons) o a blanc (FonsW)
void Fons(CColor int_fons);
void FonsN(void);
void FonsB(void);

// Entorn V3D: Carregar fitxer imatge de formats admesos per la llibreria DevIL/OpenIL 
//				com a textura
GLint loadIMA_ILUT(char *filename);
GLint loadIMA_SOIL(char *filename);
void SetTextureParameters(GLint IDText, bool extensT, bool filterT, bool blendT, bool genMipmpT);

//----------------- FUNCIONS LLIBRERIA GLU
#define __glPi 3.14159265358979323846

void gluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx,
	GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy,
	GLdouble upz);

void gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);

static void __gluMakeIdentityf(GLfloat m[16]);
static void __gluMakeIdentityd(GLdouble m[16]);
static void normalize(float v[3]);
static void cross(float v1[3], float v2[3], float result[3]);
#endif