#ifndef __C_GAME_H
#define __C_GAME_H

#include <ShSDK/ShSDK.h>

class CGame : public CShPluginPongListener
{
public:

	enum EGameState
	{
		e_game_state_menu,
		e_game_state_playing,
		e_game_state_waiting,
		e_game_state_end,
		e_game_state_restarting,
	};

	enum EMenuState
	{
		e_menu_state_accueil,
		e_menu_state_niveau,
		e_menu_state_commandes,
		e_menu_state_options,
	};

	CGame();
	~CGame();

	void Initialize();
	void Update(float dt);
	void Release();
	void LaunchLevel(void);

	virtual void OnWin(int gagnant);
	
	void SetGameState(EGameState eGameState);
	void SetMenuState(EMenuState eMenuState);

public:
	//
	// Getters/Setters
	void		SetMaxPoint	(int iMaxPoint);
	int			GetMaxPoint	(void);

public:
	bool m_bRestart;

	static bool ButtonJouerClick(ShGUIControlButton * pButton);
	static bool ButtonNiveauClick(ShGUIControlButton * pButton);
	static bool ButtonCommandesClick(ShGUIControlButton * pButton);
	static bool ButtonOptionsClick(ShGUIControlButton * pButton);
	static bool ButtonQuitterClick(ShGUIControlButton * pButton);
	static bool ButtonMenuClick(ShGUIControlButton * pButton);
	static bool ButtonRetourClick(ShGUIControlButton * pButton);
	static bool ButtonOptionsGaucheClick(ShGUIControlButton * pButton);
	static bool ButtonOptionsDroiteClick(ShGUIControlButton * pButton);

private:
	CShIdentifier	m_GameLevel;

	ShEntity2 *		m_pbackground1;
	ShEntity2 *		m_pbackground2;

	ShUser *		m_pUser;
	
	EGameState		m_eGameState;
	EMenuState		m_eMenuState;


	int				m_iPointsGauche;
	int				m_iPointsDroit;
	int				m_iMaxPoint;

	ShTextZone *	m_pInfosGauche;
	ShTextZone *	m_pInfosDroite;
	ShTextZone *	m_pPtsGauche;
	ShTextZone *	m_pPtsDroit;
	ShTextZone *	m_pInfosJeu;
	ShTextZone *	m_pGauche;
	ShTextZone *	m_pDroit;
	ShTextZone *	m_pScoreGauche;
	ShTextZone *	m_pScoreUnion;
	ShTextZone *	m_pScoreDroite;

	ShGUIControlPanel * m_pPanelMenu;
	ShGUIControlPanel * m_pPanelFin;
	ShGUIControlPanel * m_pPanelNiveau;
	ShGUIControlPanel * m_pPanelCommandes;
	ShGUIControlPanel * m_pPanelOptions;
};

#endif // __C_GAME_H
