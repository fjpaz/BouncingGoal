// ****************************************************************
// Copyright (c) 2013 by Francisco Javier Paz Menendez
//
// This content is licensed under the MIT License. See LICENSE.txt
// in the top directory of this distribution for more information
// ****************************************************************

#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <SDL/SDL_mixer.h>
#include <QHash>
#include <QObject>
#include <stdint.h>

typedef enum GameSound
{
    SOUND_COLLISION,
    SOUND_GOAL
} GameSound;

class SoundPlayer : public QObject
{
    Q_OBJECT
public:
    explicit SoundPlayer(QObject *parent = 0);
    virtual ~SoundPlayer();
    
signals:
    
public slots:
    void playCollision();
    void playGoal();
    void playMusic();
    void stopMusic();
    void stopSounds();

private:
    void playSound(GameSound sound);

    Mix_Music* _music;
    QHash<GameSound, Mix_Chunk*> _sounds;
};

#endif // SOUNDPLAYER_H
