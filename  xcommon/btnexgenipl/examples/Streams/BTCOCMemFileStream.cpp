#include "stdafx.h"
#include "BTCOCMemFileStream.h"

BTCOCMemFileStream::BTCOCMemFileStream(CMemFile& MemFile) : m_MemFile(MemFile)
{
}

BTCOCMemFileStream::~BTCOCMemFileStream()
{
}

int BTCOCMemFileStream::Close()
{
	return 0; // Do not close CMemFile, otherwise it will lost it's content.
}

long BTCOCMemFileStream::Write(const void* pData, long lCount)
{
	m_MemFile.Write( pData, lCount);

	return lCount;	// Normally we will return number of written bytes, but CFile did
					// not return any value, so we have to return lCount. You can catch
					// CFileException to process error messages.
					// Read BTCIOStream.h for more details.
}

long BTCOCMemFileStream::Seek(long lOffset, unsigned int nOrigin)
{
	switch( nOrigin)
	{
		case Begin:		nOrigin = CFile::begin;
						break;

		case Current:	nOrigin = CFile::current;
						break;

		case End:		nOrigin = CFile::end;
						break;
	}

	return m_MemFile.Seek( lOffset, nOrigin);
}

bool BTCOCMemFileStream::IsOpen()
{
	return true; // CMemFile is always opened.
}
