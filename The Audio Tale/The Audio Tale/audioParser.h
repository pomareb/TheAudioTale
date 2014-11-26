
#pragma once

#include "fmod.hpp"
#include "fmod_errors.h"
#include "string"
#include "iostream"
#include "windows.h"
#include <queue>

class audioParser
{
	FMOD::Channel		*channel;
	FMOD::Channel		*muteChannel;
	FMOD::System		*system;

	//tempo
	float *frequency;
	float *volume;

	//frequency tab
	float *spec;

	// Normalization toggle and sample size
	bool enableNormalize;
	int sampleSize;

	// Beat detection parameters
	float beatThresholdVolume;
	int beatThresholdBar;
	unsigned int beatSustain;
	unsigned int beatPostIgnore;

	float beatLastTick;
	float beatIgnoreLastTick;
	float startTime;

	//bpm
	float bpmEstimate;

	//max vol
	float maxVol;

	// List of how many ms ago the last beats were
	std::queue<int> beatTimes;
	unsigned int beatTrackCutoff;


public:
	audioParser();
	~audioParser();
	void setSystem(FMOD::System *);
	void setChannels(FMOD::Channel *, FMOD::Channel *);
	void startMuteChannel();
	bool playPauseChannel();
	void parse();
};

