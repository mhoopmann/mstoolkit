/*
RAMPface - The code is
open source under the FreeBSD License, please see LICENSE file
for detailed information.

Copyright (C) 2011, Mike Hoopmann, Institute for Systems Biology
Version 1.0, January 4, 2011.
Version 1.1, March 14, 2012.
*/

#include "mzParser.h"
using namespace std;
using namespace mzParser;

int mzParser::checkFileType(const char* fname){
  char file[4096];
  char ext[4096];
  char *tok;
  char *nextTok;
  char preExt[4096];
  unsigned int i;

  if (strlen(fname) < 4) {
    cerr << "Incomplete file name. No file loaded: " << fname << endl;
    return 0;
  }

  ext[0]='\0';

  strcpy(file,fname);
  tok=strtok_r(file,".\n",&nextTok);
  while(tok!=NULL){
    strcpy(preExt,ext);
    strcpy(ext,tok);
    tok=strtok_r(NULL,".\n",&nextTok);
  }

  for(i=0;i<strlen(ext);i++) ext[i]=toupper(ext[i]);
  for(i=0;i<strlen(preExt);i++) preExt[i]=toupper(preExt[i]);

#ifdef MZP_HDF
  if (!strcmp(ext, "MZMLB")) return 6;
  if (!strcmp(ext, "MZ5")) return 5;
#else
  if (!strcmp(ext, "MZ5") || !strcmp(ext, "MZMLB")) {
    cerr << "HDF5 formatted data (.mz5 and .mzMLb) support was disabled during compilation of mzParser." << endl;
    cerr << "Please recompile mzParser with 'MZP_HDF' to enable HDF5 file support." << endl;
    return 0;
  }
#endif

  if(!strcmp(ext,"MZML")) return 1;
  if(!strcmp(ext,"MZXML")) return 2;
  if(!strcmp(ext,"GZ")) {
    if(!strcmp(preExt,"MZML")) return 3;
    if(!strcmp(preExt,"MZXML")) return 4;
    cerr << "Unknown .gz file. Only .mzML.gz and .mzXML.gz allowed. No file loaded." << endl;
    return 0;
  }
  cerr << "Unknown file type. No file loaded." << fname << endl;
  return 0;
}

void mzParser::getPrecursor(const struct ScanHeaderStruct *scanHeader,int index,double &mz,double &monoMZ,double &intensity,int &charge,int &possibleCharges,int *&possibleChargeArray){
  int i,j,k;
  double d;
  
  if(index==0){
    mz=scanHeader->precursorMZ;
    d=scanHeader->selectionWindowLower; //these are place holders until the values are actually returned;
    d=scanHeader->selectionWindowUpper;
    monoMZ=scanHeader->precursorMonoMZ;
    intensity=scanHeader->precursorIntensity;
    d = scanHeader->isolationWindowLower; //these are place holders until the values are actually returned;
    d = scanHeader->isolationWindowUpper;
    charge=scanHeader->precursorCharge;
    possibleCharges=scanHeader->numPossibleCharges;
    if(possibleChargeArray!=NULL) delete[] possibleChargeArray;
    if(possibleCharges>0){
      possibleChargeArray = new int[possibleCharges];
      for(i=0;i<possibleCharges;i++) memcpy(&possibleChargeArray[i],&scanHeader->possibleCharges[i*4],sizeof(int));
    } else {
      possibleChargeArray = NULL;
    }
  } else {
    j=0;
    for(i=1;i<scanHeader->precursorCount;i++){
      memcpy(&mz,&scanHeader->additionalPrecursors[j+=8],sizeof(double));
      memcpy(&d, &scanHeader->additionalPrecursors[j += 8], sizeof(double)); //selectionWindowLower
      memcpy(&d, &scanHeader->additionalPrecursors[j += 8], sizeof(double)); //selectionWindowUpper
      memcpy(&monoMZ,&scanHeader->additionalPrecursors[j+=8],sizeof(double));
      memcpy(&intensity,&scanHeader->additionalPrecursors[j+=8],sizeof(double));
      memcpy(&d, &scanHeader->additionalPrecursors[j += 8], sizeof(double)); //isolationWindowLower
      memcpy(&d, &scanHeader->additionalPrecursors[j += 8], sizeof(double)); //isolationWindowUpper
      memcpy(&charge,&scanHeader->additionalPrecursors[j+=4],sizeof(int));
      memcpy(&possibleCharges,&scanHeader->additionalPrecursors[j+=4],sizeof(int));
      if(possibleChargeArray!=NULL) delete[] possibleChargeArray;
      if(possibleCharges>0){
        possibleChargeArray = new int[possibleCharges];
        for(k=0;k<possibleCharges;k++) memcpy(&possibleChargeArray[k],&scanHeader->additionalPrecursors[j+=4],sizeof(int));
      } else {
        possibleChargeArray = NULL;
      }
      if(i==index) break;
    }
  }
}

void mzParser::getPrecursor(const struct ScanHeaderStruct* scanHeader, int index, sPrecursorIon& pi) {
  int i, j, k;

  pi.clear();
  if (index == 0) {
    pi.mz = scanHeader->precursorMZ;
    pi.isoLowerMZ = scanHeader->selectionWindowLower; //these are place holders until the values are actually returned;
    pi.isoUpperMZ = scanHeader->selectionWindowUpper;
    pi.monoMZ = scanHeader->precursorMonoMZ;
    pi.intensity = scanHeader->precursorIntensity;
    pi.isoLowerOffset = scanHeader->isolationWindowLower; //these are place holders until the values are actually returned;
    pi.isoUpperOffset = scanHeader->isolationWindowUpper;
    pi.charge = scanHeader->precursorCharge;
    if (scanHeader->numPossibleCharges > 0) {
      for (i = 0; i < scanHeader->numPossibleCharges; i++) {
        memcpy(&k, &scanHeader->possibleCharges[i * 4], sizeof(int));
        pi.possibleCharges.push_back(k);
      }
    } 
  } else {
    j = 0;
    int possibleCharges;
    for (i = 1; i < scanHeader->precursorCount; i++) {
      memcpy(&pi.mz, &scanHeader->additionalPrecursors[j], sizeof(double));j+=8;
      memcpy(&pi.isoLowerMZ, &scanHeader->additionalPrecursors[j], sizeof(double)); j += 8; //selectionWindowLower
      memcpy(&pi.isoUpperMZ, &scanHeader->additionalPrecursors[j], sizeof(double)); j += 8; //selectionWindowUpper
      memcpy(&pi.monoMZ, &scanHeader->additionalPrecursors[j], sizeof(double)); j += 8;
      memcpy(&pi.intensity, &scanHeader->additionalPrecursors[j], sizeof(double)); j += 8;
      memcpy(&pi.isoLowerOffset, &scanHeader->additionalPrecursors[j], sizeof(double)); j += 8;
      memcpy(&pi.isoUpperOffset, &scanHeader->additionalPrecursors[j], sizeof(double));j+=8;
      memcpy(&pi.charge, &scanHeader->additionalPrecursors[j], sizeof(int));j+=4;
      memcpy(&possibleCharges, &scanHeader->additionalPrecursors[j], sizeof(int)); j += 4;
      if (possibleCharges > 0) {
        for (k = 0; k < possibleCharges; k++) {
          memcpy(&k, &scanHeader->possibleCharges[j], sizeof(int)); j += 4;
          pi.possibleCharges.push_back(k);
        }
      }
      if (i == index) break;
    }
  }
}

