/*
Copyright 2005-2016, Michael R. Hoopmann

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include "Spectrum.h"
#include <iostream>
#include <iomanip>

using namespace std;
using namespace MSToolkit;

Spectrum::Spectrum(){
  //cout<<"in Spectrum constructor!"<<endl;

  rTime=0;
  charge=0;
  positiveScan = true;
  scanNumber=0;
  scanNumber2=0;
  msLevel = 2;
  monoMZ=new vector<double>;
  mz=new vector<double>;
  sps=new vector<double>;
  precursor=new vector<MSPrecursorInfo>;
  TIC=0;
  IIT=0;
  compensationVoltage=0;
  convA=0;
  convB=0;
	convC=0;
	convD=0;
	convE=0;
	convI=0;
  BPI=0;
  BPM=0;
  selectionWinLower = 0;
  selectionWinUpper = 0;
  centroidStatus=2;
  inverseReducedIonMobility=0;
  ionMobilityDriftTime=0;

  fileType=Unspecified;
  vPeaks = new vector<Peak_T>;
  vIonMobility = new vector<double>;
  vEZ = new vector<EZState>;
  vZ = new vector<ZState>;
  actMethod=mstNA;

  strcpy(rawFilter,"");
  strcpy(nativeID,"");

  userParams = new vector<MSUserParam>;
}



Spectrum::~Spectrum(){
  if(vPeaks) delete vPeaks;
  if(vIonMobility) delete vIonMobility;
  if(vEZ) delete vEZ;
  if(vZ) delete vZ;
	if(mz) delete mz;
  if(monoMZ) delete monoMZ;
  if(sps) delete sps;
  if(precursor) delete precursor;
  if(userParams) delete userParams;
}

Spectrum::Spectrum(const Spectrum& s){

  unsigned int i;

  rTime = s.rTime;
  charge = s.charge;
  positiveScan = s.positiveScan;
  scanNumber = s.scanNumber;
  scanNumber2 = s.scanNumber2;
  msLevel = s.msLevel;
  monoMZ = new vector<double>;
  for(i=0;i<s.monoMZ->size();i++){
		monoMZ->push_back(s.monoMZ->at(i));
	}
  mz = new vector<double>;
	for(i=0;i<s.mz->size();i++){
		mz->push_back(s.mz->at(i));
	}
  sps = new vector<double>(*s.sps);
  precursor=new vector<MSPrecursorInfo>(*s.precursor);
  fileType = s.fileType;
  IIT = s.IIT;
  TIC = s.TIC;
  compensationVoltage = s.compensationVoltage;
  convA = s.convA;
  convB = s.convB;
  convC = s.convC;
  convD = s.convD;
  convE = s.convE;
  convI = s.convI;
  BPI = s.BPI;
  BPM = s.BPM;
  selectionWinLower=s.selectionWinLower;
  selectionWinUpper=s.selectionWinUpper;
  centroidStatus = s.centroidStatus;
  inverseReducedIonMobility=s.inverseReducedIonMobility;
  ionMobilityDriftTime=s.ionMobilityDriftTime;

  vPeaks = new vector<Peak_T>;
  for(i=0;i<s.vPeaks->size();i++){
    vPeaks->push_back(s.vPeaks->at(i));
  }
  vEZ = new vector<EZState>;
  for(i=0;i<s.vEZ->size();i++){
    vEZ->push_back(s.vEZ->at(i));
  }
  vZ = new vector<ZState>;
  for(i=0;i<s.vZ->size();i++){
    vZ->push_back(s.vZ->at(i));
  }
  vIonMobility = new vector<double>(*s.vIonMobility);
  strcpy(rawFilter,s.rawFilter);
  strcpy(nativeID,s.nativeID);
  scanDescription=s.scanDescription;

  userParams = new vector<MSUserParam>(*s.userParams);
}

Spectrum& Spectrum::operator=(const Spectrum& s){
	//cout<<"in Spectrum ="<<endl;
  unsigned int i;
  if (this != &s) {
    delete vPeaks;
    delete vIonMobility;
    delete vEZ;
    delete vZ;
    delete monoMZ;
		delete mz;
    delete sps;
    delete precursor;
    delete userParams;
    monoMZ = new vector<double>;
    for(i=0;i<s.monoMZ->size();i++){
		  monoMZ->push_back(s.monoMZ->at(i));
	  }
		mz = new vector<double>;
		for(i=0;i<s.mz->size();i++){
			mz->push_back(s.mz->at(i));
		}
    sps = new vector<double>(*s.sps);
    precursor = new vector<MSPrecursorInfo>(*s.precursor);
    vPeaks = new vector<Peak_T>(*s.vPeaks);
    vIonMobility = new vector<double>(*s.vIonMobility);
    vEZ = new vector<EZState>(*s.vEZ);
    vZ = new vector<ZState>(*s.vZ);
    userParams = new vector<MSUserParam>(*s.userParams);
    rTime = s.rTime;
    charge = s.charge;
    positiveScan = s.positiveScan;
    scanNumber = s.scanNumber;
    scanNumber2 = s.scanNumber2;
    msLevel = s.msLevel;
    BPI = s.BPI;
    BPM = s.BPM;
    compensationVoltage = s.compensationVoltage;
    convA = s.convA;
    convB = s.convB;
    convC = s.convC;
    convD = s.convD;
    convE = s.convE;
    convI = s.convI;
    TIC = s.TIC;
    IIT = s.IIT;
    fileType = s.fileType;
    centroidStatus = s.centroidStatus;
    inverseReducedIonMobility=s.inverseReducedIonMobility;
    ionMobilityDriftTime=s.ionMobilityDriftTime;
    selectionWinLower = s.selectionWinLower;
    selectionWinUpper = s.selectionWinUpper;
    strcpy(rawFilter,s.rawFilter);
    strcpy(nativeID,s.nativeID);
    scanDescription = s.scanDescription;
  }
  return *this;
}

Peak_T& Spectrum::operator[](const int& i) {
	return vPeaks->operator[](i);
}

Peak_T& Spectrum::operator[](const unsigned int& i) {
  return vPeaks->operator[](i);
}

Peak_T& Spectrum::operator[](const size_t& i) {
  return vPeaks->operator[](i);
}


/* ----- Begin Functions ----- */


