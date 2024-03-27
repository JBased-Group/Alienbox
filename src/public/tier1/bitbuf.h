//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//===========================================================================//

// NOTE: old_bf_read is guaranteed to return zeros if it overflows.

#ifndef BITBUF_H
#define BITBUF_H

#ifdef _WIN32
#pragma once
#endif


#include "mathlib/mathlib.h"
#include "mathlib/vector.h"
#include "basetypes.h"
#include "tier0/dbg.h"

#if _DEBUG
#define BITBUF_INLINE inline
#else
#define BITBUF_INLINE FORCEINLINE
#endif



//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------

class Vector;
class QAngle;

//-----------------------------------------------------------------------------
// You can define a handler function that will be called in case of 
// out-of-range values and overruns here.
//
// NOTE: the handler is only called in debug mode.
//
// Call SetBitBufErrorHandler to install a handler.
//-----------------------------------------------------------------------------

typedef enum
{
	BITBUFERROR_VALUE_OUT_OF_RANGE=0,		// Tried to write a value with too few bits.
	BITBUFERROR_BUFFER_OVERRUN,				// Was about to overrun a buffer.

	BITBUFERROR_NUM_ERRORS
} BitBufErrorType;


typedef void (*BitBufErrorHandler)( BitBufErrorType errorType, const char *pDebugName );

namespace bitbuf
{
	// ZigZag Transform:  Encodes signed integers so that they can be
	// effectively used with varint encoding.
	//
	// varint operates on unsigned integers, encoding smaller numbers into
	// fewer bytes.  If you try to use it on a signed integer, it will treat
	// this number as a very large unsigned integer, which means that even
	// small signed numbers like -1 will take the maximum number of bytes
	// (10) to encode.  ZigZagEncode() maps signed integers to unsigned
	// in such a way that those with a small absolute value will have smaller
	// encoded values, making them appropriate for encoding using varint.
	//
	//       int32 ->     uint32
	// -------------------------
	//           0 ->          0
	//          -1 ->          1
	//           1 ->          2
	//          -2 ->          3
	//         ... ->        ...
	//  2147483647 -> 4294967294
	// -2147483648 -> 4294967295
	//
	//        >> encode >>
	//        << decode <<

	inline uint32 ZigZagEncode32(int32 n)
	{
		// Note:  the right-shift must be arithmetic
		return(n << 1) ^ (n >> 31);
	}

	inline int32 ZigZagDecode32(uint32 n)
	{
		return(n >> 1) ^ -static_cast<int32>(n & 1);
	}

	inline uint64 ZigZagEncode64(int64 n)
	{
		// Note:  the right-shift must be arithmetic
		return(n << 1) ^ (n >> 63);
	}

	inline int64 ZigZagDecode64(uint64 n)
	{
		return(n >> 1) ^ -static_cast<int64>(n & 1);
	}

	const int kMaxVarintBytes = 10;
	const int kMaxVarint32Bytes = 5;
}



#if defined( _DEBUG )
	extern void InternalBitBufErrorHandler( BitBufErrorType errorType, const char *pDebugName );
	#define CallErrorHandler( errorType, pDebugName ) InternalBitBufErrorHandler( errorType, pDebugName );
#else
	#define CallErrorHandler( errorType, pDebugName )
#endif


// Use this to install the error handler. Call with NULL to uninstall your error handler.
void SetBitBufErrorHandler( BitBufErrorHandler fn );


//-----------------------------------------------------------------------------
// Helpers.
//-----------------------------------------------------------------------------

inline int BitByte( int bits )
{
	// return PAD_NUMBER( bits, 8 ) >> 3;
	return (bits + 7) >> 3;
}

//-----------------------------------------------------------------------------
enum EBitCoordType
{
	kCW_None,
	kCW_LowPrecision,
	kCW_Integral
};

//-----------------------------------------------------------------------------
// Used for serialization
//-----------------------------------------------------------------------------

class bf_write
{
public:
	bf_write();
					
					// nMaxBits can be used as the number of bits in the buffer. 
					// It must be <= nBytes*8. If you leave it at -1, then it's set to nBytes * 8.
	bf_write( void *pData, int nBytes, int nMaxBits = -1 );
	bf_write( const char *pDebugName, void *pData, int nBytes, int nMaxBits = -1 );

	// Start writing to the specified buffer.
	// nMaxBits can be used as the number of bits in the buffer. 
	// It must be <= nBytes*8. If you leave it at -1, then it's set to nBytes * 8.
	void			StartWriting( void *pData, int nBytes, int iStartBit = 0, int nMaxBits = -1 );

