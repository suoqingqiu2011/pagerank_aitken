#include"stdio.h"
#include"stdlib.h"
#include <string.h>
#include "time.h"

#define epsilon 1e-6

/* la chaine pour stocker des probas non_nuls*/
struct matrice{
	float proba;
	int ligne;
	int colonne;
	struct matrice* suiv;
};

typedef struct matrice Element;
typedef Element* Mat;

Mat ajout_matrice(Mat mat,float proba, int i, int j){
	Element* e = malloc(sizeof(Element));
	e->proba = proba;
	e->ligne = i;
	e->colonne = j;
	e->suiv = mat;
	return e;
}

void initialiser_tab(Mat* tableau_arrive,int *n){
    	int i;
	for(i=0; i<(*n); i++){
		tableau_arrive[i] = NULL;
	}
}

double valeur_absolue(double a, double b){
	if(a<b) return b-a;
	else return a-b;
}

Mat* read_fiche(int *n,int *m){
	int depart;
	int degre;
	int successeur;
	float proba;
	int i,j;
	Mat* tableau_arrive;


	FILE* file=NULL;
	//file = fopen("web3.txt","r");
	file = fopen("in-2004v2.txt","r");

	if(file!= NULL) {
		fscanf(file, "%d", n);
		fscanf(file, "%d", m);
		printf("nb etat:%d nb flech:%d\n",*n,*m);

		tableau_arrive = malloc((*n)*sizeof(Mat*)+1); //tableau de pointeurs
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

void ranking_matM(Mat* tableau_arrive, int n){
	
	double norme=1.0;
	double *pi0;
	double *pi_pre;
	double *pi_suiv;

	int i,j,t;
	int compteur=1;
	Mat temp;

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

void ranking_matG(Mat* tableau_arrive,float alpha,int* T,int n){
	double norme=1.0;
	double *pi0;
	double *pi_pre;
	double *pi_suiv;
	double *tmp;
	double deta;

	int i,j,t;
	int compteur=1;
	Mat temp;

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
					//printf("tmp[%d] %f\n",j,tmp[j]);
					//printf("pi_pre[%d] %f\n",temp->ligne-1,pi_pre[temp->ligne-1]);
				}
				temp = temp->suiv;
			}
			//printf(" tmp[%d]:%f ",j,tmp[j]);

			pi_suiv[j] = alpha*tmp[j] + (1.0-alpha)*(1.0/n) + alpha*deta*(1.0/n);
			//printf("pi_suiv[%d]:%f ",j,pi_suiv[j]);  printf("\n********************\n");
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


void ranking_sor(Mat* tableau_arrive,float omega,int n){
	double norme=1.0;
	double *pi0;
	double *pi_k;
	double *pi_k_1;
	double *y;
	double *g;
	double *z;
	double G=0.0;
	double alpha=0.85;

	double tmp1=0;
	double tmp2=0;
	

	int i,j;
	int compteur=1;
	Mat temp1;
	Mat temp2;	

	pi0 = (double*) malloc((n)*sizeof(double)+1);
	pi_k = (double*) malloc((n)*sizeof(double)+1);
	pi_k_1 = (double*) malloc((n)*sizeof(double)+1);
	y = (double*) malloc((n)*sizeof(double)+1);
	g = (double*) malloc((n)*sizeof(double)+1);
	z = (double*) malloc((n)*sizeof(double)+1);	


	for(i=0;i<(n);i++){
		pi0[i] = 1.0/(n);
		//printf("pi0[%d]:%f \n",i, pi0[i]);
	}

	while(norme > epsilon ){
		norme=0.0;
		for(i=0;i<(n);i++){
			if(compteur==1){
				pi_k[i] = pi0[i]; //pi(k)
				pi_k_1[i] = pi0[i]; //pi(k+1)
			}else{
				pi_k[i] = pi_k_1[i];
			}
			//printf("pi_k_1[%d]:%.10f\n",i,pi_k_1[i]);
		}//printf("-----------------------------------\n");
		
		for(i=0;i<n;i++){
			tmp1=0.0;
			tmp2=0.0;  
			
			temp1 = tableau_arrive[i];
			while(temp1 != NULL){
				//printf("l:%d c:%d proba:%f \n",temp1->ligne-1,temp1->colonne-1,temp1->proba);
				G = alpha*(temp1->proba) + (1-alpha)*(1.0/n);
				if(i==temp1->colonne-1){	
					if((temp1->ligne-1)<i){
						tmp1 += (double)G* pi_k_1[temp1->ligne-1]; 
						//printf("pik1[%d]:%.10f\n",temp1->ligne-1,pi_k_1[temp1->ligne-1]);
						//printf("l:%d c:%d proba:%f \n",temp1->ligne-1,temp1->colonne-1,temp1->proba);
						//printf("compteur:%d tmp1:%f \n",compteur,tmp1);
					}else if((temp1->ligne-1)>i && (temp1->ligne-1)<n){
						tmp2 += (double)G * pi_k[temp1->ligne-1];
						//printf("pik[%d]:%.10f\n",temp1->ligne-1,pi_k[temp1->ligne-1]);
						//printf("l:%d c:%d proba:%f \n",temp1->ligne-1,temp1->colonne-1,temp1->proba);
						//printf(" tmp2:%f \n",tmp2);					
					}
				}
				if(i==temp1->colonne-1 && i==temp1->ligne-1){
					g[i] = alpha*(temp1->proba) + (1-alpha)*(1.0/n);				
				}else{
					g[i] = (1-alpha)*(1.0/n);				
				}
				temp1 = temp1->suiv;
				//printf("-------------------------\n");
			}
			
			z[i]=(tmp1+tmp2)/(1.0-g[i]);
			
			
			//printf("z[%d]: %.10f\n",i,z[i]);			

			y[i]=0.0;
			temp2 = tableau_arrive[i];
			while(temp2 != NULL){
				G = (double) alpha*temp2->proba + (1-alpha)*(1.0/n);
				if(i==temp2->colonne-1){
					y[i] += (double)G * pi_k[temp2->ligne-1];
				}
				temp2 = temp2->suiv;
			}
			//printf("y[%d]:%.10f\n",i,y[i]);
			pi_k_1[i] = omega*y[i]+ (1-omega)*z[i]; 
			
			//printf("pi_k_1[%d]:%.10f\n",i,pi_k_1[i]);printf("++++++++++++++++\n");
		}//printf("\n");  
		
		for(j=0;j<(n);j++){
			norme+=valeur_absolue(pi_k_1[j],pi_k[j]);
		}
		printf("ite:%dnorme: %.10f\n",compteur,norme);
		//printf("************************\n");
		compteur++;
	}
	
	free(pi0);
	free(pi_k);
	free(pi_k_1);
	free(y);
	free(g);
	free(z);
}


int main(){
	int n=0;
	int m=0;
	int i;
	Mat* tableau_arrive = NULL;
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

	Mat temp;
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
	printf("\n<<< Methode 1 (methode de puissance) :>>>\n");
	start1 = clock();

	//ranking_matM(tableau_arrive, n);

	finish1 = clock();
	printf( "\n%f seconds\n", (double)(finish1-start1) / CLOCKS_PER_SEC);

		/****** Methode de puissance ameliore ******/

	float alpha=0.85;
	printf("\n<<< Methode 2 (methode de puissance ameliore) :>>>\n");
	start2 = clock();

	ranking_matG(tableau_arrive,alpha,T,n);

	finish2 = clock();
	printf( "\n%f seconds\n", (double)(finish2-start2) / CLOCKS_PER_SEC);
	
	float omega=0.8;
	printf("\n<<< Methode 3 (sor):>>>\n");
	start3 = clock();

	ranking_sor(tableau_arrive,omega,n);

	finish3 = clock();
	printf( "\n%f seconds\n", (double)(finish3-start3) / CLOCKS_PER_SEC);

	free(tableau_arrive);
	free(T);
	return 0;
}
