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
#ifndef _MSREADER_H
#define _MSREADER_H

#include "Spectrum.h"
#include "MSObject.h"
#include "mzParser.h"
#include <string>
#include <algorithm>
#include <cstdio>
#include <cstdlib>

//For mzXML Writing
//#include "mzXMLWriter.h"
//#include "MSToolkitInterface.h"

#ifdef _MSC_VER
#ifndef _NO_THERMORAW
#import "libid:F0C5F3E3-4F2A-443E-A74D-0AABE3237494" rename_namespace("XRawfile")
//#import "libid:5FE970A2-29C3-11D3-811D-00104B304896" rename_namespace("XRawfile")
//using namespace XRawfile;
#endif
#endif

//Macros for 64-bit file support
#ifdef _MSC_VER
#ifndef _NO_THERMORAW
#include "RAWReader.h"
#endif
typedef __int64 f_off;
#define fseek(h,p,o) _fseeki64(h,p,o)
#define ftell(h) _ftelli64(h)
#else

#ifndef _LARGEFILE_SOURCE
#error "need to define _LARGEFILE_SOURCE!!"
#endif    /* end _LARGEFILE_SOURCE */

typedef off_t f_off;
#define fseek(h,p,o) fseeko(h,p,o)
#define ftell(h) ftello(h)

#endif /* end _MSC_VER */ 

namespace MSToolkit {
class MSReader {
 public:
  //Constructors & Destructors
  MSReader();
  ~MSReader();

  //Functions
	void addFilter(MSSpectrumType m);
  void appendFile(char* c, bool text, Spectrum& s);
  void appendFile(char* c, bool text, MSObject& m);
  void appendFile(char* c, Spectrum& s);
  void appendFile(char* c, MSObject& m);
  
  MSFileFormat checkFileFormat(const char *fn);

  std::string          getCurrentFile();
  MSSpectrumType  getFileType();
  MSHeader&       getHeader();
  //void            getInstrument(char* str);
  void            getInstrument(std::string& str);
  int             getLastScan();
  //void            getManufacturer(char* str);
  void            getManufacturer(std::string& str);
  int             getPercent();
  

  //get specific header informations
  void setPrecision(int i, int j);
  void setPrecisionInt(int i);
  void setPrecisionMZ(int i);
  void writeFile(const char* c, bool text, MSObject& m);
  void writeFile(const char* c, MSFileFormat ff, MSObject& m, const char* sha1Report=NULL);

  bool readMGFFile(const char* c, Spectrum& s); //Note, no random-access of MGF files.
  bool readMGFFile2(const char* c, Spectrum& s); //Note, no random-access of MGF files.
  bool readMSTFile(const char* c, bool text, Spectrum& s, int scNum=0);
  bool readMZPFile(const char* c, Spectrum& s, int scNum=0);
  bool readFile(const char* c, Spectrum& s, int scNum=0);

  bool nextSpectrum(Spectrum& s);
  bool prevSpectrum(Spectrum& s);
  
  void setFilter(std::vector<MSSpectrumType>& m);
  void setFilter(MSSpectrumType m);

  //For RAW files
  bool lookupRT(char* c, int scanNum, float& rt);
  void setAverageRaw(bool b, int width=1, long cutoff=1000);
  void setLabel(bool b);  //label data contains all centroids (including noise and excluded peaks)
  void setRawFilter(char* c);
  void setRawFilterExact(bool b);

  //For MGF files
  void setHighResMGF(bool b);
  void setOnePlusMGF(bool b);

  //File compression
  void setCompression(bool b);

  //for Sqlite
  void createIndex(); 

 protected:

 private:
  //Data Members
  FILE *fileIn;
  MSHeader header;
  int headerIndex;
  MSSpectrumType fileType;
  f_off lEnd;
  f_off lPivot;
  f_off lFWidth;
  int iIntensityPrecision;
  int iMZPrecision;
  int iVersion;
  int iFType;
  int lastReadScanNum;
  MSFileFormat lastFileFormat;
  std::string sCurrentFile;
  std::string sInstrument;
  std::string sManufacturer;

  //File compression
  bool compressMe;

  //mzXML support variables;
  mzParser::ramp_fileoffset_t  *pScanIndex;
  mzParser::RAMPFILE  *rampFileIn;
  bool rampFileOpen;
  int rampLastScan;
  int rampIndex;
  std::vector<MSSpectrumType> filter;

  //for RAW file support (even if not on windows)
  bool rawFileOpen;

  //for mgf support
  char strMGF[1024];
  bool exportMGF;
  bool highResMGF;
  bool mgfOnePlus;
  int mgfIndex;
  std::vector<int> mgfGlobalCharge;
  std::vector<std::string> mgfFiles;

  //Functions
  void closeFile();
  int openFile(const char* c, bool text=false);
  bool findSpectrum(int i);
  void readCompressSpec(FILE* fileIn, MSScanInfo& ms, Spectrum& s);
  void readSpecHeader(FILE* fileIn, MSScanInfo& ms);

  void writeBinarySpec(FILE* fileOut, Spectrum& s);
  void writeCompressSpec(FILE* fileOut, Spectrum& s);
  void writeTextSpec(FILE* fileOut, Spectrum& s);
  void writeSpecHeader(FILE* fileOut, bool text, Spectrum& s);
  
  //support for rawfiles
  #ifdef _MSC_VER
  #ifndef _NO_THERMORAW
	RAWReader cRAW;
  #endif
  #endif

};

}

#endif