	// Restart buffer writing.
	void			Reset();

	// Get the base pointer.
	unsigned char*	GetBasePointer() { return m_pData; }

	// Enable or disable assertion on overflow. 99% of the time, it's a bug that we need to catch,
	// but there may be the occasional buffer that is allowed to overflow gracefully.
	void			SetAssertOnOverflow( bool bAssert );

	// This can be set to assign a name that gets output if the buffer overflows.
	const char*		GetDebugName();
	void			SetDebugName( const char *pDebugName );


// Seek to a specific position.
public:
	
	void			SeekToBit( int bitPos );


// Bit functions.
public:

	void			WriteOneBit(int nValue);
	void			WriteOneBitNoCheck(int nValue);
	void			WriteOneBitAt( int iBit, int nValue );
	
	// Write signed or unsigned. Range is only checked in debug.
	void			WriteUBitLong( unsigned int data, int numbits, bool bCheckRange=true );
	void			WriteSBitLong( int data, int numbits );
	
	// Tell it whether or not the data is unsigned. If it's signed,
	// cast to unsigned before passing in (it will cast back inside).
	void			WriteBitLong(unsigned int data, int numbits, bool bSigned);

	// Write a list of bits in.
	bool			WriteBits(const void *pIn, int nBits);

	// writes an unsigned integer with variable bit length
	void			WriteUBitVar( unsigned int data );

	// Copy the bits straight out of pIn. This seeks pIn forward by nBits.
	// Returns an error if this buffer or the read buffer overflows.
	bool			WriteBitsFromBuffer( class bf_read *pIn, int nBits );
	
	void			WriteBitAngle( float fAngle, int numbits );
	void			WriteBitCoord (const float f);
	void			WriteBitCoordMP( const float f, EBitCoordType coordType );
	void 			WriteBitCellCoord( const float f, int bits, EBitCoordType coordType );
	void			WriteBitFloat(float val);
	void			WriteBitVec3Coord( const Vector& fa );
	void			WriteBitNormal( float f );
	void			WriteBitVec3Normal( const Vector& fa );
	void			WriteBitAngles( const QAngle& fa );


// Byte functions.
public:

	void			WriteChar(int val);
	void			WriteByte( unsigned int val );
	void			WriteShort(int val);
	void			WriteWord( unsigned int val );
	void			WriteLong(long val);
	void			WriteLongLong(int64 val);
	void			WriteFloat(float val);
	bool			WriteBytes( const void *pBuf, int nBytes );

	// Returns false if it overflows the buffer.
	bool			WriteString(const char *pStr);
	bool			WriteString(const wchar_t *pStr);


// Status.
public:

	// How many bytes are filled in?
	int				GetNumBytesWritten();
	int				GetNumBitsWritten();
	int				GetMaxNumBits();
	int				GetNumBitsLeft();
	int				GetNumBytesLeft();
	unsigned char*	GetData();

	// Has the buffer overflowed?
	bool			CheckForOverflow(int nBits);
	inline bool		IsOverflowed() const {return m_bOverflow;}

	inline void		SetOverflowFlag();


public:
	// The current buffer.
	unsigned char*	m_pData;
	int				m_nDataBytes;
	int				m_nDataBits;
	
	// Where we are in the buffer.
	int				m_iCurBit;
	
private:

	// Errors?
	bool			m_bOverflow;

	bool			m_bAssertOnOverflow;
	const char		*m_pDebugName;
};


//-----------------------------------------------------------------------------
// Inlined methods
//-----------------------------------------------------------------------------

// How many bytes are filled in?
inline int bf_write::GetNumBytesWritten()	
{
	return BitByte(m_iCurBit);
}

inline int bf_write::GetNumBitsWritten()	
{
	return m_iCurBit;
}

inline int bf_write::GetMaxNumBits()		
{
	return m_nDataBits;
}

inline int bf_write::GetNumBitsLeft()	
{
	return m_nDataBits - m_iCurBit;
}

inline int bf_write::GetNumBytesLeft()	
{
	return GetNumBitsLeft() >> 3;
}

inline unsigned char* bf_write::GetData()			
{
	return m_pData;
}

inline bool bf_write::CheckForOverflow(int nBits)
{
	if ( m_iCurBit + nBits > m_nDataBits )
	{
		SetOverflowFlag();
		CallErrorHandler( BITBUFERROR_BUFFER_OVERRUN, GetDebugName() );
	}
	
	return m_bOverflow;
}

inline void bf_write::SetOverflowFlag()
{
	if ( m_bAssertOnOverflow )
	{
		Assert( false );
	}

	m_bOverflow = true;
}

