using namespace std;

__device__ static float atomicMinF(float* address, float val)
{
    int* address_as_i = (int*) address;
    int old = *address_as_i, assumed;
    do {
        assumed = old;
        old = ::atomicCAS(address_as_i, assumed,
            __float_as_int(::fminf(val, __int_as_float(assumed))));
    } while (assumed != old);
    return __int_as_float(old);
}

__global__ void sssp_kernel_data(int M, int nz,char *d_worklist_in,char *d_worklist_out,
   bool *d_change, int *d_sources, int *d_sinks, float *d_weights, float *d_dist){

  int index = blockIdx.x * blockDim.x + threadIdx.x,v;
  if(index>=0 && index<M){

    // printf("index %d\n",index);
    // for(int j=0;j<M;j++)
    //   printf("%f ",d_dist[j] );
    // printf("\n");
    if(d_worklist_in[index] == '1'){

      for(int i=d_sources[index]; i<d_sources[index+1]; i++){
        v = d_sinks[i];
        // printf("1 %d %d\n",v,index);
        // printf("%f %f %f\n",d_dist[index] , d_weights[i], d_dist[v]);
        if(d_dist[index] + d_weights[i] < d_dist[v]){
          atomicMinF(&d_dist[v], d_dist[index] + d_weights[i]);
          d_worklist_out[v]='1';
          *d_change = true;
          // printf("kernal v=%d change %d\n",v,*d_change);
        }
      }
    }
  }

}

void data_driven(int M, int nz, int* sources, int* sinks, float* weights){

  /***********************allocating kernel memory***********************************/
  bool *d_change;
  int *d_sources, *d_sinks;
  float *d_weights, *d_dist;


  cudaMalloc( &d_change, sizeof(bool));
  cudaMalloc( (void **) &d_sources, (M+1) * sizeof(int)   );
  cudaMalloc( (void **) &d_sinks  , (nz)  * sizeof(int)   );
  cudaMalloc( (void **) &d_weights, (nz)  * sizeof(float) );
  cudaMalloc( (void **) &d_dist   , (M)   * sizeof(float) );

  cudaMemcpy(d_sources, sources, (M+1) * sizeof(int)  , cudaMemcpyHostToDevice);
  cudaMemcpy(d_sinks  , sinks  , (nz)  * sizeof(int)  , cudaMemcpyHostToDevice);
  cudaMemcpy(d_weights, weights, (nz)  * sizeof(float), cudaMemcpyHostToDevice);


  /***********************allocated and copied in kernel***********************************/

  char *worklist_in  = new char[M];
  char *worklist_out = new char[M];
  memset(worklist_in , '0', sizeof worklist_in);
  memset(worklist_out, '0', sizeof worklist_out);
  char *d_worklist_in;
  char *d_worklist_out;
  cudaMalloc( (void **) &d_worklist_in , (M) * sizeof(char) );
  cudaMalloc( (void **) &d_worklist_out, (M) * sizeof(char) );

  float *dist = new float[M];
  int sssp_sources[] = {0, 500-1, 1000-1, 10000-1, 50000-1, 100000-1};//sourcesof sssp
  // int sssp_sources[] = {0,2};
  for(int s=0; s<6; s++){
  // for(int s=0; s<2; s++){
    struct timespec tstart={0,0}, tend={0,0};
    clock_gettime(CLOCK_MONOTONIC, &tstart);

    for(int i=0;i<M;i++) //initially setting all distances to INFINITY
      dist[i] = FLT_MAX;


    dist[sssp_sources[s]] = 0; //setting the distance of source from itself to be zero
    cudaMemcpy(d_dist   , dist   , (M)   * sizeof(float), cudaMemcpyHostToDevice);
    worklist_in[sssp_sources[s]] = '1'; //adding s to worklist_in
                          //worklist_out is done phi previously line 44
    bool change = true;  //to see if any relaxations have happened

    while(change){
      change = false;
      //cout<<"Before Change "<<change<<endl;
      cudaMemcpy(d_worklist_in , worklist_in , (M) * sizeof(char), cudaMemcpyHostToDevice);
      cudaMemcpy(d_worklist_out, worklist_out, (M) * sizeof(char), cudaMemcpyHostToDevice);

      cudaMemcpy(d_change, &change, sizeof(bool), cudaMemcpyHostToDevice);
      sssp_kernel_data<<<(M + (THREADS_PER_BLOCK-1)) / THREADS_PER_BLOCK, THREADS_PER_BLOCK >>>
                (M, nz, d_worklist_in, d_worklist_out, d_change, d_sources, d_sinks, d_weights, d_dist);
      cudaMemcpy(&change, d_change, sizeof(bool), cudaMemcpyDeviceToHost);
      //cout<<"After Change "<<change<<endl;


      cudaMemcpy(worklist_in, d_worklist_out, (M) * sizeof(char), cudaMemcpyDeviceToHost);
      memset(worklist_out, '0', sizeof worklist_out);


    }
    cudaMemcpy(dist,d_dist, (M)   * sizeof(float),cudaMemcpyDeviceToHost);
    clock_gettime(CLOCK_MONOTONIC, &tend);

    float t = ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
              ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);
    printtofile("../results/distances_datad",sssp_sources[s],dist,M);
    printtimetofile("../times/times_datad",sssp_sources[s],t);
  }

  free(dist);
  free(worklist_in);
  free(worklist_out);
  cudaFree(d_change);
  cudaFree(d_sources);
  cudaFree(d_sinks);
  cudaFree(d_weights);
  cudaFree(d_dist);
  cudaFree(d_worklist_in);
  cudaFree(d_worklist_out);

}
