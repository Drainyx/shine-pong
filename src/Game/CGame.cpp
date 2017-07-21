#include "Game.h"

static const CShIdentifier levelIdentifier("pong_simple_obstacles");

CGame::CGame()
{

}

CGame::~CGame()
{

}

void CGame::OnWin(int gagnant)
{
	if (gagnant == 1) // Droite
	{
		m_iPointsDroit++;
		ShTextZone::SetText(m_pPtsDroit, CShString::FromInt(m_iPointsDroit));
		if (m_iPointsDroit == m_iMaxPoints)
		{
			m_eGameState = e_game_state_end;
		}
		else
		{
			m_eGameState = e_game_state_waiting;
		}
	}
	else if (gagnant == 2) // Gauche
	{
		m_iPointsGauche++;
		ShTextZone::SetText(m_pPtsGauche, CShString::FromInt(m_iPointsGauche));
		if (m_iPointsGauche == m_iMaxPoints)
		{
			m_eGameState = e_game_state_end;
		}
		else
		{
			m_eGameState = e_game_state_waiting;
		}
	}
}

void CGame::Initialize()
{
	ShLevel::Load(levelIdentifier); 

	m_pbackground1 = ShEntity2::Find(levelIdentifier, CShIdentifier("background1"));
	SH_ASSERT(shNULL != m_pbackground1);
	ShObject::SetShow(m_pbackground1, true, true);
	m_pbackground2 = ShEntity2::Find(levelIdentifier, CShIdentifier("background2"));
	SH_ASSERT(shNULL != m_pbackground2);
	ShObject::SetShow(m_pbackground2, true, true);

	m_pUser = ShUser::GetUser(0);
	SH_ASSERT(shNULL != m_pUser);

	m_bRestart = false;

	m_iMaxPoints = 3;

	m_eGameState = e_game_state_menu;
	m_eMenuState = e_menu_state_accueil;

	m_pInfosGauche = ShTextZone::Create(levelIdentifier, GID(NULL), CShIdentifier("layer_default"), CShAABB2(-1.0f, 1.0f, -1.0f, 1.0f), CShVector3(-1400.0f, -850.0f, 0.0f), CShEulerAngles_ZERO, CShVector3(1.0f, 1.0f, 1.0f), ShTextZone::e_align_center_center, false, CShIdentifier("sst_medium_55.abc"), CShRGBAf(255.0f, 255.0f, 255.0f, 0.7f), true);
	m_pInfosDroite = ShTextZone::Create(levelIdentifier, GID(NULL), CShIdentifier("layer_default"), CShAABB2(-1.0f, 1.0f, -1.0f, 1.0f), CShVector3(1300.0f, -850.0f, 0.0f), CShEulerAngles_ZERO, CShVector3(1.0f, 1.0f, 1.0f), ShTextZone::e_align_center_center, false, CShIdentifier("sst_medium_55.abc"), CShRGBAf(255.0f, 255.0f, 255.0f, 0.7f), true);
	ShTextZone::SetText(m_pInfosGauche, CShString("Z/S"));
	ShTextZone::SetText(m_pInfosDroite, CShString("Up/Down"));
	
	m_pPtsDroit = ShTextZone::Create(levelIdentifier, GID(NULL), CShIdentifier("layer_default"), CShAABB2(-1.0f, 1.0f, -1.0f, 1.0f), CShVector3(1100.0f, -400.0f, -0.001f), CShEulerAngles_ZERO, CShVector3(2.0f, 2.0f, 1.0f), ShTextZone::e_align_center_center, false, CShIdentifier("sst_medium_55.abc"), CShRGBAf(0.0f, 0.0f, 0.0f, 0.2f), true);
	m_pPtsGauche = ShTextZone::Create(levelIdentifier, GID(NULL), CShIdentifier("layer_default"), CShAABB2(-1.0f, 1.0f, -1.0f, 1.0f), CShVector3(-1100.0f, -400.0f, -0.001f), CShEulerAngles_ZERO, CShVector3(2.0f, 2.0f, 1.0f), ShTextZone::e_align_center_center, false, CShIdentifier("sst_medium_55.abc"), CShRGBAf(0.0f, 0.0f, 0.0f, 0.2f), true);

	m_pInfosJeu = ShTextZone::Create(levelIdentifier, GID(NULL), CShIdentifier("layer_default"), CShAABB2(-1000.0f, 1000.0f, -1.0f, 1.0f), CShVector3(0.0f, -900.0f, 0.0f), CShEulerAngles_ZERO, CShVector3(2.0f, 2.0f, 1.0f), ShTextZone::e_align_center_center, false, CShIdentifier("sst_medium_55.abc"), CShRGBAf(255.0f, 255.0f, 255.0f, 0.7f), true);

	m_pGauche = ShTextZone::Create(levelIdentifier, GID(NULL), CShIdentifier("layer_default"), CShAABB2(-1.0f, 1.0f, -1.0f, 1.0f), CShVector3(-800.0f, 400.0f, 10.1f), CShEulerAngles_ZERO, CShVector3(2.0f, 2.0f, 1.0f), ShTextZone::e_align_center_center, false, CShIdentifier("sst_medium_55.abc"), CShRGBAf(255.0f, 255.0f, 255.0f, 0.7f), true);
	m_pDroit = ShTextZone::Create(levelIdentifier, GID(NULL), CShIdentifier("layer_default"), CShAABB2(-1.0f, 1.0f, -1.0f, 1.0f), CShVector3(700.0f, 400.0f, 10.1f), CShEulerAngles_ZERO, CShVector3(2.0f, 2.0f, 1.0f), ShTextZone::e_align_center_center, false, CShIdentifier("sst_medium_55.abc"), CShRGBAf(255.0f, 255.0f, 255.0f, 0.7f), true);
	m_pScoreGauche = ShTextZone::Create(levelIdentifier, GID(NULL), CShIdentifier("layer_default"), CShAABB2(-1.0f, 1.0f, -1.0f, 1.0f), CShVector3(-300.0f, 400.0f, 10.1f), CShEulerAngles_ZERO, CShVector3(2.0f, 2.0f, 1.0f), ShTextZone::e_align_center_center, false, CShIdentifier("sst_medium_55.abc"), CShRGBAf(255.0f, 255.0f, 255.0f, 0.7f), true);
	m_pScoreDroite = ShTextZone::Create(levelIdentifier, GID(NULL), CShIdentifier("layer_default"), CShAABB2(-1.0f, 1.0f, -1.0f, 1.0f), CShVector3(300.0f, 400.0f, 10.1f), CShEulerAngles_ZERO, CShVector3(2.0f, 2.0f, 1.0f), ShTextZone::e_align_center_center, false, CShIdentifier("sst_medium_55.abc"), CShRGBAf(255.0f, 255.0f, 255.0f, 0.7f), true);
	m_pScoreUnion = ShTextZone::Create(levelIdentifier, GID(NULL), CShIdentifier("layer_default"), CShAABB2(-1.0f, 1.0f, -1.0f, 1.0f), CShVector3(0.0f, 400.0f, 10.1f), CShEulerAngles_ZERO, CShVector3(2.0f, 2.0f, 1.0f), ShTextZone::e_align_center_center, false, CShIdentifier("sst_medium_55.abc"), CShRGBAf(255.0f, 255.0f, 255.0f, 0.7f), true);
	
	//
	// GUI
	ShGUIControl * pControlRoot = ShGUI::AddGUIAndSSS(CShIdentifier("gui_menu"), shNULL);

	//
	// Gets GUI Elements
	m_pPanelMenu = (ShGUIControlPanel*)	ShGUIControl::GetElementById(CShIdentifier("panel_buttons_menu"), pControlRoot);
	SH_ASSERT(shNULL != m_pPanelMenu);

	ShGUIControlButton * pButtonJouer = (ShGUIControlButton*)ShGUIControl::GetElementById(CShIdentifier("button_menu_play"), m_pPanelMenu);
	SH_ASSERT(shNULL != pButtonJouer);
	ShGUIControlButton * pButtonNiveau = (ShGUIControlButton*)ShGUIControl::GetElementById(CShIdentifier("button_menu_level"), m_pPanelMenu);
	SH_ASSERT(shNULL != pButtonNiveau);
	ShGUIControlButton * pButtonCommandes = (ShGUIControlButton*)ShGUIControl::GetElementById(CShIdentifier("button_menu_controls"), m_pPanelMenu);
	SH_ASSERT(shNULL != pButtonCommandes);
	ShGUIControlButton * pButtonOptions = (ShGUIControlButton*)ShGUIControl::GetElementById(CShIdentifier("button_menu_settings"), m_pPanelMenu);
	SH_ASSERT(shNULL != pButtonOptions);
	ShGUIControlButton * pButtonQuitter = (ShGUIControlButton*)ShGUIControl::GetElementById(CShIdentifier("button_menu_exit"), m_pPanelMenu);
	SH_ASSERT(shNULL != pButtonQuitter);

	m_pPanelFin = (ShGUIControlPanel*) ShGUIControl::GetElementById(CShIdentifier("panel_buttons_end"), pControlRoot);
	SH_ASSERT(shNULL != m_pPanelFin);
	ShGUIControlButton * pButtonMenu = (ShGUIControlButton*)ShGUIControl::GetElementById(CShIdentifier("button_end_menu"), m_pPanelFin);
	SH_ASSERT(shNULL != pButtonMenu);

	m_pPanelNiveau = (ShGUIControlPanel*)ShGUIControl::GetElementById(CShIdentifier("panel_buttons_level"), pControlRoot);
	SH_ASSERT(shNULL != m_pPanelNiveau);
	ShGUIControlButton * pButtonNiveauRetour = (ShGUIControlButton*)ShGUIControl::GetElementById(CShIdentifier("button_level_back"), m_pPanelNiveau);
	SH_ASSERT(shNULL != pButtonNiveauRetour);
	
	m_pPanelCommandes = (ShGUIControlPanel*)ShGUIControl::GetElementById(CShIdentifier("panel_buttons_controls"), pControlRoot);
	SH_ASSERT(shNULL != m_pPanelCommandes);
	ShGUIControlButton * pButtonCommandesRetour = (ShGUIControlButton*)ShGUIControl::GetElementById(CShIdentifier("button_controls_back"), m_pPanelCommandes);
	SH_ASSERT(shNULL != pButtonCommandesRetour);
	ShGUIControlText * 	pTextCommandes = (ShGUIControlText*)ShGUIControl::GetElementById(CShIdentifier("text_controls"), m_pPanelCommandes);
	SH_ASSERT(shNULL != pButtonCommandes);

	m_pPanelOptions = (ShGUIControlPanel*)ShGUIControl::GetElementById(CShIdentifier("panel_buttons_settings"), pControlRoot);
	SH_ASSERT(shNULL != m_pPanelOptions);
	ShGUIControlButton * pButtonOptionsRetour = (ShGUIControlButton*)ShGUIControl::GetElementById(CShIdentifier("button_settings_back"), m_pPanelOptions);
	SH_ASSERT(shNULL != pButtonOptionsRetour);

	//
	// Adds Slots Fct Ptr
	ShGUIControlButton::AddSlotFctPtrClick(pButtonJouer, (pSlotSDKButtonClick)ButtonJouerClick);
	ShGUIControlButton::AddSlotFctPtrClick(pButtonNiveau, (pSlotSDKButtonClick)ButtonNiveauClick);
	ShGUIControlButton::AddSlotFctPtrClick(pButtonCommandes, (pSlotSDKButtonClick)ButtonCommandesClick);
	ShGUIControlButton::AddSlotFctPtrClick(pButtonOptions, (pSlotSDKButtonClick)ButtonOptionsClick);
	ShGUIControlButton::AddSlotFctPtrClick(pButtonQuitter, (pSlotSDKButtonClick)ButtonQuitterClick);
	ShGUIControlButton::AddSlotFctPtrClick(pButtonMenu, (pSlotSDKButtonClick)ButtonMenuClick);
	ShGUIControlButton::AddSlotFctPtrClick(pButtonNiveauRetour, (pSlotSDKButtonClick)ButtonRetourClick);
	ShGUIControlButton::AddSlotFctPtrClick(pButtonCommandesRetour, (pSlotSDKButtonClick)ButtonRetourClick);
	ShGUIControlButton::AddSlotFctPtrClick(pButtonOptionsRetour, (pSlotSDKButtonClick)ButtonRetourClick);

	ShGUIControl::Show(m_pPanelMenu);
	ShGUIControl::Hide(m_pPanelFin);
	ShGUIControl::Hide(m_pPanelNiveau);
	ShGUIControl::Hide(m_pPanelCommandes);
	ShGUIControl::Hide(m_pPanelOptions);

	//ShGUIControlText * pText = shNULL;
	//ShGUIControlText::SetText(pText, CShString(""), true, false);
	//CShWString wstr = ShGUIControlText::GetText(pText);
	//CShWString(L"sdf");
	//int i = 0;
	//CShWString::FromInt(i);


}