ramp_fileoffset_t mzParser::getIndexOffset(RAMPFILE *pFI){
  switch(pFI->fileType){
    case 1:
    case 3:
      return (ramp_fileoffset_t)pFI->mzML->getIndexOffset();
      break;
    case 2:
    case 4:
      return (ramp_fileoffset_t)pFI->mzXML->getIndexOffset();
      break;
    default:
      return -1;
  }
}

InstrumentStruct* mzParser::getInstrumentStruct(RAMPFILE *pFI){
  InstrumentStruct* r=(InstrumentStruct *) calloc(1,sizeof(InstrumentStruct));
  if(r==NULL) {
    printf("Cannot allocate memory\n");
    return NULL;
  } else {
    strcpy(r->analyzer,"UNKNOWN");
    strcpy(r->detector,"UNKNOWN");
    strcpy(r->ionisation,"UNKNOWN");
    strcpy(r->manufacturer,"UNKNOWN");
    strcpy(r->model,"UNKNOWN");
	  strcpy(r->serial,"UNKNOWN");							
  }

  switch(pFI->fileType){
    case 1:
    case 3:
    case 6:
      if(pFI->mzML->getInstrument()->size()>0){
        if(pFI->mzML->getInstrument()->at(0).analyzer.size()>1) strcpy(r->analyzer, &pFI->mzML->getInstrument()->at(0).analyzer[0]);
        if(pFI->mzML->getInstrument()->at(0).detector.size()>1) strcpy(r->detector, &pFI->mzML->getInstrument()->at(0).detector[0]);
        if(pFI->mzML->getInstrument()->at(0).ionization.size()>1) strcpy(r->ionisation, &pFI->mzML->getInstrument()->at(0).ionization[0]);
        if(pFI->mzML->getInstrument()->at(0).manufacturer.size()>1) strcpy(r->manufacturer, &pFI->mzML->getInstrument()->at(0).manufacturer[0]);
        if(pFI->mzML->getInstrument()->at(0).model.size()>1) strcpy(r->model, &pFI->mzML->getInstrument()->at(0).model[0]);
		    if(pFI->mzML->getInstrument()->at(0).serial.size()>1) strcpy(r->serial, &pFI->mzML->getInstrument()->at(0).serial[0]);
      }
      break;

    case 2:
    case 4:
      if(pFI->mzXML->getInstrument().analyzer.size()>1) strcpy(r->analyzer, &pFI->mzXML->getInstrument().analyzer[0]);
      if(pFI->mzXML->getInstrument().detector.size()>1) strcpy(r->detector, &pFI->mzXML->getInstrument().detector[0]);
      if(pFI->mzXML->getInstrument().ionization.size()>1) strcpy(r->ionisation, &pFI->mzXML->getInstrument().ionization[0]);
      if(pFI->mzXML->getInstrument().manufacturer.size()>1) strcpy(r->manufacturer, &pFI->mzXML->getInstrument().manufacturer[0]);
      if(pFI->mzXML->getInstrument().model.size()>1) strcpy(r->model, &pFI->mzXML->getInstrument().model[0]);
      if(pFI->mzXML->getInstrument().serial.size()>1) strcpy(r->serial, &pFI->mzXML->getInstrument().serial[0]);
      break;

    case 5:
    default:
      break;
  }

  return r;
}

int mzParser::getScanNumberFromOffset(RAMPFILE *pFI, ramp_fileoffset_t lScanIndex){
  vector<cindex>* v;
#ifdef MZP_HDF
  vector<cMz5Index>* v2;
#endif

  if (lScanIndex<0) return -1;

  switch (pFI->fileType){
  case 1:
  case 3:
  case 6:
    v = pFI->mzML->getSpecIndex();
    break;
  case 2:
  case 4:
    v = pFI->mzXML->getIndex();
    break;
#ifdef MZP_HDF
  case 5:
    v2 = pFI->mz5->getSpecIndex();
    break;
#endif
  default:
    return -1;
    break;
  }

  size_t sz;
  size_t lower;
  size_t mid;
  size_t upper;

  //binary search non mz5 files
  if (pFI->fileType!=5){
    sz = v->size();
    lower = 0;
    mid = sz / 2;
    upper = sz-1;

    while (lower<=upper){
      if (v->at(mid).offset == lScanIndex) return (int)v->at(mid).scanNum;
      if (v->at(mid).offset < lScanIndex) lower = mid+1;
      else upper = mid-1;
      mid = (lower + upper) / 2;
    }

#ifdef MZP_HDF
  } else { //binary search mz5 files
    sz = v2->size();
    lower = 0;
    mid = sz / 2;
    upper = sz - 1;

    while (lower<=upper){
      if (v2->at(mid).offset == lScanIndex) return (int)v2->at(mid).scanNum;
      if (v2->at(mid).offset < lScanIndex) lower = mid + 1;
      else upper = mid - 1;
      mid = (lower + upper) / 2;
    }
#endif
  }

  v = NULL;
#ifdef MZP_HDF
  v2 = NULL;
#endif

  return -1;
}

