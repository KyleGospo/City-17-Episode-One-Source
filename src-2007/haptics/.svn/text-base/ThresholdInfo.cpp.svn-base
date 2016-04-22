#include "cbase.h"
#include "thresholdinfo.h"
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// Input Theshold Helper ( this again is for cleaner/easier access/processing in the code. )
ConVar hap_debug_threshhold("hap_debug_threshhold", "0", FCVAR_ARCHIVE);

Vector ThresholdInfo::GetBaseScale()
{
	return Vector(
		XScale ? XScale->GetFloat() : 1,
		YScale ? YScale->GetFloat() : 1,
		ZScale ? ZScale->GetFloat() : 1);
}

// if scale convars set to null, scale will be set to (1,1,1)
ThresholdInfo::ThresholdInfo(	const ConVar *LowLimit,		const ConVar *LowScale,
								const ConVar *MidScale,
								const ConVar *HighLimit,	const ConVar *HighScale,
								
								const ConVar *ScaleX, 
								const ConVar *ScaleY,
								const ConVar *ScaleZ)
{
	LimitLow	= LowLimit;
	LimitHigh	= HighLimit;

	ScaleLow	= LowScale;
	ScaleMid	= MidScale;
	ScaleHigh	= HighScale;

	XScale = ScaleX;
	YScale = ScaleY;
	ZScale = ScaleZ;
}

Vector ThresholdInfo::ProcessVector(Vector vIn)
{
	Vector vOut = vIn;
	for(int i = 0; i!=3; i++)
	{
		if(abs(vOut[i]) < LimitLow->GetFloat())
			vOut[i] *= ScaleLow->GetFloat();
		else if(abs(vOut[i]) < LimitHigh->GetFloat())
			vOut[i] *= ScaleMid->GetFloat();
		else
			vOut[i] *= ScaleHigh->GetFloat();
	}
	vOut *= GetBaseScale();
	if(hap_debug_threshhold.GetInt())
	{
		Msg("Threshold: Old( %f, %f, %f ) new ( %f, %f, %f )\n",vIn.x,vIn.y,vIn.z,vOut.x,vOut.y,vOut.z);
		if(hap_debug_threshhold.GetInt()>1)
		{
			PrintData();
			hap_debug_threshhold.SetValue(0);
		}
	}
	return vOut;
}
void ThresholdInfo::PrintData()
{

	Msg("Printing Threshold:\n");
	const ConVar *all[] = 
	{
		LimitLow,
		LimitHigh,
		ScaleLow,
		ScaleMid,
		ScaleHigh,
		XScale,
		YScale,
		ZScale,
	};
	for(int i = 0;i!= ARRAYSIZE(all);i++)
	{
		Msg("ConVar %s = %s\n", all[i]->GetName(), all[i]->GetString());
	}
	Msg("End\n");
}