inline void bf_write::WriteOneBitNoCheck(int nValue)
{
	if(nValue)
		m_pData[m_iCurBit >> 3] |= (1 << (m_iCurBit & 7));
	else
		m_pData[m_iCurBit >> 3] &= ~(1 << (m_iCurBit & 7));

	++m_iCurBit;
}

inline void bf_write::WriteOneBit(int nValue)
{
	if( !CheckForOverflow(1) )
		WriteOneBitNoCheck( nValue );
}


inline void	bf_write::WriteOneBitAt( int iBit, int nValue )
{
	if( iBit+1 > m_nDataBits )
	{
		SetOverflowFlag();
		CallErrorHandler( BITBUFERROR_BUFFER_OVERRUN, GetDebugName() );
		return;
	}

	if( nValue )
		m_pData[iBit >> 3] |= (1 << (iBit & 7));
	else
		m_pData[iBit >> 3] &= ~(1 << (iBit & 7));
}


inline void bf_write::WriteUBitLong( unsigned int curData, int numbits, bool bCheckRange )
{
#ifdef _DEBUG
	// Make sure it doesn't overflow.
	if ( bCheckRange && numbits < 32 )
	{
		if ( curData >= (unsigned long)(1 << numbits) )
		{
			CallErrorHandler( BITBUFERROR_VALUE_OUT_OF_RANGE, GetDebugName() );
		}
	}
	Assert( numbits >= 0 && numbits <= 32 );
#endif

	extern unsigned long g_BitWriteMasks[32][33];

	// Bounds checking..
	if ((m_iCurBit+numbits) > m_nDataBits)
	{
		m_iCurBit = m_nDataBits;
		SetOverflowFlag();
		CallErrorHandler( BITBUFERROR_BUFFER_OVERRUN, GetDebugName() );
		return;
	}

	int nBitsLeft = numbits;
	int iCurBit = m_iCurBit;

	// Mask in a dword.
	unsigned int iDWord = iCurBit >> 5;
	Assert( (iDWord*4 + sizeof(long)) <= (unsigned int)m_nDataBytes );

	unsigned long iCurBitMasked = iCurBit & 31;

	uint32 dword = LoadLittleDWord( (uint32*)m_pData, iDWord );

	dword &= g_BitWriteMasks[iCurBitMasked][nBitsLeft];
	dword |= curData << iCurBitMasked;

	// write to stream (lsb to msb ) properly
	StoreLittleDWord( (uint32*)m_pData, iDWord, dword );

	// Did it span a dword?
	int nBitsWritten = 32 - iCurBitMasked;
	if ( nBitsWritten < nBitsLeft )
	{
		nBitsLeft -= nBitsWritten;
		curData >>= nBitsWritten;

		// read from stream (lsb to msb) properly 
		dword = LoadLittleDWord( (uint32*)m_pData, iDWord+1 );

		dword &= g_BitWriteMasks[0][nBitsLeft];
		dword |= curData;

		// write to stream (lsb to msb) properly 
		StoreLittleDWord( (uint32*)m_pData, iDWord+1, dword );
	}

	m_iCurBit += numbits;
}


//-----------------------------------------------------------------------------
// This is useful if you just want a buffer to write into on the stack.
//-----------------------------------------------------------------------------

template<int SIZE>
class old_bf_write_static : public bf_write
{
public:
	inline old_bf_write_static() : bf_write(m_StaticData, SIZE) {}

	char	m_StaticData[SIZE];
};




class CBitBuffer
{
public:
	char const * m_pDebugName;
	bool m_bOverflow;
	int m_nDataBits;
	size_t m_nDataBytes;

	void SetDebugName( char const *pName )
	{
		m_pDebugName = pName;
	}

	CBitBuffer( void )
	{
		m_bOverflow = false;
		m_pDebugName = NULL;
		m_nDataBits = -1;
		m_nDataBytes = 0;
	}

	FORCEINLINE void SetOverflowFlag( void )
	{
		m_bOverflow = true;
	}

	FORCEINLINE bool IsOverflowed( void ) const
	{
		return m_bOverflow;
	}

	static const uint32 s_nMaskTable[33];							// 0 1 3 7 15 ..

};

class CBitWrite : public CBitBuffer
{
	uint32 m_nOutBufWord;
	int m_nOutBitsAvail;
	uint32 *m_pDataOut;
	uint32 *m_pBufferEnd;
	uint32 *m_pData;
	bool m_bFlushed;

public:
	void StartWriting( void *pData, int nBytes, int iStartBit = 0, int nMaxBits = -1 );


