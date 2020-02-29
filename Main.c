#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include "time.h"
#include "Graphe.h"
#include "Powers.h"
#include "Aitken.h"
#include "AitkenQuad.h"
int main(int argc, char *argv[]){
	int n=0;
	int m=0;
	int i;
	MatCreuse* tableau_arrive = NULL;
	int *E;
	clock_t start1, finish1;
	clock_t start2, finish2;
	clock_t start3, finish3;	
	clock_t start4, finish4;	
	
	// lecture graphe etc 
	tableau_arrive = read_fiche(&n,&m,argv[2]);
	
	E = (int*) malloc((n)*sizeof(int)+1);

   
	for(i=0; i<n; i++){
		E[i]=1;
	}
	
	
	printf("\nMatrice creuse est  \n");
	MatCreuse temp;
	for(i=0; i<n; i++) {
		temp = tableau_arrive[i];
		while(temp != NULL){
			//printf("line:%d column:%d :%f\n", temp->ligne,temp->colonne,temp->proba);

			E[(temp->ligne)-1]=0;//definir la valeur de 'f-T (T= E ??)'
			temp = temp->suiv;
		}
	}
	
	printf("\n<<< Methode Power >>>\n");
	start1 = clock();

	//~ ranking_matM(tableau_arrive, n);

	finish1= clock();
	printf( "\n%f seconds\n", (double)(finish1-start1) / CLOCKS_PER_SEC);
	
	printf("\n<<< Methode Power Google >>>\n");
	start2 = clock();

	ranking_matG(tableau_arrive,E,n);

	finish2 = clock();
	printf( "\n%f seconds\n", (double)(finish2-start2) / CLOCKS_PER_SEC);
	
	
	printf("\n<<< Methode Aitken >>>\n");
	start3 = clock();

	Aitken(tableau_arrive,E,n);

	finish3 = clock();
	printf( "\n%f seconds\n", (double)(finish3-start3) / CLOCKS_PER_SEC);
	
	
	printf("\n<<< Methode Aitken Quatrique>>>\n");
	start4 = clock();

	//~ AitkenQuadratique(tableau_arrive,E,n);

	finish4 = clock();
	printf( "\n%f seconds\n", (double)(finish4-start4) / CLOCKS_PER_SEC);
	
	free(tableau_arrive);
	free(E);
}
