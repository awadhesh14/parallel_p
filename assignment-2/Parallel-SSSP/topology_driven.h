using namespace std;

__global__ void sssp_kernel_topology
      (int M, int nz, bool *d_change, int *d_sources, int *d_sinks, float *d_weights, float *d_dist){

  int index = blockIdx.x * blockDim.x + threadIdx.x,v;
  if(index>=0 && index<M){
    if(d_dist[index] != FLT_MAX){
      for(int i=d_sources[index]; i<d_sources[index+1]; i++){
        v = d_sinks[i];
        if(dist[index] + d_weights[i] < d_dist[v]){
          d_dist[v] = dist[index] + d_weights[i] ;
          *d_change = true;
        }
      }
    }
  }

}

void topology_driven(int M, int nz, int* sources, int* sinks, float* weights){
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
  float *dist = new float[M];
  int sssp_sources[] = {0, 500-1, 1000-1, 10000-1, 50000-1, 100000-1};//sourcesof sssp
  for(int s=0; s<6; s++){
    struct timespec tstart={0,0}, tend={0,0};
    clock_gettime(CLOCK_MONOTONIC, &tstart);

    for(int i=0;i<M;i++) //initially setting all distances to INFINITY
      dist[i] = FLT_MAX;
    cudaMemcpy(d_dist   , dist   , (M)   * sizeof(float), cudaMemcpyHostToDevice);
    //s = 0; //s is the single source in SSSP
    dist[sssp_sources[s]] = 0; //setting the distance of source from itself to be zero

    bool change = true;  //to see if any relaxations have happened

    while(change){
      change = false;
      cudaMemcpy(d_change, &change, sizeof(bool), cudaMemcpyHostToDevice);
      sssp_kernel_topology<<<(M + (THREADS_PER_BLOCK-1)) / THREADS_PER_BLOCK, THREADS_PER_BLOCK >>>
                        (M, nz, d_change, d_sources, d_sinks, d_weights, d_dist);
      cudaMemcpy(&change, d_change, sizeof(bool), cudaMemcpyDeviceToHost);

      clock_gettime(CLOCK_MONOTONIC, &tend);

      float t = ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
                ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);
      printtofile("../results/distances_topology",sssp_sources[s],dist,M);
      printtimetofile("../times/times_topology",sssp_sources[s],t);
    }

  }

  free(dist);
  cudaFree(d_change);
  cudaFree(d_sources);
  cudaFree(d_sinks);
  cudaFree(d_weights);
  cudaFree(d_dist);
}