	CBitWrite( void *pData, int nBytes, int nBits = -1 )
	{
		m_bFlushed = false;
		StartWriting( pData, nBytes, 0, nBits );
	}
	
	CBitWrite( const char *pDebugName, void *pData, int nBytes, int nBits = -1 )
	{
		m_bFlushed = false;
		SetDebugName( pDebugName );
		StartWriting( pData, nBytes, 0, nBits );
	}

	CBitWrite( void )
	{
		m_bFlushed = false;
	}

	~CBitWrite( void )
	{
		TempFlush();
		Assert( (! m_pData ) || m_bFlushed );
	}
	FORCEINLINE int GetNumBitsLeft( void ) const
	{
		return m_nOutBitsAvail + ( 32 * ( m_pBufferEnd - m_pDataOut -1 ) );
	}

	FORCEINLINE void Reset( void )
	{
		m_bOverflow = false;
		m_nOutBitsAvail = 32;
		m_pDataOut = m_pData;
		m_nOutBufWord = 0;
		
	}

	FORCEINLINE void TempFlush( void )
	{
		// someone wants to know how much data we have written, or the pointer to it, so we'd better make
		// sure we write our data
		if ( m_nOutBitsAvail != 32 )
		{
			if ( m_pDataOut == m_pBufferEnd )
			{
				SetOverflowFlag();
			}
			else
			{
				StoreLittleDWord( m_pDataOut, 0, LoadLittleDWord(m_pDataOut,0) & ~s_nMaskTable[ 32 - m_nOutBitsAvail ] | m_nOutBufWord );
			}
		}
		m_bFlushed = true;
	}

	FORCEINLINE unsigned char *GetBasePointer()
	{
		TempFlush();
		return reinterpret_cast< unsigned char *>( m_pData );
	}

	FORCEINLINE unsigned char *GetData()
	{
		return GetBasePointer();
	}

	FORCEINLINE void Finish();
	FORCEINLINE void Flush();
	FORCEINLINE void FlushNoCheck();
	FORCEINLINE void WriteOneBit(int nValue);
	FORCEINLINE void WriteOneBitNoCheck(int nValue);
	FORCEINLINE void WriteUBitLong( unsigned int data, int numbits, bool bCheckRange=true );
	FORCEINLINE void WriteSBitLong( int data, int numbits );
	FORCEINLINE void WriteUBitVar( unsigned int data );
	FORCEINLINE void WriteBitFloat( float flValue );
	FORCEINLINE void WriteFloat( float flValue );
	bool WriteBits(const void *pInData, int nBits);
	void WriteBytes( const void *pBuf, int nBytes );
	void SeekToBit( int nSeekPos );

	FORCEINLINE int GetNumBitsWritten( void ) const
	{
		return ( 32 - m_nOutBitsAvail ) + ( 32 * ( m_pDataOut - m_pData ) );
	}

	FORCEINLINE int GetNumBytesWritten( void ) const
	{
		return ( GetNumBitsWritten() + 7 ) >> 3;
	}


	FORCEINLINE void WriteLong(long val)
	{
		WriteSBitLong( val, 32 );
	}



	FORCEINLINE void WriteChar( int val )
	{
		WriteSBitLong(val, sizeof(char) << 3 );
	}

	FORCEINLINE void WriteByte( int val )
	{
		WriteUBitLong(val, sizeof(unsigned char) << 3, false );
	}

	FORCEINLINE void WriteShort(int val)
	{
		WriteSBitLong(val, sizeof(short) << 3);
	}

	FORCEINLINE void WriteWord(int val)
	{
		WriteUBitLong(val, sizeof(unsigned short) << 3);
	}
	
	bool WriteString( const char *pStr );
	bool WriteString( const wchar_t *pStr );

	void WriteLongLong( int64 val );

	void WriteBitAngle( float fAngle, int numbits );
	void WriteBitCoord (const float f);
	void WriteBitCoordMP( const float f, EBitCoordType coordType );
	void WriteBitCellCoord( const float f, int bits, EBitCoordType coordType );
	void WriteBitVec3Coord( const Vector& fa );
	void WriteBitNormal( float f );
	void WriteBitVec3Normal( const Vector& fa );
	void WriteBitAngles( const QAngle& fa );

	// Copy the bits straight out of pIn. This seeks pIn forward by nBits.
	// Returns an error if this buffer or the read buffer overflows.
	bool WriteBitsFromBuffer( class bf_read *pIn, int nBits );
	
};