void CGame::Update(float dt)
{
	switch (m_eGameState) 
	{
		case e_game_state_waiting:
		{
			ShObject::SetShow(m_pbackground1, false, true);
			ShObject::SetShow(m_pbackground2, false, true);
			ShTextZone::SetText(m_pInfosJeu, CShString("Press Space to play"));
			if (ShUser::HasTriggeredAction(m_pUser, CShIdentifier("play")))
			{
				ShTextZone::SetText(m_pInfosJeu, CShString(" "));
				m_eGameState = e_game_state_playing;
			}
		} 
		break;

		// Fin d'une partie
		case e_game_state_end:
		{
			ShObject::SetShow(m_pbackground1, true, true);
			ShObject::SetShow(m_pbackground2, true, true);
			ShGUIControl::Show(m_pPanelFin);
			ShTextZone::SetText(m_pGauche, CShString("Player 1"));
			ShTextZone::SetText(m_pDroit, CShString("Player 2"));
			ShTextZone::SetText(m_pScoreGauche, CShString::FromInt(m_iPointsGauche));
			ShTextZone::SetText(m_pScoreDroite, CShString::FromInt(m_iPointsDroit));
			ShTextZone::SetText(m_pScoreUnion, CShString("-"));
		}
		break;
	}

	if (ShUser::HasTriggeredAction(m_pUser, CShIdentifier("quit")))
	{
		ShApplication::RequestQuit();
	}
}

