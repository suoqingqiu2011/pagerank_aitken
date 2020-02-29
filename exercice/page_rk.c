#include"stdio.h"
#include"stdlib.h"
#include <string.h>
#include "time.h"

#define epsilon 1e-6

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
	file = fopen("web1.txt","r");
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
				//printf("pi_pre[%d]:%f\n",j,pi_pre[j]);
				//printf("pi_suiv[%d]:%f\n",j,pi_suiv[j]);
				//printf("tableau_arrive[%d]:%s \n",j,tableau_arrive[j]);
				//printf("temp->colonne-1: %d ",temp->colonne-1);
				//printf("temp->ligne-1: %d, pi_pre[temp->ligne-1]:%f temp->proba:%f\n",temp->ligne-1,pi_pre[temp->ligne-1],temp->proba);
				if(j==temp->colonne-1){
					pi_suiv[j] += pi_pre[temp->ligne-1]*(double)temp->proba;
				}
				temp = temp->suiv;
			}
			//printf("%f ",pi_suiv[j]);
		}//printf("\n");

		for(j=0;j<(n);j++){
			norme+=valeur_absolue(pi_pre[j],pi_suiv[j]);
		}
		printf("ite:%dnorme: %.10f\n",compteur,norme);
		compteur++;
	}
	free(pi0);
	free(pi_pre);
	free(pi_suiv);
}

void ranking_matG(MatCreuse* tableau_arrive,float alpha,int* T,int n){
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

	for(i=0;i<(n);i++){
		pi0[i] = 1.0/(n);
		//printf("pi0[%d]:%f \n",i, pi0[i]);
	}

	while(norme > epsilon){
		deta=0.0;
		norme=0.0;
		for(i=0;i<(n);i++){
			if(compteur==1){
				pi_pre[i] = pi0[i];	
			}else{
				pi_pre[i] = pi_suiv[i];
			}
			pi_suiv[i]=0.0;
		}//printf("\n");

		for(i=0;i<(n);i++){
			deta+=pi_pre[i]*T[i];
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

			pi_suiv[j] = alpha*tmp[j] + (1.0-alpha)*(1.0/n) + alpha*deta*(1.0/n);
			//printf("pi_suiv[%d]:%f ",j,pi_suiv[j]);
		}//printf("\n");
		for(j=0;j<(n);j++){
			norme+=valeur_absolue(pi_pre[j],pi_suiv[j]);
		}
		printf("ite:%dnorme: %.10f\n",compteur,norme);
		compteur++;
	}
	free(pi0);
	free(pi_pre);
	free(pi_suiv);
	free(tmp);
}


void sor(MatCreuse* tableau_arrive,float damp_val,float omega,int n,double *pi0,double *pi_k,double *pi_k_1,double *pi_tmp,int compteur){
	double tmp1;
	double tmp2;
	double norme=0.0;
	int i,j;
	
	MatCreuse temp1;
	MatCreuse temp2;

	
	if(compteur!=1){
		for(i=0;i<(n);i++){
			pi_k[i] = pi_k_1[i];
		}
	}

	for(i=0;i<(n);i++){
		tmp1=0.0;
		tmp2=0.0;
		for(j=0;j<i;j++){  
			temp1 = tableau_arrive[j];
			while(temp1 != NULL){
				if(i==temp1->ligne-1 && j==temp1->colonne-1){
					tmp1 += (double)temp1->proba * pi_k_1[temp1->colonne-1];
				}
				temp1 = temp1->suiv;
			}
		}
		//printf(" tmp1:%f \n",tmp1);
		for(j=i;j<n;j++){ 
			temp1 = tableau_arrive[j];
			while(temp1 != NULL){
				if(i==temp1->ligne-1 && j==temp1->colonne-1){
					tmp2 += (double)temp1->proba * pi_k_1[temp1->colonne-1];
				}
				temp1 = temp1->suiv;
			}
		}
		//printf(" tmp2:%f \n",tmp2);
		for(j=0;j<(n);j++){
			pi_tmp[j]=0.0;
			temp2 = tableau_arrive[j];
			while(temp2 != NULL){
				if(j==temp2->colonne-1){
					pi_tmp[j] += pi_k[temp2->ligne-1]*(double)temp2->proba;
				}
				temp2 = temp2->suiv;
			}
		//printf("%f ",pi_tmp[j]);
		}//printf("\n");
		//printf("pi_k[%d]:%f ",i,pi_k[i]);
		pi_k_1[i] = omega*(damp_val*tmp1 + damp_val*tmp2 + (1.0-damp_val) ) + (1-omega)*pi_tmp[i];
		//printf("pi_k_1[%d]:%f\n",i,pi_k_1[i]);
	}//printf("\n");

	for(j=0;j<(n);j++){
		norme+=valeur_absolue(pi_k_1[j],pi_k[j]);
	}
	printf("ite:%dnorme: %.10f\n",compteur,norme);
	compteur++;
	if(norme > epsilon){
		sor(tableau_arrive,damp_val,omega,n,pi0,pi_k,pi_k_1,pi_tmp,compteur);	
	}

}

