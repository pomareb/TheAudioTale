
#include "FMODLoader.h"

FMODLoader::FMODLoader()
{
	this->cnt = 0;
	std::cout << "FMOD init ..." << std::endl;
	this->checkVersion();
	std::cout << "You are using FMOD " << this->version << ". This program requires " << FMOD_VERSION << std::endl;
	this->checkSoundCard();
	std::cout << "SoundCard test is OK !" << std::endl;
	this->initFMOD();
	std::cout << "Init Finished, ready to play sounds" << std::endl;

	// Load song
	this->enableNormalize = false;
	this->sampleSize = 64;

	// Set beat detection parameters
	this->beatThresholdVolume = 0.3f;
	this->beatThresholdBar = 0;
	this->beatSustain = 150;
	this->beatPostIgnore = 500;
	this->beatTrackCutoff = 10000;

	this->spec = new float[sampleSize];

	this->beatLastTick = 0;
	this->beatIgnoreLastTick = 0;

	this->frequency = 0;
	this->volume = 0;

	this->startTime = GetTickCount();
	
	this->isItBeat = false;
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
	FMODErrorCheck(this->system->createStream(filename, FMOD_CREATESTREAM, 0, &this->audioStream));
	FMODErrorCheck(this->system->createStream(filename, FMOD_CREATESTREAM, 0, &this->muteStream));
	return(true);
};

bool FMODLoader::playSound()
{
	FMODErrorCheck(this->system->playSound(FMOD_CHANNEL_FREE, this->audioStream, true, &channel));
	FMODErrorCheck(this->system->playSound(FMOD_CHANNEL_FREE, this->muteStream, true, &muteChannel));
	this->muteChannel->setMute(true);
	return(true);
};

bool FMODLoader::playMuteChannel()
{
	bool isPaused;
	this->muteChannel->getPaused(&isPaused);
	this->muteChannel->setPaused(!isPaused);
	return !isPaused;
}

bool FMODLoader::playPauseChannels()
{
	bool isPaused;
	this->channel->getPaused(&isPaused);
	this->channel->setPaused(!isPaused);
	this->muteChannel->setPaused(!isPaused);
	return !isPaused;
}

bool FMODLoader::getIsItABeat()
{
	return this->isItBeat;
}

float FMODLoader::getFrequency()
{
	return *this->frequency;
}

float FMODLoader::getVolume()
{
	return *this->volume;
}

float FMODLoader::getBPM()
{
	return this->bpmEstimate;
}

float* FMODLoader::getSpec()
{
	return this->spec;
}

void FMODLoader::parse()
{
	this->system->update();
	// Frequency analysis
	float *specLeft, *specRight;
	specLeft = new float[this->sampleSize];
	specRight = new float[this->sampleSize];
	//getTempo
	this->muteChannel->getFrequency(this->frequency);

	//getAmplitude
	this->muteChannel->getVolume(this->volume);

	// Get average spectrum for left and right stereo channels
	this->muteChannel->getSpectrum(specLeft, this->sampleSize, 0, FMOD_DSP_FFT_WINDOW_RECT);
	this->muteChannel->getSpectrum(specRight, this->sampleSize, 1, FMOD_DSP_FFT_WINDOW_RECT);

	for (int i = 0; i < this->sampleSize; i++)
		this->spec[i] = (specLeft[i] + specRight[i]) / 2;

	// Find max volume
	auto maxIterator = std::max_element(&spec[0], &spec[this->sampleSize]);
	this->maxVol = *maxIterator;

	// Find frequency range of each array item
	float hzRange = (44100 / 2) / static_cast<float>(this->sampleSize);

	//detect beat
	if (!enableNormalize)
	{
		if (spec[beatThresholdBar] >= beatThresholdVolume && beatLastTick == 0 && beatIgnoreLastTick == 0)
		{
			std::cout << spec[beatThresholdBar] << std::endl;
			beatLastTick = GetTickCount() - startTime;
			beatTimes.push(beatLastTick);
			isItBeat = true;
			cnt++;
			if (cnt % 2 == 0)
				std::cout << "beat!" << std::endl;
			else
				std::cout << "BEAT" << std::endl;
			while ((GetTickCount() - startTime) - beatTimes.front() > beatTrackCutoff)
			{
				beatTimes.pop();
				if (beatTimes.size() == 0)
					break;
			}
		}
		else if (beatIgnoreLastTick == 0 && beatLastTick != 0)
		{
			beatLastTick = 0;
			beatIgnoreLastTick = GetTickCount();
			isItBeat = false;
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
