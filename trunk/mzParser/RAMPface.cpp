//#include "RAMPface.h"
#include "mzParser.h"

int checkFileType(const char* fname){
	char file[256];
	char ext[256];
	char *tok;

	strcpy(file,fname);
	tok=strtok(file,".\n");
	while(tok!=NULL){
		strcpy(ext,tok);
		tok=strtok(NULL,".\n");
	}

	for(unsigned int i=0;i<strlen(ext);i++) ext[i]=toupper(ext[i]);

	if(!strcmp(ext,"MZML")) return 1;
	if(!strcmp(ext,"MZXML")) return 2;
	if(!strcmp(ext,"GZ")) {
		cout << ".gz is not allowed. Please deflate your data. No file loaded." << endl;
	}
	return 0;
}

ramp_fileoffset_t getIndexOffset(RAMPFILE *pFI){
	switch(pFI->fileType){
		case 1:
			return (ramp_fileoffset_t)pFI->mzML->getIndexOffset();
			break;
		case 2:
			return (ramp_fileoffset_t)pFI->mzXML->getIndexOffset();
			break;
		default:
			return -1;
	}
}

void rampCloseFile(RAMPFILE *pFI){
	if(pFI!=NULL) {
		delete pFI;
		pFI=NULL;
	}
}

const char** rampListSupportedFileTypes(){
	if (!data_Ext.size()) { // needs init
		data_Ext.push_back(".mzXML");
		data_Ext.push_back(".mzML");
		data_Ext.push_back(NULL); // end of list
	}
	return &(data_Ext[0]);
}

RAMPFILE* rampOpenFile(const char* filename){
	int i=checkFileType(filename);
	if(i==0){
		return NULL;
	} else {
		RAMPFILE* r=new RAMPFILE();
		r->bs = new BasicSpectrum();
		r->fileType=i;
		switch(i){
			case 1: //mzML
				r->mzML=new SAXMzmlHandler(r->bs);
				if(!r->mzML->load(filename)){
					delete r;
					return NULL;
				} else {
					return r;
				}
			case 2: //mzXML
				r->mzXML=new SAXMzxmlHandler(r->bs);
				if(!r->mzXML->load(filename)){
					delete r;
					return NULL;
				} else {
					return r;
				}
			default:
				delete r;
				return NULL;
		}
	}

}

char* rampValidFileType(const char *buf){
	char ext[256];
	const char* result = strrchr(buf,'.');

	strcpy(ext,result);
	for(unsigned int i=0;i<strlen(ext);i++) ext[i]=toupper(ext[i]);

	if(!strcmp(ext,".MZML")) return (char*) result;
	if(!strcmp(ext,".MZXML")) return (char*) result;
	if(!strcmp(ext,".GZ")) {
		cout << ".gz is not allowed. Please deflate your data. No file loaded." << endl;
	}
	if(!strcmp(ext,".MZDATA")) {
		cout << ".mzData is not supported. Please convert to mzXML or mzML." << endl;
	}
	result=NULL;
	return (char*) result;
}

