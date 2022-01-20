#include "pch.h"
#include "BoostRandomizerPlugin.h"
#include "utils\parser.h"

BAKKESMOD_PLUGIN(BoostRandomizerPlugin, "Boost Randomizer Plugin", "1.0", PERMISSION_ALL)

void BoostRandomizerPlugin::onLoad() {
	this->cvarManager->registerCvar(
		"boost_randomizer_time_to_randomize",
		"10",
		"Choose how many seconds to randomize boost",
		true,
		true,
		0.0f,
		true,
		120.0f,
		false
	);

	cvarManager->registerCvar(
		"boost_randomizer_range_values",
		"(0, 100)",
		"The amount of boost that you could recieve",
		true,
		true,
		0.0f,
		true,
		100.0f
	);

	cvarManager->registerNotifier("boost_randomizer_start", [this](std::vector<std::string> params) {
		lastCooldownTime = cvarManager->getCvar("boost_randomizer_time_to_randomize").getFloatValue();
		this->pluginEnabled = true;
		this->randomize();
	}, "Starts the recovery mode which will bump you with the given settings.", PERMISSION_ALL);

	cvarManager->registerNotifier("boost_randomizer_stop", [this](std::vector<std::string> params) {
		this->pluginEnabled = false;
	}, "Stops the recovery plugin.", PERMISSION_ALL);
}

void BoostRandomizerPlugin::randomize() {
	gameWrapper->SetTimeout([this](GameWrapper* gw) {
		this->randomize();
	}, this->getRandTimeout());
}

float BoostRandomizerPlugin::getRandTimeout() {
	ServerWrapper training = gameWrapper->GetGameEventAsServer();
	float lastRand = training.GetSecondsElapsed() - lastRandTime;
	if (lastRand > lastCooldownTime) {
		randomizeBoost();
		return lastCooldownTime;
	}
	return lastCooldownTime - lastRand + 0.1f;
}

void BoostRandomizerPlugin::randomizeBoost() {
	auto server = gameWrapper->GetGameEventAsServer();
	auto cars = server.GetCars();
	int size = cars.Count();
	for (int i = 0; i < size; i++) {
		auto car = cars.Get(i);
		BoostWrapper boost = car.GetBoostComponent();
		float newBoost = cvarManager->getCvar("boost_randomizer_range_values").getFloatValue() / 100;
		this->log(std::to_string(newBoost));
		boost.SetBoostAmount(newBoost);
	}
	lastRandTime = server.GetSecondsElapsed();
	lastCooldownTime = cvarManager->getCvar("boost_randomizer_time_to_randomize").getFloatValue();
}

void BoostRandomizerPlugin::onUnload() {

}

void BoostRandomizerPlugin::log(std::string msg) {
	cvarManager->log(msg);
}
