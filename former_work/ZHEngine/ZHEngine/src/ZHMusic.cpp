#ifndef _ZH_DLL
#define _ZH_DLL
#endif

#include "ZHMusic.h"


namespace ZH
{





Music::Music(void)
{
	

	_soundListSize	=	0;
	//创建声音系统
	FMOD::System_Create(&_system);

	//初始化
	_system->init(32, FMOD_INIT_NORMAL, 0);


	_recycle.clear();

}
//
bool Music::addToPlayList(TCHAR* fileName)
{


	if(_ifExist(fileName,_soundList))
	{
		return false;
	}
	else
	{
		//创建音乐流
		FMOD::Sound* soundTemp;

		try{
		
		_system->createStream(fileName, FMOD_HARDWARE, 0, &soundTemp);}  //创建声音 
		catch(...)
		{
			
		}

		soundTemp->setMode(FMOD_LOOP_OFF);  //关闭循环  

		SoundStruct* temp	=	new SoundStruct;
		temp->soundName	=	new TCHAR[MAX_PATH];
		strcpy(temp->soundName,fileName);
		//temp->channel	=	soundTemp;


		_soundList.insert(std::make_pair(temp,soundTemp));

		_soundListSize	=	_soundList.size();

		return true;
	}

}


bool Music::playMusic(TCHAR* fileName,bool loop,bool single,float volume)
{
	try{



	FMOD::Sound* soundTemp	=	NULL;
	
	std::map<SoundStruct*,FMOD::Sound*>::iterator ite;

		for(ite=_recycle.begin();ite!=_recycle.end();ite++)
		{
			if((strcmp(((*ite).first)->soundName,fileName))==0)
			{

				soundTemp	=	(*ite).second;
				if(single==true)
				{
					unsigned int curTime	=	static_cast<unsigned int>(timeGetTime());
					if(curTime-((*ite).first)->_lastTime<((*ite).first)->_len+200)
						return true;
					else
					{
											
						((*ite).first)->_lastTime		=	curTime;
					}


				}

				//soundTemp->
			}


		}


	if(soundTemp==NULL)
	{
	
		
		if(	FMOD_OK!=_system->createStream(fileName, FMOD_HARDWARE, 0, &soundTemp))  //创建声音
		{

			return false;
		}

		soundTemp->setMode(FMOD_LOOP_OFF);  //关闭循环  


		if(loop)
			soundTemp->setMode(FMOD_LOOP_NORMAL);
		else
			soundTemp->setMode(FMOD_LOOP_OFF);

		_system->playSound(FMOD_CHANNEL_FREE, soundTemp, false, &_channel);		 
		_channel->setVolume(volume);    //播放声音

		SoundStruct* temp	=	new SoundStruct;
		temp->_lastTime	=	static_cast<unsigned int>(timeGetTime());
		temp->soundName	=	new TCHAR[MAX_PATH];
		strcpy(temp->soundName,fileName);
		temp->channel	=	_channel;
		
		unsigned int llen=0;
		soundTemp->getLength(&llen,FMOD_TIMEUNIT_MS);
		temp->_len	=	llen;

		_recycle.insert(std::make_pair(temp,soundTemp));
		
		return true;

	}

	if(loop)
		soundTemp->setMode(FMOD_LOOP_NORMAL);
	else
		soundTemp->setMode(FMOD_LOOP_OFF);



	_system->playSound(FMOD_CHANNEL_FREE, soundTemp, false, &_channel);		 
	_channel->setVolume(volume);    //播放声音
	
	}
	catch(...)
	{

	}
	
	return true;
}



bool Music::_ifExist(TCHAR* fileName,SOUNDLIST& listIn)
{
	
	std::map<SoundStruct*,FMOD::Sound*>::iterator ite;
	for(ite=listIn.begin();ite!=listIn.end();ite++)
	{
		if(strcmp(((*ite).first)->soundName,fileName)==0)
			return true;


	}

	return false;



}




bool Music::playWholeList()
{

	DWORD id;

	_beginthreadex(
			0,
			0,
			(THREAD_FUNC)(Music::ThreadFunc),
			(LPVOID)this,
			0,
			(THREAD_ID)(&id)
			);

	
	return true;
}


DWORD WINAPI Music::ThreadFunc(LPVOID param)
{
	Music* _object	=	static_cast<Music*>(param);

	SOUNDLIST_ITERATOR _iterator;
	static FMOD::Sound*	soundTemp;

	std::cout<<"==============================="<<std::endl;	
	std::cout<<"Total "<<_object->_soundList.size()<<" in the playList"<<std::endl;
	
	
	int i=0;
	for(_iterator	=	_object->_soundList.begin();_iterator!=_object->_soundList.end();_iterator++)
	{

		std::cout<<i++<<":   "<<((*_iterator).first)->soundName<<std::endl;

	}
	std::cout<<"==============================="<<std::endl<<std::endl<<std::endl;	

	while(true)
	{
		for(_iterator	=_object->_soundList.begin();_iterator!=_object->_soundList.end();_iterator++)
		{
			

			soundTemp	=	(*_iterator).second;
			
			try{
			_object->_system->playSound(FMOD_CHANNEL_FREE,soundTemp, false, &(_object->_channel));		 
			_object->_channel->setVolume(0.5);    //播放声音	  
			}
			catch(...)
			{

			}


			unsigned int len=0;
			soundTemp->getLength(&len,FMOD_TIMEUNIT_MS);
			
			std::cout<<len<<std::endl;
			//std::cout<<"Playing...."<<(*_iterator).first.c_str()<<std::endl;

			Sleep(static_cast<DWORD>(len)+1000);
			

		}
	}
	std::cout<<"Play PlayList End"<<std::endl; 

	
	return 0;
}


Music::~Music(void)
{
	for(_iterator	=	_soundList.begin();_iterator!=_soundList.end();_iterator++)
	{
		if((*_iterator).first!=NULL)
		{
			delete ((*_iterator).first);
			//(*_iterator).first	=	NULL;
		
		}

		(*_iterator).second->release();

	}


	for(_iterator	=	_recycle.begin();_iterator!=_recycle.end();_iterator++)
	{
		if((*_iterator).first!=NULL)
		{
			delete ((*_iterator).first);
			//(*_iterator).first	=	NULL;
		
		}

		(*_iterator).second->release();

	}

	if(_system)
	{	
		_system->close();
		_system->release();
	}

}




}