void CBitWrite::Finish( void )
{
	if ( m_nOutBitsAvail != 32 )
	{
		if ( m_pDataOut == m_pBufferEnd )
		{
			SetOverflowFlag();
		}
		StoreLittleDWord( m_pDataOut, 0, m_nOutBufWord );
	}
}

void CBitWrite::FlushNoCheck( void )
{
	StoreLittleDWord( m_pDataOut++, 0, m_nOutBufWord );
	m_nOutBitsAvail = 32;
	m_nOutBufWord = 0;										// ugh - I need this because of 32 bit writes. a<<=32 is a nop
	
}
void CBitWrite::Flush( void )
{
	if ( m_pDataOut == m_pBufferEnd )
	{
		SetOverflowFlag();
	}
	else
	{
		StoreLittleDWord( m_pDataOut++, 0, m_nOutBufWord );
	}
	m_nOutBufWord = 0;										// ugh - I need this because of 32 bit writes. a<<=32 is a nop
	m_nOutBitsAvail = 32;
	
}
void CBitWrite::WriteOneBitNoCheck( int nValue )
{
	m_nOutBufWord |= ( nValue & 1 ) << ( 32 - m_nOutBitsAvail );
	if ( --m_nOutBitsAvail == 0 )
	{
		FlushNoCheck();
	}
}

void CBitWrite::WriteOneBit( int nValue )
{
	m_nOutBufWord |= ( nValue & 1 ) << ( 32 - m_nOutBitsAvail );
	if ( --m_nOutBitsAvail == 0 )
	{
		Flush();
	}
}

FORCEINLINE void CBitWrite::WriteUBitLong( unsigned int nData, int nNumBits, bool bCheckRange )
{

#ifdef _DEBUG
	// Make sure it doesn't overflow.
	if ( bCheckRange && nNumBits < 32 )
	{
		Assert( nData <= (unsigned long)(1 << nNumBits ) );
	}
	Assert( nNumBits >= 0 && nNumBits <= 32 );
#endif
	if ( nNumBits <= m_nOutBitsAvail )
	{
		if ( bCheckRange )
			m_nOutBufWord |= ( nData ) << ( 32 - m_nOutBitsAvail );
		else
			m_nOutBufWord |= ( nData & s_nMaskTable[ nNumBits] ) << ( 32 - m_nOutBitsAvail );
		m_nOutBitsAvail -= nNumBits;
		if ( m_nOutBitsAvail == 0 )
		{
			Flush();
		}
	}
	else
	{
		// split dwords case
		int nOverflowBits = ( nNumBits - m_nOutBitsAvail );
		m_nOutBufWord |= ( nData & s_nMaskTable[m_nOutBitsAvail] ) << ( 32 - m_nOutBitsAvail );
		Flush();
		m_nOutBufWord = ( nData >> ( nNumBits - nOverflowBits ) );
		m_nOutBitsAvail = 32 - nOverflowBits;
	}
}

FORCEINLINE void CBitWrite::WriteSBitLong( int nData, int nNumBits )
{
	WriteUBitLong( ( uint32 ) nData, nNumBits, false );
}

FORCEINLINE void CBitWrite::WriteUBitVar( unsigned int n )
{
	if ( n < 16 )
		WriteUBitLong( n, 6 );
	else
		if ( n < 256 )
			WriteUBitLong( ( n & 15 ) | 16 | ( ( n & ( 128 | 64 | 32 | 16 ) ) << 2 ), 10 );
		else
			if ( n < 4096 )
				WriteUBitLong( ( n & 15 ) | 32 | ( ( n & ( 2048 | 1024 | 512 | 256 | 128 | 64 | 32 | 16 ) ) << 2 ), 14 );
			else
			{
				WriteUBitLong( ( n & 15 ) | 48, 6 );
				WriteUBitLong( ( n >> 4 ), 32 - 4 );
			}
}

FORCEINLINE void CBitWrite::WriteBitFloat( float flValue )
{
	WriteUBitLong( *((uint32 *) &flValue ), 32 );
}

FORCEINLINE void CBitWrite::WriteFloat( float flValue )
{
	// Pre-swap the float, since WriteBits writes raw data
	LittleFloat( &flValue, &flValue );
	WriteUBitLong( *((uint32 *) &flValue ), 32 );
}

//-----------------------------------------------------------------------------
// Used for unserialization
//-----------------------------------------------------------------------------

class bf_read
{
public:
	bf_read();

	// nMaxBits can be used as the number of bits in the buffer. 
	// It must be <= nBytes*8. If you leave it at -1, then it's set to nBytes * 8.
	bf_read(const void* pData, int nBytes, int nBits = -1);
	bf_read(const char* pDebugName, const void* pData, int nBytes, int nBits = -1);

