#include "soundService.hpp"

#include <raylib.h>

#include <cstdio>
#include <stdexcept>

#include "game.hpp"
#include "gamedata.hpp"

SoundService::SoundService() {
	music = {};
	musicLoaded = false;
	lastId = "";
}

bool SoundService::loadMusic(const std::string &id) {
	auto music = Game::getResources().getUnpacker().loadMusic(id);
	if (IsMusicValid(music)) {
		unload();
		this->music = music;
		musicLoaded = true;

		TraceLog(TraceLogLevel::LOG_INFO, "Loaded packed music with id %s", id.c_str());
	} else {
		TraceLog(TraceLogLevel::LOG_WARNING, "Unable to load packed music with id %s", id.c_str());
	}

	return musicLoaded;
}

void SoundService::playMusic() const {
	if (musicLoaded) {
		PlayMusicStream(music);
	}
}

void SoundService::playSound(const std::string &id) const {
	Wave wave = Game::getResources().getUnpacker().loadWave(id);
	if (IsWaveValid(wave)) {
		Sound sound = LoadSoundFromWave(wave);
		PlaySound(sound);
		TraceLog(TraceLogLevel::LOG_INFO, "Loaded packed Wave with id %s", id.c_str());
	} else {
		TraceLog(TraceLogLevel::LOG_WARNING, "Unable to load packed Wave with id %s", id.c_str());
	}
}

void SoundService::update() const {
	if (musicLoaded) {
		UpdateMusicStream(music);
	}
}

void SoundService::unload() const {
	if (musicLoaded) {
		StopMusicStream(music);
		UnloadMusicStream(music);
	}
}
