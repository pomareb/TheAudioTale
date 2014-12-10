
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

	// Load song
	enableNormalize = false;
	sampleSize = 64;

	// Set beat detection parameters
	beatThresholdVolume = 0.3f;
	beatThresholdBar = 0;
	beatSustain = 150;
	beatPostIgnore = 500;
	beatTrackCutoff = 10000;
	bpmEstimate = 60;

	spec = new float[sampleSize];

	beatLastTick = 0;
	beatIgnoreLastTick = 0;

	volume = 0;
	frequency = 0;

	maxVol = 0;

	beatNow = false;

	startTime = GetTickCount();
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

bool FMODLoader::loadSound(char* filename, char* filetwo)
{
	FMODErrorCheck(system->createStream(filename, FMOD_CREATESTREAM, 0, &this->audioStream));
	FMODErrorCheck(system->createStream(filename, FMOD_CREATESTREAM, 0, &this->muteStream));
	return(true);
};

bool FMODLoader::playSound()
{
	FMODErrorCheck(system->playSound(FMOD_CHANNEL_FREE, this->audioStream, true, &channel));
	FMODErrorCheck(system->playSound(FMOD_CHANNEL_FREE, this->muteStream, true, &muteChannel));
	muteChannel->setMute(true);
	return(true);
};

void FMODLoader::delayedStartChannels(float time)
{
	muteChannel->setPaused(false);
	Sleep(time);
	channel->setPaused(false);
}

bool FMODLoader::playPauseChannel()
{
	bool isPaused;
	channel->getPaused(&isPaused);
	channel->setPaused(!isPaused);
	muteChannel->setPaused(!isPaused);
	return !isPaused;
}

bool FMODLoader::isBeatNow()
{
	return beatNow;
}

float FMODLoader::getFrequency()
{
	return frequency;
}

float FMODLoader::getVolume()
{
	return volume;
}

float *FMODLoader::getSpec()
{
	return spec;
}

float FMODLoader::getBpmEstimate()
{
	return bpmEstimate;
}

float FMODLoader::getMaxVol()
{
	return maxVol;
}
void FMODLoader::parse()
{
	system->update();

	// Frequency analysis
	float *specLeft, *specRight;
	specLeft = new float[sampleSize];
	specRight = new float[sampleSize];
	
	//getTempo
	channel->getFrequency(&frequency);
	
	//getAmplitude
	muteChannel->getVolume(&volume);

	// Get average spectrum for left and right stereo channels
	muteChannel->getSpectrum(specLeft, sampleSize, 0, FMOD_DSP_FFT_WINDOW_RECT);
	muteChannel->getSpectrum(specRight, sampleSize, 1, FMOD_DSP_FFT_WINDOW_RECT);

	for (int i = 0; i < sampleSize; i++)
		spec[i] = (specLeft[i] + specRight[i]) / 2;

	// Find max volume
	auto maxIterator = std::max_element(&spec[0], &spec[sampleSize]);
	maxVol = *maxIterator;

	// Find frequency range of each array item
	float hzRange = (44100 / 2) / static_cast<float>(sampleSize);

	//detect beat
	if (!enableNormalize)
	{
		if (spec[beatThresholdBar] >= beatThresholdVolume && beatLastTick == 0 && beatIgnoreLastTick == 0)
		{
			beatLastTick = startTime - GetTickCount();
			beatTimes.push(beatLastTick);
			beatNow = true;
			//std::cout << "BEATBEATBEATBEATBEATBEAT!" << std::endl;
			while ((startTime - GetTickCount()) - beatTimes.front() > beatTrackCutoff)
			{
				beatTimes.pop();
				if (beatTimes.size() == 0)
					break;
			}
		}
		else if (beatIgnoreLastTick == 0 && beatLastTick != 0)
		{
			beatNow = false;
			beatLastTick = 0;
			beatIgnoreLastTick = GetTickCount();
		}

		if (GetTickCount() - beatIgnoreLastTick >= beatPostIgnore)
			beatIgnoreLastTick = 0;
	}

	// Predict BPM
	float msPerBeat;

	if (beatTimes.size() >= 2)
	{
		msPerBeat = (beatTimes.back() - beatTimes.front()) / static_cast<float>(beatTimes.size() - 1);
		bpmEstimate = 60000 / msPerBeat;
	}
	else
		bpmEstimate = 0;

	//clear
	delete[] specLeft;
	delete[] specRight;
}

FMOD::Channel FMODLoader::getChannel()
{
	return *(this->channel);
};

FMOD::Channel FMODLoader::getMuteChannel()
{
	return *(this->muteChannel);
};

FMOD::System FMODLoader::getSystem()
{
	return *(this->system);
}