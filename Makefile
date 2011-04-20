#Set your paths here. Ignoring SQLite for now.
#No need to compile zlib. However, run "make buildlib" in expat prior to running this.
ZLIB_PATH = ../zlib
MZPARSER_PATH = ./mzParser
EXPAT_PATH = ../expat-2.0.1/lib
#SQLITE_PATH = ../sqlite3

MZPARSER = mzp_base64.o BasicSpectrum.o mzParser.o RAMPface.o saxhandler.o saxmzmlhandler.o saxmzxmlhandler.o zran.o
EXPAT = xmlparse.o xmlrole.o xmltok.o
ZLIB = adler32.o compress.o crc32.o deflate.o inffast.o inflate.o infback.o inftrees.o trees.o uncompr.o zutil.o
MSTOOLKIT = Spectrum.o MSObject.o
#READER = MSReader.o
READERLITE = MSReaderLite.o
#MSMAT = msmat.o msmat_base.o msmat_header_parser.o LazyMatrix.o crawutils.o
#SQLITE = sqlite3.o 

CC = g++
GCC = gcc
#MSMAT_FLAGS=-DOLD_BOOST
NOSQLITE = -D_NOSQLITE

CFLAGS = -O3 -static -I. -I$(ZLIB_PATH) -I$(MZPARSER_PATH) -I$(EXPAT_PATH) -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -DGCC
LIBS = -lm -lpthread -ldl

all:  $(ZLIB) $(MZPARSER) $(MSTOOLKIT) $(READERLITE) 
	ar rcs libmstoolkitlite.a $(ZLIB) $(EXPAT:xml%.o=$(EXPAT_PATH)/xml%.o) $(MZPARSER) $(MSTOOLKIT) $(READERLITE)
#	$(CC) $(CFLAGS) MSTDemo.cpp -L. -lmstoolkitlite -o MSTDemo
#	$(CC) $(CFLAGS) MSSingleScan.cpp -L. -lmstoolkitlite -o MSSingleScan
#	$(CC) $(CFLAGS) MSConvertFile.cpp -L. -lmstoolkitlite -o MSConvertFile

clean:
	rm -f *.o libmstoolkitlite.a

# zLib objects

adler32.o : $(ZLIB_PATH)/adler32.c
	$(GCC) $(CFLAGS) $(ZLIB_PATH)/adler32.c -c

compress.o : $(ZLIB_PATH)/compress.c
	$(GCC) $(CFLAGS) $(ZLIB_PATH)/compress.c -c

crc32.o : $(ZLIB_PATH)/crc32.c
	$(GCC) $(CFLAGS) $(ZLIB_PATH)/crc32.c -c

deflate.o : $(ZLIB_PATH)/deflate.c
	$(GCC) $(CFLAGS) $(ZLIB_PATH)/deflate.c -c

inffast.o : $(ZLIB_PATH)/inffast.c
	$(GCC) $(CFLAGS) $(ZLIB_PATH)/inffast.c -c

inflate.o : $(ZLIB_PATH)/inflate.c
	$(GCC) $(CFLAGS) $(ZLIB_PATH)/inflate.c -c

infback.o : $(ZLIB_PATH)/infback.c
	$(GCC) $(CFLAGS) $(ZLIB_PATH)/infback.c -c

inftrees.o : $(ZLIB_PATH)/inftrees.c
	$(GCC) $(CFLAGS) $(ZLIB_PATH)/inftrees.c -c

trees.o : $(ZLIB_PATH)/trees.c
	$(GCC) $(CFLAGS) $(ZLIB_PATH)/trees.c -c

uncompr.o : $(ZLIB_PATH)/uncompr.c
	$(GCC) $(CFLAGS) $(ZLIB_PATH)/uncompr.c -c

zutil.o : $(ZLIB_PATH)/zutil.c
	$(GCC) $(CFLAGS) $(ZLIB_PATH)/zutil.c -c



#mzParser objects
mzp_base64.o : $(MZPARSER_PATH)/mzp_base64.cpp
	$(CC) $(CFLAGS) $(MZPARSER_PATH)/mzp_base64.cpp -c

BasicSpectrum.o : $(MZPARSER_PATH)/BasicSpectrum.cpp
	$(CC) $(CFLAGS) $(MZPARSER_PATH)/BasicSpectrum.cpp -c

mzParser.o : $(MZPARSER_PATH)/mzParser.cpp
	$(CC) $(CFLAGS) $(MZPARSER_PATH)/mzParser.cpp -c

RAMPface.o : $(MZPARSER_PATH)/RAMPface.cpp
	$(CC) $(CFLAGS) $(MZPARSER_PATH)/RAMPface.cpp -c

saxhandler.o : $(MZPARSER_PATH)/saxhandler.cpp
	$(CC) $(CFLAGS) $(MZPARSER_PATH)/saxhandler.cpp -c

saxmzmlhandler.o : $(MZPARSER_PATH)/saxmzmlhandler.cpp
	$(CC) $(CFLAGS) $(MZPARSER_PATH)/saxmzmlhandler.cpp -c

saxmzxmlhandler.o : $(MZPARSER_PATH)/saxmzxmlhandler.cpp
	$(CC) $(CFLAGS) $(MZPARSER_PATH)/saxmzxmlhandler.cpp -c
	
zran.o : $(MZPARSER_PATH)/zran.cpp
	$(CC) $(CFLAGS) $(MZPARSER_PATH)/zran.cpp -c





#SQLite object
sqlite3.o : ./SQLite/sqlite3.c
	$(GCC) $(CFLAGS) SQLite/sqlite3.c -c




#MSToolkit objects

Spectrum.o : Spectrum.cpp
	$(CC) $(CFLAGS) Spectrum.cpp -c

MSReader.o : MSReader.cpp
	$(CC) $(CFLAGS) MSReader.cpp -c

MSReaderLite.o : MSReader.cpp
	$(CC) $(CFLAGS) $(NOSQLITE) MSReader.cpp -c -o MSReaderLite.o

MSObject.o : MSObject.cpp
	$(CC) $(CFLAGS) MSObject.cpp -c




#MSMAT objects

msmat.o :  ./msmat/msmat.cpp ./msmat/msmat.H
	$(CC) -o $@ $(CFLAGS) -c $<

msmat_base.o : ./msmat/msmat_base.cpp ./msmat/msmat_base.H 
	$(CC) -o $@ $(CFLAGS) -c $<

msmat_header_parser.o : ./msmat/msmat_header_parser.cpp ./msmat/msmat_header_parser.H 
	$(CC) -o $@ $(CFLAGS) -c $<

LazyMatrix.o : ./msmat/LazyMatrix.cpp ./msmat/LazyMatrix.H 
	$(CC) -o $@ $(CFLAGS) -c $<

crawutils.o : ./msmat/crawutils.cpp ./msmat/crawutils.H 
	$(CC) -o $@ $(CFLAGS) -c $<

