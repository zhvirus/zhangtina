#pragma once

#include "ZHRenderDecl.h"
#include "fmod//fmod.h"
#include "fmod//fmod_errors.h"
#include "fmod//fmod.hpp"



namespace ZH
{
	//线程函数类型
	typedef unsigned (WINAPI *THREAD_FUNC)(LPVOID lpThreadParameter);
	typedef unsigned* THREAD_ID;

	class ZH_DLL SoundStruct
	{
		public:
			TCHAR* soundName;
			FMOD::Channel* channel;
			unsigned int _len;
			unsigned int _lastTime;
		public:	
			SoundStruct()
			{
				soundName=NULL;
				channel=NULL;
				_lastTime=0;
				_len	=	0;
			}
			SoundStruct(TCHAR* inStr,FMOD::Channel* chIn)
			{
				soundName	=	inStr;
				channel	=	chIn;
			}
			~SoundStruct()
			{
				SAFE_DELETE(soundName);

			}
	};

class ZH_DLL Music
{




	typedef std::map<SoundStruct*,FMOD::Sound*> SOUNDLIST;
	typedef std::map<SoundStruct*,FMOD::Sound*>::iterator	SOUNDLIST_ITERATOR;
public:
	Music(void);
	~Music(void);

public:
	bool addToPlayList(TCHAR* fileName);
	bool playWholeList();
	bool playMusic(TCHAR* fileName,bool loop,bool single=false,float volume=0.5f);
	inline size_t getMusicListSize(){return _soundListSize;} 


private:
	bool _ifExist(TCHAR* fileName,SOUNDLIST& listIn);
	static DWORD WINAPI ThreadFunc(LPVOID param);
private:
	size_t _soundListSize;
	FMOD::System *_system;
	FMOD::Sound	*_sound;
	FMOD::Channel	*_channel;
	SOUNDLIST _soundList;
	SOUNDLIST _recycle;
	SOUNDLIST_ITERATOR _iterator;


};


}