/* Adds Result struct to end of spectrum. */
void Spectrum::add(Peak_T& p){
  vPeaks->push_back(p);
}

void Spectrum::add(double d1, float d2){
  Peak_T p;
  p.mz=d1;
  p.intensity=d2;
  vPeaks->push_back(p);
}

void Spectrum::add(double d1, float d2, double d3) {
  Peak_T p;
  p.mz = d1;
  p.intensity = d2;
  vPeaks->push_back(p);
  vIonMobility->push_back(d3);
}

void Spectrum::addEZState(EZState& z){
	vEZ->push_back(z);
}

void Spectrum::addEZState(int i, double d, float f1, float f2){
	EZState z;
	z.z=i;
	z.mh=d;
  z.pRTime=f1;
  z.pArea=f2;
	vEZ->push_back(z);
}

void Spectrum::addMZ(double d, double mono){
	mz->push_back(d);
  monoMZ->push_back(mono);
}

void Spectrum::addPrecursor(MSPrecursorInfo& pi){
  precursor->push_back(pi);
}

void Spectrum::addSPS(double d) {
  sps->push_back(d);
}

void Spectrum::addUserParam(const string& name, const string& value, const string& sType) {
  MSUserParam up;
  up.name=name;
  up.value=value;
  if(sType.find("string")!=string::npos) up.type=dtString;
  else if (sType.find("double") != string::npos) up.type = dtDouble;
  else if (sType.find("float") != string::npos) up.type = dtFloat;
  else if (sType.find("int") != string::npos) up.type = dtInt;
  userParams->push_back(up);
}

