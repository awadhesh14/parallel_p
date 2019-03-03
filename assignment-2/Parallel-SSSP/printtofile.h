using namespace std;

void printtofile(string filename,int source,float *distances,int M){
  ofstream op;
  op.open(filename.c_str() , std::ios_base::app);

  op<< "Source is "<<source<<endl;

  for (int i=0; i<M; i++){
    op<<distances[i]<<endl;
  }

  op.close();
}
void printtimetofile(string filename,int source,float time){
  ofstream op;
  op.open(filename.c_str() , std::ios_base::app);

  op<< "source = "<<time <<;

  op.close();
}
