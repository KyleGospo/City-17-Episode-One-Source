//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: area portal entity: toggles visibility areas on/off
//
// NOTE: These are not really brush entities.  They are brush entities from a 
// designer/worldcraft perspective, but by the time they reach the game, the 
// brush model is gone and this is, in effect, a point entity.
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "func_areaportalbase.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

enum areaportal_state
{
	AREAPORTAL_CLOSED = 0,
	AREAPORTAL_OPEN = 1,
};


class CAreaPortal : public CFuncAreaPortalBase
{
public:
	DECLARE_CLASS( CAreaPortal, CFuncAreaPortalBase );

					CAreaPortal();

	virtual void	Spawn( void );
	virtual void	Precache( void );
	virtual void	Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	virtual bool	KeyValue( const char *szKeyName, const char *szValue );
	virtual int		UpdateTransmitState();

	// Input handlers
	void InputOpen( inputdata_t &inputdata );
	void InputClose( inputdata_t &inputdata );
	void InputToggle( inputdata_t &inputdata );

	virtual bool	UpdateVisibility( const Vector &vOrigin, float fovDistanceAdjustFactor, bool &bIsOpenOnClient );

	DECLARE_DATADESC();

private:
	bool	UpdateState( void );

protected:
	int		m_state;
};

LINK_ENTITY_TO_CLASS( func_areaportal, CAreaPortal );

BEGIN_DATADESC( CAreaPortal )

	DEFINE_KEYFIELD( m_portalNumber, FIELD_INTEGER, "portalnumber" ),
	DEFINE_FIELD( m_state, FIELD_INTEGER ),

	// Inputs
	DEFINE_INPUTFUNC( FIELD_VOID, "Open",  InputOpen ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Close", InputClose ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Toggle",  InputToggle ),

	// TODO: obsolete! remove	
	DEFINE_INPUTFUNC( FIELD_VOID, "TurnOn",  InputClose ),
	DEFINE_INPUTFUNC( FIELD_VOID, "TurnOff", InputOpen ),

END_DATADESC()



CAreaPortal::CAreaPortal()
{
	m_state = AREAPORTAL_OPEN;
}


void CAreaPortal::Spawn( void )
{
	AddEffects( EF_NORECEIVESHADOW | EF_NOSHADOW );
	Precache();
}


//-----------------------------------------------------------------------------
// Purpose: notify the engine of the state at startup/restore
//-----------------------------------------------------------------------------
void CAreaPortal::Precache( void )
{
	UpdateState();
}


//------------------------------------------------------------------------------
// Purpose :
//------------------------------------------------------------------------------
void CAreaPortal::InputClose( inputdata_t &inputdata )
{
	m_state = AREAPORTAL_CLOSED;
	UpdateState();
}


//------------------------------------------------------------------------------
// Purpose :
//------------------------------------------------------------------------------
void CAreaPortal::InputOpen( inputdata_t &inputdata )
{
	m_state = AREAPORTAL_OPEN;
	UpdateState();
}


//------------------------------------------------------------------------------
// Purpose :
//------------------------------------------------------------------------------
void CAreaPortal::InputToggle( inputdata_t &inputdata )
{
	m_state = ((m_state == AREAPORTAL_OPEN) ? AREAPORTAL_CLOSED : AREAPORTAL_OPEN);
	UpdateState();
}


bool CAreaPortal::UpdateVisibility( const Vector &vOrigin, float fovDistanceAdjustFactor, bool &bIsOpenOnClient )
{
	if ( m_state )
	{
		// We're not closed, so give the base class a chance to close it.
		return BaseClass::UpdateVisibility( vOrigin, fovDistanceAdjustFactor, bIsOpenOnClient );
	}
	else
	{
		bIsOpenOnClient = false;
		return false;
	}
}


//------------------------------------------------------------------------------
// Purpose :
// Input   :
// Output  :
//------------------------------------------------------------------------------
void CAreaPortal::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if ( useType == USE_ON )
	{
		m_state = AREAPORTAL_OPEN;
	}
	else if ( useType == USE_OFF )
	{
		m_state = AREAPORTAL_CLOSED;
	}
	else
	{
		return;
	}

	UpdateState();
}


bool CAreaPortal::KeyValue( const char *szKeyName, const char *szValue )
{
	if( FStrEq( szKeyName, "StartOpen" ) )
	{
		m_state = (atoi(szValue) != 0) ? AREAPORTAL_OPEN : AREAPORTAL_CLOSED;

		return true;
	}
	else
	{
		return BaseClass::KeyValue( szKeyName, szValue );
	}
}


bool CAreaPortal::UpdateState()
{
	engine->SetAreaPortalState( m_portalNumber, m_state );
	return !!m_state;
}


int CAreaPortal::UpdateTransmitState()
{
	// Our brushes are kept around so don't transmit anything since we don't want to draw them.
	return SetTransmitState( FL_EDICT_DONTSEND );
}

class CAreaPortalOneWay : public CAreaPortal
{
	DECLARE_CLASS( CAreaPortalOneWay, CAreaPortal );
	DECLARE_DATADESC();
 
public:
	Vector	 m_vecOpenVector;
	bool	 m_bAvoidPop;
	bool	 m_bOneWayActive;
 
	void Spawn();
	void Activate();
	int  Restore(IRestore &restore);
	bool UpdateVisibility( const Vector &vOrigin, float fovDistanceAdjustFactor, bool &bIsOpenOnClient );
 
	void InputDisableOneWay( inputdata_t &inputdata );
	void InputEnableOneWay( inputdata_t &inputdata );
	void InputToggleOneWay( inputdata_t &inputdata );
	void InputInvertOneWay( inputdata_t &inputdata );
 
protected:
	void RemoteUpdate( bool IsOpen );
 
