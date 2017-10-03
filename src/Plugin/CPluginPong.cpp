#include "CPluginPong.h"

#include "ShSDK/ShSDK.h"

CPluginPong::CPluginPong(CShPluginPongListener * pListener) : CShPlugin(CShIdentifier("pong")), m_pListener(pListener), m_random((int)this)
{

}

CPluginPong::~CPluginPong(void)
{

}

void CPluginPong::OnPreUpdate(float dt)
{

}

void CPluginPong::OnPostUpdate(float dt)
{
	switch (m_eState)
	{
		case e_state_playing:
		{
			OnPostUpdatePlaying(dt);
		}
		break;

		case e_state_launching:
		{
			OnPostUpdateLaunching(dt);
		}
		break;
	}
}

void CPluginPong::OnPlayStart(const CShIdentifier & levelIdentifier)
{
	m_pUser = ShUser::GetUser(0);
	SH_ASSERT(shNULL != m_pUser);
	
	DefaultData();
	m_eGagnantPoint = e_point_gagnant_none;
	m_eState = e_state_launching;

	CShArray<ShEntity2*> aEntities;
	ShEntity2::GetEntity2Array(levelIdentifier, aEntities);

	for (int i = 0; i < aEntities.GetCount(); ++i)
	{
		ShEntity2 * pEntity = aEntities[i];
		FindDataSets(pEntity);
	}

	CShArray<ShPrefab*> aPrefabs;
	ShPrefab::GetPrefabArray(levelIdentifier, aPrefabs);

	for (int i = 0; i < aPrefabs.GetCount(); ++i)
	{
		ShPrefab * pPrefab = aPrefabs[i];
		FindDataSets(pPrefab);
	}
}

void CPluginPong::OnPlayStop(const CShIdentifier & levelIdentifier)
{

}

void CPluginPong::EndGame()
{
	m_eState = e_state_stopped;
}

