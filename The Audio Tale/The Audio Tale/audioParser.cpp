
#include "audioParser.h"


audioParser::audioParser()
{
	// Load song
	enableNormalize = false;
	sampleSize = 64;

	// Set beat detection parameters
	beatThresholdVolume = 0.3f;
	beatThresholdBar = 0;
	beatSustain = 150;
	beatPostIgnore = 250;
	beatTrackCutoff = 10000;

	spec = new float[sampleSize];

	beatLastTick = 0;
	beatIgnoreLastTick = 0;

	frequency = 0;
	volume = 0;

	startTime = GetTickCount();
}


audioParser::~audioParser()
{
	system->release();
}


void audioParser::setChannels(FMOD::Channel *c, FMOD::Channel *d)
{
	this->channel = c;
	this->muteChannel = d;
	//muteChannel->setMute(true);
}

void audioParser::startMuteChannel()
{
	muteChannel->setPaused(false);
}

bool audioParser::playPauseChannel()
{
	bool isPaused;
	channel->getPaused(&isPaused);
	channel->setPaused(!isPaused);
	muteChannel->setPaused(!isPaused);
	return !isPaused;
}

void audioParser::setSystem(FMOD::System *s)
{
	this->system = s;
}

void audioParser::parse()
{
	system->update();

	// Frequency analysis
	float *specLeft, *specRight;
	specLeft = new float[sampleSize];
	specRight = new float[sampleSize];

	//getTempo
	muteChannel->getFrequency(frequency);

	//getAmplitude
	muteChannel->getVolume(volume);

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

			while ((startTime - GetTickCount()) - beatTimes.front() > beatTrackCutoff)
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