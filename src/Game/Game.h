#pragma once

#include <ShSDK/ShSDK.h>
#include <ShEntryPoint/ShEntryPoint.h>

#include "CPluginPong.h"
#include "CGame.h"

void OnPreInitialize	(void);
void OnPostInitialize	(void);
void OnPreUpdate		(float dt);
void OnPostUpdate		(float dt);
void OnPreRelease		(void);
void OnPostRelease		(void);

extern CGame g_game;