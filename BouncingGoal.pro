# ****************************************************************
# Copyright (c) 2013 by Francisco Javier Paz Menendez
#
# This content is licensed under the MIT License. See LICENSE.txt
# in the top directory of this distribution for more information
# ****************************************************************

QT       += core gui opengl

TARGET = BouncingGoal
TEMPLATE = app


SOURCES += main.cpp\
    Team.cpp \
    CollisionSensor.cpp \
    DynamicEntity.cpp \
    EntityFactory.cpp \
    GenericEntity.cpp \
    MainWindow.cpp \
    PlayerEntity.cpp \
    SoundPlayer.cpp \
    World.cpp

HEADERS  += \
    Team.h \
    CollisionSensor.h \
    Constants.h \
    DynamicEntity.h \
    EntityFactory.h \
    GenericEntity.h \
    MainWindow.h \
    PlayerEntity.h \
    SoundPlayer.h \
    World.h

FORMS    += mainwindow.ui

win32: LIBS += -lBox2D -lSDL_mixer
else:  LIBS += -lBox2D -lSDL_mixer

unix: QMAKE_CXXFLAGS_DEBUG += -pg
unix: QMAKE_LFLAGS_DEBUG += -pg

QMAKE_CXXFLAGS_RELEASE += -DQT_NO_DEBUG_OUTPUT -DQT_NO_WARNING_OUTPUT

OTHER_FILES += \
    images/Team2Tail.png \
    images/Team2Player3Highlight.png \
    images/Team2Player3.png \
    images/Team2Player2Highlight.png \
    images/Team2Player2.png \
    images/Team2Player1Highlight.png \
    images/Team2Player1.png \
    images/Team1Tail.png \
    images/Team1Player3Highlight.png \
    images/Team1Player3.png \
    images/Team1Player2Highlight.png \
    images/Team1Player2.png \
    images/Team1Player1Highlight.png \
    images/Team1Player1.png \
    images/Stadium.png \
    images/PlayerTail.xcf \
    images/Player.xcf \
    images/Floor.png \
    images/Ball.png \
    sounds/Music.ogg \
    sounds/Goal.ogg \
    sounds/Collision.ogg

# Need to execute make install as a post-build step
images.path = $${OUT_PWD}/images
images.files += $$files(images/*.png)
sounds.path = $${OUT_PWD}/sounds
sounds.files += $$files(sounds/*.ogg)
INSTALLS += images sounds