void mzParser::getScanSpanRange(const struct ScanHeaderStruct *scanHeader, int *startScanNum, int *endScanNum) {
   if (0 == scanHeader->mergedResultStartScanNum || 0 == scanHeader->mergedResultEndScanNum) {
      *startScanNum = scanHeader->acquisitionNum;
      *endScanNum = scanHeader->acquisitionNum;
   } else {
      *startScanNum = scanHeader->mergedResultStartScanNum;
      *endScanNum = scanHeader->mergedResultEndScanNum;
   }
}

void mzParser::rampCloseFile(RAMPFILE *pFI){
  if(pFI!=NULL) {
    delete pFI;
    pFI=NULL;
  }
}

string mzParser::rampConstructInputFileName(const string &basename){
  int len;
  char *buf = new char[len = (int)(basename.length()+100)]; 
  mzParser::rampConstructInputPath(buf, len, "", basename.c_str());
  string result(buf);
  delete[] buf;
  return result;
}

char* mzParser::rampConstructInputFileName(char *buf,int buflen,const char *basename){
  return mzParser::rampConstructInputPath(buf, buflen, "", basename);
}

char* mzParser::rampConstructInputPath(char *buf, int inbuflen, const char *dir_in, const char *basename){

  if( (int)(strlen(dir_in)+strlen(basename)+1) > inbuflen ){
    //Can't output error messages in TPP software that use this function
    //printf("rampConstructInputPath error: buffer too small for file\n");
    return NULL;
  }

  FILE* f;
  char* result = NULL;
  char base[4096];
  strcpy(base,basename);

  //Try opening the base name first, then with directory:
  for(int j=0;j<2;j++){
    for(int i=0;i<5;i++){

      if(j==1){
        strcpy(buf,dir_in);
        strcat(buf,"/");
        strcat(buf,base);
      } else {
        strcpy(buf,base);
      }

      switch(i){
        case 0:  strcat(buf,".mzML");  break;
        case 1:  strcat(buf,".mzXML"); break;
        case 2:  strcat(buf,".mzML.gz"); break;
        case 3:  strcat(buf,".mzXML.gz");  break;
        case 4:  strcat(buf,".mz5"); break;
        default: break;
      }
      
      f=fopen(buf,"r");
      if(f==NULL) continue;
        
      fclose(f);
      result=buf;
      return result;
    }
  }

  //Can't output error messages in TPP software that use this function
  //printf("rampConstructInputPath: file not found\n");
  strcpy(buf,"");
  result=NULL;
  return result;

}


const char** mzParser::rampListSupportedFileTypes(){
  if (!data_Ext.size()) { // needs init
    data_Ext.push_back(".mzXML");
    data_Ext.push_back(".mzML");
    data_Ext.push_back(".mzXML.gz");
    data_Ext.push_back(".mzML.gz");
    data_Ext.push_back(".mz5");
    data_Ext.push_back(NULL); // end of list
  }
  return &(data_Ext[0]);
}

RAMPFILE* mzParser::rampOpenFile(const char* filename){
  int i=mzParser::checkFileType(filename);
  if(i==0){
    return NULL;
  } else {
    RAMPFILE* r=new RAMPFILE();
    r->fileName=filename;
    r->bs = new BasicSpectrum();
    r->fileType=i;
    switch(i){
      case 1: //mzML
      case 3:
      case 6:
        r->mzML=new mzpSAXMzmlHandler(r->bs);
        if(i==3)r->mzML->setGZCompression(true);
        else r->mzML->setGZCompression(false);
        if(i==6) r->mzML->setMZMLB(true);
        else r->mzML->setMZMLB(false);
        if(!r->mzML->load(filename)){
          delete r;
          return NULL;
        } else {
          return r;
        }
      case 2: //mzXML
      case 4:
        r->mzXML=new mzpSAXMzxmlHandler(r->bs);
        if(i==4) r->mzXML->setGZCompression(true);
        else r->mzXML->setGZCompression(false);
        if(!r->mzXML->load(filename)){
          delete r;
          return NULL;
        } else {
          return r;
        }
#ifdef MZP_HDF
      case 5: //mz5
        r->mz5Config = new mzpMz5Config();
        r->mz5=new mzpMz5Handler(r->mz5Config, r->bs);
        if(!r->mz5->readFile(filename)){
          delete r;
          return NULL;
        } else {
          return r;
        }
#endif
      default:
        delete r;
        return NULL;
    }
  }

}

char* mzParser::rampValidFileType(const char *buf){
  char ext[4096];
  char preExt[4096];

  const char* result=NULL;
  const char* result2=NULL;
  const char* p;

  unsigned int i;

  p=strchr(buf,'.');
  while(p!=NULL){
    result2=result;
    result=p;
    p=strchr(p+1,'.');
  }

  if(result==NULL) return (char*) result;

  strcpy(ext,result);
  for(i=0;i<strlen(ext);i++) ext[i]=toupper(ext[i]);

  if(result2){
    strcpy(preExt,result2);
    for(i=0;i<strlen(preExt);i++) preExt[i]=toupper(preExt[i]);
  }

  if(!strcmp(ext,".MZML")) return (char*) result;
  if(!strcmp(ext,".MZXML")) return (char*) result;
  if(!strcmp(ext,".MZ5")) return (char*) result;
  if(!strcmp(ext,".GZ")) {
    if(!strcmp(preExt,".MZML.GZ")) return (char*) result2;
    if(!strcmp(preExt,".MZXML.GZ")) return (char*) result2;
    cout << "Unknown .gz file. Only .mzML.gz and .mzXML.gz allowed. No file loaded." << endl;
  }
  if(!strcmp(ext,".MZDATA")) {
    cout << ".mzData is not supported. Please convert to mz5, mzXML, or mzML." << endl;
  }
  result=NULL;
  return (char*) result;
}