void Spectrum::addUserParam(const MSUserParam& up){
  userParams->push_back(up);
}

void Spectrum::addZState(ZState& z){
	vZ->push_back(z);
}

void Spectrum::addZState(int i, double d){
	ZState z;
	z.z=i;
	z.mh=d;
	vZ->push_back(z);
}

/* Returns Result struct of single element in the spectrum. */
Peak_T& Spectrum::at(const int& i){
  return vPeaks->operator [](i);
}

Peak_T& Spectrum::at(const unsigned int& i){
  return vPeaks->operator [](i);
}

Peak_T& Spectrum::at(const size_t& i) {
  return vPeaks->operator [](i);
}

EZState& Spectrum::atEZ(const int& i){
	return vEZ->operator [](i);
}

EZState& Spectrum::atEZ(const unsigned int& i){
	return vEZ->operator [](i);
}

EZState& Spectrum::atEZ(const size_t& i) {
  return vEZ->operator [](i);
}

double& Spectrum::atIM(const int& i) {
  return vIonMobility->operator [](i);
}

double& Spectrum::atIM(const unsigned int& i) {
  return vIonMobility->operator [](i);
}

double& Spectrum::atIM(const size_t& i) {
  return vIonMobility->operator [](i);
}

ZState& Spectrum::atZ(const int& i){
	return vZ->operator [](i);
}

ZState& Spectrum::atZ(const unsigned int& i){
	return vZ->operator [](i);
}

ZState& Spectrum::atZ(const size_t& i) {
  return vZ->operator [](i);
}

/* Clears the spectrum */
void Spectrum::clear(){
	delete vPeaks;
	vPeaks = new vector<Peak_T>;
  delete vIonMobility;
  vIonMobility = new vector<double>;
  delete vEZ;
  vEZ = new vector<EZState>;
	delete vZ;
	vZ = new vector<ZState>;
	delete mz;
	mz = new vector<double>;
  delete monoMZ;
  monoMZ = new vector<double>;
  delete sps;
  sps = new vector<double>;
  delete precursor;
  precursor = new vector<MSPrecursorInfo>;
	scanNumber = 0;
  scanNumber2 = 0;
	rTime = 0;
	charge = 0;
    positiveScan = true;
    msLevel = 0;
  convA = 0;
  convB = 0;
  TIC = 0;
  IIT = 0;
  BPI = 0;
  BPM = 0;
  selectionWinLower=0;
  selectionWinUpper=0;
  fileID.clear();
	fileType = Unspecified;
  actMethod=mstNA;
  inverseReducedIonMobility=0;
  ionMobilityDriftTime=0;
  scanDescription.clear();
  delete userParams;
  userParams=new vector<MSUserParam>;
}

void Spectrum::clearMZ(){
	delete mz;
	mz = new vector<double>;
  delete monoMZ;
	monoMZ = new vector<double>;
  delete precursor;
  precursor = new vector<MSPrecursorInfo>;
}

void Spectrum::clearPeaks(){
	delete vPeaks;
	vPeaks = new vector<Peak_T>;
  delete vIonMobility;
  vIonMobility = new vector<double>;
}

/* Erases element i in the spectrum. */
void Spectrum::erase(unsigned int i){
  vector<Peak_T>::iterator vi;
  vi=vPeaks->begin()+i;
  vPeaks->erase(vi);

  if(vIonMobility->size()>0){
    vector<double>::iterator ii;
    ii=vIonMobility->begin()+i;
    vIonMobility->erase(ii);
  }
}

