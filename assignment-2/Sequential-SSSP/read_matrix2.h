#include <stdio.h>
#include <stdlib.h>

struct Edge
{
    int src, sink;
    double wght;
};


Edge* read_edges(char *path,int *V,int *E){
  FILE *f;
  char line[100];
  if ((f = fopen(path,"r")) == NULL){
    printf("error in file open\n");
    exit(1);
  }
  while(true){
    if(fgets(line,100,f) == NULL){
      break;//inputs are over
    }
    if (line[0] == 'p'){
      char useless[20];
      sscanf(line,"%s %s %d %d",useless,useless,V,E);
      break;
    }
  }
   Edge *edges = new Edge[*E];
   // printf("%d %d \n",*V,*E);
   char _t;
   int i=0,u,v;
   double w;
   while(fgets(line,100,f) != NULL){
     if(line[0] == 'a'){
       sscanf(line,"%c %d %d %lg",&_t,&u,&v,&w);
       // printf("%c %d %d %lg\n",_t,u,v,w);
       edges[i].src = u-1;
       edges[i].sink = v-1;
       edges[i].wght = w;
       // printf("%c %d %d %lg\n",_t,edges[i].src,edges[i].sink,edges[i].wght);
       // edges[i].src--;
       // edges[i].sink--;
       i++;
     }
   }
   // printf("%d\n",i);
  if (f !=stdin) fclose(f);
  return edges;
}