void CPluginPong::OnPostUpdatePlaying(float dt)
{
	m_positionBalle = ShEntity2::GetWorldPosition(m_pballe);
	m_positionDroit = ShEntity2::GetPosition(m_pjDroit);
	m_positionGauche = ShEntity2::GetPosition(m_pjGauche);

	CShVector3 dirBalle(m_velocity.getNormalized(), 0.0f);

	float fangle_impact_face = dirBalle.DotProduct(CShVector3(1.0f, 0.0f, 0.0f));
	float fangle_impact_bord = dirBalle.DotProduct(CShVector3(0.0f, 1.0f, 0.0f));

	float fangle_impact_face_inverse = dirBalle.DotProduct(CShVector3(-1.0f, 0.0f, 0.0f));
	float fangle_impact_bord_inverse = dirBalle.DotProduct(CShVector3(0.0f, -1.0f, 0.0f));

	// Déplacement des raquettes
	if (m_bdeplacementGaucheOk)
	{
		DeplacementGauche(dt);
	}

	if (m_bdeplacementDroitOk)
	{
		DeplacementDroite(dt);
	}

	float fBoostGauche = 2500.0f;
	float fBoostDroit = 2500.0f;

	if (ShUser::HasTriggeredAction(m_pUser, CShIdentifier("boost1")))
	{
		m_fVitesseRaquetteGauche = fBoostGauche;
	}
	if (ShUser::HasTriggeredAction(m_pUser, CShIdentifier("unboost1")))
	{
		m_fVitesseRaquetteGauche = m_fVitesseDefautRaquetteGauche;
	}
	
	if (ShUser::HasTriggeredAction(m_pUser, CShIdentifier("boost2")))
	{
		m_fVitesseRaquetteDroit = fBoostDroit;
	}
	if (ShUser::HasTriggeredAction(m_pUser, CShIdentifier("unboost2")))
	{
		m_fVitesseRaquetteDroit = m_fVitesseDefautRaquetteDroit;
	}


	//Collision Balle / Raquette Gauche
	for (int i = 0; i < 8; ++i)
	{
		CShSegment2 local(m_aSegmentCollisionRaquetteGauche[i]);
		local.getP1() += ShEntity2::GetWorldPosition2(m_pjGauche);
		local.getP2() += ShEntity2::GetWorldPosition2(m_pjGauche);

		if (shDistance(m_positionBalle, CShSegment3(CShVector3(local.m_p1, 0.0), CShVector3(local.m_p2, 0.0))) <= m_fTailleBalle / 2)
		{
			m_bdeplacementGaucheOk = false;

			CShVector2 diff = local.m_p2 - local.m_p1;
			CShVector2 normal(-diff.m_y, diff.m_x);
			normal.Normalize();

			CShVector2 dir = m_velocity.getNormalized();

			float dot = dir.DotProduct(normal);

			if (dot > 0.0f)
			{
				m_velocity = dir - 2 * dot * normal;

				m_fVitesseBalle += m_fAccelerationBalleGauche;
				m_velocity = m_velocity.getNormalized() * m_fVitesseBalle;
			}
		}
		else
		{
			m_bdeplacementGaucheOk = true;
		}
	}

	// Collision Balle / Raquette Droite
	for (int i = 0; i < 8; ++i)
	{
		CShSegment2 local(m_aSegmentCollisionRaquetteDroit[i]);
		local.getP1() += ShEntity2::GetWorldPosition2(m_pjDroit);
		local.getP2() += ShEntity2::GetWorldPosition2(m_pjDroit);

		if (shDistance(m_positionBalle, CShSegment3(CShVector3(local.m_p1, 0.0), CShVector3(local.m_p2, 0.0))) <= m_fTailleBalle / 2)
		{
			m_bdeplacementDroitOk = false;

			CShVector2 diff = local.m_p2 - local.m_p1;
			CShVector2 normal(-diff.m_y, diff.m_x);
			normal.Normalize();

			CShVector2 dir = m_velocity.getNormalized();

			float dot = dir.DotProduct(normal);

			if (dot > 0.0f)
			{
				m_velocity = dir - 2 * dot * normal;

				m_fVitesseBalle += m_fAccelerationBalleDroit;
				m_velocity = m_velocity.getNormalized() * m_fVitesseBalle;
			}
		}
		else
		{
			m_bdeplacementDroitOk = true;
		}
	}

	// Collision Balle / Obstacle
	for (int i = 0; i < m_aSegmentCollisionObstacle.GetCount(); ++i)
	{
		if (shDistance(m_positionBalle, CShSegment3(CShVector3(m_aSegmentCollisionObstacle[i].getP1(), 0.0), CShVector3(m_aSegmentCollisionObstacle[i].getP2(), 0.0))) <= m_fTailleBalle / 2)
		{
			CShVector2 diff = m_aSegmentCollisionObstacle[i].getP2() - m_aSegmentCollisionObstacle[i].getP1();
			CShVector2 normal(-diff.m_y, diff.m_x);
			normal.Normalize();

			CShVector2 dir = m_velocity.getNormalized();

			float dot = dir.DotProduct(normal);

			if (dot > 0.0f)
			{
				m_velocity = dir - 2 * dot * normal;

				m_fVitesseBalle += m_fAccelerationBalleObstacle;
				m_velocity = m_velocity.getNormalized() * m_fVitesseBalle;
			}
		}
	}

	ShEntity2::Translate(m_pballe, CShVector3(m_velocity, 0.0f) * dt);

	// Lorsqu' un joueur gagne un point
	if (shDistance(m_positionBalle, m_pSegmentGagneDroit) <= m_fTailleBalle / 2)
	{
	
		m_eGagnantPoint = e_point_gagnant_droit;
		m_eState = e_state_launching;
		if (shNULL != m_pListener)
		{
			m_pListener->OnWin(1);
		}
	}
	else if (shDistance(m_positionBalle, m_pSegmentGagneGauche) <= m_fTailleBalle / 2)
	{

		m_eGagnantPoint = e_point_gagnant_gauche;
		m_eState = e_state_launching;
		if (shNULL != m_pListener)
		{
			m_pListener->OnWin(2);
		}
	}
}

