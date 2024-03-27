//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================

#define __STDC_LIMIT_MACROS
#include <stdint.h>

#include "tier1/utlstring.h"
#include "tier1/strtools.h"
#include <ctype.h>

// NOTE: This has to be the last file included!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Simple string class. 
//-----------------------------------------------------------------------------

CUtlString::operator const char* () const
{
	return Get();
}


CUtlString::CUtlString(const char* pString)
	: m_Storage(NULL)
{
	Set(pString);
}

CUtlString::CUtlString() : m_Storage(NULL)
{
	Set("");
}

CUtlString::CUtlString(const CUtlString& string) : m_Storage(NULL)
{
	Set(string.Get());
}

//-----------------------------------------------------------------------------
// Either allocates or reallocates memory to the length
//
// Allocated space for length characters.  It automatically adds space for the 
// nul and the cached length at the start of the memory block.  Will adjust
// m_Storage and explicitly set the nul at the end before returning.
void *CUtlString::AllocMemory( uint32 length )
{
	m_Storage.SetLength(length+1);
	m_Storage[ length ] = 0;

	return m_Storage.Get();
}

//-----------------------------------------------------------------------------
void CUtlString::SetDirect( const char *pValue, int nChars )
{
	if ( pValue && nChars > 0 )
	{
		if ( pValue == m_Storage.Get() )
		{
			AssertMsg( nChars == Q_strlen((char*)m_Storage.Get()), "CUtlString::SetDirect does not support resizing strings in place." );
			return; // Do nothing. Realloc in AllocMemory might move pValue's location resulting in a bad memcpy.
		}

		Assert( nChars <= Min<int>( strnlen(pValue, nChars) + 1, nChars ) );
		AllocMemory( nChars );
		Q_memcpy(m_Storage.Get(), pValue, nChars );
	}
	else
	{
		Purge();
	}

}


void CUtlString::Set( const char *pValue )
{
	int length = pValue ? V_strlen( pValue ) : 0;
	SetDirect( pValue, length );
}

// Sets the length (used to serialize into the buffer )
void CUtlString::SetLength( int nLen )
{
	if ( nLen > 0 )
	{
#ifdef _DEBUG
		int prevLen = m_Storage.Length();
#endif
		AllocMemory( nLen );
#ifdef _DEBUG
		if ( nLen > prevLen )
		{
			V_memset((char*)m_Storage.Get() + prevLen, 0xEB, nLen - prevLen );
		}
#endif
	}
	else
	{
		Purge();
	}
}

const char* CUtlString::Get() const
{
	return (const char*)m_Storage.Get();
}

char* CUtlString::Get( )
{	
	return (char*)m_Storage.Get();
}

char CUtlString::operator[]( int i ) const
{
	if ( !m_Storage.Get() )
		return '\0';

	if ( i >= Length() )
	{
		return '\0';
	}

	return m_Storage[i];
}

void CUtlString::Clear()
{
	Purge();
}

void CUtlString::Purge()
{
	m_Storage.Purge();
}

bool CUtlString::IsEqual_CaseSensitive( const char *src ) const
{
	if ( !src )
	{
		return (Length() == 0);
	}
	return ( V_strcmp( Get(), src ) == 0 );
}

bool CUtlString::IsEqual_CaseInsensitive( const char *src ) const
{
	if ( !src )
	{
		return (Length() == 0);
	}
	return ( V_stricmp( Get(), src ) == 0 );
}


void CUtlString::ToLower()
{
	if ( !m_Storage.Get() )
	{
		return;
	}

	V_strlower( (char*)m_Storage.Get() );
}

void CUtlString::ToUpper()
{
	if (!m_Storage.Get())
	{
		return;
	}

	V_strupr((char*)m_Storage.Get());
}

CUtlString &CUtlString::operator=( const CUtlString &src )
{
	SetDirect( src.Get(), src.Length() );
	return *this;
}

CUtlString &CUtlString::operator=( const char *src )
{
	Set( src );
	return *this;
}

bool CUtlString::operator==( const CUtlString &src ) const
{
	if ( IsEmpty() )
	{
		if ( src.IsEmpty() )
		{
			return true;
		}

		return false;
	}
	else
	{
		if ( src.IsEmpty() )
		{
			return false;
		}

		return Q_strcmp((char*)m_Storage.Get(), (char*)src.m_Storage.Get() ) == 0;
	}
}

bool CUtlString::operator==(const char* src) const
{
	return Q_strcmp(src, (char*)m_Storage.Get());
}

