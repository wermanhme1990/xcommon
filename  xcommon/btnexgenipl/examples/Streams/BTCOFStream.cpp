#include "stdafx.h"
#include "BTCOFStream.h"

BTCOFStream::BTCOFStream()
{
}

BTCOFStream::~BTCOFStream()
{
	(void)Close();
}

bool BTCOFStream::Open(LPCTSTR lpszFileName /* = NULL */)
{
	if(!lpszFileName)
		return false;

	// Try to open/create a file for output.
	return m_File.Open( lpszFileName, CFile::modeCreate | CFile::modeWrite);
}

int BTCOFStream::Close()
{
	if( IsOpen())
		m_File.Close();

	return 0;
}

long BTCOFStream::Write(const void* pData, long lCount)
{
	if(!IsOpen())
		return -1;

	m_File.Write( pData, lCount);

	return lCount;	// Normally we will return number of written bytes, but CFile did
					// not return any value, so we have to return lCount. You can catch
					// CFileException to process error messages.
					// Read BTCIOStream.h for more details.
}

long BTCOFStream::Seek(long lOffset, unsigned int nOrigin)
{
	if(!IsOpen())
		return -1;

	switch( nOrigin)
	{
		case Begin:		nOrigin = CFile::begin;
						break;

		case Current:	nOrigin = CFile::current;
						break;

		case End:		nOrigin = CFile::end;
						break;
	}

	return m_File.Seek( lOffset, nOrigin);
}

bool BTCOFStream::IsOpen()
{
	return m_File.m_hFile != CFile::hFileNull;
}
