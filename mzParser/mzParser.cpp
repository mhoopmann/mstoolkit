#include "mzParser.h"

MzParser::MzParser(BasicSpectrum* s){
	spec=s;
	fileType=0;
	mzML=NULL;
	mzXML=NULL;
}

MzParser::~MzParser(){
	spec=NULL;
	if(mzML!=NULL) delete mzML;
	if(mzXML!=NULL) delete mzXML;
}

int MzParser::highScan(){
	switch(fileType){
		case 1:
			return mzML->highScan();
			break;
		case 2:
			return mzXML->highScan();
			break;
		default:
			break;
	}
	return 0;
}

bool MzParser::load(char* fname){
	if(mzML!=NULL) {
		delete mzML;
		mzML=NULL;
	}
	if(mzXML!=NULL) {
		delete mzXML;
		mzXML=NULL;
	}
	fileType=checkFileType(fname);
	switch(fileType){
		case 1:
			mzML = new SAXMzmlHandler(spec);
			return mzML->load(fname);
			break;
		case 2:
			mzXML = new SAXMzxmlHandler(spec);
			return mzXML->load(fname);
			break;
		default:
			break;
	}
	return false;
}

int MzParser::lowScan(){
	switch(fileType){
		case 1:
			return mzML->lowScan();
			break;
		case 2:
			return mzXML->lowScan();
			break;
		default:
			break;
	}
	return 0;
}

bool MzParser::readSpectrum(int num){
	switch(fileType){
		case 1:
			return mzML->readSpectrum(num);
			break;
		case 2:
			return mzXML->readSpectrum(num);
			break;
		default:
			break;
	}
	return false;
}

bool MzParser::readSpectrumHeader(int num){
	switch(fileType){
		case 1:
			return mzML->readHeader(num);
			break;
		case 2:
			return mzXML->readHeader(num);
			break;
		default:
			break;
	}
	return false;
}

int MzParser::checkFileType(char* fname){
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
