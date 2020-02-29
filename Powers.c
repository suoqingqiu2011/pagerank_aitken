#include "Powers.h"
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
	printf("Pertinence finales : \n");
	for(j=0;j<(n);j++){ 
		printf("%f \n",pi_suiv[j]);
	}
	printf("fin \n");
	printf("ite:%d norme: %.10f\n",compteur,norme);
	free(pi0);
	free(pi_pre);
	free(pi_suiv);
	free(tmp);
}
