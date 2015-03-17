# MSToolkit #

The MSToolkit is a light-weight C++ library for reading, writing, and manipulating mass spectrometry data. The MSToolkit is easily linked to virtually any C++ algorithm for simple, fast file reading and analysis.


### Supported File Formats ###
  1. **mzML** including internal compression (zlib and numpress) and external compression (.mzML.gz) _read-only_
  1. **mzXML** including internal compression (zlib) and external compression (.mzXML.gz) _read-only_
  1. **mgf** _read-only_
  1. **ms1** _read/write_
  1. **ms2** _read/write_
  1. **bms1** _read/write_
  1. **bms2** _read/write_
  1. **cms1** _read/write_
  1. **cms2** _read/write_
  1. **RAW** Thermo proprietary file format (Windows only, requires Xcalibur/MSFileReader) _read-only_


### Simple Interface ###
  * Open any file format from a single function call.
  * Store any spectrum in a simple, comprehensive data structure.
  * Sequential or random-access file reading.


### Easy Integration ###
  * All headers included from a single location.
  * Single library file easily linked by the compiler.
