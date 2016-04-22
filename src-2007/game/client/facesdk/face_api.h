#ifndef FACEAPI_H
#define FACEAPI_H

// Precompiled header file
#include "stdafx.h"
// Some simple utilities for error handling, printing and console key-press interpreting.
#include "utils.h"

typedef void (STDCALL *headPoseCallback)(void *, smEngineHeadPoseData head_pose);

class FaceAPI
{
public:
	FaceAPI();
	void init(); //smCoord3f *head_pos, smCoord3f *head_rot);
	void start(headPoseCallback callback);
	void end();
	void reset();
	bool m_bFaceAPIHasCamera;
	bool m_bFaceAPIInitialized;
private:
	void createFirstCamera();
	void CheckResult( const char* failedSystem );
	smReturnCode result;

	//smCoord3f *head_pos;
	//smCoord3f *head_rot;
	smCameraHandle camera;
	smEngineHandle engine;
	//smVideoDisplay video_display;
	//smWindowHandle video_display_handle;
};

#endif FACEAPI_H