//MH: Read header is redundant with readPeaks, which automatically reads the header.
//But due to legacy issues, this function must exist.
void mzParser::readHeader(RAMPFILE *pFI, ramp_fileoffset_t lScanIndex, struct ScanHeaderStruct *scanHeader, int iIndex, BasicSpectrum **bs){

  vector<cindex>* v;
  sPrecursorIon p;
  unsigned int i;

#ifdef MZP_HDF
  vector<cMz5Index>* v2;
#endif

  //memset(scanHeader,0,sizeof(struct ScanHeaderStruct));
  scanHeader->acquisitionNum=-1;
  scanHeader->activationMethod[0]='\0';
  scanHeader->basePeakIntensity=0.0;
  scanHeader->basePeakMZ=0.0;
  scanHeader->centroid=false;
  scanHeader->collisionEnergy=0.0;
  scanHeader->compensationVoltage=0.0;
  scanHeader->inverseReducedIonMobility=0.0;			
  scanHeader->ionMobility=false;
  scanHeader->ionMobilityDriftTime=0;
  scanHeader->isolationMZ=0;
  scanHeader->isolationWindowLower=0;
  scanHeader->isolationWindowUpper=0;
  scanHeader->filePosition=0;
  scanHeader->filterLine[0]='\0';
  scanHeader->highMZ=0.0;
  scanHeader->idString[0]='\0';
  scanHeader->ionisationEnergy=0.0;
  scanHeader->lowMZ=0.0;
  scanHeader->mergedScan=0;
  scanHeader->mergedResultScanNum=0;
  scanHeader->mergedResultStartScanNum=0;
  scanHeader->mergedResultEndScanNum=0;
  scanHeader->msLevel=0;
  scanHeader->numPossibleCharges=0;
  scanHeader->precursorCharge=0;
  scanHeader->precursorCount=0;
  scanHeader->precursorIntensity=0.0;
  scanHeader->precursorMonoMZ=0.0;
  scanHeader->precursorMZ=0.0;
  scanHeader->precursorScanNum=-1;
  scanHeader->retentionTime=0.0;
  scanHeader->scanDescription[0]='\0';
  scanHeader->scanType[0]='\0';
  scanHeader->selectedIonMZ=0;
  scanHeader->selectionWindowLower=0;
  scanHeader->selectionWindowUpper=0;
  scanHeader->totIonCurrent=0.0;
  scanHeader->scanIndex=0;
  scanHeader->seqNum=-1;

  if(lScanIndex<0) return;
  if(lScanIndex!=pFI->lLastScanIndex){
  
    switch(pFI->fileType){
      case 1:
      case 3:
      case 6:
        if (!pFI->mzML->readHeaderFromOffset((f_off)lScanIndex,iIndex)){
          v = NULL;
          pFI->lLastScanIndex=0;
          return;
        }
        break;
      case 2:
      case 4:
        if (!pFI->mzXML->readHeaderFromOffset((f_off)lScanIndex)){
          v = NULL;
          pFI->lLastScanIndex = 0;
          return;
        }
        break;
#ifdef MZP_HDF
      case 5:
        v2=pFI->mz5->getSpecIndex();
        for(i=0;i<v2->size();i++) {
          if(v2->at(i).offset==(f_off)lScanIndex) {
            if(!pFI->mz5->readHeader((int)v2->at(i).scanNum)){
              v2=NULL;
              pFI->lLastScanIndex = 0;
              return;
            }
            break;
          }
        }
        break;
#endif
      default:
        pFI->bs->clear();
        v=NULL;
#ifdef MZP_HDF
        v2=NULL;
#endif
        pFI->lLastScanIndex = 0;
        return;
    }
    v=NULL;
#ifdef MZP_HDF
    v2=NULL;
#endif
    pFI->lLastScanIndex = lScanIndex;
  }

  scanHeader->acquisitionNum=pFI->bs->getScanNum();
  scanHeader->basePeakIntensity=pFI->bs->getBasePeakIntensity();
  scanHeader->basePeakMZ=pFI->bs->getBasePeakMZ();
  scanHeader->centroid=pFI->bs->getCentroid();
  scanHeader->positiveScan=pFI->bs->getPositiveScan();
  scanHeader->collisionEnergy=pFI->bs->getCollisionEnergy();
  scanHeader->compensationVoltage=pFI->bs->getCompensationVoltage();																		
  scanHeader->highMZ=pFI->bs->getHighMZ();
  scanHeader->inverseReducedIonMobility=pFI->bs->getInverseReducedIonMobility();		
  scanHeader->ionInjectionTime = pFI->bs->getIonInjectionTime();
  scanHeader->ionMobilityDriftTime = pFI->bs->getIonMobilityDriftTime();
  scanHeader->ionMobility=pFI->bs->getIonMobilityScan();
  scanHeader->lowMZ=pFI->bs->getLowMZ();
  scanHeader->msLevel=pFI->bs->getMSLevel();
  scanHeader->peaksCount=pFI->bs->getPeaksCount();
  scanHeader->precursorScanNum=pFI->bs->getPrecursorScanNum();
  scanHeader->retentionTime=(double)pFI->bs->getRTime(false);
  scanHeader->totIonCurrent=pFI->bs->getTotalIonCurrent();
  scanHeader->scanIndex=pFI->bs->getScanIndex();
  scanHeader->seqNum=pFI->bs->getScanIndex();

  int j=0;
  int k;
  double d;
  scanHeader->precursorCount=pFI->bs->getPrecursorIonCount();
  for(i=0;i<(unsigned int)scanHeader->precursorCount;i++){
    p=pFI->bs->getPrecursorIon(i);
    if(i==0){
      scanHeader->selectedIonMZ=p.mz;
      scanHeader->isolationMZ=p.isoMZ;
      scanHeader->precursorCharge=p.charge;
      scanHeader->precursorIntensity=p.intensity;
      scanHeader->precursorMonoMZ=p.monoMZ;
      scanHeader->isolationWindowLower=p.isoLowerOffset;
      scanHeader->isolationWindowUpper=p.isoUpperOffset;
      if(p.isoMZ==0) {
        scanHeader->precursorMZ=p.mz;
        scanHeader->selectionWindowLower = p.mz - p.isoLowerOffset;
        scanHeader->selectionWindowUpper = p.mz + p.isoUpperOffset;
      } else {
        scanHeader->precursorMZ=p.isoMZ;
        scanHeader->selectionWindowLower=p.isoMZ-p.isoLowerOffset;
        scanHeader->selectionWindowUpper=p.isoMZ+p.isoUpperOffset;
      }
      scanHeader->numPossibleCharges=(int)p.possibleCharges.size();
      for(k=0;k<scanHeader->numPossibleCharges;k++){
        memcpy(&scanHeader->possibleCharges[k*4],&p.possibleCharges[k],sizeof(int));
        if(k==7){
          cout << "Warning: too many possible charges for precursor in scan " << scanHeader->acquisitionNum << endl;
          break;
        }
      }
    } else {
      if( (j+64+p.possibleCharges.size()*4) > PRECURSORARRAY_LENGTH-1) {
        cout << "Warning: too many precursors. Must improve RAMP interface." << endl;
        break;
      }
      if (p.isoMZ == 0) {
        memcpy(&scanHeader->additionalPrecursors[j], &p.mz, sizeof(double)); j+=8;
        d = p.mz - p.isoLowerOffset;
        memcpy(&scanHeader->additionalPrecursors[j], &d, sizeof(double)); j += 8;
        d = p.mz + p.isoUpperOffset;
        memcpy(&scanHeader->additionalPrecursors[j], &d, sizeof(double)); j += 8;
      } else {
        memcpy(&scanHeader->additionalPrecursors[j],&p.isoMZ,sizeof(double)); j += 8;
        d = p.isoMZ - p.isoLowerOffset;
        memcpy(&scanHeader->additionalPrecursors[j],&d, sizeof(double)); j += 8;
        d = p.isoMZ + p.isoUpperOffset;
        memcpy(&scanHeader->additionalPrecursors[j],&d, sizeof(double)); j += 8;
      }
      memcpy(&scanHeader->additionalPrecursors[j],&p.monoMZ,sizeof(double)); j += 8;
      memcpy(&scanHeader->additionalPrecursors[j],&p.intensity,sizeof(double)); j += 8;
      memcpy(&scanHeader->additionalPrecursors[j],&p.isoLowerOffset,sizeof(double)); j += 8;
      memcpy(&scanHeader->additionalPrecursors[j],&p.isoUpperOffset,sizeof(double)); j += 8;
      memcpy(&scanHeader->additionalPrecursors[j],&p.charge,sizeof(int)); j+=4;
      k=(int)p.possibleCharges.size();
      memcpy(&scanHeader->additionalPrecursors[j],&k,sizeof(int)); j+=4;
      for(k=0;k<(int)p.possibleCharges.size();k++){
        memcpy(&scanHeader->additionalPrecursors[j],&p.possibleCharges[k],sizeof(int)); j+=4;
      }
    }
  }
  
  string ts;
  pFI->bs->getFilterLine(ts);
  strcpy(scanHeader->filterLine,ts.c_str());
  pFI->bs->getIDString(ts);
  strcpy(scanHeader->idString,ts.c_str());

  switch(pFI->bs->getActivation()){
    case 1: strcpy(scanHeader->activationMethod,"CID"); break;
    case 2: strcpy(scanHeader->activationMethod,"HCD"); break;
    case 3: strcpy(scanHeader->activationMethod,"ETD"); break;
    case 4: strcpy(scanHeader->activationMethod,"ETD+SA"); break;
    case 5: strcpy(scanHeader->activationMethod,"ECD"); break;
    default: strcpy(scanHeader->activationMethod,""); break;
  }

  //process user params
  size_t index=0;
  sUParam up=pFI->bs->getUserParam(index++);
  while(!up.name.empty()){
    if(up.name.compare("scan description")==0) strcpy(scanHeader->scanDescription,up.value.c_str());
    up = pFI->bs->getUserParam(index++);
  }

  //pass the BasicSpectrum back to the caller, if a pointer was provided, for any additional
  //processing beyond what RAMP provides.
  if (bs != NULL) *bs = pFI->bs;

}