void CPluginPong::OnPostUpdateLaunching(float dt)
{
	m_positionDroit = ShEntity2::GetWorldPosition(m_pjDroit);
	m_positionGauche = ShEntity2::GetWorldPosition(m_pjGauche);

	m_fVitesseRaquetteGauche = m_fVitesseDefautRaquetteGauche;
	m_fVitesseRaquetteDroit = m_fVitesseDefautRaquetteDroit;

	switch (m_eGagnantPoint)
	{
	case e_point_gagnant_none:
	{
		ShEntity2::SetPosition2(m_pballe, 0.0f, 0.0f);
	}
	break;
	case e_point_gagnant_droit:
	{
		ShEntity2::SetPosition2(m_pballe, m_positionGauche.m_x + m_fLongueurRaquette / 2 + m_fTailleBalle / 2 + 1, m_positionGauche.m_y);
	}
	break;
	case e_point_gagnant_gauche:
	{
		ShEntity2::SetPosition2(m_pballe, m_positionDroit.m_x - m_fLongueurRaquette / 2 - m_fTailleBalle / 2 - 1, m_positionDroit.m_y);
	}
	break;
	}

	float random_x;
	float random_y;

	do {
		random_x = m_random.GetNextFloat(-1.5f, 1.5f);
		random_y = m_random.GetNextFloat(-1.5f, 1.5f);
	} while ((random_x > -0.4 && random_x < 0.4) || (random_y > -0.4 && random_y < 0.4));

	CShVector2 direction(random_x, random_y);
	m_velocity = direction * m_fVitesseBalleDefaut;

	// Déplacement des raquettes
	if (m_bdeplacementGaucheOk)
	{
		DeplacementGauche(dt);
	}

	if (m_bdeplacementDroitOk)
	{
		DeplacementDroite(dt);
	}

	if (ShUser::HasTriggeredAction(m_pUser, CShIdentifier("play")))
	{
		m_eState = e_state_playing;
		m_fVitesseBalle = m_fVitesseBalleDefaut;
		m_velocity = m_velocity.getNormalized() * m_fVitesseBalle;
	}
}

void CPluginPong::FindDataSets(ShObject * pObject)
{
	unsigned int counting = ShObject::GetDataSetCount(pObject);

	for (int j = 0; j < counting; ++j)
	{
		ShDataSet * pDataSet = ShEntity2::GetDataSet(pObject, 0);

		if (shNULL != pDataSet)
		{
			const CShIdentifier & identifier = ShDataSet::GetDataSetIdentifier(pDataSet);

			if (identifier == CShIdentifier("pong_obstacle"))
			{
				HandleDataSetObstacle(pObject, pDataSet);
			}
			else if (identifier == CShIdentifier("pong_balle"))
			{
				HandleDataSetBalle(pObject, pDataSet);
			}
			else if (identifier == CShIdentifier("pong_murB"))
			{
				HandleDataSetMurB(pObject, pDataSet);
			}
			else if (identifier == CShIdentifier("pong_murH"))
			{
				HandleDataSetMurH(pObject, pDataSet);
			}
			else if (identifier == CShIdentifier("pong_raquetteGauche"))
			{
				HandleDataSetRaquetteGauche(pObject, pDataSet);
			}
			else if (identifier == CShIdentifier("pong_raquetteDroit"))
			{
				HandleDataSetRaquetteDroit(pObject, pDataSet);
			}
			else if (identifier == CShIdentifier("pong_gagneGauche"))
			{
				CreateSegmentGagneGauche(pObject);
			}
			else if (identifier == CShIdentifier("pong_gagneDroit"))
			{
				CreateSegmentGagneDroit(pObject);
			}
		}
	}
}

void CPluginPong::HandleDataSetObstacle(ShObject * pObject, ShDataSet * pDataSet)
{
	CreateSegmentsObstacle(pObject);

	unsigned int countData = ShDataSet::GetDataCount(pDataSet);

	for (int i = 0; i < countData; ++i)
	{
		const CShIdentifier & identifier = ShDataSet::GetDataIdentifier(pDataSet, i);

		if (identifier == CShIdentifier("acceleration_ball"))
		{
			bool ok = ShDataSet::GetDataValue(pDataSet, i, m_fAccelerationBalleObstacle);
			SH_ASSERT(ok);
		}
	}
}

