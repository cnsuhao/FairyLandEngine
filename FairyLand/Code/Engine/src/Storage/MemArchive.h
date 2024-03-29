//============================================================================
// MemArchive.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// MemArchive
//----------------------------------------------------------------------------

class MemArchive : public IDataArchive
{
private:
	struct MemResource
	{
		String		mName;
		Oword		mChecksum;
		_dword		mSize;
		_byte*		mBuffer;

		operator StringPtr ( ) const
			{ return mName; }
	};

	typedef Array< MemResource, StringPtr > ResourceArray;

	String			mName;
	String			mPath;

	_byte*			mBaseBuffer;
	_dword			mBaseLength;

	ResourceArray	mResourceArray;

	static _void OnFoundMemResource( StringPtr resname, _dword chunkoffset, _dword chunksize, _dword resoffset, _dword ressize, const Oword& reschecksum, _dword userdata );

	_bool	CreateResourceHelper( StringPtr resname, const _void* buffer, _dword size, const Oword& checksum );
	_bool	DeleteResourceHelper( _dword index );
	_dword	SearchResourceHelper( StringPtr resname );

public:
	MemArchive( );
	~MemArchive( );

	_bool	CreateArchive( StringPtr archivename, StringPtr mountpath );
	_bool	OpenArchive( StringPtr archivename, StringPtr mountpath, _byte* buffer, _dword length );

	virtual _void			Release( );

	virtual StringPtr		GetName( ) const;
	virtual StringPtr		GetPath( ) const;

	virtual _dword			GetArchiveType( ) const;
	virtual _dword			GetResourceNumber( ) const;

	virtual IDataStream*	OpenResource( StringPtr resname );
	virtual _bool			CreateResource( StringPtr resname, const _void* buffer, _dword size, const Oword* checksum, _dword* finish );
	virtual _bool			DeleteResource( StringPtr resname );
	virtual _bool			SearchResource( StringPtr resname, _dword* size, Oword* checksum );

	virtual _dword			EnumArchive( FileSystem::OnFoundFile funcpointer, _void* parameter1, _void* parameter2, _float* percentage );
};

};