CUtlString &CUtlString::operator+=( const CUtlString &rhs )
{
	const int lhsLength( Length() );
	const int rhsLength( rhs.Length() );

	if (!rhsLength)
	{
		return *this;
	}

	const int requestedLength( lhsLength + rhsLength );

	
	AllocMemory( requestedLength );
	Q_memcpy((char*)m_Storage.Get() + lhsLength, rhs.m_Storage.Get(), rhsLength);

	return *this;
}

CUtlString &CUtlString::operator+=( const char *rhs )
{
	const int lhsLength( Length() );
	const int rhsLength( V_strlen( rhs ) );
	const int requestedLength( lhsLength + rhsLength );

	if (!requestedLength)
	{
		return *this;
	}

	AllocMemory( requestedLength );
	Q_memcpy((char*)m_Storage.Get() + lhsLength, rhs, rhsLength);

	return *this;
}

CUtlString &CUtlString::operator+=( char c )
{
	const int lhsLength( Length() );

	AllocMemory( lhsLength + 1 );
	m_Storage[ lhsLength ] = c;

	return *this;
}

CUtlString &CUtlString::operator+=( int rhs )
{
	Assert( sizeof( rhs ) == 4 );

	char tmpBuf[ 12 ];	// Sufficient for a signed 32 bit integer [ -2147483648 to +2147483647 ]
	V_snprintf( tmpBuf, sizeof( tmpBuf ), "%d", rhs );
	tmpBuf[ sizeof( tmpBuf ) - 1 ] = '\0';

	return operator+=( tmpBuf );
}

CUtlString &CUtlString::operator+=( double rhs )
{
	char tmpBuf[ 256 ];	// How big can doubles be???  Dunno.
	V_snprintf( tmpBuf, sizeof( tmpBuf ), "%lg", rhs );
	tmpBuf[ sizeof( tmpBuf ) - 1 ] = '\0';

	return operator+=( tmpBuf );
}

bool CUtlString::MatchesPattern( const CUtlString &Pattern, int nFlags ) const
{
	const char *pszSource = String();
	const char *pszPattern = Pattern.String();
	bool	bExact = true;

	while( 1 )
	{
		if ( ( *pszPattern ) == 0 )
		{
			return ( (*pszSource ) == 0 );
		}

		if ( ( *pszPattern ) == '*' )
		{
			pszPattern++;

			if ( ( *pszPattern ) == 0 )
			{
				return true;
			}

			bExact = false;
			continue;
		}

		int nLength = 0;

		while( ( *pszPattern ) != '*' && ( *pszPattern ) != 0 )
		{
			nLength++;
			pszPattern++;
		}

		while( 1 )
		{
			const char *pszStartPattern = pszPattern - nLength;
			const char *pszSearch = pszSource;

			for( int i = 0; i < nLength; i++, pszSearch++, pszStartPattern++ )
			{
				if ( ( *pszSearch ) == 0 )
				{
					return false;
				}

				if ( ( *pszSearch ) != ( *pszStartPattern ) )
				{
					break;
				}
			}

			if ( pszSearch - pszSource == nLength )
			{
				break;
			}

			if ( bExact == true )
			{
				return false;
			}

			if ( ( nFlags & PATTERN_DIRECTORY ) != 0 )
			{
				if ( ( *pszPattern ) != '/' && ( *pszSource ) == '/' )
				{
					return false;
				}
			}

			pszSource++;
		}

		pszSource += nLength;
	}
}


int CUtlString::Format( const char *pFormat, ... )
{
	va_list marker;

	va_start( marker, pFormat );
	int len = FormatV( pFormat, marker );
	va_end( marker );

	return len;
}

//--------------------------------------------------------------------------------------------------
// This can be called from functions that take varargs.
//--------------------------------------------------------------------------------------------------

int CUtlString::FormatV( const char *pFormat, va_list marker )
{
	char tmpBuf[ 4096 ];	//< Nice big 4k buffer, as much memory as my first computer had, a Radio Shack Color Computer

	//va_start( marker, pFormat );
	int len = V_vsprintf_safe( tmpBuf, pFormat, marker );
	//va_end( marker );
	Set( tmpBuf );
	return len;
}

//-----------------------------------------------------------------------------
// Strips the trailing slash
//-----------------------------------------------------------------------------
void CUtlString::StripTrailingSlash()
{
	if ( IsEmpty() )
		return;

	int nLastChar = Length() - 1;
	char c = m_Storage[ nLastChar ];
	if ( c == '\\' || c == '/' )
	{
		SetLength( nLastChar );
	}
}

void CUtlString::FixSlashes( char cSeparator/*=CORRECT_PATH_SEPARATOR*/ )
{
	if (m_Storage.Get())
	{
		V_FixSlashes((char*)m_Storage.Get(), cSeparator);
	}
}

