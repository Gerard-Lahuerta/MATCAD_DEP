#define Marc_Bosom_Saperas 1606776
#define Ona_Sánchez_Núñez 1601181

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

typedef struct{
    unsigned capacitat,contingut; //capacitat que te la gerra per guardar aigua ; contingut d'aigua que conté en el moment
}gerra;

typedef struct{
    unsigned *continguts;
    unsigned origen, desti;
    unsigned anterior;
}configuracio;

configuracio transvasament(int, int, unsigned[], gerra[], int);
void mostrarcami(unsigned, configuracio *, int, int);
int posicio (configuracio, gerra[], int);

int main(int argc, char *argv[]){
    int ngerres;
    int total=0,maxlitres=0;
    int maxconf=1; //maxconf=1 per quan multipliquem el 1r cop no 0

    sscanf(argv[1], "%d", &ngerres);
    if(ngerres<=0){
      printf("La combinació no és possible. Ha hagut un error a l'hora d'entrar les dades\n");
      return -1;
    }
    gerra gerres[ngerres]; //{capacitat, litres que té ara}

    for(int i=0; i<ngerres; i++){
      sscanf(argv[2*i+2], "%d", &gerres[i].capacitat);
      sscanf(argv[2*i+3], "%d", &gerres[i].contingut);
        if(gerres[i].capacitat<=0 || gerres[i].contingut>gerres[i].capacitat){
          printf("La combinació no és possible. Ha hagut un error a l'hora d'entrar les dades\n");
          return -1;
        }
        maxlitres+=gerres[i].contingut;
    }

    for(int i=0; i<ngerres; i++){
      total += gerres[i].contingut;
    }

    if(total>maxlitres){
      printf("La combinació no és possible. Ha hagut un error a l'hora d'entrar les dades\n");
      return -1;
    }


    for(int i=0; i<ngerres; i++){ //Hem de posar maxconf aquí perquè necessitem que ja s'hagi fet gerres[]
      maxconf*=(gerres[i].capacitat+1);
    }

    configuracio configuracions[maxconf+1]; //per guardar totes les configuracions de cada gerra

    for(int i=0; i<maxconf+1; i++){ //per reservar la memòria de cada contingut de cada configuració
      configuracions[i].continguts = (unsigned*)malloc(ngerres*sizeof(unsigned));
      if(configuracions[i].continguts==NULL){
        printf("No s'ha pogut reservar la memòria necessària\n");
        return -1;
      }
    }

    printf("Gerres amb capacitats |");

    for(unsigned l=0;l<ngerres;l++){
      printf("%u|",gerres[l].capacitat); //print de les primeres capacitats de les gerres
    }
    printf("\n");

    int estat_config[maxconf]; //estat config serà una llista amb un únic índex per a cada configuració, inicialitzem amb llargada màxima
    for(unsigned i=0; i<maxconf; i++){
      estat_config[i]=0;
    }

    /*for(unsigned i=0;i<gerres[0].capacitat+1;i++){
        for(unsigned j=0;j<gerres[1].capacitat+1;j++){
            for(unsigned k=0;k<gerres[2].capacitat+1;k++){
                estat_config[i][j][k]=0; //inicialitza totes les configuracions possibles a 0
            }
        }
    }*/

    for(unsigned l=0;l<ngerres;l++){
      configuracions[0].continguts[l]=gerres[l].contingut; //recull el contingut de cadascuna de les gerres i les guarda com a primera configuració.
    }
    printf("Configuració inicial  |");


    for(unsigned l=0;l<ngerres;l++){
      printf("%u|",configuracions[0].continguts[l]); //imprimeix la primera configuració (800)
    }
    printf("\n\n");

    //estat_config[configuracions[0].continguts[0]][configuracions[0].continguts[1]][configuracions[0].continguts[2]]=1; //canvia l'estat de la primera configuració per tal de no repetir-la més endavant

    estat_config[posicio(configuracions[0], gerres, ngerres)]=1;

    // configuracions[0].origen=0;
    // configuracions[0].desti=0;
    configuracions[0].anterior=UINT_MAX; //posem un nombre molt gran
    int num_conf=1; //nombre de configuracions fetes

    configuracions[0].origen=0;
    configuracions[0].desti=0;

    for(unsigned i=0;i<num_conf && num_conf<maxconf;i++){ //és un contador, no és gerra
        for(unsigned j=0;j<ngerres && num_conf<maxconf;j++){ //fa referencia a una gerra
            for(unsigned k=0;k<ngerres && num_conf<maxconf;k++){ //fa referencia a una altre gerra
                if(j==k){ //fa una nova iteració del for per evitar errors en l'execució
                  continue;
                }

                if(configuracions[i].continguts[j]==0){ //fa una nova iteració del for per evitar errors en l'execució
                  continue;
                }
                if(configuracions[i].continguts[k]==gerres[k].capacitat){ //fa una nova iteració del for per evitar errors en l'execució
                  continue;
                }

                configuracions[num_conf]=transvasament(j, k, configuracions[i].continguts, gerres, ngerres); //la nova configuració neix de la gerra i

                /*if(estat_config[configuracions[num_conf].continguts[0]][configuracions[num_conf].continguts[1]][configuracions[num_conf].continguts[2]]==1){ //Ja hem passat per aquí
                  continue;
                }

                estat_config[configuracions[num_conf].continguts[0]][configuracions[num_conf].continguts[1]][configuracions[num_conf].continguts[2]]=1; //l'estat passa a ser 1
*/

                if(estat_config[posicio(configuracions[num_conf], gerres, ngerres)]==1){ //si ja hem passat per aquí
                  continue;
                }

                estat_config[posicio(configuracions[num_conf], gerres, ngerres)]=1; //l'estat passa a ser 1

                configuracions[num_conf].origen=j;
                configuracions[num_conf].desti=k;
                configuracions[num_conf].anterior=i;

                mostrarcami(num_conf, configuracions, ngerres,maxconf);
                printf("\n");
                num_conf++;
            }
        }
    }
    /*
    if(num_conf==maxconf){
        printf("Hem arribat al màxim de configuracions i pot ser no s'han fet totes\n");
        return 1;
    }
    */
    printf("S'han trobat %u configuracions possibles\n",num_conf);
    return 0;
}