//MH: Read header is redundant with readPeaks, which automatically reads the header.
//But due to legacy issues, this function must exist.
void readHeader(RAMPFILE *pFI, ramp_fileoffset_t lScanIndex, struct ScanHeaderStruct *scanHeader){

	vector<cindex>* v;
	unsigned int i;

	//memset(scanHeader,0,sizeof(struct ScanHeaderStruct));
	scanHeader->acquisitionNum=0;
	scanHeader->activationMethod[0]='\0';
	scanHeader->basePeakIntensity=0.0;
	scanHeader->basePeakMZ=0.0;
	scanHeader->collisionEnergy=0.0;
	scanHeader->filePosition=0;
	scanHeader->highMZ=0.0;
	scanHeader->ionisationEnergy=0.0;
	scanHeader->lowMZ=0.0;
	scanHeader->msLevel=0;
	scanHeader->precursorCharge=0;
	scanHeader->precursorIntensity=0.0;
	scanHeader->precursorMZ=0.0;
	scanHeader->precursorScanNum=0;
	scanHeader->retentionTime=0.0;
	scanHeader->totIonCurrent=0.0;

	if(lScanIndex<0) return;
	
	switch(pFI->fileType){
		case 1:
			v=pFI->mzML->getIndex();
			for(i=0;i<v->size();i++) {
				if(v->at(i).offset==(f_off)lScanIndex) {
					if(!pFI->mzML->readHeader(v->at(i).scanNum)){
						v=NULL;
						return;
					}
					break;
				}
			}
			break;
		case 2:
			v=pFI->mzXML->getIndex();
			for(i=0;i<v->size();i++) {
				if(v->at(i).offset==(f_off)lScanIndex) {
					if(!pFI->mzXML->readHeader(v->at(i).scanNum)){
						v=NULL;
						return;
					}
					break;
				}
			}
			break;
		default:
			pFI->bs->clear();
			v=NULL;
			return;
	}
	v=NULL;

	scanHeader->acquisitionNum=pFI->bs->getScanNum();
	scanHeader->basePeakIntensity=pFI->bs->getBasePeakIntensity();
	scanHeader->basePeakMZ=pFI->bs->getBasePeakMZ();
	scanHeader->collisionEnergy=pFI->bs->getCollisionEnergy();
	scanHeader->highMZ=pFI->bs->getHighMZ();
	scanHeader->lowMZ=pFI->bs->getLowMZ();
	scanHeader->msLevel=pFI->bs->getMSLevel();
	scanHeader->peaksCount=pFI->bs->getPeaksCount();
	scanHeader->precursorCharge=pFI->bs->getPrecursorCharge();
	scanHeader->precursorMZ=pFI->bs->getPrecursorMZ();
	scanHeader->retentionTime=(double)pFI->bs->getRTime(false);
	scanHeader->totIonCurrent=pFI->bs->getTotalIonCurrent();

	switch(pFI->bs->getActivation()){
		case 1: strcpy(scanHeader->activationMethod,"CID"); break;
		case 2: strcpy(scanHeader->activationMethod,"HCD"); break;
		case 3: strcpy(scanHeader->activationMethod,"ETD"); break;
		case 4: strcpy(scanHeader->activationMethod,"ETD+SA"); break;
		case 5: strcpy(scanHeader->activationMethod,"ECD"); break;
		default: strcpy(scanHeader->activationMethod,""); break;
	}

}

//MH: Indexes in RAMP are stored in an array indexed by scan number, with -1 for the offset
//if the scan number does not exist.
ramp_fileoffset_t* readIndex(RAMPFILE *pFI, ramp_fileoffset_t indexOffset, int *iLastScan){
	vector<cindex>* v;
	ramp_fileoffset_t* rIndex;
	unsigned int i;
	switch(pFI->fileType){
		case 1:
			v=pFI->mzML->getIndex();
			rIndex = (ramp_fileoffset_t *) malloc((pFI->mzML->highScan()+1)*sizeof(ramp_fileoffset_t));
			memset(rIndex,-1,(pFI->mzML->highScan()+1)*sizeof(ramp_fileoffset_t));
			for(i=0;i<v->size();i++) rIndex[v->at(i).scanNum]=(ramp_fileoffset_t)v->at(i).offset;
			rIndex[i]=-1;
			*iLastScan=v->at(i-1).scanNum;
			break;
		case 2:
			v=pFI->mzXML->getIndex();
			rIndex = (ramp_fileoffset_t *) malloc((pFI->mzXML->highScan()+1)*sizeof(ramp_fileoffset_t));
			memset(rIndex,-1,(pFI->mzXML->highScan()+1)*sizeof(ramp_fileoffset_t));
			for(i=0;i<v->size();i++) rIndex[v->at(i).scanNum]=(ramp_fileoffset_t)v->at(i).offset;
			rIndex[i]=-1;
			*iLastScan=v->at(i-1).scanNum;
			break;
		default:
			rIndex=NULL;
			*iLastScan=0;
			break;
	}
	v=NULL;
	return rIndex;
}

void readMSRun(RAMPFILE *pFI, struct RunHeaderStruct *runHeader){

	vector<cindex>* v;

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
			v=pFI->mzML->getIndex();
			runHeader->scanCount=v->size();
			pFI->mzML->readHeader(v->at(0).scanNum);
			runHeader->dStartTime=pFI->bs->getRTime(false);
			pFI->mzML->readHeader(v->at(v->size()-1).scanNum);
			runHeader->dEndTime=pFI->bs->getRTime(false);
			pFI->bs->clear();
			v=NULL;
			break;
		case 2:
			v=pFI->mzXML->getIndex();
			runHeader->scanCount=v->size();
			pFI->mzXML->readHeader(v->at(0).scanNum);
			runHeader->dStartTime=pFI->bs->getRTime(false);
			pFI->mzXML->readHeader(v->at(v->size()-1).scanNum);
			runHeader->dEndTime=pFI->bs->getRTime(false);
			pFI->bs->clear();
			v=NULL;
			break;
		default:
			break;
	}

}

