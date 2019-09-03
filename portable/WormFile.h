// WormFile.h compressed WORM file using Snappy or Brotli
// Copyright 2017/3/14 Robin.Rowe@CinePaint.org
// License open source MIT
// File extension: 2017_Q1_mail.wrm

#ifndef WormFile_h
#define WormFile_h

#include "StdFile.h"

namespace portable
{

class WormFile
{	StdFile ioFile;
	unsigned offset;
	const char* fileSig;
    const char* blockSig;
	bool WriteFileHeader();
	bool ReadFileHeader();
	bool WriteBlockHeader(unsigned size, unsigned crc = 0);
	unsigned ReadBlockHeader();
public:
	enum LastError 
	{	NONE,
		FILE_NOT_FOUND,
		WRITE_ERROR,
		SEEK_ERROR,
		READ_ERROR,
        INVALID_FORMAT,
        BLOCK_ERROR
	};
	LastError lastError;
    WormFile()
	{	lastError=NONE;
		offset=0;
		fileSig="WRM:None";
        blockSig = "WRM:";
	}
	bool Open(const char* filename)
	{	if(!ioFile.Open(filename,"ab+"))
		{	lastError = FILE_NOT_FOUND;
			return false;
		}
		return true;
	}
	unsigned Write(const char* data,size_t length);// returns blob offset
	unsigned Seek(unsigned offset);// returns blob size
	bool Read(char* buffer,size_t len);// uncompressed data
	void Close()
	{	ioFile.Close();
	}
};

}

#endif