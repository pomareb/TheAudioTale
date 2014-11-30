
#include "FMODLoader.h"

FMODLoader::FMODLoader()
{
	std::cout << "FMOD init ..." << std::endl;
	this->checkVersion();
	std::cout << "You are using FMOD " << this->version << ". This program requires " << FMOD_VERSION << std::endl;
	this->checkSoundCard();
	std::cout << "SoundCard test is OK !" << std::endl;
	this->initFMOD();
	std::cout << "Init Finished, ready to play sounds" << std::endl;
};

void FMODLoader::checkVersion()
{
	this->result = FMOD::System_Create(&system);
	FMODErrorCheck(this->result);
	this->result = system->getVersion(&this->version);
	FMODErrorCheck(result);

	if (this->version < FMOD_VERSION)
	{
		std::cout << "Error! You are using an old version of FMOD " << this->version << ". This program requires " << FMOD_VERSION << std::endl;
		//return 0;
	}
};

void FMODLoader::checkSoundCard()
{
	this->result = system->getNumDrivers(&this->numDrivers);
	FMODErrorCheck(this->result);
	if (this->numDrivers == 0)
	{
		this->result = system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		FMODErrorCheck(this->result);
	}
	else
	{
		this->result = system->getDriverCaps(0, &this->caps, 0, &this->speakerMode);
		FMODErrorCheck(this->result);
		this->result = system->setSpeakerMode(this->speakerMode);
		FMODErrorCheck(this->result);
		if (this->caps & FMOD_CAPS_HARDWARE_EMULATED)
		{
			this->result = system->setDSPBufferSize(1024, 10);
			FMODErrorCheck(this->result);
		}
		this->result = system->getDriverInfo(0, this->name, 256, 0);
		FMODErrorCheck(this->result);
		if (strstr(this->name, "SigmaTel"))
		{
			result = system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
			FMODErrorCheck(this->result);
		}
	}
};

void FMODLoader::initFMOD()
{
	this->result = system->init(100, FMOD_INIT_NORMAL, 0);
	if (this->result == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		this->result = system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		FMODErrorCheck(this->result);

		this->result = system->init(100, FMOD_INIT_NORMAL, 0);
	}
	FMODErrorCheck(this->result);
};

void FMODLoader::FMODErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		std::cout << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
		exit(-1);
	}
};

bool FMODLoader::loadSound(char* filename)
{
	system->createSound(filename, FMOD_DEFAULT, 0, &this->audio);
	//system->createStream(filename, FMOD_DEFAULT, 0, &this->audioStream);
	return(true);
};

bool FMODLoader::playSound()
{
	system->playSound(FMOD_CHANNEL_FREE, this->audio, false, NULL);
//	system->playSound(FMOD_CHANNEL_FREE, this->audio, false, &this->channel);
	return(true);
};


FMOD::Channel FMODLoader::getChannel()
{
	return *(this->channel);
};