void ranking_sor(MatCreuse* tableau_arrive,float damp_val,float omega,int n){
	//double norme=1.0;
	double *pi0;
	double *pi_k;
	double *pi_k_1;
	double *pi_tmp;
	//double tmp1;
	//double tmp2;

	int i;
	int compteur=1;	

	pi0 = (double*) malloc((n)*sizeof(double)+1);
	pi_k = (double*) malloc((n)*sizeof(double)+1);
	pi_k_1 = (double*) malloc((n)*sizeof(double)+1);
	pi_tmp = (double*) malloc((n)*sizeof(double)+1);

	for(i=0;i<(n);i++){
		pi0[i] = 1.0/(n);
		//printf("pi0[%d]:%f \n",i, pi0[i]);
	}
	for(i=0;i<(n);i++){	
		pi_k[i] = pi0[i];
		pi_k_1[i] = 0;
	}
	sor(tableau_arrive,damp_val,omega,n,pi0,pi_k,pi_k_1,pi_tmp,compteur);
	
	free(pi0);
	free(pi_k);
	free(pi_k_1);
	free(pi_tmp);
}


int main(){
	int n=0;
	int m=0;
	int i;
	MatCreuse* tableau_arrive = NULL;
	int *T;
	clock_t start1, finish1;
	clock_t start2, finish2;
	clock_t start3, finish3;	

	tableau_arrive = read_fiche(&n,&m);
	
	T = (int*) malloc((n)*sizeof(int)+1);

	for(i=0; i<n; i++){
		T[i]=1;
	}
	printf("\nMatrice creuse est  \n");

	MatCreuse temp;
	for(i=0; i<n; i++) {
		temp = tableau_arrive[i];
		while(temp != NULL){
			//printf("line:%d column:%d :%f\n", temp->ligne,temp->colonne,temp->proba);

			T[(temp->ligne)-1]=0;//definir la valeur de 'f-T'
			temp = temp->suiv;
		}
	}

	/*for(i=0; i<n; i++) {
		printf("T[%d]:%d\n",i,T[i]);
	}*/

		/****** Methode de puissance ******/
	/*printf("\n<<< Methode 1 >>>\n");
	start1 = clock();

	ranking_matM(tableau_arrive, n);

	finish1 = clock();
	printf( "\n%f seconds\n", (double)(finish1-start1) / CLOCKS_PER_SEC);

		/****** Methode de puissance ameliore ******/

	/*float alpha=0.85;
	printf("\n<<< Methode 2 >>>\n");
	start2 = clock();

	ranking_matG(tableau_arrive,alpha,T,n);

	finish2 = clock();
	printf( "\n%f seconds\n", (double)(finish2-start2) / CLOCKS_PER_SEC);*/
	
	float damp_val=0.85;
	float omega=0.8;
	printf("\n<<< Methode 3 :sor>>>\n");
	start3 = clock();

	ranking_sor(tableau_arrive,damp_val,omega,n);

	finish3 = clock();
	printf( "\n%f seconds\n", (double)(finish3-start3) / CLOCKS_PER_SEC);

	free(tableau_arrive);
	free(T);
	return 0;
}