void CPluginPong::HandleDataSetBalle(ShObject * pObject, ShDataSet * pDataSet)
{
	unsigned int count = ShDataSet::GetDataCount(pDataSet);

	for (int i = 0; i < count; ++i)
	{
		const CShIdentifier & identifier = ShDataSet::GetDataIdentifier(pDataSet, i);

		if (identifier == CShIdentifier("velocity_ball"))
		{
			bool ok = ShDataSet::GetDataValue(pDataSet, i, m_fVitesseBalleDefaut);
			SH_ASSERT(ok);
		}
	}
	if (ShObject::GetTypeIdentifier(pObject) == CShIdentifier("entity2"))
	{
		m_pballe = (ShEntity2 *)pObject;
		SH_ASSERT(shNULL != m_pballe);
		m_fTailleBalle = ShEntity2::GetWidth(m_pballe);
	}
}


void CPluginPong::HandleDataSetMurB(ShObject * pObject, ShDataSet * pDataSet)
{
	CreateSegmentsMurB(pObject);

	unsigned int count = ShDataSet::GetDataCount(pDataSet);

	for (int i = 0; i < count; ++i)
	{
		const CShIdentifier & identifier = ShDataSet::GetDataIdentifier(pDataSet, i);

		if (identifier == CShIdentifier("acceleration_ball"))
		{
			bool ok = ShDataSet::GetDataValue(pDataSet, i, m_fAccelerationBalleMur);
			SH_ASSERT(ok);
		}
	}
}

void CPluginPong::HandleDataSetMurH(ShObject * pObject, ShDataSet * pDataSet)
{
	CreateSegmentsMurH(pObject);

	unsigned int count = ShDataSet::GetDataCount(pDataSet);

	for (int i = 0; i < count; ++i)
	{
		const CShIdentifier & identifier = ShDataSet::GetDataIdentifier(pDataSet, i);

		if (identifier == CShIdentifier("acceleration_ball"))
		{
			bool ok = ShDataSet::GetDataValue(pDataSet, i, m_fAccelerationBalleMur);
			SH_ASSERT(ok);
		}
	}
}

void CPluginPong::HandleDataSetRaquetteGauche(ShObject * pObject, ShDataSet * pDataSet)
{
	CreateSegmentsRaquetteGauche(pObject);

	unsigned int count = ShDataSet::GetDataCount(pDataSet);

	for (int i = 0; i < count; ++i)
	{
		const CShIdentifier & identifier = ShDataSet::GetDataIdentifier(pDataSet, i);

		if (identifier == CShIdentifier("velocity_racquet"))
		{
			bool ok = ShDataSet::GetDataValue(pDataSet, i, m_fVitesseRaquetteGauche);
			SH_ASSERT(ok);
		}
		else if (identifier == CShIdentifier("acceleration_ball"))
		{
			bool ok = ShDataSet::GetDataValue(pDataSet, i, m_fAccelerationBalleGauche);
			SH_ASSERT(ok);
		}
	}
}

void CPluginPong::HandleDataSetRaquetteDroit(ShObject * pObject, ShDataSet * pDataSet)
{
	CreateSegmentsRaquetteDroit(pObject);

	unsigned int count = ShDataSet::GetDataCount(pDataSet);

	for (int i = 0; i < count; ++i)
	{
		const CShIdentifier & identifier = ShDataSet::GetDataIdentifier(pDataSet, i);

		if (identifier == CShIdentifier("velocity_racquet"))
		{
			bool ok = ShDataSet::GetDataValue(pDataSet, i, m_fVitesseRaquetteDroit);
			SH_ASSERT(ok);
		}
		else if (identifier == CShIdentifier("acceleration_ball"))
		{
			bool ok = ShDataSet::GetDataValue(pDataSet, i, m_fAccelerationBalleDroit);
			SH_ASSERT(ok);
		}
	}
}

