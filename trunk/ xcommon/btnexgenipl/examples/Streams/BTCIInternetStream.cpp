#include "stdafx.h"
#include "BTCIInternetStream.h"

BTCIInternetStream::BTCIInternetStream() : m_hFile(NULL), m_pBuffer(NULL), m_pBufferPtr(NULL), m_lSize(0)
{
}

BTCIInternetStream::~BTCIInternetStream()
{
	(void)Close();
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

bool BTCIInternetStream::Open(const bt_char *lpszUrl /* = NULL */)
{
	if(!lpszUrl)
		return false;

	HINTERNET hInternetSession = InternetOpen( "Binary Technologies::NexgenIPL", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	if( NULL == hInternetSession)
		return false;

	unsigned long ulContext = 1;

	m_hFile = InternetOpenUrl( hInternetSession, lpszUrl, NULL, 0, 0, ulContext);

	if( NULL == m_hFile)
	{
		InternetCloseHandle( hInternetSession);

		hInternetSession = InternetOpen( "Binary Technologies::NexgenIPL", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_FROM_CACHE);

		if( NULL == hInternetSession)
			return false;

		m_hFile = InternetOpenUrl( hInternetSession, lpszUrl, NULL, 0, 0, ulContext);

		if( NULL == m_hFile)
		{
			InternetCloseHandle( hInternetSession);

			return false;
		}
	}

	unsigned char ucBuffer[INTERNETSTREAM_BUFFSIZE];
	unsigned long ulBytesRead = 0;

	m_lSize   = 0;
	m_pBuffer = (unsigned char *)malloc( 0);

	if( NULL == m_pBuffer)
		return false;

	do
	{
		if(!InternetReadFile( m_hFile, ucBuffer, INTERNETSTREAM_BUFFSIZE, &ulBytesRead))
		{
			InternetCloseHandle( m_hFile);
			InternetCloseHandle( hInternetSession);
		
			return false;
		}

		m_pBuffer = (unsigned char *)realloc( m_pBuffer, m_lSize + ulBytesRead);

		if( NULL == m_pBuffer)
		{
			m_lSize      = 0;
			m_pBufferPtr = NULL;

			InternetCloseHandle( m_hFile);
			InternetCloseHandle( hInternetSession);

			return false;
		}

		memcpy( m_pBuffer + m_lSize, ucBuffer, ulBytesRead);

		m_lSize += ulBytesRead;
	}
	while( ulBytesRead > 0);

	// Insert here error handling (e.g. 404).

	InternetCloseHandle( m_hFile);
	InternetCloseHandle( hInternetSession);

	return IsOpen();
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

int BTCIInternetStream::Close()
{
	if( NULL != m_pBuffer)
	{
		free( m_pBuffer);
		m_pBuffer = NULL;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

long BTCIInternetStream::Read(void* pData, long lCount)
{
	if(!IsOpen())
		return -1;

	// Calculate new number of bytes to copy if we overrun end of file.
	if( GetPosition() + lCount > m_lSize)
	{
		if( lCount > m_lSize)
			lCount = m_lSize;
		else
			lCount = m_lSize - (long)(m_pBufferPtr + lCount);
	}

	if( lCount < 0)
		return -1;

	memcpy( pData, m_pBufferPtr, lCount);

	m_pBufferPtr += lCount;

	return lCount;
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

long BTCIInternetStream::Seek(long lOffset, unsigned int nOrigin)
{
	if(!IsOpen())
		return -1;

	switch( nOrigin)
	{
		case Begin:		m_pBufferPtr = m_pBuffer + lOffset;
						break;

		case Current:	m_pBufferPtr += lOffset;
						break;

		case End:		m_pBufferPtr = m_pBuffer + m_lSize - lOffset;
						break;
	}

	long lCurPos = m_pBufferPtr - m_pBuffer;

	if( lCurPos < 0)
		lCurPos = -1; // Out of range.

	return lCurPos;
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

bool BTCIInternetStream::IsOpen()
{
	return m_pBuffer != NULL;
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

long BTCIInternetStream::GetPosition()
{
	return Seek( 0L, BTCIOStream::Current);
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

long BTCIInternetStream::GetSize()
{
	long lSize, lCurPos;

	// Retrieve current position.
	lCurPos = Seek( 0L, BTCIOStream::Current);

	// Retrieve size of stream.
	lSize = Seek( 0L, BTCIOStream::End);

	// Reposition pointer to origin.
	Seek( lCurPos, BTCIOStream::Begin);

	return lSize;
}