	// Start reading from the specified buffer.
	// pData's start address must be dword-aligned.
	// nMaxBits can be used as the number of bits in the buffer. 
	// It must be <= nBytes*8. If you leave it at -1, then it's set to nBytes * 8.
	void			StartReading(const void* pData, int nBytes, int iStartBit = 0, int nBits = -1);

	// Restart buffer reading.
	void			Reset();

	// Enable or disable assertion on overflow. 99% of the time, it's a bug that we need to catch,
	// but there may be the occasional buffer that is allowed to overflow gracefully.
	void			SetAssertOnOverflow(bool bAssert);

	// This can be set to assign a name that gets output if the buffer overflows.
	const char* GetDebugName() const { return m_pDebugName; }
	void			SetDebugName(const char* pName);

	void			ExciseBits(int startbit, int bitstoremove);


	// Bit functions.
public:

	// Returns 0 or 1.
	int				ReadOneBit();


protected:

	unsigned int	CheckReadUBitLong(int numbits);		// For debugging.
	int				ReadOneBitNoCheck();				// Faster version, doesn't check bounds and is inlined.
	bool			CheckForOverflow(int nBits);


public:

	// Get the base pointer.
	const unsigned char* GetBasePointer() { return reinterpret_cast<unsigned char const*>(m_pData); }

	BITBUF_INLINE int TotalBytesAvailable(void) const
	{
		return m_nDataBytes;
	}

	// Read a list of bits in.
	void            ReadBits(void* pOut, int nBits);
	// Read a list of bits in, but don't overrun the destination buffer.
	// Returns the number of bits read into the buffer. The remaining
	// bits are skipped over.
	int             ReadBitsClamped_ptr(void* pOut, size_t outSizeBytes, size_t nBits);
	// Helper 'safe' template function that infers the size of the destination
	// array. This version of the function should be preferred.
	// Usage: char databuffer[100];
	//        ReadBitsClamped( dataBuffer, msg->m_nLength );
	template <typename T, size_t N>
	int             ReadBitsClamped(T(&pOut)[N], size_t nBits)
	{
		return ReadBitsClamped_ptr(pOut, N * sizeof(T), nBits);
	}

	float			ReadBitAngle(int numbits);

	unsigned int	ReadUBitLong(int numbits) RESTRICT;
	unsigned int	ReadUBitLongNoInline(int numbits) RESTRICT;
	unsigned int	PeekUBitLong(int numbits);
	int				ReadSBitLong(int numbits);

	// reads an unsigned integer with variable bit length
	unsigned int	ReadUBitVar();
	unsigned int	ReadUBitVarInternal(int encodingType);

	// reads a varint encoded integer
	uint32			ReadVarInt32();
	uint64			ReadVarInt64();
	int32			ReadSignedVarInt32();
	int64			ReadSignedVarInt64();

	// You can read signed or unsigned data with this, just cast to 
	// a signed int if necessary.
	unsigned int	ReadBitLong(int numbits, bool bSigned);

	float			ReadBitCoord();
	float			ReadBitCoordMP(bool bIntegral, bool bLowPrecision);
	float			ReadBitFloat();
	float			ReadBitNormal();
	void			ReadBitVec3Coord(Vector& fa);
	void			ReadBitVec3Normal(Vector& fa);
	void			ReadBitAngles(QAngle& fa);

	// Faster for comparisons but do not fully decode float values
	unsigned int	ReadBitCoordBits();
	unsigned int	ReadBitCoordMPBits(bool bIntegral, bool bLowPrecision);

	// Byte functions (these still read data in bit-by-bit).
public:

	BITBUF_INLINE int	ReadChar() { return (char)ReadUBitLong(8); }
	BITBUF_INLINE int	ReadByte() { return ReadUBitLong(8); }
	BITBUF_INLINE int	ReadShort() { return (short)ReadUBitLong(16); }
	BITBUF_INLINE int	ReadWord() { return ReadUBitLong(16); }
	BITBUF_INLINE long ReadLong() { return ReadUBitLong(32); }
	int64			ReadLongLong();
	float			ReadFloat();
	bool			ReadBytes(void* pOut, int nBytes);

	// Returns false if bufLen isn't large enough to hold the
	// string in the buffer.
	//
	// Always reads to the end of the string (so you can read the
	// next piece of data waiting).
	//
	// If bLine is true, it stops when it reaches a '\n' or a null-terminator.
	//
	// pStr is always null-terminated (unless bufLen is 0).
	//
	// pOutNumChars is set to the number of characters left in pStr when the routine is 
	// complete (this will never exceed bufLen-1).
	//
	bool			ReadString(char* pStr, int bufLen, bool bLine = false, int* pOutNumChars = NULL);

