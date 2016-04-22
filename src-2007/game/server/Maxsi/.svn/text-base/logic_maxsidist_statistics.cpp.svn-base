//========= Copyright © 2009, Team Gear & Maxsi Distribution, All rights reserved. =========
//	
//	Defines a logical entity which passes statistics calls to the Maxsi Distribution DLL, for use in maps.
//
//==========================================================================================

#include "cbase.h"
#include "entityinput.h"
#include "entityoutput.h"

// Maxsi Distribution
#include "Maxsi/MaxsiDistributionSDK.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// Allows map logic to send Maxsi Distribution statistics to the MD DLL.
class CLogicMaxsiStats : public CLogicalEntity
{
public:
	DECLARE_CLASS( CLogicMaxsiStats, CLogicalEntity );

	CLogicMaxsiStats();

protected:

	// Inputs
	void InputSendStats( inputdata_t &inputdata );
	void InputSetFunctionTo( inputdata_t &inputdata );
	void InputSetValueTo( inputdata_t &inputdata );
	void InputEnable( inputdata_t &inputdata );
	void InputDisable( inputdata_t &inputdata );
	void InputToggle( inputdata_t &inputdata );
	
	bool			m_bDisabled;
	string_t		m_iszStatisticsFunction;
	string_t		m_iszStatisticsValue;

	COutputEvent	m_OnSentStats;

	DECLARE_DATADESC();
};


LINK_ENTITY_TO_CLASS( logic_maxsidist_statistics, CLogicMaxsiStats );


BEGIN_DATADESC( CLogicMaxsiStats )

	DEFINE_KEYFIELD( m_bDisabled, FIELD_BOOLEAN, "StartDisabled" ),
	DEFINE_KEYFIELD( m_iszStatisticsFunction, FIELD_STRING, "StatisticsFunction" ),
	DEFINE_KEYFIELD( m_iszStatisticsValue, FIELD_STRING, "StatisticsValue" ),

	// Inputs
	DEFINE_INPUTFUNC( FIELD_VOID, "SendStats", InputSendStats ),
	DEFINE_INPUTFUNC( FIELD_STRING, "SetFunctionTo", InputSetFunctionTo ),
	DEFINE_INPUTFUNC( FIELD_STRING, "SetValueTo", InputSetValueTo ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Enable", InputEnable ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Disable", InputDisable ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Toggle", InputToggle ),

	// Outputs
	DEFINE_OUTPUT( m_OnSentStats, "OnSentStats" ),

END_DATADESC()



//-----------------------------------------------------------------------------
// Purpose: Constructor.
//-----------------------------------------------------------------------------
CLogicMaxsiStats::CLogicMaxsiStats( void )
{
	m_iszStatisticsFunction		=	NULL_STRING;
	m_iszStatisticsValue		=	NULL_STRING;
}

//-----------------------------------------------------------------------------
// Purpose: Sends the Statistics call to the Maxsi Distribution DLL.
//-----------------------------------------------------------------------------
void CLogicMaxsiStats::InputSendStats( inputdata_t &inputdata )
{
	// If we're active, and our string's aren't null.
	if ( !m_bDisabled && m_iszStatisticsFunction != NULL_STRING && m_iszStatisticsValue != NULL_STRING )
	{
		//Send the call off to the Maxsi Distribution wrapper.
		/*if( g_MaxsiDistribution != NULL )
		{
			//Convert our strings into chars.
			char *maxsiFunction = (char *)STRING( m_iszStatisticsFunction );
			char *maxsiValue = (char *)STRING( m_iszStatisticsValue );

			g_MaxsiDistribution->Statistic(maxsiFunction,maxsiValue);
		}*/
		
		m_OnSentStats.FireOutput( inputdata.pActivator, this );
	}
}

//------------------------------------------------------------------------------
// Purpose: Sets the value of the function string via I/O input.
//------------------------------------------------------------------------------
void CLogicMaxsiStats::InputSetFunctionTo( inputdata_t &inputdata )
{
	m_iszStatisticsFunction = MAKE_STRING( inputdata.value.String() );
}

//------------------------------------------------------------------------------
// Purpose: Sets the value of the value string via I/O input.
//------------------------------------------------------------------------------
void CLogicMaxsiStats::InputSetValueTo( inputdata_t &inputdata )
{
	m_iszStatisticsValue = MAKE_STRING( inputdata.value.String() );
}

//------------------------------------------------------------------------------
// Purpose: Turns on the relay, allowing it to fire outputs.
//------------------------------------------------------------------------------
void CLogicMaxsiStats::InputEnable( inputdata_t &inputdata )
{
	m_bDisabled = false;
}

//------------------------------------------------------------------------------
// Purpose: Turns off the relay, preventing it from firing outputs.
//------------------------------------------------------------------------------
void CLogicMaxsiStats::InputDisable( inputdata_t &inputdata )
{ 
	m_bDisabled = true;
}

//------------------------------------------------------------------------------
// Purpose: Toggles the enabled/disabled state of the relay.
//------------------------------------------------------------------------------
void CLogicMaxsiStats::InputToggle( inputdata_t &inputdata )
{ 
	m_bDisabled = !m_bDisabled;
}