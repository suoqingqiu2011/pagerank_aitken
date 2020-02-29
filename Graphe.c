#define epsilon 1e-6
#define alpha 0.85
#include "Graphe.h"
//~ struct matriceCreuse{
	//~ float proba;
	//~ int ligne;
	//~ int colonne;
	//~ struct matriceCreuse* suiv;
//~ };

//~ typedef struct matriceCreuse Element;
//~ typedef Element* MatCreuse;

MatCreuse ajout_matrice(MatCreuse mat,float proba, int i, int j){
	Element* e = malloc(sizeof(Element));
	e->proba = proba;
	e->ligne = i;
	e->colonne = j;
	e->suiv = mat;
	return e;
}

void initialiser_tab(MatCreuse* tableau_arrive,int *n){
    int i;
	for(i=0; i<(*n); i++){
		tableau_arrive[i] = NULL;
	}
}

double valeur_absolue(double a, double b){
	if(a<b) return b-a;
	else return a-b;
}

MatCreuse* read_fiche(int *n,int *m,char* fichier){
	int depart;
	int degre;
	int successeur;
	float proba;
	int i,j;
	MatCreuse* tableau_arrive;


	FILE* file=NULL;
	file = fopen("web2.txt","r");
	//~ file = fopen("./graphes/Stanford.txt","r");
	//~ file = fopen("./graphes/in-2004v2.txt","r");

	if(file!= NULL) {
		fscanf(file, "%d", n);
		fscanf(file, "%d", m);
		printf("nb etat:%d nb flech:%d\n",*n,*m);

		tableau_arrive = malloc((*n)*sizeof(MatCreuse*)+1); //tableau de pointeurs
		initialiser_tab(tableau_arrive,n);

		for(i=0;i<(*n);i++){
			fscanf(file, "%d %d",&depart,&degre);
			//printf("\ndepart:%d degre:%d\n", depart, degre);

			for(j=0;j< degre;j++){
				fscanf(file, "%d %f",&successeur,&proba);
				//printf("successeur:%d proba:%.1f ", successeur, proba);

				tableau_arrive[successeur-1]=ajout_matrice(tableau_arrive[successeur-1], proba, depart, successeur);
				//printf("tableau_arrive[%d]->proba: %f ,%d,%d \n",successeur, tableau_arrive[successeur-1]->proba,tableau_arrive[successeur-1]->ligne,tableau_arrive[successeur-1]->colonne);

			}
		}
		printf("\n");
	}else{
		fprintf(file,"Can not open the file or it does not exist\n");
	}

	fclose(file);
	return tableau_arrive;
}
