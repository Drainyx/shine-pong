#include "Game.h"

static const CShIdentifier levelSimpleIdentifier("pong_simple");
static const CShIdentifier levelSimpleObstaclesIdentifier("pong_simple_obstacles");
static const CShIdentifier levelMurIdentifier("pong_mur");
static const CShIdentifier levelMurObstaclesIdentifier("pong_mur_obstacles");

CGame::CGame()
{

}

CGame::~CGame()
{

}

void CGame::LaunchLevel()
{
	CShIdentifier levelIdentifier = m_GameLevel;

	ShLevel::Load(levelIdentifier);

	m_pInfosGauche = ShTextZone::Create(levelIdentifier, GID(NULL), CShIdentifier("layer_default"), CShAABB2(-1.0f, 1.0f, -1.0f, 1.0f), CShVector3(-1400.0f, -850.0f, 0.0f), CShEulerAngles_ZERO, CShVector3(1.0f, 1.0f, 1.0f), ShTextZone::e_align_center_center, false, CShIdentifier("sst_medium_55.abc"), CShRGBAf(255.0f, 255.0f, 255.0f, 0.7f), true);
	m_pInfosDroite = ShTextZone::Create(levelIdentifier, GID(NULL), CShIdentifier("layer_default"), CShAABB2(-1.0f, 1.0f, -1.0f, 1.0f), CShVector3(1300.0f, -850.0f, 0.0f), CShEulerAngles_ZERO, CShVector3(1.0f, 1.0f, 1.0f), ShTextZone::e_align_center_center, false, CShIdentifier("sst_medium_55.abc"), CShRGBAf(255.0f, 255.0f, 255.0f, 0.7f), true);
	ShTextZone::SetText(m_pInfosGauche, CShString("Z/S"));
	ShTextZone::SetText(m_pInfosDroite, CShString("Up/Down"));

	m_pPtsDroit = ShTextZone::Create(levelIdentifier, GID(NULL), CShIdentifier("layer_default"), CShAABB2(-1.0f, 1.0f, -1.0f, 1.0f), CShVector3(1100.0f, -400.0f, -0.001f), CShEulerAngles_ZERO, CShVector3(2.0f, 2.0f, 1.0f), ShTextZone::e_align_center_center, false, CShIdentifier("sst_medium_55.abc"), CShRGBAf(0.0f, 0.0f, 0.0f, 0.2f), true);
	m_pPtsGauche = ShTextZone::Create(levelIdentifier, GID(NULL), CShIdentifier("layer_default"), CShAABB2(-1.0f, 1.0f, -1.0f, 1.0f), CShVector3(-1100.0f, -400.0f, -0.001f), CShEulerAngles_ZERO, CShVector3(2.0f, 2.0f, 1.0f), ShTextZone::e_align_center_center, false, CShIdentifier("sst_medium_55.abc"), CShRGBAf(0.0f, 0.0f, 0.0f, 0.2f), true);

	m_pInfosJeu = ShTextZone::Create(levelIdentifier, GID(NULL), CShIdentifier("layer_default"), CShAABB2(-1000.0f, 1000.0f, -1.0f, 1.0f), CShVector3(0.0f, -900.0f, 0.0f), CShEulerAngles_ZERO, CShVector3(2.0f, 2.0f, 1.0f), ShTextZone::e_align_center_center, false, CShIdentifier("sst_medium_55.abc"), CShRGBAf(255.0f, 255.0f, 255.0f, 0.7f), true);
}

