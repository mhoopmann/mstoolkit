/*	
	BasicSpectrum.cpp
	Copyright (C) 2010-2011, Mike Hoopmann
	Institute for Systems Biology
	Version 1.0, Dec. 29, 2010
*/

//#include "BasicSpectrum.h"
#include "mzParser.h"


//------------------------------------------
//  Constructors & Destructors
//------------------------------------------
BasicSpectrum::BasicSpectrum() {
	activation=none;
	basePeakIntensity=0.0;
	basePeakMZ=0.0;
	highMZ=0.0;
	lowMZ=0.0;
	msLevel=1;
	peaksCount=0;
	precursorCharge=0;
	precursorMZ=0.0;
	rTime=0.0f;
	scanNum=0;
	totalIonCurrent=0.0;
	vData.clear();
}
BasicSpectrum::BasicSpectrum(const BasicSpectrum& s){
	vData.clear();
	for(unsigned int i=0;i<s.vData.size();i++) vData.push_back(s.vData[i]);
	activation=s.activation;
	basePeakIntensity=s.basePeakIntensity;
	basePeakMZ=s.basePeakMZ;
	highMZ=s.highMZ;
	lowMZ=s.lowMZ;
	msLevel=s.msLevel;
	peaksCount=s.peaksCount;
	precursorCharge=s.precursorCharge;
	precursorMZ=s.precursorMZ;
	rTime=s.rTime;
	scanNum=s.scanNum;
	totalIonCurrent=s.totalIonCurrent;
}
BasicSpectrum::~BasicSpectrum() { }

//------------------------------------------
//  Operator overloads
//------------------------------------------
BasicSpectrum& BasicSpectrum::operator=(const BasicSpectrum& s){
	if (this != &s) {
		vData.clear();
		for(unsigned int i=0;i<s.vData.size();i++) vData.push_back(s.vData[i]);
		activation=s.activation;
		basePeakIntensity=s.basePeakIntensity;
		basePeakMZ=s.basePeakMZ;
		highMZ=s.highMZ;
		lowMZ=s.lowMZ;
		msLevel=s.msLevel;
		peaksCount=s.peaksCount;
		precursorCharge=s.precursorCharge;
		precursorMZ=s.precursorMZ;
		rTime=s.rTime;
		scanNum=s.scanNum;
		totalIonCurrent=s.totalIonCurrent;
	}
	return *this;
}
specDP& BasicSpectrum::operator[ ](const unsigned int index) {
	return vData[index];
}

//------------------------------------------
//  Modifiers
//------------------------------------------
void BasicSpectrum::addDP(specDP dp) { vData.push_back(dp);}
void BasicSpectrum::clear(){
	activation=none;
	basePeakIntensity=0.0;
	basePeakMZ=0.0;
	highMZ=0.0;
	lowMZ=0.0;
	msLevel=1;
	peaksCount=0;
	precursorCharge=0;
	precursorMZ=0;
	rTime=0.0f;
	scanNum=0;
	totalIonCurrent=0.0;
	vData.clear();
}
void BasicSpectrum::setActivation(int a){ activation=a;}
void BasicSpectrum::setBasePeakIntensity(double d){ basePeakIntensity=d;}
void BasicSpectrum::setBasePeakMZ(double d){ basePeakMZ=d;}
void BasicSpectrum::setCollisionEnergy(double d){ collisionEnergy=d;}
void BasicSpectrum::setHighMZ(double d){ highMZ=d;}
void BasicSpectrum::setLowMZ(double d){ lowMZ=d;}
void BasicSpectrum::setMSLevel(int level){ msLevel=level;}
void BasicSpectrum::setPeaksCount(int i){ peaksCount=i;}
void BasicSpectrum::setPrecursorCharge(int z){ precursorCharge=z;}
void BasicSpectrum::setPrecursorMZ(double mz){ precursorMZ=mz;}
void BasicSpectrum::setRTime(float f){ rTime=f;}
void BasicSpectrum::setScanNum(int num){ scanNum=num;}
void BasicSpectrum::setTotalIonCurrent(double d){ totalIonCurrent=d;}

//------------------------------------------
//  Accessors
//------------------------------------------
int BasicSpectrum::getActivation(){ return activation;}
double BasicSpectrum::getBasePeakIntensity(){ return basePeakIntensity;}
double BasicSpectrum::getBasePeakMZ(){ return basePeakMZ;}
double BasicSpectrum::getCollisionEnergy(){ return collisionEnergy;}
double BasicSpectrum::getHighMZ(){ return highMZ;}
double BasicSpectrum::getLowMZ(){ return lowMZ;}
int BasicSpectrum::getMSLevel(){ return msLevel;}
int BasicSpectrum::getPeaksCount(){ return peaksCount;}
int BasicSpectrum::getPrecursorCharge(){ return precursorCharge;}
double BasicSpectrum::getPrecursorMZ(){ return precursorMZ;}
float BasicSpectrum::getRTime(bool min){
	if(min) return rTime;
	else return rTime*60;
}
int BasicSpectrum::getScanNum(){ return scanNum;}
double BasicSpectrum::getTotalIonCurrent(){ return totalIonCurrent;}
unsigned int BasicSpectrum::size(){ return vData.size();}
