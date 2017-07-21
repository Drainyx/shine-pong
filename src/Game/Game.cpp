#include "Game.h"

#include "PluginFactoryPong.h"

CGame g_game;

/**
 * Called before engine initialization
 */
void ShEntryPoint::OnPreInitialize(void)
{
	RegisterPluginPong(&g_game);
	// ShDisplay::SetFullScreen();
}

/**
 * Called after engine initialization
 */
void ShEntryPoint::OnPostInitialize(void)
{
	ShCamera * pCamera = ShCamera::Create(CShIdentifier("global"), CShIdentifier("default_camera"), false);
	SH_ASSERT(shNULL != pCamera);
	ShCamera::SetCurrent2D(pCamera);
	ShCamera::SetNearPlaneDistance(pCamera, 0.0f);
	ShCamera::SetFarPlaneDistance(pCamera, 5000.0f);
	ShCamera::SetPosition(pCamera, CShVector3(0.0f, 0.0f, 2800.0f));
	ShCamera::SetViewport(pCamera, 1280.0f, 720.0f);
	ShCamera::SetTarget(pCamera, CShVector3(0.0f, 0.0f, 0.0f));

	
	g_game.Initialize();
}

/**
 * Called on each frame, before the engine update
 */
void ShEntryPoint::OnPreUpdate(float dt)
{
	g_game.Update(dt);
}

/**
 * Called on each frame, after the engine update
 */
void ShEntryPoint::OnPostUpdate(float dt)
{
	if (g_game.m_bRestart)
	{
		g_game.Release();
		g_game.Initialize();
	}
}

/**
 * Called before the engine release
 */
void ShEntryPoint::OnPreRelease(void)
{
	g_game.Release();
	ShLevel::Release();
}

/**
 * Called after the engine release
 */
void ShEntryPoint::OnPostRelease(void)
{
	UnRegisterPluginPong();
}
