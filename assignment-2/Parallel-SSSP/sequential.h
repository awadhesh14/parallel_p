

void sequential(int M,int nz, Edge *edges){

  float *dist = new float[M];
  int change;
  // int sssp_sources[] = {0, 500-1, 1000-1, 10000-1, 50000-1, 100000-1};//sourcesof sssp
  int sssp_sources[] = {0,2};
  // for(int s=0; s<6; s++){
  int lala=1;
  cout<<(lala++)<<endl;//1

  for(int s=0; s<2; s++){
    struct timespec tstart={0,0}, tend={0,0};
    clock_gettime(CLOCK_MONOTONIC, &tstart);
cout<<(lala++)<<endl;//2
    for(int i=0;i<M;i++)
      dist[i]=FLT_MAX;
    dist[sssp_sources[s]] = 0; //setting the distance of source from itself to be zero
cout<<(lala++)<<endl;//3
    for(int i=0;i<M;i++){
      change = 0;
      for(int j=0;j<nz;j++){
        if(dist[edges[j].src] + edges[j].wght < dist[edges[j].sink] ){
          change = 1;
          dist[edges[j].sink] = dist[edges[j].src] + edges[j].wght;
        }
      }
      cout<<(lala++)<<endl;//4
      if(change==0)
        break;
    }
    cout<<(lala++)<<endl;//5
    clock_gettime(CLOCK_MONOTONIC, &tend);

    float t = ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
              ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);
    printtofile("../results/distances_sequential",sssp_sources[s],dist,M);
    printtimetofile("../times/times_sequential",sssp_sources[s],t);
  }
  cout<<(lala++)<<endl;//6

}