void CGame::Initialize()
{
	m_pUser = ShUser::GetUser(0);
	SH_ASSERT(shNULL != m_pUser);

	m_bRestart = false;

	m_iMaxPoint = 3;

	m_GameLevel = levelSimpleObstaclesIdentifier;
	m_eGameState = e_game_state_menu;
	m_eMenuState = e_menu_state_accueil;
	
	//
	// GUI
	ShGUIControl * pControlRoot = ShGUI::AddGUIAndSSS(CShIdentifier("gui_menu"), shNULL);

	//
	// Gets GUI Elements
	m_pPanelBackground = (ShGUIControlPanel*)ShGUIControl::GetElementById(CShIdentifier("panel_background"), pControlRoot);
	SH_ASSERT(shNULL != m_pPanelBackground);
	ShGUIControlText * pTextBackground = (ShGUIControlText*)ShGUIControl::GetElementById(CShIdentifier("background_name"), m_pPanelBackground);
	SH_ASSERT(shNULL != pTextBackground);

	m_pPanelMenu = (ShGUIControlPanel*)	ShGUIControl::GetElementById(CShIdentifier("panel_menu"), pControlRoot);
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

	m_pPanelFin = (ShGUIControlPanel*) ShGUIControl::GetElementById(CShIdentifier("panel_end"), pControlRoot);
	SH_ASSERT(shNULL != m_pPanelFin);
	ShGUIControlButton * pButtonMenu = (ShGUIControlButton*)ShGUIControl::GetElementById(CShIdentifier("button_end_menu"), m_pPanelFin);
	SH_ASSERT(shNULL != pButtonMenu);
	ShGUIControlButton * pButtonRejouer = (ShGUIControlButton*)ShGUIControl::GetElementById(CShIdentifier("button_end_replay"), m_pPanelFin);
	SH_ASSERT(shNULL != pButtonRejouer);
	ShGUIControlText * pTextFin = (ShGUIControlText*)ShGUIControl::GetElementById(CShIdentifier("text_end"), m_pPanelFin);
	SH_ASSERT(shNULL != pTextFin);
	ShGUIControlText * pTextFinJoueur1 = (ShGUIControlText*)ShGUIControl::GetElementById(CShIdentifier("text_end_player_1"), m_pPanelFin);
	SH_ASSERT(shNULL != pTextFinJoueur1);
	ShGUIControlText * pTextFinJoueur2 = (ShGUIControlText*)ShGUIControl::GetElementById(CShIdentifier("text_end_player_2"), m_pPanelFin);
	SH_ASSERT(shNULL != pTextFinJoueur2);
	ShGUIControlText * pTextFinScoreJoueur1 = (ShGUIControlText*)ShGUIControl::GetElementById(CShIdentifier("text_end_score_player_1"), m_pPanelFin);
	SH_ASSERT(shNULL != pTextFinScoreJoueur1);
	ShGUIControlText * pTextFinScoreJoueur2 = (ShGUIControlText*)ShGUIControl::GetElementById(CShIdentifier("text_end_score_player_2"), m_pPanelFin);
	SH_ASSERT(shNULL != pTextFinScoreJoueur2);
	ShGUIControlText * pTextFinScoreUnion = (ShGUIControlText*)ShGUIControl::GetElementById(CShIdentifier("text_end_score_union"), m_pPanelFin);
	SH_ASSERT(shNULL != pTextFinScoreUnion);

	m_pPanelNiveau = (ShGUIControlPanel*)ShGUIControl::GetElementById(CShIdentifier("panel_level"), pControlRoot);
	SH_ASSERT(shNULL != m_pPanelNiveau);
	ShGUIControlButton * pButtonNiveauRetour = (ShGUIControlButton*)ShGUIControl::GetElementById(CShIdentifier("button_level_back"), m_pPanelNiveau);
	SH_ASSERT(shNULL != pButtonNiveauRetour);
	ShGUIControlRadioButton * pRadioButtonNiveauSimple = (ShGUIControlRadioButton*)ShGUIControl::GetElementById(CShIdentifier("radiobutton_level_simple"), m_pPanelNiveau);
	SH_ASSERT(shNULL != pRadioButtonNiveauSimple);
	ShGUIControlRadioButton * pRadioButtonNiveauMur = (ShGUIControlRadioButton*)ShGUIControl::GetElementById(CShIdentifier("radiobutton_level_mur"), m_pPanelNiveau);
	SH_ASSERT(shNULL != pRadioButtonNiveauMur);
	ShGUIControlCheckBox * pCheckBoxNiveauObstacles = (ShGUIControlCheckBox *)ShGUIControl::GetElementById(CShIdentifier("checkbox_level_obstacles"), m_pPanelNiveau);
	SH_ASSERT(shNULL != pCheckBoxNiveauObstacles);

	m_pPanelCommandes = (ShGUIControlPanel*)ShGUIControl::GetElementById(CShIdentifier("panel_controls"), pControlRoot);
	SH_ASSERT(shNULL != m_pPanelCommandes);
	ShGUIControlButton * pButtonCommandesRetour = (ShGUIControlButton*)ShGUIControl::GetElementById(CShIdentifier("button_controls_back"), m_pPanelCommandes);
	SH_ASSERT(shNULL != pButtonCommandesRetour);
	ShGUIControlText * 	pTextCommandes = (ShGUIControlText*)ShGUIControl::GetElementById(CShIdentifier("text_controls"), m_pPanelCommandes);
	SH_ASSERT(shNULL != pTextCommandes);

	m_pPanelOptions = (ShGUIControlPanel*)ShGUIControl::GetElementById(CShIdentifier("panel_settings"), pControlRoot);
	SH_ASSERT(shNULL != m_pPanelOptions);
	ShGUIControlButton * pButtonOptionsRetour = (ShGUIControlButton*)ShGUIControl::GetElementById(CShIdentifier("button_settings_back"), m_pPanelOptions);
	SH_ASSERT(shNULL != pButtonOptionsRetour);
	ShGUIControlButton * pButtonOptionsGauche = (ShGUIControlButton*)ShGUIControl::GetElementById(CShIdentifier("button_settings_left"), m_pPanelOptions);
	SH_ASSERT(shNULL != pButtonOptionsGauche);
	ShGUIControlButton * pButtonOptionsDroite = (ShGUIControlButton*)ShGUIControl::GetElementById(CShIdentifier("button_settings_right"), m_pPanelOptions);
	SH_ASSERT(shNULL != pButtonOptionsDroite);
	ShGUIControlText * pTextInfosOptions = (ShGUIControlText*)ShGUIControl::GetElementById(CShIdentifier("text_settings_information_points"), m_pPanelOptions);
	SH_ASSERT(shNULL != pTextInfosOptions);
	ShGUIControlText * pTextOptions = (ShGUIControlText*)ShGUIControl::GetElementById(CShIdentifier("text_settings_points"), m_pPanelOptions);
	SH_ASSERT(shNULL != pTextOptions);

	//
	// Adds Slots Fct Ptr
	ShGUIControlButton::AddSlotFctPtrClick(pButtonJouer, (pSlotSDKButtonClick)ButtonJouerClick);
	ShGUIControlButton::AddSlotFctPtrClick(pButtonNiveau, (pSlotSDKButtonClick)ButtonNiveauClick);
	ShGUIControlButton::AddSlotFctPtrClick(pButtonCommandes, (pSlotSDKButtonClick)ButtonCommandesClick);
	ShGUIControlButton::AddSlotFctPtrClick(pButtonOptions, (pSlotSDKButtonClick)ButtonOptionsClick);
	ShGUIControlButton::AddSlotFctPtrClick(pButtonQuitter, (pSlotSDKButtonClick)ButtonQuitterClick);
	ShGUIControlButton::AddSlotFctPtrClick(pButtonMenu, (pSlotSDKButtonClick)ButtonMenuClick);
	ShGUIControlButton::AddSlotFctPtrClick(pButtonRejouer, (pSlotSDKButtonClick)ButtonRejouerClick);
	ShGUIControlButton::AddSlotFctPtrClick(pButtonNiveauRetour, (pSlotSDKButtonClick)ButtonRetourClick);
	ShGUIControlButton::AddSlotFctPtrClick(pButtonCommandesRetour, (pSlotSDKButtonClick)ButtonRetourClick);
	ShGUIControlButton::AddSlotFctPtrClick(pButtonOptionsRetour, (pSlotSDKButtonClick)ButtonRetourClick);
	ShGUIControlButton::AddSlotFctPtrClick(pButtonOptionsGauche, (pSlotSDKButtonClick)ButtonOptionsGaucheClick);
	ShGUIControlButton::AddSlotFctPtrClick(pButtonOptionsDroite, (pSlotSDKButtonClick)ButtonOptionsDroiteClick);

	ShGUIControl::Show(m_pPanelBackground);
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

//	ShGUIControlRadioButton::AddSlotFctPtrSelected();
}

