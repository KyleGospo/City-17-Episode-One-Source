#include "shaderlib/cshader.h"
class noise_ps20b_Static_Index
{
public:
	noise_ps20b_Static_Index( )
	{
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
#endif // _DEBUG
		return 0;
	}
};
#define shaderStaticTest_noise_ps20b 0
class noise_ps20b_Dynamic_Index
{
private:
	int m_nSHOWGRAINCONTROL;
#ifdef _DEBUG
	bool m_bSHOWGRAINCONTROL;
#endif
public:
	void SetSHOWGRAINCONTROL( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nSHOWGRAINCONTROL = i;
#ifdef _DEBUG
		m_bSHOWGRAINCONTROL = true;
#endif
	}
	void SetSHOWGRAINCONTROL( bool i )
	{
		m_nSHOWGRAINCONTROL = i ? 1 : 0;
#ifdef _DEBUG
		m_bSHOWGRAINCONTROL = true;
#endif
	}
public:
	noise_ps20b_Dynamic_Index()
	{
#ifdef _DEBUG
		m_bSHOWGRAINCONTROL = false;
#endif // _DEBUG
		m_nSHOWGRAINCONTROL = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllDynamicVarsDefined = m_bSHOWGRAINCONTROL;
		Assert( bAllDynamicVarsDefined );
#endif // _DEBUG
		return ( 1 * m_nSHOWGRAINCONTROL ) + 0;
	}
};
#define shaderDynamicTest_noise_ps20b psh_forgot_to_set_dynamic_SHOWGRAINCONTROL + 0
