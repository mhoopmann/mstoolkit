#include <iostream>
#include <iomanip>
#include <cmath>
#include "MSToolkitTypes.h"
#include "MSReader.h"
#include "MSObject.h"
#include "Spectrum.h"

using namespace std;
using namespace MSToolkit;

//#ifdef _MSC_VER 
//#ifndef MSSINGLESCAN_MAIN
//int mssinglescan_main(int argc, char * argv[] ) {
//#else
//int main(int argc, char *argv[]){
//#endif
//#else
int main(int argc, char *argv[]){
//#endif


	//Here are all the variable we are going to need
	MSReader r;
	Spectrum s;
  int j;

  if(argc==1){
    printf("DESCRIPTION: Reads an MS/MS spectrum from any MSToolkit supported file type and outputs to screen in MS2 format.\n\n");
    printf("USAGE: MSSingleScan [scan number] [file]\n");
    exit(0);
  }

	r.setFilter(MS1);
  r.addFilter(MS2);
	r.addFilter(MSX);
  r.addFilter(SRM);

  /*
  float max=0;
  r.readFile(argv[1],s);
  while(s.getScanNumber()>0){
    if(s.size()!=1) {
      r.readFile(NULL,s);
      continue;
    }
    //if(s[0].intensity>max){
    if(fabs(881.5-s.getMZ())<0.5){
      max=s[0].intensity;
      printf("%d\t%lf\t%f\n",s.getScanNumber(),s[0].mz,s[0].intensity);
    }
    r.readFile(NULL,s);
  }
  exit(0);
  */

  /*
  double precursor=atof(argv[2]);
  double dif;
  double target;
  int best;
  int i;
  for(i=3;i<argc;i++){
    target=atof(argv[i]);
    r.readFile(argv[1],s);
    while(s.getScanNumber()>0){
      if(s.size()>1) {
        r.readFile(NULL,s);
        continue;
      }
      if(s.getMZ()>precursor-2.5 && s.getMZ()<precursor+2.5) {
        dif=100.0;
        for(j=0;j<s.size();j++) {
          if(fabs(s[j].mz-target)<dif){
            dif=fabs(s[j].mz-target);
            best=j;
          }
        }
        if(dif<1.0){
          printf("%d\t%f\t%lf\t%f\n",s.getScanNumber(),s.getRTime(),s[best].mz,s[best].intensity);
        }
      }
      r.readFile(NULL,s);
    }
  }
  exit(0);
  */



	//r.readFile(argv[2],s);
  char nativeID[256];
	r.readFile(argv[2],s,atoi(argv[1]));
  if(s.getScanNumber()==0) exit(-1);


  char szNativeID[128];
  if (s.getNativeID(szNativeID, 128))
      printf("success:  scan %d  nativeID: %s\n", s.getScanNumber(), szNativeID);
  else
      printf("failure:  scan %d\n", s.getScanNumber());

  s.getNativeID(nativeID, 256);
  printf("%s\n",nativeID);
  printf("S\t%d\t%d",s.getScanNumber(),s.getScanNumber());
	for(j=0;j<s.sizeMZ();j++){
		printf("\t%.*lf",4,s.getMZ(j));
	}
	printf("\n");
	if(s.getRTime()>0) printf("I\tRTime\t%.*f\n",4,s.getRTime());
	//printf("I\tConvA\t%.6lf\n",s.getConversionA());
	//printf("I\tConvB\t%.6lf\n",s.getConversionB());
	//printf("I\tConvC\t%.6lf\n",s.getConversionC());
	//printf("I\tConvD\t%.6lf\n",s.getConversionD());
	//printf("I\tConvE\t%.6lf\n",s.getConversionE());
	//printf("I\tConvI\t%.6lf\n",s.getConversionI());
	for(j=0;j<s.sizeZ();j++){
		printf("Z\t%d\t%.*f\n",s.atZ(j).z,6,s.atZ(j).mz);
	}

	for(j=0;j<s.size();j++){
		printf("%.4f %.4f\n",s.at(j).mz,s.at(j).intensity); 
	}

  return 0;

}
  

