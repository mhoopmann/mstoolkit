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
#ifndef _SPECTRUM_H
#define _SPECTRUM_H

#include "MSToolkitTypes.h"
#include <vector>
#include <cstring>
#include <cstdlib>
#include <iomanip>

namespace MSToolkit {
class Spectrum {
 public:
  //Constructors & Destructors
  Spectrum();
  Spectrum(char*);
  Spectrum(char, unsigned int);
  Spectrum(const Spectrum&);
  ~Spectrum();

  //Operator Functions
  Spectrum& operator=(const Spectrum&);
	Peak_T& operator[](const int&);
  Peak_T& operator[](const unsigned int&);
  Peak_T& operator[](const size_t&);

  //Functions
  void	    			add(Peak_T&);
  void			    	add(double,float);
  void			    	add(double, float, double); //third value is ion mobility
  void            addEZState(int,double,float,float);
  void            addEZState(EZState&);
	void						addMZ(double, double mono=0);
  void            addPrecursor(MSPrecursorInfo&);
  void            addSPS(double);
  void            addUserParam(const std::string&, const std::string&, const std::string&);
  void            addUserParam(const MSUserParam&);
  void    				addZState(int,double);
  void		    		addZState(ZState&);
  Peak_T&			    at(const int&);
  Peak_T&	    		at(const unsigned int&);
  Peak_T&         at(const size_t&);
  EZState&        atEZ(const int&);
  EZState&        atEZ(const unsigned int&);
  EZState&        atEZ(const size_t&);
  double&         atIM(const int&);
  double&         atIM(const unsigned int&);
  double&         atIM(const size_t&);
  ZState&			    atZ(const int&);
  ZState&	    		atZ(const unsigned int&);
  ZState&         atZ(const size_t&);
  void			    	clear();
	void						clearMZ();
	void						clearPeaks();
  void				    erase(unsigned int);
  void				    erase(unsigned int, unsigned int);
  void            eraseEZ(unsigned int);
  void            eraseEZ(unsigned int, unsigned int);
  void				    eraseZ(unsigned int);
  void				    eraseZ(unsigned int, unsigned int);
  MSActivation    getActivationMethod();
  float           getArea();
  float           getBPI();
  double          getBPM();
  int             getCentroidStatus();
  int				      getCharge();
  bool            getPositiveScan();
  double          getCompensationVoltage();
  double          getConversionA();
  double          getConversionB();
  double          getConversionC();
  double          getConversionD();
  double          getConversionE();
  double          getConversionI();
  std::string     getFileID();
  MSSpectrumType  getFileType();
  double          getInverseReducedIonMobility();
  float           getIonInjectionTime();
  double          getIonMobilityDriftTime();
  double    			getMonoMZ(int index=0);
  double    			getMZ(int index=0);
  bool            getNativeID(char*,int);
  MSPrecursorInfo getPrecursor(int index=0);
  bool            getRawFilter(char*,int);
  float		    		getRTime();
  float           getRTimeApex();
  std::string     getScanDescription();
  int	      			getScanNumber(bool second=false);
  double          getScanWindowLower();
  double          getScanWindowUpper();
  double          getSelWindowLower();
  double          getSelWindowUpper();
  double    			getSPS(size_t index = 0);
  double          getTIC();
  int             getMsLevel();
  MSUserParam     getUserParam(const size_t& index);
  MSUserParam     getUserParam(const int& index);
  bool            hasIonMobilityArray();
  void            setActivationMethod(MSActivation);
  void            setArea(float);
  void            setBPI(float);
  void            setBPM(double);
  void            setCentroidStatus(int);
  void			    	setCharge(int);
  void            setPositiveScan(bool);
  void            setCompensationVoltage(double);
  void            setConversionA(double);
  void            setConversionB(double);
  void            setConversionC(double);
  void            setConversionD(double);
  void            setConversionE(double);
  void            setConversionI(double);
  void            setFileID(std::string);
  void    				setFileType(MSSpectrumType);
  void            setInverseReducedIonMobility(double);
  void            setIonInjectionTime(float);
  void            setIonMobilityDriftTime(double);
  void		    		setMZ(double, double mono=0);
  void            setNativeID(const char*);
  void            setRawFilter(char*);
  void				    setRTime(float);
  void            setRTimeApex(float);
  void            setScanDescription(std::string);
  void    				setScanNumber(int, bool second=false);
  void            setScanWindow(double lower, double upper); //the mass range of the spectrum
  void            setSelWindow(double lower, double upper); //the mass range of the selected/acquired ions
  void            setTIC(double);
  void            setMsLevel(int level);
  int			      	size();
  int             sizeEZ();
	int							sizeMZ();   //also returns size of monoMZ
  int             sizeSPS();
  int             sizePrecursor();
  int     				sizeZ();
  size_t          sizeUserParams();
  void		    		sortIntensity();
  void				    sortIntensityRev();
  void    				sortMZ();
  void            setPeaks( std::vector<Peak_T> peaks);
  void		    		sortMZRev();

  //for sqlite format
  void setScanID(int scanID);
  int getScanID();

  //const vector<Peak_T>* getPeaks();
  std::vector<Peak_T>* getPeaks();
  //void setPeaks(vector<Peak_T> peaks);
  float getTotalIntensity();
 
  //for debugging
  void printMe();

 protected:

  //Data Members
  std::vector<Peak_T>   *vPeaks;
  std::vector<EZState>  *vEZ;        //extended z-lines with charge state, M+H, and peak information.
  std::vector<double>   *vIonMobility;
  std::vector<ZState>   *vZ;         //presumed charge states and M+H; M can be monoisotopic or selected.
  int		           charge;
  bool                 positiveScan;
  float		         rTime;
  int		           scanNumber;
  int              scanNumber2;
  int              msLevel;
  std::vector<double>   *monoMZ;     //the monoisotopic m/z of the selected ion(s)
  std::vector<double>   *mz;         //the selected ion(s) in m/z
  std::vector<MSPrecursorInfo> *precursor;
  std::vector<double>   *sps;        //SPS masses
  std::string      fileID;
  MSSpectrumType   fileType;
  MSActivation     actMethod;
  int              scanID;       //index for sqlite
  float            IIT;
  float            BPI;          //Base Peak Intensity
  double           compensationVoltage;
  double           convA;
  double           convB;
	double           convC;
	double           convD;
	double           convE;
	double           convI;
  double           inverseReducedIonMobility;
  double           ionMobilityDriftTime;
  double           selectionWinLower;
  double           selectionWinUpper;
  double           TIC;
  double           BPM;             //Base Peak Mass
  float            rTimeApex;       //retention time of precursor apex (MS2)
  float            area;            //summed peak areas of precursor (MS2)
  char             nativeID[256];   //spectrumNativeID in mzML files
  char             rawFilter[256];  //RAW file header line
  int              centroidStatus;  //0=profile, 1=centroid, 2=unknown
  double           scanWinLower;    //the instrument spectrum m/z range
  double           scanWinUpper;    //the instrument spectrum m/z range
  std::string      scanDescription;

  std::vector<MSUserParam> *userParams;

  //private:
  //Functions
  static int compareIntensity(const void *p1,const void *p2);
  static int compareMZ(const void *p1,const void *p2);
  static int compareIntensityRev(const void *p1,const void *p2);
  static int compareMZRev(const void *p1,const void *p2);

};

}
#endif

