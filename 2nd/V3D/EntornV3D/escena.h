//******** PRACTICA VISUALITZACIÓ 3D (Facultat de Ciències / Escola d'Enginyeria - UAB)
//******** Entorn bàsic VS2019 MULTIFINESTRA amb OpenGL, interfície MFC i Status Bar
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (Setembre 2021)
// escena.h : interface de escena.cpp
//

#ifndef GLSOURCE_H
#define GLSOURCE_H
/* ------------------------------------------------------------------------- */
/*                            Funcions de les escenes                        */
/* ------------------------------------------------------------------------- */
// Entorn V3D: dibuixa_EscenaGL -> Dibuix de l'escena GL
void dibuixa_EscenaGL(char objecte, CColor col_object, bool ref_mat, bool sw_mat[4],
	bool textur, GLint texturID[NUM_MAX_TEXTURES],
	int nptsU, CPunt3D PC_u[MAX_PATCH_CORBA], GLdouble pasCS, bool sw_PC,GLdouble QMat[16],
	bool dib_TFrenet, bool dib_TDarboux, GLfloat croata);

// Entorn V3D: dibuixa -> Funció que dibuixa objectes simples de la llibreria GLUT segons obj
void dibuixa(char obj);

// Entorn V3D. ESCENA: dibuixaEscena_Fixe: Funció que dibuixa tots els elements fixes d'ESCENA.
void dibuixaEscena_Fixe(GLint CloudsT);

// Entorn V3D. ESCENA: dibuixaEscena: Funció que dibuixa tots els elements de l'ESCENA, tant fixes com mòbils.
void dibuixaEscena(GLint CloudsT, bool transparent, CPunt3D pos_t, CPunt3D angles_t, GLdouble QMat[16]);

// Entorn V3D: Truck - Objecte fet per alumnes.
void truck(bool textu,GLint VTextu[NUM_MAX_TEXTURES]);
void neumatic(bool textur, GLint VTextur[NUM_MAX_TEXTURES]);
void llanta(bool textur, GLint VTextur[NUM_MAX_TEXTURES]);
void sea(void);

// Entorn V3D: Tie (Nau enemiga Star Wars) - Objecte fet per alumnes.
void tie();
void Alas();
void Motor();
void Canon();
void Cuerpo();
void Cabina();
#endif