void CPluginPong::CreateSegmentsObstacle(ShObject * pObject)
{
	CShArray<ShObject*> achildren;
	ShObject::GetChildArray(pObject, achildren);

	for (int n = 0; n < achildren.GetCount(); ++n)
	{
		if (ShObject::GetTypeIdentifier(achildren[n]) == CShIdentifier("collision_shape"))
		{
			ShCollisionShape * pCollisionShape = (ShCollisionShape*)achildren[n];

			unsigned int countPoint = ShCollisionShape::GetPointCount(pCollisionShape);

			CShArray<CShVector2> aPointCollision;

			for (int i = 0; i <= countPoint; ++i)
			{
				const CShVector2 pos = ShObject::GetWorldPosition2(pObject);

				aPointCollision.Add(ShCollisionShape::GetPoint(pCollisionShape, i));

				if (i > 0)
				{
					m_aSegmentCollisionObstacle.Add(CShSegment2(pos + aPointCollision[i - 1], pos + aPointCollision[i]));
				}
				if (i == countPoint)
				{
					m_aSegmentCollisionObstacle.Add(CShSegment2(pos + aPointCollision[i], pos + aPointCollision[0]));
				}
			}
		}
	}
}

void CPluginPong::CreateSegmentsMurH(ShObject * pObject)
{
	CShArray<ShObject*> achildren;
	ShObject::GetChildArray(pObject, achildren);

	for (int n = 0; n < achildren.GetCount(); ++n)
	{
		if (ShObject::GetTypeIdentifier(achildren[n]) == CShIdentifier("collision_segment"))
		{
			ShCollisionSegment * pCollisionSegment = (ShCollisionSegment*)achildren[n];

			const CShVector2 pos = ShObject::GetWorldPosition2(pObject);

			CShSegment2 Segment = CShSegment2(pos + ShCollisionSegment::GetP1(pCollisionSegment), pos + ShCollisionSegment::GetP2(pCollisionSegment));

			m_aSegmentCollisionMurH.Add(Segment);
			m_aSegmentCollisionObstacle.Add(Segment);
		}
	}
}

void CPluginPong::CreateSegmentsMurB(ShObject * pObject)
{
	CShArray<ShObject*> achildren;
	ShObject::GetChildArray(pObject, achildren);

	for (int n = 0; n < achildren.GetCount(); ++n)
	{
		if (ShObject::GetTypeIdentifier(achildren[n]) == CShIdentifier("collision_segment"))
		{
			ShCollisionSegment * pCollisionSegment = (ShCollisionSegment*)achildren[n];

			const CShVector2 pos = ShObject::GetWorldPosition2(pObject);

			CShSegment2 Segment = CShSegment2(pos + ShCollisionSegment::GetP1(pCollisionSegment), pos + ShCollisionSegment::GetP2(pCollisionSegment));

			m_aSegmentCollisionMurB.Add(Segment);
			m_aSegmentCollisionObstacle.Add(Segment);
		}
	}
}

void CPluginPong::CreateSegmentsRaquetteGauche(ShObject * pObject) 
{
	CShArray<ShObject*> achildren;
	ShObject::GetChildArray(pObject, achildren);

	for (int n = 0; n < achildren.GetCount(); ++n)
	{
		if (ShObject::GetTypeIdentifier(achildren[n]) == CShIdentifier("collision_shape"))
		{
			ShCollisionShape * pCollisionShape = (ShCollisionShape*)achildren[n];

			unsigned int countPoint = ShCollisionShape::GetPointCount(pCollisionShape);

			CShArray<CShVector2> aPointCollision;

			for (int i = 0; i <= countPoint; ++i)
			{
				aPointCollision.Add(ShCollisionShape::GetPoint(pCollisionShape, i));
			}

			m_aSegmentCollisionRaquetteGauche[0] = CShSegment2(aPointCollision[0], aPointCollision[1]);
			m_aSegmentCollisionRaquetteGauche[1] = CShSegment2(aPointCollision[1], aPointCollision[2]);
			m_aSegmentCollisionRaquetteGauche[2] = CShSegment2(aPointCollision[2], aPointCollision[3]);
			m_aSegmentCollisionRaquetteGauche[3] = CShSegment2(aPointCollision[3], aPointCollision[4]);
			m_aSegmentCollisionRaquetteGauche[4] = CShSegment2(aPointCollision[4], aPointCollision[5]);
			m_aSegmentCollisionRaquetteGauche[5] = CShSegment2(aPointCollision[5], aPointCollision[6]);
			m_aSegmentCollisionRaquetteGauche[6] = CShSegment2(aPointCollision[6], aPointCollision[7]);
			m_aSegmentCollisionRaquetteGauche[7] = CShSegment2(aPointCollision[7], aPointCollision[0]);
		}
		else if (ShObject::GetTypeIdentifier(achildren[n]) == CShIdentifier("entity2"))
		{
			m_pjGauche = (ShEntity2 *)achildren[n];
			SH_ASSERT(shNULL != m_pjGauche);
			m_fLongueurRaquette = ShEntity2::GetWidth(m_pjGauche);
			m_fHauteurRaquette = ShEntity2::GetHeight(m_pjGauche);
		}
	}
}