	// Reads a string and allocates memory for it. If the string in the buffer
	// is > 2048 bytes, then pOverflow is set to true (if it's not NULL).
	char* ReadAndAllocateString(bool* pOverflow = 0);

	// Returns nonzero if any bits differ
	int				CompareBits(bf_read* RESTRICT other, int bits) RESTRICT;
	int				CompareBitsAt(int offset, bf_read* RESTRICT other, int otherOffset, int bits) RESTRICT;

	// Status.
public:
	int				GetNumBytesLeft();
	int				GetNumBytesRead();
	int				GetNumBitsLeft();
	int				GetNumBitsRead() const;
	inline void            GrabNextDWord(bool bOverFlowImmediately);

	// Has the buffer overflowed?
	inline bool		IsOverflowed() const { return m_bOverflow; }

	inline bool		Seek(int iBit);					// Seek to a specific bit.
	inline bool		SeekRelative(int iBitDelta);	// Seek to an offset from the current position.

	// Called when the buffer is overflowed.
	void			SetOverflowFlag();


public:

	char const* m_pDebugName;
	bool m_bOverflow;
	int m_nDataBits;
	size_t m_nDataBytes;

	uint32 m_nInBufWord;
	int m_nBitsAvail;
	uint32 const* m_pDataIn;
	uint32 const* m_pBufferEnd;
	uint32 const* m_pData;

	static const uint32 s_nMaskTable[33];							// 0 1 3 7 15 ..

};

//-----------------------------------------------------------------------------
// Inlines.
//-----------------------------------------------------------------------------

inline int bf_read::GetNumBytesRead()
{
	return BitByte(GetNumBitsRead());
}

inline int bf_read::GetNumBitsLeft()
{
	return m_nDataBits - GetNumBitsRead();
}

inline int bf_read::GetNumBytesLeft()
{
	return GetNumBitsLeft() >> 3;
}

inline int bf_read::GetNumBitsRead() const
{
	if (!m_pData)									   // pesky null ptr bitbufs. these happen.
		return 0;
	int nCurOfs = (32 - m_nBitsAvail) + (8 * sizeof(m_pData[0]) * (m_pDataIn - m_pData - 1));
	int nAdjust = 8 * (m_nDataBytes & 3);
	return MIN(nCurOfs + nAdjust, m_nDataBits);
}

inline bool bf_read::Seek(int param_2)

{
	bool bVar1;
	uint32* pUVar2;
	int iVar3;
	const uint32* pUVar4;
	uint uVar5;
	uint uVar6;

	bVar1 = true;
	if (((int)param_2 < 0) || (this->m_nDataBits < (int)param_2)) {
		param_2 = this->m_nDataBits;
		this->m_bOverflow = true;
		bVar1 = false;
	}
	uVar6 = this->m_nDataBytes & 3;
	if ((this->m_nDataBytes < 4) ||
		((uVar6 != 0 && ((int)(param_2 + ((int)param_2 >> 0x1f & 7U)) >> 3 < (int)uVar6)))) {
		pUVar4 = this->m_pData;
		if (pUVar4 != (uint32*)0x0) {
			this->m_nInBufWord = (uint) * (byte*)pUVar4;
			pUVar2 = (uint32*)((int)pUVar4 + 1);
			if (1 < uVar6) {
				this->m_nInBufWord = this->m_nInBufWord | (uint) * (byte*)(uint32*)((int)pUVar4 + 1) << 8;
				pUVar2 = (uint32*)((int)pUVar4 + 2);
			}
			pUVar4 = pUVar2;
			if (2 < uVar6) {
				this->m_nInBufWord = this->m_nInBufWord | (uint) * (byte*)pUVar4 << 0x10;
				pUVar4 = (uint32*)((int)pUVar4 + 1);
			}
		}
		this->m_pDataIn = pUVar4;
		this->m_nInBufWord = this->m_nInBufWord >> (char)(param_2 & 0x1f);
		iVar3 = uVar6 * 8 - (param_2 & 0x1f);
		goto LAB_102584d9;
	}
	uVar5 = param_2 + uVar6 * -8;
	pUVar4 = (uint32*)
		((int)this->m_pData + uVar6 + ((int)(uVar5 + ((int)uVar5 >> 0x1f & 0x1fU)) >> 5) * 4);
	this->m_pDataIn = pUVar4;
	if (this->m_pData == (uint32*)0x0) {
	LAB_10258454:
		this->m_nBitsAvail = 1;
	LAB_1025845b:
		this->m_nInBufWord = 0;
	}
	else {
		this->m_nBitsAvail = 0x20;
		if (pUVar4 == this->m_pBufferEnd) {
			this->m_pDataIn = pUVar4 + 1;
			goto LAB_10258454;
		}
		if (this->m_pBufferEnd < pUVar4) {
			this->m_bOverflow = true;
			goto LAB_1025845b;
		}
		this->m_nInBufWord = *pUVar4;
		this->m_pDataIn = pUVar4 + 1;
	}
	this->m_nInBufWord = this->m_nInBufWord >> (char)(uVar5 & 0x1f);
	iVar3 = 0x20 - (uVar5 & 0x1f);
	if (this->m_nBitsAvail < iVar3) {
		this->m_nBitsAvail = this->m_nBitsAvail;
		return bVar1;
	}
LAB_102584d9:
	this->m_nBitsAvail = iVar3;
	return bVar1;
}