//MH: Matching the index is very indirect, but requires less code,
//making this wrapper much easier to read
RAMPREAL* readPeaks(RAMPFILE* pFI, ramp_fileoffset_t lScanIndex){
	vector<cindex>* v;
	unsigned int i;
	RAMPREAL* pPeaks=NULL;

	if(lScanIndex<0) return pPeaks;

	switch(pFI->fileType){
		case 1:
			v=pFI->mzML->getIndex();
			for(i=0;i<v->size();i++) {
				if(v->at(i).offset==(f_off)lScanIndex) {
					pFI->mzML->readSpectrum(v->at(i).scanNum);
					break;
				}
			}
			break;
		case 2:
			v=pFI->mzXML->getIndex();
			for(i=0;i<v->size();i++) {
				if(v->at(i).offset==(f_off)lScanIndex) {
					pFI->mzXML->readSpectrum(v->at(i).scanNum);
					break;
				}
			}
			break;
		default:
			pFI->bs->clear();
			break;
	}
	v=NULL;


	unsigned int j=0;
	if(pFI->bs->size()>0){
		pPeaks = (RAMPREAL *) malloc((pFI->bs->size()+1) * 2 * sizeof(RAMPREAL) + 1);
		for(i=0;i<pFI->bs->size();i++){
			pPeaks[j++]=pFI->bs->operator [](i).mz;
			pPeaks[j++]=pFI->bs->operator [](i).intensity;
		}
	} else {
		pPeaks = (RAMPREAL *) malloc(2 * sizeof(RAMPREAL));
	}
	pPeaks[j]=-1;

	return pPeaks;
}

int readPeaksCount(RAMPFILE *pFI, ramp_fileoffset_t lScanIndex){
	ScanHeaderStruct s;
	readHeader(pFI, lScanIndex, &s);
	return s.peaksCount;
}


//--------------------------------------------------
// DEAD FUNCTIONS
//--------------------------------------------------
InstrumentStruct* getInstrumentStruct(RAMPFILE *pFI){
	cerr << "call to unsupported function: getInstrumentStruct(RAMPFILE *pFI)" << endl;
	return NULL;
}
string rampConstructInputFileName(const string &basename){
	cerr << "call to unsupported function: rampConstructInputFileName(const std::string &basename)" << endl;
	return "";
}

char* rampConstructInputFileName(char *buf,int buflen,const char *basename){
	cerr << "call to unsupported function: rampConstructInputFileName(char *buf,int buflen,const char *basename)" << endl;
	return "";
}

char* rampConstructInputPath(char *buf, int inbuflen, const char *dir_in, const char *basename){
	cerr << "call to unsupported function: rampConstructInputPath(char *buf, int inbuflen, const char *dir_in, const char *basename)" << endl;
	return "";
}

int rampSelfTest(char *filename){
	cerr << "call to unsupported function: rampSelfTest(char *filename)" << endl;
	return 0;
}

char* rampTrimBaseName(char *buf){
	cerr << "call to unsupported function: rampTrimBaseName(char *buf)" << endl;
	return "";
}

int rampValidateOrDeriveInputFilename(char *inbuf, int inbuflen, char *spectrumName){
	cerr << "call to unsupported function: rampValidateOrDeriveInputFilename(char *inbuf, int inbuflen, char *spectrumName)" << endl;
	return 0;
}

int readMsLevel(RAMPFILE *pFI, ramp_fileoffset_t lScanIndex){
	cerr << "call to unsupported function: readMsLevel(RAMPFILE *pFI, ramp_fileoffset_t lScanIndex)" << endl;
	return 0;
}

double readStartMz(RAMPFILE *pFI, ramp_fileoffset_t lScanIndex){
	cerr << "call to unsupported function: readStartMz(RAMPFILE *pFI, ramp_fileoffset_t lScanIndex)" << endl;
	return 0.0;
}

double readEndMz(RAMPFILE *pFI, ramp_fileoffset_t lScanIndex){
	cerr << "call to unsupported function: readEndMz(RAMPFILE *pFI, ramp_fileoffset_t lScanIndex)" << endl;
	return 0.0;
}

void readRunHeader(RAMPFILE *pFI, ramp_fileoffset_t *pScanIndex, struct RunHeaderStruct *runHeader, int iLastScan){
	cerr << "call to unsupported function: readRunHeader(RAMPFILE *pFI, ramp_fileoffset_t *pScanIndex, struct RunHeaderStruct *runHeader, int iLastScan)" << endl;
	return;
}

