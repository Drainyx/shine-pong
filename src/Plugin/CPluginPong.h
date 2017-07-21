#ifndef __C_PLUGIN_PONG_H
#define __C_PLUGIN_PONG_H

#if defined(_WIN32)
#	if defined(Plugin_Editor_EXPORTS)
#		define PLUGIN_PONG_EXPORT __declspec(dllexport)
#	else
#		define PLUGIN_PONG_EXPORT //__declspec(dllimport)
#	endif // Plugin_EXPORTS
#else // defined(_WIN32)
#	define PLUGIN_PONG_EXPORT
#endif

#include "ShSDK/ShSDK.h"
#include "ShEngineExt/ShEngineExt.h"

class CShPluginPongListener
{
public:
	virtual void OnWin(int gagnant) = 0;
};

class CPluginPong : public CShPlugin
{
public:

	enum EState
	{
		e_state_playing,
		e_state_launching,
		e_state_stopped,
	};

	enum EGagnantPoint
	{
		e_point_gagnant_none,
		e_point_gagnant_gauche,
		e_point_gagnant_droit,
	};

	//
	// Constructor / Destructor
	explicit CPluginPong(CShPluginPongListener * pListener);
	virtual ~CPluginPong(void);

	//
	// Update
	virtual	void OnPreUpdate(float dt) override;
	virtual	void OnPostUpdate(float dt) override;

	//
	// Play/Stop
	virtual	void OnPlayStart(const CShIdentifier & levelIdentifier);
	virtual	void OnPlayStop(const CShIdentifier & levelIdentifier);
	void EndGame();

protected:

	void OnPostUpdatePlaying(float dt);
	void OnPostUpdateLaunching(float dt);
	void DefaultData();
	void FindDataSets(ShObject * pObject);
	
	void HandleDataSetObstacle(ShObject * pObject, ShDataSet * pDataSet);
	void HandleDataSetBalle(ShObject * pObject, ShDataSet * pDataSet);
	void HandleDataSetMurB(ShObject * pObject, ShDataSet * pDataSet);
	void HandleDataSetMurH(ShObject * pObject, ShDataSet * pDataSet);
	void HandleDataSetRaquetteGauche(ShObject * pObject, ShDataSet * pDataSet);
	void HandleDataSetRaquetteDroit(ShObject * pObject, ShDataSet * pDataSet);

	void CreateSegmentsObstacle(ShObject * pObject);
	void CreateSegmentsMurB(ShObject * pObject);
	void CreateSegmentsMurH(ShObject * pObject);
	void CreateSegmentsRaquetteGauche(ShObject * pObject);
	void CreateSegmentsRaquetteDroit(ShObject * pObject);
	void CreateSegmentGagneGauche(ShObject * pObject);
	void CreateSegmentGagneDroit(ShObject * pObject);

	void DeplacementGauche(float dt);
	void DeplacementDroite(float dt);

public:

	bool m_bRestart;

private:

	ShEntity2 *		m_pballe;
	ShEntity2 *		m_pjGauche;
	ShEntity2 *		m_pjDroit;
	ShUser *		m_pUser;

	CShVector2		m_velocity;
	CShVector3		m_positionGauche;
	CShVector3		m_positionDroit;
	CShVector3		m_positionBalle;

	float			m_fTailleBalle;
	float			m_fHauteurRaquette;
	float			m_fLongueurRaquette;
	float			m_fVitesseBalle;
	float			m_fVitesseRaquetteGauche;
	float			m_fVitesseRaquetteDroit;
	float			m_fVitesseDefautRaquetteGauche;
	float			m_fVitesseDefautRaquetteDroit;
	float			m_fAccelerationBalleObstacle;
	float			m_fAccelerationBalleGauche;
	float			m_fAccelerationBalleDroit;
	float			m_fAccelerationBalleMur;
	float			m_fVitesseBalleDefaut;

	EState			m_eState;
	EGagnantPoint	m_eGagnantPoint;

	CShArray<CShSegment2>	m_aSegmentCollisionObstacle;
	CShSegment2				m_aSegmentCollisionRaquetteGauche[8];
	CShSegment2				m_aSegmentCollisionRaquetteDroit[8];
	CShSegment3				m_pSegmentGagneGauche;
	CShSegment3				m_pSegmentGagneDroit;
	CShArray<CShSegment2>	m_aSegmentCollisionMurB;
	CShArray<CShSegment2>	m_aSegmentCollisionMurH;

	bool			m_bdeplacementGaucheOk;
	bool			m_bdeplacementDroitOk;

	int				m_iMaxObstacles;
	
	CShRandomValue	m_random;
	
	ShDataSet *				pDataSet;
	CShPluginPongListener * m_pListener;
};

#endif // __C_PLUGIN_PONG_H