configuracio transvasament(int j, int k, unsigned continguts[], gerra gerres[], int ngerres){ //configuracions = confinguracions[i]
  configuracio configuracions;

  configuracions.continguts = (unsigned*)malloc(ngerres*sizeof(unsigned));
  if(configuracions.continguts==NULL){
    printf("No s'ha pogut reservar la memòria necessària\n");
    exit(-1);
  }


  for(unsigned l=0;l<ngerres;l++){
    configuracions.continguts[l]=continguts[l]; //Recull les configuracions i en la posició num_conf del vector configuracions
  }

  configuracions.continguts[k]=configuracions.continguts[k]+configuracions.continguts[j]; //suma el contingut de la gerra j i k i ho posa a k (com si passés l'aigua)
  configuracions.continguts[j]=0;//la gerra j queda sense aigua

  if(configuracions.continguts[k]>gerres[k].capacitat){ //Si supera la capacitat que té la gerra
    configuracions.continguts[j]=configuracions.continguts[k]-gerres[k].capacitat; //el sobrant ho passa a la gerra j
    configuracions.continguts[k]=gerres[k].capacitat; //k queda a la màxima capacitat
  }
  return configuracions;
}

void mostrarcami(unsigned estat_actual, configuracio *configuracions, int ngerres,int maxconf){
  unsigned cami[maxconf+1]; //declarem la funció on anirem guardant les coses
  cami[0]=estat_actual; // A la posició 0 del vector guardem la posició inicial
  int i=1; //Comencem en i=1 perquè la posició 0 ja està guardada

  while(estat_actual>0){ //anem guardant al camí tots els anteriors
    cami[i]=configuracions[estat_actual].anterior;
    estat_actual=configuracions[estat_actual].anterior; //l'actual passi a ser l'anterior
    i++;
  }

  for(int j=i-1;j>0;j--){ // Imprimim el vector cami del revés per tal que el camí surti del dret, i-1 perque al while d'abans la i augmenta 1 posició que no utilitzem just abans de sortir
    printf("|"); //el node origen
    for(int i=0; i<ngerres; i++){
    printf("%d|",configuracions[cami[j]].continguts[i]); //el node origen
    }
    printf("-(%d,%d)-",configuracions[cami[j-1]].origen,configuracions[cami[j-1]].desti); //imprimeix contingut del 0, després origen i destí del 4 després el contingut 4...
  }

  printf("|"); //el node origen
  for(int i=0; i<ngerres; i++){
  printf("%d|",configuracions[cami[0]].continguts[i]);
  }
  printf("\n");
}

int posicio (configuracio configuracions,gerra gerres[],int ngerres){
  int id_configuracio=0;
  int multiplicador=1; //perquè quan multipliqui no quedi tot 0

  for(int i=0;i<ngerres;i++){ //en el cas {8,8,8} capacitat i {0,8,0} congingut quedaria:
    id_configuracio+=configuracions.continguts[i]*multiplicador; //0+9*8+0
    multiplicador*=(gerres[i].capacitat+1); //9+9+9
  }
  return id_configuracio; //9*8=72
}
