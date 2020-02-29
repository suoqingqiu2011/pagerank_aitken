#include "Aitken.h"
#include "Graphe.h"

void copie(double* pi_k,double* tmp,int n)
{
	for(int i = 0 ; i < n ; i++)
	{
		pi_k[i] = tmp[i];
	}
}

void Projection(double* pi_k, double* pi_k_moins1, double* pi_k_moins2,int n)
{
	double g = 0;
	double h = 0;
	double tmp1 = 0;
	double* tmp = (double*) malloc((n)*sizeof(double)+1);
	int i = 0 ;
	for(i = 0 ; i < n ; i++)
	{
		tmp1 = (pi_k_moins1[i] - pi_k_moins2[i]);
		g = tmp1 * tmp1;
		h = pi_k[i]-(2*pi_k_moins1[i]) + pi_k_moins2[i];
		if (!(h != 0 && pi_k[i] > (g/h)))
		{
			return;
		}
		tmp[i] =  pi_k[i] - (g/h);
	}	
	copie(pi_k,tmp,n);
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
		
		if(k  == 15){
			Projection(pi_k,pi_k_moins1,pi_k_moins2,n);	//on est censé acceler periodiquement
			 printf("ite:%d norme: %.10f\n",k,norme);	//on est censé acceler periodiquement
		}
		if(k == 16) {
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
