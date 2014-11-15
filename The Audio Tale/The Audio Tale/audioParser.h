
#pragma once

#include "fmod.hpp"
#include "fmod_errors.h"
#include "string"
#include "iostream"

class audioParser
{
	FMOD::Channel		*channel;

public:
	audioParser();
	~audioParser();
	void setChannel(FMOD::Channel *);
	void parse();
};

