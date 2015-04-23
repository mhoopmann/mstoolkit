# MSToolkit

The MSToolkit is a light-weight C++ library for reading, writing, and manipulating mass spectrometry data. The MSToolkit is easily linked to virtually any C++ algorithm for simple, fast file reading and analysis.


### Supported File Formats
  * *mzML* including internal compression (zlib and numpress) and external compression (.mzML.gz) _read-only_
  * *mzXML* including internal compression (zlib) and external compression (.mzXML.gz) _read-only_
  * *mgf* _read-only_
  * *ms1* _read/write_
  * *ms2* _read/write_
  * *bms1* _read/write_
  * *bms2* _read/write_
  * *cms1* _read/write_
  * *cms2* _read/write_
  * *RAW* Thermo proprietary file format (Windows only, requires Xcalibur/MSFileReader) _read-only_


### Simple Interface
  * Open any file format from a single function call.
  * Store any spectrum in a simple, comprehensive data structure.
  * Sequential or random-access file reading.


### Easy Integration
  * All headers included from a single location.
  * Single library file easily linked by the compiler.

