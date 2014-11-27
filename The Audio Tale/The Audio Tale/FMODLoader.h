
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
	bool isItBeat;
	int cnt;
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
	bool playMuteChannel();
	bool playPauseChannels();
	void parse();

	bool getIsItABeat();
	float getFrequency();
	float getVolume();
	float getBPM();
	float *getSpec();
};