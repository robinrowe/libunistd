// WormFile.cpp compressed WORM file using Snappy or Brotli
// Copyright 2017/3/14 Robin.Rowe@CinePaint.org
// License open source MIT

#include "WormFile.h"

namespace portable
{

bool WormFile::WriteFileHeader()
{	if(!ioFile.Write(fileSig))
	{	lastError = WRITE_ERROR;
		return false;
	}
	return true;
}

bool WormFile::ReadFileHeader()
{	const unsigned length = 8;
	char buffer[length+1];
	if(!ioFile.Read(buffer,length))
	{	lastError = READ_ERROR;
		return false;
	}
	buffer[length] = 0;
	if(strcmp(buffer,fileSig))
	{	lastError = INVALID_FORMAT;
		return false;
	}
	return true;
}

bool WormFile::WriteBlockHeader(unsigned size,unsigned crc)
{	if(!ioFile.Write(blockSig,4))
	{	lastError = WRITE_ERROR;
		return false;
	}
    if (!ioFile.Write(size))
    {   lastError = WRITE_ERROR;
        return false;
    }
    if(!ioFile.Write(crc))
	{	lastError = WRITE_ERROR;
		return false;
	}
    return true;
}
	
unsigned WormFile::ReadBlockHeader()
{   char buffer[5];
    if (!ioFile.Read(buffer, 4))
    {   lastError = READ_ERROR;
        return false;
    }
    buffer[4]=0;
    if(strcmp(blockSig,buffer))
    {   lastError = BLOCK_ERROR;
        return false;
    }
    unsigned size = 0;
    if (!ioFile.Read(size))
    {   lastError = READ_ERROR;
        return false;
    }
    unsigned crc = 0;
    if (!ioFile.Read(crc))
    {   lastError = READ_ERROR;
        return false;
    }
    return size;
}

unsigned WormFile::Write(const char* data,size_t length)
{	// returns blob offset
	offset = ioFile.Tell();
	if(!offset)
	{	if(!WriteFileHeader())
		{	return 0;
	}	}
	if(!WriteBlockHeader((unsigned)length))
	{	return 0;
	}
	if(!ioFile.Write(data,length))
	{	lastError=WRITE_ERROR;
		return false;
	}
	return offset;
}

unsigned WormFile::Seek(unsigned offset)
{	// returns blob size
	if(!ioFile.Seek(offset))
	{	lastError = SEEK_ERROR;
		return 0;
	}
	return ReadBlockHeader();
}

bool WormFile::Read(char* buffer,size_t len)
{	// uncompressed data
	if(!ioFile.Read(buffer,len))
	{	lastError = READ_ERROR;
		return false;
	}
	return true;
}

}