void CGame::Release()
{
	ShLevel::Release();
}


bool CGame::ButtonJouerClick(ShGUIControlButton * pButton)
{
	g_game.SetGameState(CGame::e_game_state_waiting);
	return true;
}

bool CGame::ButtonNiveauClick(ShGUIControlButton * pButton)
{
	g_game.SetMenuState(CGame::e_menu_state_niveau);
	return true;
}

bool CGame::ButtonCommandesClick(ShGUIControlButton * pButton)
{
	g_game.SetMenuState(CGame::e_menu_state_commandes);
	return true;
}

bool CGame::ButtonOptionsClick(ShGUIControlButton * pButton)
{
	g_game.SetMenuState(CGame::e_menu_state_options);
	return true;
}

bool CGame::ButtonQuitterClick(ShGUIControlButton * pButton)
{
	ShApplication::RequestQuit();
	return true;
}

bool CGame::ButtonMenuClick(ShGUIControlButton * pButton)
{
	g_game.SetGameState(CGame::e_game_state_menu);
	g_game.SetMenuState(CGame::e_menu_state_accueil);
	return true;
}

bool CGame::ButtonRetourClick(ShGUIControlButton * pButton)
{
	g_game.SetMenuState(CGame::e_menu_state_accueil);
	return true;
}


