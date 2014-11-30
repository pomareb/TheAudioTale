
#pragma once
#include "fmod.hpp"
#include "fmod_errors.h"
#include "string"
#include "iostream"

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
	FMOD::Channel		*channel;

	void checkVersion();
	void checkSoundCard();
	void initFMOD();

public:
	FMODLoader();
	void FMODErrorCheck(FMOD_RESULT result);
	bool loadSound(char* filename);
	bool playSound();
	FMOD::Channel getChannel();

};