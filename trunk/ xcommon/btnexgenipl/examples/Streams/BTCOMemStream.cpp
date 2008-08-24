#include "stdafx.h"
#include "BTCOMemStream.h"

#include <malloc.h>

BTCOMemStream::BTCOMemStream() : m_lSize(0), m_ppOrigin(NULL), m_pCurrent(NULL)
{
}

BTCOMemStream::~BTCOMemStream()
{
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

bool BTCOMemStream::Open(unsigned char** ppData)
{
	if( NULL == *ppData)
		*ppData = new unsigned char;

	m_lSize    = 0;
	m_ppOrigin = ppData;
	m_pCurrent = *m_ppOrigin;

	return IsOpen();
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

int BTCOMemStream::Close()
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

long BTCOMemStream::Write(const void* pData, long lCount)
{
	if(!IsOpen())
		return -1;

	long lCurPos = m_pCurrent - *m_ppOrigin;

	// Reallocate memory buffer if writing to the end of the stream.
	if( lCurPos >= m_lSize)
	{
		long lSeekBeyondEOF = lCurPos - m_lSize;

		*m_ppOrigin = (unsigned char*)realloc( *m_ppOrigin, m_lSize + lCount + lSeekBeyondEOF);

		if( NULL == *m_ppOrigin)
			return -1;

		m_lSize = _msize( *m_ppOrigin);
	}

	// Copy new data to stream.
	memcpy( *m_ppOrigin + lCurPos, pData, lCount);

	// Recalculate new current position.
	m_pCurrent = *m_ppOrigin + lCurPos + lCount;

	return lCount;
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

long BTCOMemStream::Seek(long lOffset, unsigned int nOrigin)
{
	if(!IsOpen())
		return -1;

	switch( nOrigin)
	{
		case Begin:		m_pCurrent  = *m_ppOrigin + lOffset;
						break;

		case Current:	m_pCurrent += lOffset;
						break;

		case End:		m_pCurrent  = *m_ppOrigin + m_lSize - lOffset;
						break;
	}

	long lCurPos = m_pCurrent - *m_ppOrigin;

	if( lCurPos < 0)
		lCurPos = -1; // Out of range.

	return lCurPos;
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

bool BTCOMemStream::IsOpen()
{
	if( NULL == m_ppOrigin)
		return false;

	return *m_ppOrigin != NULL;
}