void CGame::Update(float dt)
{
	switch (m_eGameState) 
	{
		case e_game_state_waiting:
		{
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
			ShGUIControl::Show(m_pPanelBackground);
			ShGUIControl::Show(m_pPanelFin);
		}
		break;
	}

	if (ShUser::HasTriggeredAction(m_pUser, CShIdentifier("quit")))
	{
		ShApplication::RequestQuit();
	}
}

void CGame::OnWin(int gagnant)
{
	if (gagnant == 1) // Droite
	{
		m_iPointsDroit++;
		if (m_iPointsDroit == m_iMaxPoint)
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
		if (m_iPointsGauche == m_iMaxPoint)
		{
			m_eGameState = e_game_state_end;
		}
		else
		{
			m_eGameState = e_game_state_waiting;
		}
	}
	ShTextZone::SetText(m_pPtsGauche, CShString::FromInt(m_iPointsGauche));
	ShTextZone::SetText(m_pPtsDroit, CShString::FromInt(m_iPointsDroit));
}

void CGame::Release()
{
	ShLevel::Release();
}


bool CGame::ButtonJouerClick(ShGUIControlButton * pButton)
{
	g_game.SetGameState(CGame::e_game_state_waiting);
	g_game.LaunchLevel();
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

bool CGame::ButtonOptionsGaucheClick(ShGUIControlButton * pButton)
{
	ShApplication::RequestQuit();
	return true;
}

bool CGame::ButtonOptionsDroiteClick(ShGUIControlButton * pButton)
{
	ShApplication::RequestQuit();
	
	g_game.SetMaxPoint(g_game.GetMaxPoint() + 1);
	//ShGUIControltext::SetText(g_game.GetSettingsMaxScoreText());

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

bool CGame::ButtonRejouerClick(ShGUIControlButton * pButton)
{
	g_game.Release();
	g_game.LaunchLevel();
	g_game.SetGameState(CGame::e_game_state_waiting);
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
		case e_game_state_playing:
		{
			ShGUIControl::Hide(m_pPanelBackground);
			ShGUIControl::Hide(m_pPanelMenu);
		}
		case e_game_state_waiting:
		{
			ShGUIControl::Hide(m_pPanelBackground);
			ShGUIControl::Hide(m_pPanelMenu);
			ShGUIControl::Hide(m_pPanelFin);
			m_iPointsDroit = 0;
			m_iPointsGauche = 0;
		}
		break;
		case e_game_state_menu:
		{
			CGame::Release();
			ShGUIControl::Hide(m_pPanelFin);
			ShGUIControl::Show(m_pPanelBackground);
			ShGUIControl::Show(m_pPanelMenu);
		}
		break;
		case e_game_state_end:
		{
			ShGUIControl::Show(m_pPanelBackground);
			ShGUIControl::Show(m_pPanelFin);
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
			ShGUIControl::Show(m_pPanelBackground);
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

void CGame::SetMaxPoint(int iMaxPoint)
{
	m_iMaxPoint = iMaxPoint;
}

int CGame::GetMaxPoint()
{
	return m_iMaxPoint;
}
/*
ShGUIControlText * CGame::GetSettingsMaxScoreText()
{
	return (shNULL);
}*/