/* Erases element i to element j, inclusive, in the spectrum. */
void Spectrum::erase(unsigned int i, unsigned int j){
  vector<Peak_T>::iterator vi1;
  vector<Peak_T>::iterator vi2;
  vi1=vPeaks->begin()+i;
  vi2=vPeaks->begin()+j+1;
  vPeaks->erase(vi1,vi2);

  if(vIonMobility->size()>0){
    vector<double>::iterator ii1;
    vector<double>::iterator ii2;
    ii1 = vIonMobility->begin() + i;
    ii2 = vIonMobility->begin() + j + 1;
    vIonMobility->erase(ii1, ii2);
  }
}

void Spectrum::eraseEZ(unsigned int i){
  vector<EZState>::iterator vi;
  vi=vEZ->begin()+i;
  vEZ->erase(vi);
}

/* Erases element i to element j, inclusive, in the spectrum. */
void Spectrum::eraseEZ(unsigned int i, unsigned int j){
  vector<EZState>::iterator vi1;
  vector<EZState>::iterator vi2;
  vi1=vEZ->begin()+i;
  vi2=vEZ->begin()+j+1;
  vEZ->erase(vi1,vi2);
}

void Spectrum::eraseZ(unsigned int i){
  vector<ZState>::iterator vi;
  vi=vZ->begin()+i;
  vZ->erase(vi);
}

/* Erases element i to element j, inclusive, in the spectrum. */
void Spectrum::eraseZ(unsigned int i, unsigned int j){
  vector<ZState>::iterator vi1;
  vector<ZState>::iterator vi2;
  vi1=vZ->begin()+i;
  vi2=vZ->begin()+j+1;
  vZ->erase(vi1,vi2);
}

MSActivation Spectrum::getActivationMethod(){
  return actMethod;
}

float Spectrum::getBPI(){
  return BPI;
}

double Spectrum::getBPM(){
  return BPM;
}

int Spectrum::getCentroidStatus(){
  return centroidStatus;
}

int Spectrum::getCharge(){
  return charge;
}

bool Spectrum::getPositiveScan(){
  return positiveScan;
}
double Spectrum::getCompensationVoltage(){
  return compensationVoltage;
}

double Spectrum::getConversionA(){
  return convA;
}

double Spectrum::getConversionB(){
  return convB;
}

double Spectrum::getConversionC(){
  return convC;
}

double Spectrum::getConversionD(){
  return convD;
}

double Spectrum::getConversionE(){
  return convE;
}

double Spectrum::getConversionI(){
  return convI;
}

string Spectrum::getFileID(){
  return fileID;
}

MSSpectrumType Spectrum::getFileType(){
	return fileType;
}

double Spectrum::getInverseReducedIonMobility(){
  return inverseReducedIonMobility;
}

float Spectrum::getIonInjectionTime(){
  return IIT;
}

double Spectrum::getIonMobilityDriftTime() {
  return ionMobilityDriftTime;
}

double Spectrum::getMonoMZ(int index){
	if(index>=(int)monoMZ->size()) return -1.0;
  return monoMZ->at(index);
}

double Spectrum::getMZ(int index){
	if(index>=(int)mz->size()) return -1.0;
  return mz->at(index);
}

bool Spectrum::getNativeID(char* c, int sz){
  if(sz<(int)strlen(nativeID)) {
    cout << "Buffer too small to retrieve spectrumNativeID. " << sizeof(c) << " " << strlen(nativeID) << endl;
    return false;
  } else {
    strcpy(c,nativeID);
    return true;
  }
}

MSPrecursorInfo Spectrum::getPrecursor(int index){
  if(index>=(int)precursor->size()) return MSPrecursorInfo();
  return precursor->at(index);
}

bool Spectrum::getRawFilter(char* c, int sz){
  if(sz<(int)strlen(rawFilter)) {
    cout << "Buffer too small to retrieve RAW filter. " << sizeof(c) << " " << strlen(rawFilter) << endl;
    return false;
  } else {
		strcpy(c,rawFilter);
    return true;
  }
}

float Spectrum::getRTime(){
  return rTime;
}

string Spectrum::getScanDescription(){
  return scanDescription;
}