void CPluginPong::CreateSegmentsRaquetteDroit(ShObject * pObject)
{
	CShArray<ShObject*> achildren;
	ShObject::GetChildArray(pObject, achildren);

	for (int n = 0; n < achildren.GetCount(); ++n)
	{
		if (ShObject::GetTypeIdentifier(achildren[n]) == CShIdentifier("collision_shape"))
		{
			ShCollisionShape * pCollisionShape = (ShCollisionShape*)achildren[n];

			unsigned int countPoint = ShCollisionShape::GetPointCount(pCollisionShape);

			CShArray<CShVector2> aPointCollision;

			for (int i = 0; i <= countPoint; ++i)
			{
				aPointCollision.Add(ShCollisionShape::GetPoint(pCollisionShape, i));
			}

			m_aSegmentCollisionRaquetteDroit[0] = CShSegment2(aPointCollision[0], aPointCollision[1]);
			m_aSegmentCollisionRaquetteDroit[1] = CShSegment2(aPointCollision[1], aPointCollision[2]);
			m_aSegmentCollisionRaquetteDroit[2] = CShSegment2(aPointCollision[2], aPointCollision[3]);
			m_aSegmentCollisionRaquetteDroit[3] = CShSegment2(aPointCollision[3], aPointCollision[4]);
			m_aSegmentCollisionRaquetteDroit[4] = CShSegment2(aPointCollision[4], aPointCollision[5]);
			m_aSegmentCollisionRaquetteDroit[5] = CShSegment2(aPointCollision[5], aPointCollision[6]);
			m_aSegmentCollisionRaquetteDroit[6] = CShSegment2(aPointCollision[6], aPointCollision[7]);
			m_aSegmentCollisionRaquetteDroit[7] = CShSegment2(aPointCollision[7], aPointCollision[0]);
		}
		else if (ShObject::GetTypeIdentifier(achildren[n]) == CShIdentifier("entity2"))
		{
			m_pjDroit = (ShEntity2 *)achildren[n];
			SH_ASSERT(shNULL != m_pjDroit);
			m_fLongueurRaquette = ShEntity2::GetWidth(m_pjDroit);
			m_fHauteurRaquette = ShEntity2::GetHeight(m_pjDroit);
		}
	}
}

void CPluginPong::CreateSegmentGagneGauche(ShObject * pObject)
{
	CShArray<ShObject*> achildren;
	ShObject::GetChildArray(pObject, achildren);

	for (int n = 0; n < achildren.GetCount(); ++n)
	{
		if (ShObject::GetTypeIdentifier(achildren[n]) == CShIdentifier("collision_segment"))
		{
			ShCollisionSegment * pCollisionSegment = (ShCollisionSegment*)achildren[n];
			
			const CShVector2 pos = ShObject::GetWorldPosition2(pObject);

			CShVector3 point1 = CShVector3(pos + ShCollisionSegment::GetP1(pCollisionSegment), 0.0f);
			CShVector3 point2 = CShVector3(pos + ShCollisionSegment::GetP2(pCollisionSegment), 0.0f);

			m_pSegmentGagneGauche = CShSegment3(point1, point2);
		}
	}
}


