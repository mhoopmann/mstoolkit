# MSToolkit

The MSToolkit is a light-weight C++ library for reading, writing, and manipulating mass spectrometry data. The MSToolkit is easily linked to virtually any C++ algorithm for simple, fast file reading and analysis.

### Version 84.0.1, September 28 2023

### Supported File Formats
  * *mzML* including internal compression (zlib and numpress) and external compression (.mzML.gz) _read/write\*_
  * *mzMLb* _read-only_ (Requires HDF5)
  * *mz5* _read-only_ (Requires HDF5)
  * *mzXML* including internal compression (zlib) and external compression (.mzXML.gz) _read-only_
  * *mzID* _read/write_ (a.k.a. mzIdentML)
  * *mgf* _read/write_
  * *ms1* _read/write_
  * *ms2* _read/write_
  * *bms1* _read/write_
  * *bms2* _read/write_
  * *cms1* _read/write_
  * *cms2* _read/write_
  * *RAW* Thermo proprietary file format (Windows only, requires Xcalibur/MSFileReader) _read-only_
  
  _\* Note: .mzML writing produces funtional files, but currently does not export all meta data. Spectral peak data is complete. .mzML.gz files
  are not produced by the toolkit, and must be gzipped externally._
  _\* Note: .mzID reading and writing is in active development to support all the weird unexpected stuff people do with that format._


### Simple Interface
  * Open any file format from a single function call.
  * Store any spectrum in a simple, comprehensive data structure.
  * Sequential or random-access file reading.


### Easy Integration
  * All headers included from a single location.
  * Single library file easily linked by the compiler.

### Compiling
A few hints:
 * On GNU/Linux, try "make all" to build the library. On VS/Windows, you'll have to build your own solution.
 * To support .mzMLb and .mz5, HDF5 must be installed. Enable these formats by opening the Makefile and setting HDF5 and HDF5_DIR at the top of the file.
 * Add _NOSQLITE to build smaller library without SQLite. SQLite is only required for special case usage.
 * Add _NO_THERMORAW to build without Thermo file support in Windows. This is not necessary in GNU/Linux, where Thermo support is disabled by default.
 * Older versions of MSVC may require building with XML_STATIC declared.
 * Declaring WIN32 may still be required for compiling 64-bit libraries with MSVC.
 * MAKEFILE.nmake is for MSVC builds without creating your own solution file. Use the x64 Native Tools Command Prompt in VS (tested on VS2019) and type nmake /f MAKEFILE.nmake all
 * CMakelists.txt is provided for CMake builds on Linux and Windows.
	* On Windows, use the x64 Native Tools Command Prompt in VS (tested on VS2022) and type `cmake -S . -B build && cmake --build build --config Release`
	* On Linux, use the cmake and type `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build`
### License
Code written for the MSToolkit uses the Apache License, Version 2.0. All 3rd party software included in the MSToolkit library retains its original license.