//-----------------------------------------------------------------------------
// Trim functions
//-----------------------------------------------------------------------------
void CUtlString::TrimLeft( char cTarget )
{
	int nIndex = 0;

	if ( IsEmpty() )
	{
		return;
	}

	while(m_Storage[nIndex] == cTarget )
	{
		++nIndex;
	}

	// We have some whitespace to remove
	if ( nIndex > 0 )
	{
		memcpy( m_Storage.Get(), &m_Storage[nIndex], Length() - nIndex);
		SetLength( Length() - nIndex );
	}
}


void CUtlString::TrimLeft( const char *szTargets )
{
	int i;

	if ( IsEmpty() )
	{
		return;
	}

	for( i = 0; m_Storage[i] != 0; i++ )
	{
		bool bWhitespace = false;

		for( int j = 0; szTargets[j] != 0; j++ )
		{
			if (m_Storage[i] == szTargets[j] )
			{
				bWhitespace = true;
				break;
			}
		}

		if ( !bWhitespace )
		{
			break;
		}
	}

	// We have some whitespace to remove
	if ( i > 0 )
	{
		memcpy(m_Storage.Get(), &m_Storage[i], Length() - i);
		SetLength( Length() - i );
	}
}


void CUtlString::TrimRight( char cTarget )
{
	const int nLastCharIndex = Length() - 1;
	int nIndex = nLastCharIndex;

	while ( nIndex >= 0 && m_Storage[nIndex] == cTarget )
	{
		--nIndex;
	}

	// We have some whitespace to remove
	if ( nIndex < nLastCharIndex )
	{
		m_Storage[nIndex + 1] = 0;
		SetLength( nIndex + 2 );
	}
}


void CUtlString::TrimRight( const char *szTargets )
{
	const int nLastCharIndex = Length() - 1;
	int i;

	for( i = nLastCharIndex; i > 0; i-- )
	{
		bool bWhitespace = false;

		for( int j = 0; szTargets[j] != 0; j++ )
		{
			if (m_Storage[i] == szTargets[j] )
			{
				bWhitespace = true;
				break;
			}
		}

		if ( !bWhitespace )
		{
			break;
		}
	}

	// We have some whitespace to remove
	if ( i < nLastCharIndex )
	{
		m_Storage[i + 1] = 0;
		SetLength( i + 2 );
	}
}


void CUtlString::Trim( char cTarget )
{
	TrimLeft( cTarget );
	TrimRight( cTarget );
}


void CUtlString::Trim( const char *szTargets )
{
	TrimLeft( szTargets );
	TrimRight( szTargets );
}


CUtlString CUtlString::Slice( int32 nStart, int32 nEnd ) const
{
	int length = Length();
	if ( length == 0 )
	{
		return CUtlString();
	}

	if ( nStart < 0 )
		nStart = length - (-nStart % length);
	else if ( nStart >= length )
		nStart = length;

	if ( nEnd == INT32_MAX )
		nEnd = length;
	else if ( nEnd < 0 )
		nEnd = length - (-nEnd % length);
	else if ( nEnd >= length )
		nEnd = length;
	
	if ( nStart >= nEnd )
		return CUtlString();

	const char *pIn = String();

	CUtlString ret;
	ret.SetDirect( pIn + nStart, nEnd - nStart );
	return ret;
}

// Grab a substring starting from the left or the right side.
CUtlString CUtlString::Left( int32 nChars ) const
{
	return Slice( 0, nChars );
}

CUtlString CUtlString::Right( int32 nChars ) const
{
	return Slice( -nChars );
}

CUtlString CUtlString::Replace( char cFrom, char cTo ) const
{
	if (!m_Storage.Get())
	{
		return CUtlString();
	}

	CUtlString ret = *this;
	int len = ret.Length();
	for ( int i=0; i < len; i++ )
	{
		if ( ret.m_Storage[i] == cFrom )
			ret.m_Storage[i] = cTo;
	}

	return ret;
}