void CGame::SetGameState(EGameState eGameState)
{
	m_eGameState = eGameState;
	switch (m_eGameState)
	{
		case e_game_state_waiting:
		{
			ShGUIControl::Hide(m_pPanelMenu);
			ShGUIControl::Hide(m_pPanelFin);
			m_iPointsDroit = 0;
			m_iPointsGauche = 0;
			ShTextZone::SetText(m_pPtsDroit, CShString("0"));
			ShTextZone::SetText(m_pPtsGauche, CShString("0"));
		}
		break;
		case e_game_state_menu:
		{
			ShGUIControl::Hide(m_pPanelFin);
			ShGUIControl::Show(m_pPanelMenu);
		}
		break;
	}
}

void CGame::SetMenuState(EMenuState eMenuState)
{
	m_eMenuState = eMenuState;
	switch (m_eMenuState)
	{
		case e_menu_state_accueil:
		{
			ShTextZone::SetText(m_pGauche, CShString(" "));
			ShTextZone::SetText(m_pDroit, CShString(" "));
			ShTextZone::SetText(m_pScoreGauche, CShString(" "));
			ShTextZone::SetText(m_pScoreDroite, CShString(" "));
			ShTextZone::SetText(m_pScoreUnion, CShString(" "));
			ShGUIControl::Show(m_pPanelMenu);
			ShGUIControl::Hide(m_pPanelNiveau);
			ShGUIControl::Hide(m_pPanelCommandes);
			ShGUIControl::Hide(m_pPanelOptions);
		}
		break;
		case e_menu_state_niveau:
		{
			ShGUIControl::Hide(m_pPanelMenu);
			ShGUIControl::Show(m_pPanelNiveau);
		}
		break;
		case e_menu_state_commandes:
		{
			ShGUIControl::Hide(m_pPanelMenu);
			ShGUIControl::Show(m_pPanelCommandes);

		}
		break;
		case e_menu_state_options:
		{
			ShGUIControl::Hide(m_pPanelMenu);
			ShGUIControl::Show(m_pPanelOptions);
		}
		break;
	}
}