int Spectrum::getScanNumber(bool second){
  if(second) return scanNumber2;
  else return scanNumber;
}

double Spectrum::getScanWindowLower(){
  return scanWinLower;
}

double Spectrum::getScanWindowUpper(){
  return scanWinUpper;
}

double Spectrum::getSelWindowLower(){
  return selectionWinLower;
}

double Spectrum::getSelWindowUpper(){
  return selectionWinUpper;
}

double Spectrum::getSPS(size_t index) {
  if (index >= sps->size()) return 0;
  return sps->at(index);
}

double Spectrum::getTIC(){
  return TIC;
}

MSUserParam Spectrum::getUserParam(const size_t& index) {
  if (index >= userParams->size())  return MSUserParam();
  return userParams->at(index);
}

MSUserParam Spectrum::getUserParam(const int& index) {
  if (index >= (int)userParams->size()) return MSUserParam();
  return userParams->at(index);
}

bool Spectrum::hasIonMobilityArray(){
  if(vIonMobility->size()>0) return true;
  return false;
}

void Spectrum::setBPI(float f){
  BPI=f;
}

void Spectrum::setBPM(double d){
  BPM=d;
}

void Spectrum::setCentroidStatus(int i){
  if(i>2) centroidStatus=2;
  else centroidStatus=i;
}

void Spectrum::setCharge(int i){
  charge=i;
}

void Spectrum::setPositiveScan(bool p){
  positiveScan = p;
}

void Spectrum::setCompensationVoltage(double d){
  compensationVoltage=d;
}

void Spectrum::setConversionA(double d){
  convA=d;
}

void Spectrum::setConversionB(double d){
  convB=d;
}

void Spectrum::setConversionC(double d){
  convC=d;
}

void Spectrum::setConversionD(double d){
  convD=d;
}

void Spectrum::setConversionE(double d){
  convE=d;
}

void Spectrum::setConversionI(double d){
  convI=d;
}

void Spectrum::setFileID(string s){
  fileID=s;
}

void Spectrum::setFileType(MSSpectrumType f){
	fileType=f;
}

void Spectrum::setInverseReducedIonMobility(double d){
  inverseReducedIonMobility=d;
}

void Spectrum::setIonInjectionTime(float f){
  IIT=f;
}

void Spectrum::setIonMobilityDriftTime(double d) {
  ionMobilityDriftTime = d;
}

void Spectrum::setMZ(double d, double mono){
	clearMZ();
	mz->push_back(d);
  monoMZ->push_back(mono);
}

void Spectrum::setNativeID(const char* c){
  if(strlen(c)>256) cout << "Error - spectrumNativeID filter larger than 256 characters." << endl;
  else strcpy(nativeID,c);
}

void Spectrum::setRawFilter(char* c){
  if(strlen(c)>256) cout << "Error - RAW filter larger than 256 characters." << endl;
  else strcpy(rawFilter,c);
}

void Spectrum::setRTime(float d){
  rTime=d;
}

void Spectrum::setScanDescription(string s) {
  scanDescription=s;
}

void Spectrum::setScanNumber(int i, bool second){
  if(second)scanNumber2=i;
  else scanNumber=i;
}

void Spectrum::setScanWindow(double lower, double upper){
  scanWinLower = lower;
  scanWinUpper = upper;
}

void Spectrum::setSelWindow(double lower, double upper){
  selectionWinLower=lower;
  selectionWinUpper=upper;
}

void Spectrum::setTIC(double d){
  TIC=d;
}

void Spectrum::setMsLevel(int level) {
  msLevel = level;
}

int Spectrum::getMsLevel() {
  return msLevel;
}

int Spectrum::getScanID(){
  return scanID;
}

void Spectrum::setScanID(int scanid){
  scanID = scanid;
}

/* Returns the number of elements in the spectrum. */
int Spectrum::size(){
  return (int)vPeaks->size();
}

