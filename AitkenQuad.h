#ifndef __include_AitkenQuad_h__
#define __include_AitkenQuad_h__
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include "Graphe.h"
#include "AitkenQuad.h"
double sum(double* v,int n);
void ProjectionQuadratique(double* pi_k, double* pi_k_moins1, double* pi_k_moins2,double* pi_k_moins3,int n);
void AitkenQuadratique(MatCreuse* tableau_arrive,int* E, int n);
#endif 
