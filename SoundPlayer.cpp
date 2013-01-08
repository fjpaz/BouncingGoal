// ****************************************************************
// Copyright (c) 2013 by Francisco Javier Paz Menendez
//
// This content is licensed under the MIT License. See LICENSE.txt
// in the top directory of this distribution for more information
// ****************************************************************

#include "SoundPlayer.h"
#include "Constants.h"

#include <SDL/SDL.h>
#include <QFile>
#include <QDebug>

SoundPlayer::SoundPlayer(QObject *parent) :
    QObject(parent)
{
    // Init SDL Audio
    int ret = SDL_Init(SDL_INIT_AUDIO);
    Q_ASSERT(0 == ret);

    // Open Audio Device
    int frequency = 44100;
    Uint16 format = AUDIO_S16;
    int channels = 2;
    int chunk_size = 1024;

    ret = Mix_OpenAudio(frequency, format, channels, chunk_size);
    Q_ASSERT(0 == ret);

    // Allocate audio channels
    ret = Mix_AllocateChannels(kNumAudioChannels);
    Q_ASSERT(kNumAudioChannels == ret);

   // Load sounds
    _music = Mix_LoadMUS("sounds/Music.ogg");
    _sounds[SOUND_COLLISION] = Mix_LoadWAV("sounds/Collision.ogg");
    _sounds[SOUND_GOAL] = Mix_LoadWAV("sounds/Goal.ogg");
}

SoundPlayer::~SoundPlayer()
{
    // Free resources
    foreach (Mix_Chunk* chunk, _sounds.values())
    {
        Mix_FreeChunk(chunk);
    }

    _sounds.clear();

    Mix_FreeMusic(_music);
    Mix_CloseAudio();
}

void SoundPlayer::playCollision()
{
    playSound(SOUND_COLLISION);
}

void SoundPlayer::playGoal()
{
    playSound(SOUND_GOAL);
}

void SoundPlayer::playMusic()
{
    // Play the background music looping
    int ret = Mix_PlayMusic(_music, -1);
    Q_ASSERT(0 == ret);
}

void SoundPlayer::stopMusic()
{
    Mix_HaltMusic();
}

void SoundPlayer::stopSounds()
{
    stopMusic();

    // Stop all channels
    Mix_HaltChannel(-1);
}

void SoundPlayer::playSound(GameSound sound)
{
    // Play the chunk once in the first free channel
    int ret = Mix_PlayChannel(-1, _sounds[sound], 0);
    Q_ASSERT(-1 != ret);
}
