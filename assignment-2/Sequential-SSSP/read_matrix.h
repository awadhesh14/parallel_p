#include <stdio.h>
#include <stdlib.h>
#include "../input/mmio.h"

struct Edge
{
    int src, sink;
    double wght;
};

void read_matrix(char *path,int *M, int *N,int *nz,int I[],int J[],double val[]){
  // printf("%d %d %d \n",*M,*N,*nz);
  FILE *f;
  MM_typecode matcode;
  int ret_code;

  if ((f = fopen(path/*"../input/webbase-1M/webbase-1M.mtx"*/, "r")) == NULL)
      exit(1);


  if (mm_read_banner(f, &matcode) != 0)
  {
      printf("Could not process Matrix Market banner.\n");
      exit(1);
  }

  if (mm_is_complex(matcode) && mm_is_matrix(matcode) &&
          mm_is_sparse(matcode) )
  {
      printf("Sorry, this application does not support ");
      printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
      exit(1);
  }
  // printf("the matcode is %d  \n",matcode );

  /* find out size of sparse matrix .... */
  if ((ret_code = mm_read_mtx_crd_size(f, M, N, nz)) !=0){
    printf("problemmo \n");
      exit(1);
    }

    /* reseve memory for matrices */
    I = (int *) malloc((*nz) * sizeof(int));
    J = (int *) malloc((*nz) * sizeof(int));
    val = (double *) malloc((*nz) * sizeof(double));

  /* Read the matrix */
  int i;
  for (i=0; i<(*nz); i++)
  {
      fscanf(f, "%d %d %lg\n", &I[i], &J[i], &val[i]);
      I[i]--;  /* adjust from 1-based to 0-based */
      J[i]--;
  }
  if (f !=stdin) fclose(f);
  printf("--------------read complete--------------\n");

}
void read_M_N_nz(char *path,int *M, int *N,int *nz){
  FILE *f;
  MM_typecode matcode;
  int ret_code;
  if ((f = fopen(path, "r")) == NULL)
      exit(1);


  if (mm_read_banner(f, &matcode) != 0)
  {
      printf("Could not process Matrix Market banner.\n");
      exit(1);
  }

  if (mm_is_complex(matcode) && mm_is_matrix(matcode) &&
          mm_is_sparse(matcode) )
  {
      printf("Sorry, this application does not support ");
      printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
      exit(1);
  }
  // printf("the matcode is %d  \n",matcode );

  /* find out size of sparse matrix .... */
  if ((ret_code = mm_read_mtx_crd_size(f, M, N, nz)) !=0){
    printf("problemmo \n");
      exit(1);
    }
    // printf("ok\n");
    if (f !=stdin) fclose(f);
    return;
}
void read_Mat(char *path,int M,int N,int nz,Edge edges[]){
  
  FILE *f;
  MM_typecode matcode;
  int ret_code;

  if ((f = fopen(path/*"../input/webbase-1M/webbase-1M.mtx"*/, "r")) == NULL)
      exit(1);


  if (mm_read_banner(f, &matcode) != 0)
  {
      printf("Could not process Matrix Market banner.\n");
      exit(1);
  }

  if (mm_is_complex(matcode) && mm_is_matrix(matcode) &&
          mm_is_sparse(matcode) )
  {
      printf("Sorry, this application does not support ");
      printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
      exit(1);
  }


  if ((ret_code = mm_read_mtx_crd_size(f, &M, &N, &nz)) !=0)
      exit(1);



  int i;
  int p,q;
  double val;
  for (i=0; i<nz; i++)
  {
      fscanf(f, "%d %d %lg\n", &p, &q, &val);
      // printf("%d %d %lg\n",p,q,val);
      p--;q--;
      edges[i].src = p;
      edges[i].sink= q;
      edges[i].wght= val;
      // I[i]--;  /* adjust from 1-based to 0-based */
      // J[i]--;
  }
}
