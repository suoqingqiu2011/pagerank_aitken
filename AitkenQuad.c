#include "Graphe.h"
#include "AitkenQuad.h"
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
