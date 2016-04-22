#ifndef THRESHOLDINFO_H
#define THRESHOLDINFO_H
#ifdef _WIN32
#pragma once
#endif
struct ThresholdInfo{
	
	const ConVar *LimitLow;
	const ConVar *LimitHigh;

	const ConVar *ScaleLow;
	const ConVar *ScaleMid;
	const ConVar *ScaleHigh;

	const ConVar *XScale;
	const ConVar *YScale;
	const ConVar *ZScale;

	Vector GetBaseScale();

	// if scale convars set to null, scale will be set to (1,1,1)
	ThresholdInfo(	const ConVar *LowLimit,		const ConVar *LowScale,
					const ConVar *MidScale,
					const ConVar *HighLimit,	const ConVar *HighScale,
					
					const ConVar *ScaleX = NULL, 
					const ConVar *ScaleY = NULL,
					const ConVar *ScaleZ = NULL);

	Vector ProcessVector(Vector vIn);
	void PrintData();
};
#endif