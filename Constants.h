// ****************************************************************
// Copyright (c) 2013 by Francisco Javier Paz Menendez
//
// This content is licensed under the MIT License. See LICENSE.txt
// in the top directory of this distribution for more information
// ****************************************************************

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "stdint.h"

// ----------------------------------------------------------------------------
// Conversions
static const float kPI = 3.14159265359f;
static const float kDegToRad = (2.0f * kPI) / 360.0f;
static const float kRadToDeg = 360.0f / (2.0f * kPI);
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Timestep
static const int kTimeStep = 1000 / 70.0;               // 1000 / FPS
static const float kB2TimeStep = kTimeStep/1000.0f;
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Impulse Recharge
static const float kImpulseRecharge = 5.0f;
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Sound
static const uint8_t kNumAudioChannels = 16;
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Physics
static const int32_t kB2VelIterations = 8;
static const int32_t kB2PosIterations = 3;
static const float kDensity = 0.1f;
static const float kRestitution = 1.0f;
static const float kLinearDamping = 0.5f;
static const float kAngularDamping = 0.5f;
static const float kPlayerAccelerationForce = 50.0f;
static const float kPlayerRotationImpulse = 0.5f;
static const float kPlayerLinearImpulse = 150.0f;
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// World geometry
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Stadium geometry
static const float kStadiumWidth = 128.0f;
static const float kStadiumHeight = 72.0f;
static const float kStadiumBorderWidth = 1.0f;
static const float kStadiumBorderHeight = 1.0f;

// Top-left coordinates of polygon bounding rect
static const float kXStadiumLeft = -kStadiumWidth/2.0f;
static const float kYStadiumTop = -kStadiumHeight/2.0f;
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Goal geometry
static const float kGoalWidth = 10.0f;
static const float kGoalHeight = 20.0f;
static const float kGoalBorderWidth = 1.0f;
static const float kGoalBorderHeight = 1.0f;

// Top-left coordinates of polygon bounding rect
static const float kXGoalLeft = -kGoalWidth/2.0f;
static const float kYGoalTop = -kGoalHeight/2.0f;
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Player geometry
static const float kPlayerWidth = 5.0f;
static const float kPlayerHeight = 5.0f;

// Top-left coordinates of circle bounding rect
static const float kXPlayerCircleLeft = -kPlayerWidth/2;
static const float kYPlayerCircleTop = -kPlayerHeight/2;

// Top-left coordinates of box bounding rect (respect circle center)
static const float kXPlayerBoxCenter = kXPlayerCircleLeft - kPlayerWidth/16;
static const float kYPlayerBoxCenter = 0.0f;
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Ball geometry
static const float kBallWidth = 2.0f;
static const float kBallHeight = 2.0f;

// Top-left coordinates of circle bounding rect
static const float kXBallLeft = -kBallWidth/2.0f;
static const float kYBallTop = -kBallHeight/2.0f;
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Initial positions
static const float kXStadium = 0.0f;
static const float kYStadium = 0.0f;
static const float kXGoalTeam1 = kStadiumBorderWidth + kGoalWidth/2.0f;
static const float kYGoalTeam1 = kStadiumHeight / 2.0f;
static const float kAngleGoalTeam1 = 0.0f;
static const float kXGoalTeam2 = kStadiumWidth - kStadiumBorderWidth - kGoalWidth/2.0f;
static const float kYGoalTeam2 = kStadiumHeight / 2.0f;
static const float kAngleGoalTeam2 = 180.0f;
static const float kXBall = kStadiumWidth / 2.0f;
static const float kYBall = kStadiumHeight / 2.0f;
static const float kAngleTeam1 = 0.0f;
static const float kAngleTeam2 = 180.0f;
static const float kXTeam1Player1 = 3.0f * kStadiumWidth / 8.0f;
static const float kYTeam1Player1 = kStadiumHeight / 2.0f;
static const float kXTeam1Player2 = kStadiumWidth / 4.0f;
static const float kYTeam1Player2 = kStadiumHeight / 4.0f;
static const float kXTeam1Player3 = kStadiumWidth / 4.0f;
static const float kYTeam1Player3 = 3.0f * kStadiumHeight / 4.0f;
static const float kXTeam2Player1 = 5.0f * kStadiumWidth / 8.0f;
static const float kYTeam2Player1 = kStadiumHeight / 2.0f;
static const float kXTeam2Player2 = 3.0f * kStadiumWidth / 4.0f;
static const float kYTeam2Player2 = kStadiumHeight / 4.0f;
static const float kXTeam2Player3 = 3.0f * kStadiumWidth / 4.0f;
static const float kYTeam2Player3 = 3.0f * kStadiumHeight / 4.0f;
// ----------------------------------------------------------------------------

#endif // CONSTANTS_H