//MH: Indexes in RAMP are stored in an array indexed by scan number, with -1 for the offset
//if the scan number does not exist.
ramp_fileoffset_t* mzParser::readIndex(RAMPFILE *pFI, ramp_fileoffset_t indexOffset, int *iLastScan){
  vector<cindex>* v;
#ifdef MZP_HDF
  vector<cMz5Index>* v2;
#endif
  ramp_fileoffset_t* rIndex;
  size_t i;
  switch(pFI->fileType){
    case 1:
    case 3:
    case 6:
      v=pFI->mzML->getSpecIndex();
      rIndex = (ramp_fileoffset_t *) malloc((pFI->mzML->highScan()+2)*sizeof(ramp_fileoffset_t));
      memset(rIndex,-1,(pFI->mzML->highScan()+2)*sizeof(ramp_fileoffset_t));
      if(pFI->fileType==6){
        for (i = 0; i < v->size(); i++) rIndex[v->at(i).scanNum] = (ramp_fileoffset_t)i;
      } else {
        for(i=0;i<v->size();i++) rIndex[v->at(i).scanNum]=(ramp_fileoffset_t)v->at(i).offset;
      }
      rIndex[v->at(i-1).scanNum+1]=-1;
      *iLastScan=(int)v->at(i-1).scanNum;
      break;
    case 2:
    case 4:
      v=pFI->mzXML->getIndex();
      rIndex = (ramp_fileoffset_t *) malloc((pFI->mzXML->highScan()+2)*sizeof(ramp_fileoffset_t));
      memset(rIndex,-1,(pFI->mzXML->highScan()+2)*sizeof(ramp_fileoffset_t));
      for(i=0;i<v->size();i++) rIndex[v->at(i).scanNum]=(ramp_fileoffset_t)v->at(i).offset;
      rIndex[v->at(i-1).scanNum+1]=-1;
      *iLastScan=(int)v->at(i-1).scanNum;
      break;
#ifdef MZP_HDF
    case 5:
      v2=pFI->mz5->getSpecIndex();
      rIndex = (ramp_fileoffset_t *) malloc((pFI->mz5->highScan()+2)*sizeof(ramp_fileoffset_t));
      memset(rIndex,-1,(pFI->mz5->highScan()+2)*sizeof(ramp_fileoffset_t));
      for(i=0;i<v2->size();i++) rIndex[v2->at(i).scanNum]=(ramp_fileoffset_t)v2->at(i).offset;
      rIndex[v2->at(i-1).scanNum+1]=-1;
      *iLastScan=(int)v2->at(i-1).scanNum;
      break;
#endif
    default:
      rIndex=NULL;
      *iLastScan=0;
      break;
  }
  v=NULL;
#ifdef MZP_HDF
  v2=NULL;
#endif
  return rIndex;
}