CUtlString CUtlString::Replace( const char *pszFrom, const char *pszTo ) const
{
	Assert( pszTo ); // Can be 0 length, but not null
	Assert( pszFrom && *pszFrom ); // Must be valid and have one character.
	
	
	const char *pos = V_strstr( String(), pszFrom );
	if ( !pos )
	{
		return *this;
	}

	const char *pFirstFound = pos;

	// count number of search string
	int nSearchCount = 0;
	int nSearchLength = V_strlen( pszFrom );
	while ( pos )
	{
		nSearchCount++;
		int nSrcOffset = ( pos - String() ) + nSearchLength;
		pos = V_strstr( String() + nSrcOffset, pszFrom );
	}

	// allocate the new string
	int nReplaceLength = V_strlen( pszTo );
	int nAllocOffset = nSearchCount * ( nReplaceLength - nSearchLength );
	size_t srcLength = Length();
	CUtlString strDest;
	size_t destLength = srcLength + nAllocOffset;
	strDest.SetLength( destLength );

	// find and replace the search string
	pos = pFirstFound;
	int nDestOffset = 0;
	int nSrcOffset = 0;
	while ( pos )
	{
		// Found an instance
		int nCurrentSearchOffset = pos - String();
		int nCopyLength = nCurrentSearchOffset - nSrcOffset;
		V_strncpy( strDest.Get() + nDestOffset, String() + nSrcOffset, nCopyLength + 1 );
		nDestOffset += nCopyLength;
		V_strncpy( strDest.Get() + nDestOffset, pszTo, nReplaceLength + 1 );
		nDestOffset += nReplaceLength;

		nSrcOffset = nCurrentSearchOffset + nSearchLength;
		pos = V_strstr( String() + nSrcOffset, pszFrom );
	}

	// making sure that the left over string from the source is the same size as the left over dest buffer
	Assert( destLength - nDestOffset == srcLength - nSrcOffset );
	if ( destLength - nDestOffset > 0 )
	{
		V_strncpy( strDest.Get() + nDestOffset, String() + nSrcOffset, destLength - nDestOffset + 1 );
	}

	return strDest;
}

CUtlString CUtlString::AbsPath( const char *pStartingDir ) const
{
	char szNew[MAX_PATH];
	V_MakeAbsolutePath( szNew, sizeof( szNew ), this->String(), pStartingDir );
	return CUtlString( szNew );
}

CUtlString CUtlString::UnqualifiedFilename() const
{
	const char *pFilename = V_UnqualifiedFileName( this->String() );
	return CUtlString( pFilename );
}

CUtlString CUtlString::DirName() const
{
	CUtlString ret( this->String() );
	V_StripLastDir( (char*)ret.Get(), ret.Length() + 1 );
	V_StripTrailingSlash( (char*)ret.Get() );
	return ret;
}

CUtlString CUtlString::StripExtension() const
{
	char szTemp[MAX_PATH];
	V_StripExtension( String(), szTemp, sizeof( szTemp ) );
	return CUtlString( szTemp );
}

CUtlString CUtlString::StripFilename() const
{
	const char *pFilename = V_UnqualifiedFileName( Get() ); // NOTE: returns 'Get()' on failure, never NULL
	int nCharsToCopy = pFilename - Get();
	CUtlString result;
	result.SetDirect( Get(), nCharsToCopy );
	result.StripTrailingSlash();
	return result;
}

CUtlString CUtlString::GetBaseFilename() const
{
	char szTemp[MAX_PATH];
	V_FileBase( String(), szTemp, sizeof( szTemp ) );
	return CUtlString( szTemp );
}

CUtlString CUtlString::GetExtension() const
{
	char szTemp[MAX_PATH];
	V_ExtractFileExtension( String(), szTemp, sizeof( szTemp ) );
	return CUtlString( szTemp );
}


CUtlString CUtlString::PathJoin( const char *pStr1, const char *pStr2 )
{
	char szPath[MAX_PATH];
	V_ComposeFileName( pStr1, pStr2, szPath, sizeof( szPath ) );
	return CUtlString( szPath );
}

CUtlString CUtlString::operator+( const char *pOther ) const
{
	CUtlString s = *this;
	s += pOther;
	return s;
}

CUtlString CUtlString::operator+( const CUtlString &other ) const
{
	CUtlString s = *this;
	s += other;
	return s;
}

CUtlString CUtlString::operator+( int rhs ) const
{
	CUtlString ret = *this;
	ret += rhs;
	return ret;
}

//-----------------------------------------------------------------------------
// Purpose: concatenate the provided string to our current content
//-----------------------------------------------------------------------------
void CUtlString::Append( const char *pchAddition )
{
	(*this) += pchAddition;
}

void CUtlString::Append( const char *pchAddition, int nChars )
{
	nChars = Min<int>( nChars, V_strlen(	pchAddition ) );

	const int lhsLength( Length() );
	const int rhsLength( nChars );
	const int requestedLength( lhsLength + rhsLength );

	AllocMemory( requestedLength );
	const int allocatedLength( requestedLength );
	const int copyLength( allocatedLength - lhsLength < rhsLength ? allocatedLength - lhsLength : rhsLength );
	memcpy( Get() + lhsLength, pchAddition, copyLength );
	m_Storage[ allocatedLength ] = '\0';
}

// Shared static empty string.
const CUtlString &CUtlString::GetEmptyString()
{
	static const CUtlString s_emptyString;

	return s_emptyString;
}
