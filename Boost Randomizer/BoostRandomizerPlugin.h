#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"

#pragma comment(lib, "pluginsdk.lib")

class BoostRandomizerPlugin : public BakkesMod::Plugin::BakkesModPlugin
{
public:
	virtual void onLoad();
	virtual void onUnload();

private:
	bool pluginEnabled = false;
	float lastRandTime = 0.0f;
	float lastCooldownTime = 0.0f;

	void log(std::string msg);
	void randomize();
	float getRandTimeout();
	void randomizeBoost();
};