int mzParser::readMsLevel(RAMPFILE *pFI, ramp_fileoffset_t lScanIndex){
  vector<cindex>* v;
#ifdef MZP_HDF
  vector<cMz5Index>* v2;
#endif
  unsigned int i;
  
  if(lScanIndex<0) return 0;

  switch(pFI->fileType){
    case 1:
    case 3:
      v=pFI->mzML->getSpecIndex();
      for(i=0;i<v->size();i++) {
        if(v->at(i).offset==(f_off)lScanIndex) {
          pFI->mzML->readSpectrum((int)v->at(i).scanNum);
          break;
        }
      }
      break;
    case 2:
    case 4:
      v=pFI->mzXML->getIndex();
      for(i=0;i<v->size();i++) {
        if(v->at(i).offset==(f_off)lScanIndex) {
          pFI->mzXML->readSpectrum((int)v->at(i).scanNum);
          break;
        }
      }
      break;
#ifdef MZP_HDF
    case 5:
      v2=pFI->mz5->getSpecIndex();
      for(i=0;i<v2->size();i++) {
        if(v2->at(i).offset==(f_off)lScanIndex) {
          pFI->mz5->readSpectrum((int)v2->at(i).scanNum);
          break;
        }
      }
      break;
#endif
    default:
      pFI->bs->clear();
      break;
  }
  v=NULL;
#ifdef MZP_HDF
  v2=NULL;
#endif

  return pFI->bs->getMSLevel();
}

void mzParser::readMSRun(RAMPFILE *pFI, struct RunHeaderStruct *runHeader){

  vector<cindex>* v;
#ifdef MZP_HDF
  vector<cMz5Index>* v2;
#endif

  //memset(scanHeader,0,sizeof(struct ScanHeaderStruct));
  runHeader->dEndTime=0.0;
  runHeader->dStartTime=0.0;
  runHeader->endMZ=0.0;
  runHeader->highMZ=0.0;
  runHeader->lowMZ=0.0;
  runHeader->scanCount=0;
  runHeader->startMZ=0.0;
  
  switch(pFI->fileType){
    case 1:
    case 3:
      v=pFI->mzML->getSpecIndex();
      runHeader->scanCount=(int)v->size();
      pFI->mzML->readHeader((int)v->at(0).scanNum);
      runHeader->dStartTime=pFI->bs->getRTime(false);
      runHeader->lowMZ=pFI->bs->getLowMZ();
      runHeader->highMZ=pFI->bs->getHighMZ();										   									 
      pFI->mzML->readHeader((int)v->at(v->size()-1).scanNum);
      runHeader->dEndTime=pFI->bs->getRTime(false);
      pFI->bs->clear();
      v=NULL;
      break;
    case 2:
    case 4:
      v=pFI->mzXML->getIndex();
      runHeader->scanCount=(int)v->size();
      pFI->mzXML->readHeader((int)v->at(0).scanNum);
      runHeader->dStartTime=pFI->bs->getRTime(false);
	  runHeader->lowMZ=pFI->bs->getLowMZ();
      runHeader->highMZ=pFI->bs->getHighMZ();									   									 
      pFI->mzXML->readHeader((int)v->at(v->size()-1).scanNum);
      runHeader->dEndTime=pFI->bs->getRTime(false);
      pFI->bs->clear();
      v=NULL;
      break;
#ifdef MZP_HDF
    case 5:
      v2=pFI->mz5->getSpecIndex();
      runHeader->scanCount=(int)v2->size();
      pFI->mz5->readHeader((int)v2->at(0).scanNum);
      runHeader->dStartTime=pFI->bs->getRTime(false);
      runHeader->lowMZ=pFI->bs->getLowMZ();
      runHeader->highMZ=pFI->bs->getHighMZ();								   									 
      pFI->mz5->readHeader((int)v2->at(v2->size()-1).scanNum);
      runHeader->dEndTime=pFI->bs->getRTime(false);
      pFI->bs->clear();
      v2=NULL;
      break;
#endif
    default:
      break;
  }

}

//MH: Matching the index is very indirect, but requires less code,
//making this wrapper much easier to read
RAMPREAL* mzParser::readPeaks(RAMPFILE* pFI, ramp_fileoffset_t lScanIndex, int iIndex, bool ionMobility){
  vector<cindex>* v;
#ifdef MZP_HDF
  vector<cMz5Index>* v2;
#endif
  size_t i;
  RAMPREAL* pPeaks=NULL;

  if(lScanIndex<0) return pPeaks;

  //see if we already have this spectrum in memory
  if(lScanIndex!=pFI->lLastScanIndex){

    switch(pFI->fileType){
      case 1:
      case 3:
      case 6:
        pFI->mzML->readSpectrumFromOffset((f_off)lScanIndex,iIndex);
        pFI->lLastScanIndex=lScanIndex;
        break;
      case 2:
      case 4:
        pFI->mzXML->readSpectrumFromOffset((f_off)lScanIndex);
        pFI->lLastScanIndex = lScanIndex;
        break;
#ifdef MZP_HDF
      case 5:
        v2=pFI->mz5->getSpecIndex();
        for(i=0;i<v2->size();i++) {
          if(v2->at(i).offset==(f_off)lScanIndex) {
            pFI->mz5->readSpectrum((int)v2->at(i).scanNum);
            pFI->lLastScanIndex = lScanIndex;
            break;
          }
        }
        break;
#endif
      default:
        pFI->bs->clear();
        pFI->lLastScanIndex=0;
        break;
    }
    v=NULL;
#ifdef MZP_HDF
    v2=NULL;
#endif
  }

  size_t j=0;
  if(pFI->bs->size()>0){
	  if (pFI->bs->size() > pFI->peakCapacity || pFI->pPeaks==NULL) {
      pFI->peakCapacity = (int)pFI->bs->size();
      if (pFI->pPeaks != NULL) {
	      free(pFI->pPeaks);
      }
      if ((pFI->fileType == 3 || pFI->fileType == 1 || pFI->fileType == 6) && pFI->mzML->getIonMobility()){
		    pPeaks = (RAMPREAL *) malloc((pFI->bs->size()+1) * 3 * sizeof(RAMPREAL) + 1);
      } else { 														
        pPeaks = (RAMPREAL *) malloc((pFI->bs->size()+1) * 2 * sizeof(RAMPREAL) + 1);
	    }
	    pFI->pPeaks = pPeaks;
    } else {
      pPeaks = pFI->pPeaks;
    }   
    for(i=0;i<pFI->bs->size();i++){
      if ((pFI->fileType == 3 || pFI->fileType == 1 || pFI->fileType == 6) && pFI->mzML->getIonMobility()) {
        pPeaks[j++]=pFI->bs->getIonMobDP(i).mz;
        pPeaks[j++]=pFI->bs->getIonMobDP(i).intensity;
        pPeaks[j++]=pFI->bs->getIonMobDP(i).ionMobility;
      } else {								
        pPeaks[j++]=pFI->bs->operator [](i).mz;
        pPeaks[j++]=pFI->bs->operator [](i).intensity;
      }
    }
  } else {
    pPeaks = (RAMPREAL *) malloc(2 * sizeof(RAMPREAL));
  }
  pPeaks[j]=-1;

  return pPeaks;
}

