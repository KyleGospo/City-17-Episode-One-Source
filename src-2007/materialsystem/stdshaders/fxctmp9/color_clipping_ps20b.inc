#include "shaderlib/cshader.h"
class color_clipping_ps20b_Static_Index
{
public:
	color_clipping_ps20b_Static_Index( )
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
#define shaderStaticTest_color_clipping_ps20b 0
class color_clipping_ps20b_Dynamic_Index
{
private:
	int m_nSQUISH;
#ifdef _DEBUG
	bool m_bSQUISH;
#endif
public:
	void SetSQUISH( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nSQUISH = i;
#ifdef _DEBUG
		m_bSQUISH = true;
#endif
	}
	void SetSQUISH( bool i )
	{
		m_nSQUISH = i ? 1 : 0;
#ifdef _DEBUG
		m_bSQUISH = true;
#endif
	}
public:
	color_clipping_ps20b_Dynamic_Index()
	{
#ifdef _DEBUG
		m_bSQUISH = false;
#endif // _DEBUG
		m_nSQUISH = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllDynamicVarsDefined = m_bSQUISH;
		Assert( bAllDynamicVarsDefined );
#endif // _DEBUG
		return ( 1 * m_nSQUISH ) + 0;
	}
};
#define shaderDynamicTest_color_clipping_ps20b psh_forgot_to_set_dynamic_SQUISH + 0
