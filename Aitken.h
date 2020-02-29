#ifndef __include_Aitken_h__
#define __include_Aitken_h__
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include "Graphe.h"
void Projection(double* pi_k, double* pi_k_moins1, double* pi_k_moins2,int n);
void Aitken(MatCreuse* tableau_arrive,int* E, int n);
#endif 