	bool m_bRemotelyUpdated;
	bool m_bRemoteCalcWasOpen;
	CHandle<CAreaPortalOneWay> m_hNextPortal;
	CAreaPortalOneWay* m_pNextPortal;
 
private:
	void UpdateNextPortal( bool IsOpen );
 
	string_t m_strGroupName;
	Vector	 m_vecOrigin_;
};
 
LINK_ENTITY_TO_CLASS( func_areaportal_oneway, CAreaPortalOneWay );
 
BEGIN_DATADESC( CAreaPortalOneWay )
	DEFINE_KEYFIELD( m_vecOpenVector, FIELD_VECTOR, "onewayfacing" ),
	DEFINE_KEYFIELD( m_bAvoidPop, FIELD_BOOLEAN, "avoidpop" ),
	DEFINE_KEYFIELD_NOT_SAVED( m_vecOrigin_, FIELD_VECTOR, "origin_" ),
	DEFINE_KEYFIELD_NOT_SAVED( m_strGroupName, FIELD_STRING, "group" ),
	DEFINE_FIELD( m_bOneWayActive, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_hNextPortal, FIELD_EHANDLE ),
	DEFINE_INPUTFUNC( FIELD_VOID, "DisableOneWay", InputDisableOneWay ),
	DEFINE_INPUTFUNC( FIELD_VOID, "EnableOneWay", InputEnableOneWay ),
	DEFINE_INPUTFUNC( FIELD_VOID, "ToggleOneWay", InputToggleOneWay ),
	DEFINE_INPUTFUNC( FIELD_VOID, "InvertOneWay", InputInvertOneWay ),
END_DATADESC()
 
void CAreaPortalOneWay::Spawn()
{
	QAngle angOpenDir = QAngle( m_vecOpenVector.x, m_vecOpenVector.y, m_vecOpenVector.z );
	AngleVectors( angOpenDir, &m_vecOpenVector );
 
	SetLocalOrigin(m_vecOrigin_);
	m_bOneWayActive = true;
	m_bRemotelyUpdated = false;
 
	BaseClass::Spawn();
}
 
void CAreaPortalOneWay::Activate()
{
	if (m_strGroupName != NULL_STRING)
		for( unsigned short i = m_AreaPortalsElement; i != g_AreaPortals.InvalidIndex(); i = g_AreaPortals.Next(i) )
		{
			CAreaPortalOneWay* pCur = dynamic_cast<CAreaPortalOneWay*>(g_AreaPortals[i]);
 
			if ( pCur && pCur != this && strcmp( STRING(m_strGroupName),STRING(pCur->m_strGroupName) ) == 0 )
			{
				m_pNextPortal = pCur;
				m_hNextPortal = pCur;
				break;
			}
		}
 
	BaseClass::Activate();
}
 
int CAreaPortalOneWay::Restore(IRestore &restore)
{
	if ( m_hNextPortal.IsValid() )
		m_pNextPortal = m_hNextPortal.Get();
 
	return BaseClass::Restore(restore);
}
 
void CAreaPortalOneWay::InputDisableOneWay( inputdata_t &inputdata )
{
	m_bOneWayActive = false;
}
 
void CAreaPortalOneWay::InputEnableOneWay( inputdata_t &inputdata )
{
	m_bOneWayActive = true;
}
 
void CAreaPortalOneWay::InputToggleOneWay( inputdata_t &inputdata )
{
	m_bOneWayActive = !m_bOneWayActive;
}
 
void CAreaPortalOneWay::InputInvertOneWay( inputdata_t &inputdata )
{
	m_vecOpenVector.Negate();
}

void CAreaPortalOneWay::RemoteUpdate( bool IsOpen )
{
	m_bRemotelyUpdated = true;
	m_bRemoteCalcWasOpen = IsOpen;
	UpdateNextPortal(IsOpen);
}

inline void CAreaPortalOneWay::UpdateNextPortal( bool IsOpen )
{
	if (m_pNextPortal)
		m_pNextPortal->RemoteUpdate(IsOpen);
}
 
#define VIEWER_PADDING 80
 
bool CAreaPortalOneWay::UpdateVisibility( const Vector &vOrigin, float fovDistanceAdjustFactor, bool &bIsOpenOnClient )
{
	if (!m_bOneWayActive)
		return BaseClass::UpdateVisibility( vOrigin, fovDistanceAdjustFactor, bIsOpenOnClient );
 
	if( m_portalNumber == -1 || m_state == AREAPORTAL_CLOSED )
	{
		bIsOpenOnClient = false;
		return false;
	}

	if (m_bRemotelyUpdated)
	{
		m_bRemotelyUpdated = false;
		return m_bRemoteCalcWasOpen ? BaseClass::UpdateVisibility( vOrigin, fovDistanceAdjustFactor, bIsOpenOnClient ) : false;
	}
 
	float dist = VIEWER_PADDING;
	VPlane plane;
	if( engine->GetAreaPortalPlane(vOrigin,m_portalNumber,&plane) )
		dist = plane.DistTo(vOrigin);
 
	float dot = DotProduct(m_vecOpenVector,vOrigin - GetLocalOrigin());
 
	if( dot > 0 )
	{
		UpdateNextPortal(true);

		return dist < -VIEWER_PADDING ? false : true;
	}
	else
	{
		if ( !m_bAvoidPop || (m_bAvoidPop && dist > VIEWER_PADDING) )
			bIsOpenOnClient = false;

		UpdateNextPortal(false);
		return false;
	}
}