/*
 * This file provides functions to create driver objects.
 */

#include "global.h"
#include "RageLog.h"
#include "RageUtil.h"
#include "PrefsManager.h"
#include "arch.h"
#include "arch_platform.h"
#include "Foreach.h"

#include "MovieTexture/Selector_MovieTexture.h"
static void DumpAVIDebugInfo(CString);
/* Try drivers in order of preference until we find one that works. */
RageMovieTexture *MakeRageMovieTexture(RageTextureID ID)
{
	DumpAVIDebugInfo( ID.filename );

	CStringArray DriversToTry;
	split(PREFSMAN->GetMovieDrivers(), ",", DriversToTry, true);
	ASSERT(DriversToTry.size() != 0);

	CString Driver;
	RageMovieTexture *ret = NULL;

	for( unsigned i=0; ret==NULL && i<DriversToTry.size(); ++i )
	{
		Driver = DriversToTry[i];
		LOG->Trace("Initializing driver: %s", Driver.c_str());
#ifdef USE_MOVIE_TEXTURE_DSHOW
		if( !Driver.CompareNoCase("DShow") ) ret = new MovieTexture_DShow(ID);
#endif
#ifdef USE_MOVIE_TEXTURE_FFMPEG
		if( !Driver.CompareNoCase("FFMpeg") ) ret = new MovieTexture_FFMpeg(ID);
#endif
#ifdef USE_MOVIE_TEXTURE_NULL
		if( !Driver.CompareNoCase("Null") ) ret = new MovieTexture_Null(ID);
#endif
		if( ret == NULL )
		{
			LOG->Warn( "Unknown movie driver name: %s", Driver.c_str() );
			continue;
		}

		CString sError = ret->Init();
		if( sError != "" )
		{
			LOG->Info( "Couldn't load driver %s: %s", Driver.c_str(), sError.c_str() );
			SAFE_DELETE( ret );
		}
	}
	if (!ret)
		RageException::Throw("Couldn't create a movie texture");

	LOG->Trace("Created movie texture \"%s\" with driver \"%s\"",
		ID.filename.c_str(), Driver.c_str() );
	return ret;
}

#include "Sound/Selector_RageSoundDriver.h"
RageSoundDriver *MakeRageSoundDriver(CString drivers)
{
	CStringArray DriversToTry;
	split(drivers, ",", DriversToTry, true);

	ASSERT( DriversToTry.size() != 0 );

	CString Driver;
	RageSoundDriver *ret = NULL;

	for(unsigned i = 0; ret == NULL && i < DriversToTry.size(); ++i)
	{
		Driver = DriversToTry[i];
		LOG->Trace("Initializing driver: %s", DriversToTry[i].c_str());

#ifdef USE_RAGE_SOUND_ALSA9
		if(!DriversToTry[i].CompareNoCase("ALSA"))		ret = new RageSound_ALSA9;
#endif
#ifdef USE_RAGE_SOUND_ALSA9_SOFTWARE
		if(!DriversToTry[i].CompareNoCase("ALSA-sw"))		ret = new RageSound_ALSA9_Software;
#endif
#ifdef USE_RAGE_SOUND_CA
		if(!DriversToTry[i].CompareNoCase("CoreAudio"))		ret = new RageSound_CA;
#endif
#ifdef USE_RAGE_SOUND_DSOUND
		if(!DriversToTry[i].CompareNoCase("DirectSound"))	ret = new RageSound_DSound;
#endif
#ifdef USE_RAGE_SOUND_DSOUND_SOFTWARE
		if(!DriversToTry[i].CompareNoCase("DirectSound-sw"))	ret = new RageSound_DSound_Software;
#endif
#ifdef USE_RAGE_SOUND_NULL
		if(!DriversToTry[i].CompareNoCase("Null"))		ret = new RageSound_Null;
#endif
#ifdef USE_RAGE_SOUND_OSS
		if(!DriversToTry[i].CompareNoCase("OSS"))		ret = new RageSound_OSS;
#endif
#ifdef USE_RAGE_SOUND_QT1
		if(!DriversToTry[i].CompareNoCase("QT1"))		ret = new RageSound_QT1;
#endif
#ifdef USE_RAGE_SOUND_WAVE_OUT
		if(!DriversToTry[i].CompareNoCase("WaveOut"))		ret = new RageSound_WaveOut;
#endif

		if( ret == NULL )
		{
			LOG->Warn( "Unknown sound driver name: %s", DriversToTry[i].c_str() );
			continue;
		}

		CString sError = ret->Init();
		if( sError != "" )
		{
			LOG->Info( "Couldn't load driver %s: %s", DriversToTry[i].c_str(), sError.c_str() );
			SAFE_DELETE( ret );
		}
	}
	
	if(ret)
		LOG->Info("Sound driver: %s", Driver.c_str());
	
	return ret;
}

// Helper for MakeRageMovieTexture()
static void DumpAVIDebugInfo( CString fn )
{
	CString type, handler;
	if( !RageMovieTexture::GetFourCC( fn, handler, type ) )
		return;

	LOG->Trace("Movie %s has handler '%s', type '%s'", fn.c_str(), handler.c_str(), type.c_str());
}

/*
 * (c) 2002-2005 Glenn Maynard, Ben Anderson
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