int mzParser::readPeaksCount(RAMPFILE *pFI, ramp_fileoffset_t lScanIndex){
  ScanHeaderStruct s;
  readHeader(pFI, lScanIndex, &s);
  return s.peaksCount;
}

void mzParser::readRunHeader(RAMPFILE *pFI, ramp_fileoffset_t *pScanIndex, struct RunHeaderStruct *runHeader, int iLastScan){
  vector<cindex>* v;
#ifdef MZP_HDF
  vector<cMz5Index>* v2;
#endif
  unsigned int i;

  runHeader->scanCount=0;
  runHeader->dEndTime=0.0;
  runHeader->dStartTime=0.0;
  runHeader->endMZ=0.0;
  runHeader->highMZ=0.0;
  runHeader->lowMZ=0.0;
  runHeader->startMZ=0.0;
  
  switch(pFI->fileType){
    case 1:
    case 3:
      v=pFI->mzML->getSpecIndex();
      runHeader->scanCount=(int)v->size();
      
      pFI->mzML->readHeader((int)v->at(0).scanNum);
      runHeader->dStartTime=(double)pFI->bs->getRTime(false);
      runHeader->lowMZ=pFI->bs->getLowMZ();
      runHeader->highMZ=pFI->bs->getHighMZ();
      runHeader->startMZ=runHeader->lowMZ;
      runHeader->endMZ=runHeader->highMZ;
      
      for(i=1;i<v->size();i++) {
        pFI->mzML->readHeader((int)v->at(i).scanNum);
        if(pFI->bs->getLowMZ()<runHeader->lowMZ) {
          runHeader->lowMZ=pFI->bs->getLowMZ();
          runHeader->startMZ=runHeader->lowMZ;
        }
        if(pFI->bs->getHighMZ()>runHeader->highMZ){
          runHeader->highMZ=pFI->bs->getHighMZ();
          runHeader->endMZ=runHeader->highMZ;
        }
      }
      pFI->mzML->readHeader((int)v->at(v->size()-1).scanNum);
      break;

    case 2:
    case 4:
      v=pFI->mzXML->getIndex();
      runHeader->scanCount=(int)v->size();
      
      pFI->mzXML->readHeader((int)v->at(0).scanNum);
      runHeader->dStartTime=(double)pFI->bs->getRTime(false);
      runHeader->lowMZ=pFI->bs->getLowMZ();
      runHeader->highMZ=pFI->bs->getHighMZ();
      runHeader->startMZ=runHeader->lowMZ;
      runHeader->endMZ=runHeader->highMZ;
      
      for(i=1;i<v->size();i++) {
        pFI->mzXML->readHeader((int)v->at(i).scanNum);
        if(pFI->bs->getLowMZ()<runHeader->lowMZ) {
          runHeader->lowMZ=pFI->bs->getLowMZ();
          runHeader->startMZ=runHeader->lowMZ;
        }
        if(pFI->bs->getHighMZ()>runHeader->highMZ){
          runHeader->highMZ=pFI->bs->getHighMZ();
          runHeader->endMZ=runHeader->highMZ;
        }
      }
      pFI->mzXML->readHeader((int)v->at(v->size()-1).scanNum);
      break;

#ifdef MZP_HDF
    case 5:
      v2=pFI->mz5->getSpecIndex();
      runHeader->scanCount=(int)v2->size();
      
      pFI->mz5->readHeader((int)v2->at(0).scanNum);
      runHeader->dStartTime=(double)pFI->bs->getRTime(false);
      runHeader->lowMZ=pFI->bs->getLowMZ();
      runHeader->highMZ=pFI->bs->getHighMZ();
      runHeader->startMZ=runHeader->lowMZ;
      runHeader->endMZ=runHeader->highMZ;
      
      for(i=1;i<v2->size();i++) {
        pFI->mz5->readHeader((int)v2->at(i).scanNum);
        if(pFI->bs->getLowMZ()<runHeader->lowMZ) {
          runHeader->lowMZ=pFI->bs->getLowMZ();
          runHeader->startMZ=runHeader->lowMZ;
        }
        if(pFI->bs->getHighMZ()>runHeader->highMZ){
          runHeader->highMZ=pFI->bs->getHighMZ();
          runHeader->endMZ=runHeader->highMZ;
        }
      }
      pFI->mz5->readHeader((int)v2->at(v2->size()-1).scanNum);
      break;
#endif

    default:
      pFI->bs->clear();
      v=NULL;
#ifdef MZP_HDF
      v2=NULL;
#endif
      return;
  }
  v=NULL;
#ifdef MZP_HDF
  v2=NULL;
#endif

}




//--------------------------------------------------
// CACHED RAMP FUNCTIONS
//--------------------------------------------------
void mzParser::clearScanCache(struct ScanCacheStruct* cache){
  for (int i=0; i<cache->size; i++) {
    if (cache->peaks[i] == NULL) continue;
    free(cache->peaks[i]);
    cache->peaks[i] = NULL;
  }
  memset(cache->headers, 0, cache->size * sizeof(struct ScanHeaderStruct));
}