int Spectrum::sizeEZ(){
	return (int)vEZ->size();
}

int Spectrum::sizeMZ(){
	return (int)mz->size();
}

int Spectrum::sizePrecursor(){
  return (int)precursor->size();
}

int Spectrum::sizeSPS() {
  return (int)sps->size();
}

size_t Spectrum::sizeUserParams(){
  return userParams->size();
}

int Spectrum::sizeZ(){
	return (int)vZ->size();
}

float Spectrum::getTotalIntensity(){
  float totalIntensity = 0;
  for(unsigned int i=0; i<vPeaks->size(); i++) totalIntensity += (vPeaks->at(i)).intensity;
  return totalIntensity;
}

/* Sorts the spectrum by Data. */
void Spectrum::sortIntensity(){
  qsort(&vPeaks->at(0),vPeaks->size(),sizeof(Peak_T),compareIntensity);
}

/* Sorts the spectrum by Mass. */
void Spectrum::sortMZ(){
  qsort(&vPeaks->at(0),vPeaks->size(),sizeof(Peak_T),compareMZ);
}

/* Sorts the spectrum by Data. */
void Spectrum::sortIntensityRev(){
  qsort(&vPeaks->at(0),vPeaks->size(),sizeof(Peak_T),compareIntensityRev);
}

/* Sorts the spectrum by Mass. */
void Spectrum::sortMZRev(){
  qsort(&vPeaks->at(0),vPeaks->size(),sizeof(Peak_T),compareMZRev);
}

//const vector<Peak_T>* Spectrum::getPeaks(){
//	return vPeaks;
//};

vector<Peak_T>* Spectrum::getPeaks(){
	return vPeaks;
}

void Spectrum::setPeaks(vector<Peak_T> peaks) {
  if(!vPeaks->empty()) vPeaks->clear();
  for(unsigned int i=0; i<peaks.size(); i++){
    vPeaks->push_back(peaks.at(i));
  }
}

void Spectrum::setActivationMethod(MSActivation m){
  actMethod=m;
}

void Spectrum::printMe() {
  cout << "Scan Number: " << getScanNumber() << endl
       << "Mass to charge: " << getMZ() << endl
       << "S Charge: " << getCharge() << endl 
       << "RT: " << getRTime() << endl;
    
  cout << fixed;
 
  for(unsigned int i=0; i<vPeaks->size(); i++) {
    cout << setprecision(10) << vPeaks->at(i).mz << "  " << vPeaks->at(i).intensity << endl;
  }
}

//Private Functions

/* For the qsort */
int Spectrum::compareIntensity(const void *p1, const void *p2){
  const Peak_T d1 = *(Peak_T *)p1;
  const Peak_T d2 = *(Peak_T *)p2;
  if(d1.intensity<d2.intensity) return -1;
  else if(d1.intensity>d2.intensity) return 1;
  else return 0;
}

/* For the qsort */
int Spectrum::compareMZ(const void *p1, const void *p2){
  const Peak_T d1 = *(Peak_T *)p1;
  const Peak_T d2 = *(Peak_T *)p2;
  if(d1.mz<d2.mz) return -1;
  else if(d1.mz>d2.mz) return 1;
  else return 0;
}

/* For the qsort */
int Spectrum::compareIntensityRev(const void *p1, const void *p2){
  const Peak_T d1 = *(Peak_T *)p1;
  const Peak_T d2 = *(Peak_T *)p2;
  if(d1.intensity>d2.intensity) return -1;
  else if(d1.intensity<d2.intensity) return 1;
  else return 0;
}

/* For the qsort */
int Spectrum::compareMZRev(const void *p1, const void *p2){
  const Peak_T d1 = *(Peak_T *)p1;
  const Peak_T d2 = *(Peak_T *)p2;
  if(d1.mz>d2.mz) return -1;
  else if(d1.mz<d2.mz) return 1;
  else return 0;
}