// Seek to an offset from the current position.
inline bool	bf_read::SeekRelative(int iBitDelta)
{
	return Seek(GetNumBitsRead() + iBitDelta);
}

inline bool bf_read::CheckForOverflow(int nBits)
{
	if (GetNumBitsRead() + nBits > m_nDataBits)
	{
		SetOverflowFlag();
		CallErrorHandler(BITBUFERROR_BUFFER_OVERRUN, GetDebugName());
	}

	return m_bOverflow;
}

inline int bf_read::ReadOneBitNoCheck()
{
	unsigned int value = ((unsigned long* RESTRICT)m_pData)[GetNumBitsRead() >> 5] >> (GetNumBitsRead() & 31);
	SeekRelative(1);
	return value & 1;
}

inline int bf_read::ReadOneBit()
{
	if (GetNumBitsLeft() <= 0)
	{
		SetOverflowFlag();
		CallErrorHandler(BITBUFERROR_BUFFER_OVERRUN, GetDebugName());
		return 0;
	}
	return ReadOneBitNoCheck();
}

inline float bf_read::ReadBitFloat()
{
	union { uint32 u; float f; } c = { ReadUBitLong(32) };
	return c.f;
}

BITBUF_INLINE unsigned int bf_read::ReadUBitVar()
{
	// six bits: low 2 bits for encoding + first 4 bits of value
	unsigned int sixbits = ReadUBitLong(6);
	unsigned int encoding = sixbits & 3;
	if (encoding)
	{
		// this function will seek back four bits and read the full value
		return ReadUBitVarInternal(encoding);
	}
	return sixbits >> 2;
}

BITBUF_INLINE void bf_read::GrabNextDWord(bool bOverFlowImmediately)
{
	if (m_pDataIn == m_pBufferEnd)
	{
		m_nBitsAvail = 1;									// so that next read will run out of words
		m_nInBufWord = 0;
		m_pDataIn++;										// so seek count increments like old
		if (bOverFlowImmediately)
			SetOverflowFlag();
	}
	else
		if (m_pDataIn > m_pBufferEnd)
		{
			SetOverflowFlag();
			m_nInBufWord = 0;
		}
		else
		{
			Assert(reinterpret_cast<int>(m_pDataIn) + 3 < reinterpret_cast<int>(m_pBufferEnd));
			m_nInBufWord = LittleDWord(*(m_pDataIn++));
		}
}

BITBUF_INLINE unsigned int bf_read::ReadUBitLong(int numbits)
{
	if (m_nBitsAvail >= numbits)
	{
		unsigned int nRet = m_nInBufWord & s_nMaskTable[numbits];
		m_nBitsAvail -= numbits;
		if (m_nBitsAvail)
		{
			m_nInBufWord >>= numbits;
		}
		else
		{
			m_nBitsAvail = 32;
			GrabNextDWord(false);
		}
		return nRet;
	}
	else
	{
		// need to merge words
		unsigned int nRet = m_nInBufWord;
		numbits -= m_nBitsAvail;
		GrabNextDWord(true);
		if (m_bOverflow)
			return 0;
		nRet |= ((m_nInBufWord & s_nMaskTable[numbits]) << m_nBitsAvail);
		m_nBitsAvail = 32 - numbits;
		m_nInBufWord >>= numbits;
		return nRet;
	}
}

BITBUF_INLINE int bf_read::CompareBits(bf_read* RESTRICT other, int numbits) RESTRICT
{
	return (ReadUBitLong(numbits) != other->ReadUBitLong(numbits));
}

#endif