void mzParser::freeScanCache(struct ScanCacheStruct* cache){
  if (cache) {
    for (int i=0; i<cache->size; i++){
      if (cache->peaks[i] != NULL) free(cache->peaks[i]);
    }
    free(cache->peaks);
    free(cache->headers);
    free(cache);
  }
}

int mzParser::getCacheIndex(struct ScanCacheStruct* cache, int seqNum) {
  int seqNumStart = cache->seqNumStart;
  int size = cache->size;

  // First access, just set the start to seqNum.
  if (seqNumStart == 0) cache->seqNumStart = seqNum;

  // If requested scan is less than cache start, shift cache window
  // left to start at requested scan.
  else if (seqNum < seqNumStart) mzParser::shiftScanCache(cache, (int) (seqNum - seqNumStart));

  // If requested scan is greater than cache end, shift cache window
  // right so last entry is requested scan.
  else if (seqNum >= seqNumStart + size) mzParser::shiftScanCache(cache, (int) (seqNum - (seqNumStart + size - 1)));

  return (int) (seqNum - cache->seqNumStart);
}

struct ScanCacheStruct* mzParser::getScanCache(int size){
  struct ScanCacheStruct* cache = (struct ScanCacheStruct*) malloc(sizeof(struct ScanCacheStruct));
  cache->seqNumStart = 0;
  cache->size = size;
  cache->headers = (struct ScanHeaderStruct*) calloc(size, sizeof(struct ScanHeaderStruct));
  cache->peaks = (RAMPREAL**) calloc(size, sizeof(RAMPREAL*));
  return cache;
}

const struct ScanHeaderStruct* mzParser::readHeaderCached(struct ScanCacheStruct* cache, int seqNum, RAMPFILE* pFI, ramp_fileoffset_t lScanIndex){
  int i = mzParser::getCacheIndex(cache, seqNum);
  if (cache->headers[i].msLevel == 0) readHeader(pFI, lScanIndex, cache->headers + i);
  return cache->headers + i;
}

int  mzParser::readMsLevelCached(struct ScanCacheStruct* cache, int seqNum, RAMPFILE* pFI, ramp_fileoffset_t lScanIndex){
  const struct ScanHeaderStruct* header = mzParser::readHeaderCached(cache, seqNum, pFI, lScanIndex);
  return header->msLevel;
}

const RAMPREAL* mzParser::readPeaksCached(struct ScanCacheStruct* cache, int seqNum, RAMPFILE* pFI, ramp_fileoffset_t lScanIndex){
  int i = mzParser::getCacheIndex(cache, seqNum);
  if (cache->peaks[i] == NULL) cache->peaks[i] = readPeaks(pFI, lScanIndex);
  return cache->peaks[i];
}

void mzParser::shiftScanCache(struct ScanCacheStruct* cache, int nScans) {
  int i;
  cache->seqNumStart += nScans;
  if (abs(nScans) > cache->size) {
    // If the shift is larger than the size of the cache window,
    // just clear the whole cache.
    mzParser::clearScanCache(cache);
  } else if (nScans > 0) {
    // Shifting window to the right.  Memory moves right, with new
    // empty scans on the end.

    // Free the peaks that memmove will overwrite.
    for (i = 0; i < nScans; i++) {
      if (cache->peaks[i] != NULL) free(cache->peaks[i]);
    }
    memmove(cache->peaks, cache->peaks + nScans, (cache->size - nScans) * sizeof(RAMPREAL*));
    memset(cache->peaks + cache->size - nScans, 0, nScans * sizeof(RAMPREAL*));
    memmove(cache->headers, cache->headers + nScans,(cache->size - nScans) * sizeof(struct ScanHeaderStruct));
    memset(cache->headers + cache->size - nScans, 0, nScans * sizeof(struct ScanHeaderStruct));
  } else if (nScans < 0) {
    // Shifting window to the left.  Memory moves right, with new
    // empty scans at the beginning.
    nScans = -nScans;

    // Free the peaks that memmove will overwrite.
    for (i = 0; i < nScans; i++) {
      if (cache->peaks[cache->size - 1 - i] != NULL) free(cache->peaks[cache->size - 1 - i]);
    }
    memmove(cache->peaks + nScans, cache->peaks, (cache->size - nScans) * sizeof(RAMPREAL*));
    memset(cache->peaks, 0, nScans * sizeof(RAMPREAL*));
    memmove(cache->headers  + nScans, cache->headers, (cache->size - nScans) * sizeof(struct ScanHeaderStruct));
    memset(cache->headers, 0, nScans * sizeof(struct ScanHeaderStruct));
  }
}



//--------------------------------------------------
// DEAD FUNCTIONS
//--------------------------------------------------
int mzParser::isScanAveraged(struct ScanHeaderStruct *scanHeader){
  cerr << "call to unsupported function: isScanAveraged(struct ScanHeaderStruct *scanHeader)" << endl;
  return 0;
}

int mzParser::isScanMergedResult(struct ScanHeaderStruct *scanHeader){
  cerr << "call to unsupported function: isScanMergedResult(struct ScanHeaderStruct *scanHeader)" << endl;
  return 0;
}

int mzParser::rampSelfTest(char *filename){
  cerr << "call to unsupported function: rampSelfTest(char *filename)" << endl;
  return 0;
}

char* mzParser::rampTrimBaseName(char *buf){
  cerr << "call to unsupported function: rampTrimBaseName(char *buf)" << endl;
  return buf;
}

int mzParser::rampValidateOrDeriveInputFilename(char *inbuf, int inbuflen, char *spectrumName){
  cerr << "call to unsupported function: rampValidateOrDeriveInputFilename(char *inbuf, int inbuflen, char *spectrumName)" << endl;
  return 0;
}

double mzParser::readStartMz(RAMPFILE *pFI, ramp_fileoffset_t lScanIndex){
  cerr << "call to unsupported function: readStartMz(RAMPFILE *pFI, ramp_fileoffset_t lScanIndex)" << endl;
  return 0.0;
}

double mzParser::readEndMz(RAMPFILE *pFI, ramp_fileoffset_t lScanIndex){
  cerr << "call to unsupported function: readEndMz(RAMPFILE *pFI, ramp_fileoffset_t lScanIndex)" << endl;
  return 0.0;
}

void mzParser::setRampOption(long option){
  cerr << "call to unsupported function: setRampOption(long option)" << endl;
}