void CPluginPong::CreateSegmentGagneDroit(ShObject * pObject)
{
	CShArray<ShObject*> achildren;
	ShObject::GetChildArray(pObject, achildren);

	for (int n = 0; n < achildren.GetCount(); ++n)
	{
		if (ShObject::GetTypeIdentifier(achildren[n]) == CShIdentifier("collision_segment"))
		{
			ShCollisionSegment * pCollisionSegment = (ShCollisionSegment*)achildren[n];
			
			const CShVector2 pos = ShObject::GetWorldPosition2(pObject);

			CShVector3 point1 = CShVector3(pos + ShCollisionSegment::GetP1(pCollisionSegment), 0.0f);
			CShVector3 point2 = CShVector3(pos + ShCollisionSegment::GetP2(pCollisionSegment), 0.0f);

			m_pSegmentGagneDroit = CShSegment3(point1, point2);
		}
	}
}

void CPluginPong::DeplacementGauche(float dt)
{
	if (ShUser::HasTriggeredAction(m_pUser, CShIdentifier("up1")))
	{
		for (int i = 0; i < m_aSegmentCollisionMurH.GetCount(); ++i)
		{
			if (shDistance(m_positionGauche, CShSegment3(CShVector3(m_aSegmentCollisionMurH[i].getP1(), 0.0), CShVector3(m_aSegmentCollisionMurH[i].getP2(), 0.0))) > m_fHauteurRaquette / 2 + 12)
			{
				ShEntity2::Translate(m_pjGauche, CShVector3(0.0f, m_fVitesseRaquetteGauche, 0.0f) * dt);
			}
		}
	}
	else if (ShUser::HasTriggeredAction(m_pUser, CShIdentifier("down1")))
	{
		for (int i = 0; i < m_aSegmentCollisionMurB.GetCount(); ++i)
		{
			if (shDistance(m_positionGauche, CShSegment3(CShVector3(m_aSegmentCollisionMurB[i].getP1(), 0.0), CShVector3(m_aSegmentCollisionMurB[i].getP2(), 0.0))) > m_fHauteurRaquette / 2 + 12)
			{
				ShEntity2::Translate(m_pjGauche, CShVector3(0.0f, -m_fVitesseRaquetteGauche, 0.0f) * dt);
			}
		}
	}
}

void CPluginPong::DeplacementDroite(float dt)
{
	if (ShUser::HasTriggeredAction(m_pUser, CShIdentifier("up2")) && m_bdeplacementDroitOk)
	{
		for (int i = 0; i < m_aSegmentCollisionMurH.GetCount(); ++i)
		{
			if (shDistance(m_positionDroit, CShSegment3(CShVector3(m_aSegmentCollisionMurH[i].getP1(), 0.0), CShVector3(m_aSegmentCollisionMurH[i].getP2(), 0.0))) > m_fHauteurRaquette / 2 + 12)
			{
				ShEntity2::Translate(m_pjDroit, CShVector3(0.0f, m_fVitesseRaquetteDroit, 0.0f) * dt);
			}
		}
	}
	else if (ShUser::HasTriggeredAction(m_pUser, CShIdentifier("down2")) && m_bdeplacementDroitOk)
	{
		for (int i = 0; i < m_aSegmentCollisionMurB.GetCount(); ++i)
		{
			if (shDistance(m_positionDroit, CShSegment3(CShVector3(m_aSegmentCollisionMurB[i].getP1(), 0.0), CShVector3(m_aSegmentCollisionMurB[i].getP2(), 0.0))) > m_fHauteurRaquette / 2 + 12)
			{
				ShEntity2::Translate(m_pjDroit, CShVector3(0.0f, -m_fVitesseRaquetteDroit, 0.0f) * dt);
			}
		}
	}
}

void CPluginPong::DefaultData()
{
	m_fVitesseDefautRaquetteGauche = 1000.0f;
	m_fVitesseDefautRaquetteDroit = 1000.0f;
	m_fVitesseBalle = 700.0f;

	m_bdeplacementDroitOk = true;
	m_bdeplacementGaucheOk = true;
}