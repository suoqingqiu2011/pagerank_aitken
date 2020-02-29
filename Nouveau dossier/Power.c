#include"stdio.h"
#include"stdlib.h"
#include <string.h>
#include "time.h"

#define epsilon 1e-6
#define alpha 0.85
struct matriceCreuse{
	float proba;
	int ligne;
	int colonne;
	struct matriceCreuse* suiv;
};

typedef struct matriceCreuse Element;
typedef Element* MatCreuse;

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

MatCreuse* read_fiche(int *n,int *m){
	int depart;
	int degre;
	int successeur;
	float proba;
	int i,j;
	MatCreuse* tableau_arrive;


	FILE* file=NULL;
	file = fopen("web2.txt","r");
	//~ file = fopen("Stanford.txt","r");
	//file = fopen("in-2004v2.txt","r");

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

//Fonction power methode
void ranking_matM(MatCreuse* tableau_arrive, int n){
	/*double pi0[n];
	double pi_pre[n];
	double pi_suiv[n];*/
	double norme=1.0;
	double *pi0;
	double *pi_pre;
	double *pi_suiv;

	int i,j,t;
	int compteur=1;
	MatCreuse temp;

	pi0 = (double*) malloc((n)*sizeof(double)+1);
	pi_pre = (double*) malloc((n)*sizeof(double)+1);
	pi_suiv = (double*) malloc((n)*sizeof(double)+1);

	for(i=0;i<(n);i++){
		pi0[i] = 1.0/(n);
		//printf("pi0[%d]:%f \n",i, pi0[i]);
	}

	while(norme > epsilon){

		norme=0.0;
		for(i=0;i<(n);i++){
			if(compteur==1){
				pi_pre[i] = pi0[i];       
			}else{
				pi_pre[i] = pi_suiv[i];	
			}pi_suiv[i]=0.0;
		}//printf("\n");

		//printf("pi%d:\n",compteur);

		for(j=0;j<(n);j++){
			temp = tableau_arrive[j];
			while(temp != NULL){
				if(j==temp->colonne-1){
					pi_suiv[j] += pi_pre[temp->ligne-1]*(double)temp->proba;
				}
				temp = temp->suiv;
			}
			//printf("%f ",pi_suiv[j]);
		}//printf("\n");

		for(j=0;j<(n);j++){
			norme += valeur_absolue(pi_pre[j],pi_suiv[j]);
		}
	//	printf("ite:%d norme: %.10f\n",compteur,norme);
		compteur++;
	}
	printf("Pertinence finales : \n");
	for(j=0;j<(n);j++){ 
		printf("%f \n",pi_suiv[j]);
	}
	printf("fin \n");
	printf("ite:%d norme: %.10f\n",compteur,norme);
	free(pi0);
	free(pi_pre);
	free(pi_suiv);
}

//Fonction Power de Google

void ranking_matG(MatCreuse* tableau_arrive,int* E,int n){
	double norme=1.0;
	double *pi0;
	double *pi_pre;
	double *pi_suiv;
	double *tmp;
	double deta;

	int i,j,t;
	int compteur=1;
	MatCreuse temp;

	pi0 = (double*) malloc((n)*sizeof(double)+1);
	pi_pre = (double*) malloc((n)*sizeof(double)+1);
	pi_suiv = (double*) malloc((n)*sizeof(double)+1);
	tmp = (double*) malloc((n)*sizeof(double)+1);
	
	// Init P°0
	for(i=0;i<(n);i++){
		pi0[i] = 1.0/(n);
		//printf("pi0[%d]:%f \n",i, pi0[i]);
	}

	while(norme > epsilon){
		deta=0.0; //deta = e*f°t avec f vecteur ligne 1 si vecteur 0 sinon
		norme=0.0;
		for(i=0;i<(n);i++){
			if(compteur==1){
				pi_pre[i] = pi0[i];	
			}else{
				pi_pre[i] = pi_suiv[i];
			}
			pi_suiv[i]=0.0;
		}//printf("\n");
		
		// ???? deta != norme 
		for(i=0;i<(n);i++){
			deta+=pi_pre[i]*E[i];
		}//printf("deta%d: %lf \n",compteur,deta);

		
		for(j=0;j<(n);j++){ 
			tmp[j]=0.0;
			temp = tableau_arrive[j];
			while(temp != NULL){
				if(j==temp->colonne-1){
					tmp[j] += pi_pre[temp->ligne-1]*(double)temp->proba;
				}

				temp = temp->suiv;
			}
			//printf(" tmp[%d]:%f ",j,tmp[j]);

			pi_suiv[j] = alpha*tmp[j] + (1.0-alpha)*(1.0/n) + alpha*deta*(1.0/n); // formule de google 
			//printf("pi_suiv[%d]:%f ",j,pi_suiv[j]);
		}//printf("\n");
		//calcul norme 1 
		for(j=0;j<(n);j++){
			norme+=valeur_absolue(pi_pre[j],pi_suiv[j]);
		}
		//printf("ite:%d norme: %.10f\n",compteur,norme);
		compteur++;
	} 
	//~ printf("Pertinence finales : \n");
	//~ for(j=0;j<(n);j++){ 
		//~ printf("%f \n",pi_suiv[j]);
	//~ }
	//~ printf("fin \n");
	printf("ite:%d norme: %.10f\n",compteur,norme);
	free(pi0);
	free(pi_pre);
	free(pi_suiv);
	free(tmp);
}


//fonction ranking avec aitken

void Projection(double* pi_k, double* pi_k_moins1, double* pi_k_moins2,int n)
{
	double g = 0;
	double h = 0;
	double tmp = 0;
	int i = 0 ;
	for(i = 0 ; i < n ; i++)
	{
		tmp = (pi_k_moins1[i] - pi_k_moins2[i]);
		g = tmp * tmp;
		h = pi_k[i]-(2*pi_k_moins1[i]) + pi_k_moins2[i];
		//if(h != 0 && pi_k[i] > (g/h))
			pi_k[i] =  pi_k[i] - (g/h);
	}	
}

void Aitken(MatCreuse* tableau_arrive,int* E, int n){
	double norme=1.0;
	double *pi_0 = (double*) malloc((n)*sizeof(double)+1);
	double *pi_k = (double*) malloc((n)*sizeof(double)+1);
	double *pi_k_moins1 = (double*) malloc((n)*sizeof(double)+1);
	double *pi_k_moins2 = (double*) malloc((n)*sizeof(double)+1);
	double *tmp = (double*) malloc((n)*sizeof(double)+1);
	double deta;
	MatCreuse temp;
	
	int i,j,t;
	int k=1;
	// Init P°0
	for(i=0;i<(n);i++){
		pi_0[i] = 1.0/(n);
	}	
	while(norme > epsilon)
	{		
		deta=0.0;
		norme=0.0;
		for(i=0;i<(n);i++){
			if(k >=2){
				pi_k_moins2[i] = pi_k_moins1[i];
			}
			if(k==1){
				pi_k_moins1[i] = pi_0[i];	
			}
			else 
			{
				pi_k_moins1[i] = pi_k[i];
			}
			pi_k[i]=0.0;
		}

		for(i=0;i<(n);i++){
			deta+=pi_k_moins1[i]*E[i];
		}
		for(j=0;j<(n);j++){ 
			tmp[j]=0.0;
			temp = tableau_arrive[j];
			while(temp != NULL){
				if(j==temp->colonne-1){
					//~ pi_k[j] += pi_k_moins1[temp->ligne-1]*(double)temp->proba;
					tmp[j] += pi_k_moins1[temp->ligne-1]*(double)temp->proba;
				}

				temp = temp->suiv;
			}  
			//printf(" tmp[%d]:%f ",j,tmp[j]);

			pi_k[j] = alpha*tmp[j] + (1.0-alpha)*(1.0/n) + alpha*deta*(1.0/n);
			//printf("pi_suiv[%d]:%f ",j,pi_suiv[j]);
		}
		
		for(j=0;j<(n);j++){
			norme+=valeur_absolue(pi_k_moins1[j],pi_k[j]);
		}
		
		if(k  == 20){
			Projection(pi_k,pi_k_moins1,pi_k_moins2,n);	//on est censé acceler periodiquement
			 printf("ite:%d norme: %.10f\n",k,norme);	//on est censé acceler periodiquement
		}
		if(k == 21) {
			printf("ite:%d norme: %.10f\n",k,norme);
			
		}
		//printf("ite:%d norme: %f\n",k,norme);
		k++;
	}
	//~ printf("Pertinence finales : \n");
	//~ for(j=0;j<(n);j++){ 
		//~ printf("%f \n",pi_k[j]);
	//~ }
	printf("fin \n");
	printf("ite:%d norme: %.10f\n",k,norme);
	free(pi_0);
	free(pi_k);
	free(pi_k_moins1);
	free(pi_k_moins2);
	free(tmp);
}

double sum(double* v,int n){
	int sum = 0;
	for(int i=0;i<n;i++)
	{
		sum +=v[i];
	}
}

void ProjectionQuadratique(double* pi_k, double* pi_k_moins1, double* pi_k_moins2,double* pi_k_moins3,int n)
{
	double *y_k = (double*) malloc((n)*sizeof(double)+1);
	double* y_k_moins1 = (double*) malloc((n)*sizeof(double)+1);
	double* y_k_moins2 = (double*) malloc((n)*sizeof(double)+1);
	double Beta0,Beta1,Beta2;
	double Alpha1,Alpha2,Alpha3,Alpha4;
	double Gamma0,Gamma1,Gamma2,Gamma3;
	int i,j;
	for(i=0;i<n;i++)
	{
/*--------------------------------------------------------------*/
		y_k[i] = pi_k[i] - pi_k_moins3[i];
		y_k_moins1[i] = pi_k_moins1[i] - pi_k_moins3[i];
		y_k_moins2[i] = pi_k_moins2[i] - pi_k_moins3[i];
/*--------------------------------------------------------------*/	
}	/*
	for(i=0;i<n;i++)
	{
		printf("xk[%d] = %f ",i,pi_k[i]);
		printf("xk-1[%d] = %f ",i,pi_k_moins1[i]);
		printf("xk-2[%d] = %f ",i,pi_k_moins2[i]);
		printf("xk-3[%d] = %f ",i,pi_k_moins3[i]);
		printf("\n");
	}	
	printf("\n");
	
	for(i=0;i<n;i++)
	{
		printf("yk[%d] = %f ",i,y_k[i]);
		printf("yk-1[%d] = %f ",i,y_k_moins1[i]);
		printf("yk-2[%d] = %f ",i,y_k_moins2[i]);
		printf("\n");
	}	
	*/
	Alpha1 = sum(y_k_moins1,n); Alpha1 = Alpha1 * Alpha1;
	Alpha2 = sum(y_k_moins1,n) * sum(y_k_moins2,n);
	Alpha3 = Alpha2;
	Alpha4 =sum(y_k_moins2,n); Alpha4 = Alpha4 * Alpha4;
	//printf("Alphas : %f %f %f %f \n",Alpha1,Alpha2,Alpha3,Alpha4);
	for(i=0;i<n;i++){
	Gamma3 = 1;
	Gamma1 =-((Alpha4-Alpha2)*(y_k_moins1[i]+y_k_moins2[i])) * y_k[i]; 
	Gamma2 =-((Alpha1-Alpha3)*(y_k_moins1[i]+y_k_moins2[i])) * y_k[i];
	//printf("Gamma : %.10f %.10f \n",Gamma1,Gamma2);
	Beta0 = Gamma1 + Gamma2 + Gamma3 ;
	Beta1 = Gamma2 + Gamma3;
	Beta2 = Gamma3;
	//printf("Beta : %.10f %.10f \n",Beta1,Beta2);
	pi_k[i] = (Beta0 * pi_k_moins2[i] ) + (Beta1 * pi_k_moins1[i] )+(Beta2 * pi_k[i]);
	}
	for(j=0;j<(n);j++){ 
		printf("%f \n",pi_k[j]);
	}
	printf("fin \n");
}

void AitkenQuadratique(MatCreuse* tableau_arrive,int* E, int n){
	double norme=1.0;
	double *pi_0 = (double*) malloc((n)*sizeof(double)+1);
	double *pi_k = (double*) malloc((n)*sizeof(double)+1);
	double *pi_k_moins1 = (double*) malloc((n)*sizeof(double)+1);
	double *pi_k_moins2 = (double*) malloc((n)*sizeof(double)+1);
	double *pi_k_moins3 = (double*) malloc((n)*sizeof(double)+1);
	double *tmp = (double*) malloc((n)*sizeof(double)+1);
	double deta;
	MatCreuse temp;
	
	int i,j,t;
	int k=1;
	// Init P°0
	for(i=0;i<(n);i++){
		pi_0[i] = 1.0/(n);
	}	
	while(norme > epsilon)
	{		
		deta=0.0;
		norme=0.0;
		for(i=0;i<(n);i++){
			if(k>=3){
				pi_k_moins3[i] = pi_k_moins2[i];
			}
			if(k >=2){
				pi_k_moins2[i] = pi_k_moins1[i];
			}
			if(k==1){
				pi_k_moins1[i] = pi_0[i];	
			}
			else 
			{
				pi_k_moins1[i] = pi_k[i];
			}
			pi_k[i]=0.0;
		}

		for(i=0;i<(n);i++){
			deta+=pi_k_moins1[i]*E[i];
		}
		for(j=0;j<(n);j++){ 
			tmp[j]=0.0;
			temp = tableau_arrive[j];
			while(temp != NULL){
				if(j==temp->colonne-1){
					//~ pi_k[j] += pi_k_moins1[temp->ligne-1]*(double)temp->proba;
					tmp[j] += pi_k_moins1[temp->ligne-1]*(double)temp->proba;
				}

				temp = temp->suiv;
			}  
			//printf(" tmp[%d]:%f ",j,tmp[j]);

			pi_k[j] = alpha*tmp[j] + (1.0-alpha)*(1.0/n) + alpha*deta*(1.0/n);
			//printf("pi_suiv[%d]:%f ",j,pi_suiv[j]);
		}
		
		for(j=0;j<(n);j++){
			norme+=valeur_absolue(pi_k_moins1[j],pi_k[j]);
		}
		
		if(k ==10){
			 ProjectionQuadratique(pi_k,pi_k_moins1,pi_k_moins2,pi_k_moins3,n);
			 printf("ite:%d norme: %f.10\n",k,norme);	//on est censé acceler periodiquement
		}
		if(k == 11) {
			printf("ite:%d norme: %.10f\n",k,norme);
			
		}
		//printf("ite:%d norme: %f\n",k,norme);
		k++;
	}
	//~ printf("Pertinence finales : \n");
	//~ for(j=0;j<(n);j++){ 
		//~ printf("%f \n",pi_k[j]);
	//~ }
	//~ printf("fin \n");
	printf("ite:%d norme: %.10f\n",k,norme);
	free(pi_0);
	free(pi_k);
	free(pi_k_moins1);
	free(pi_k_moins2);
	free(tmp);
}

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
	tableau_arrive = read_fiche(&n,&m);
	
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

	AitkenQuadratique(tableau_arrive,E,n);

	finish4 = clock();
	printf( "\n%f seconds\n", (double)(finish4-start4) / CLOCKS_PER_SEC);
	
	free(tableau_arrive);
	free(E);
}
