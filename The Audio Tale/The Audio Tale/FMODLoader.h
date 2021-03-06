
#pragma once
#include "fmod.hpp"
#include "fmod_errors.h"
#include "string"
#include "iostream"
#include "windows.h"
#include <queue>

class FMODLoader
{
	FMOD::System		*system;
	FMOD_RESULT			result;
	unsigned int		version;
	int					numDrivers;
	FMOD_SPEAKERMODE	speakerMode;
	FMOD_CAPS			caps;
	char				name[256];

	FMOD::Sound			*audio;
	FMOD::Sound			*audioStream;
	FMOD::Sound			*muteStream;
	FMOD::Channel		*channel;
	FMOD::Channel		*muteChannel;

	bool beatNow;

	//tempo
	float frequency;
	float volume;

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

	void checkVersion();
	void checkSoundCard();
	void initFMOD();

public:
	FMODLoader();
	void FMODErrorCheck(FMOD_RESULT result);
	bool loadSound(char* filename, char* filetwo);
	bool playSound();
	bool playPauseChannel();
	void delayedStartChannels(float time);
	void parse();
	
	bool isBeatNow();
	float getFrequency();
	float getVolume();
	float *getSpec();
	float getBpmEstimate();
	float getMaxVol();

	FMOD::Channel getChannel();
	FMOD::Channel getMuteChannel();
	FMOD::System getSystem();
};