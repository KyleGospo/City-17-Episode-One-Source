//========= Copyright © 2010, Gear Development, All rights reserved. ============//
//
// Purpose: I imagine a few of you are going to wonder dearly what this is is why we're doing this.
// Well, sit down. I'm going to tell you a story:
// When Valve created the screeneffects system, they did with 1 effect running at any given time, max;
// More if render order wasn't important or you wanted to stick to a single screeneffects file.
// In City17, we need a whopping 6 screeneffects on at all times, not including any of the stock screeneffects.
// This presented us with a problem, because there weren't any ways to scientifically control the render order of the effects.
// This is our "fix". If you want to add a new effect, include it's headers here, and then define it here within this render order stack.
//
// Note: Bottom renders first.
//
//===============================================================================//
#include "cbase.h"
#include "ScreenSpaceEffects.h"
#include "c17_screeneffects.h"
#include "episodic\episodic_screenspaceeffects.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// Unsharp
ADD_SCREENSPACE_EFFECT( CUnsharpEffect, c17_unsharp );

// FXAA
ADD_SCREENSPACE_EFFECT( CFXAA, c17_fxaa );

// Chromatic Dispersion, Noise, Vignetting. In that order.
ADD_SCREENSPACE_EFFECT( CHealthEffects, c17_healthfx );

// Episodic
ADD_SCREENSPACE_EFFECT( CStunEffect, episodic_stun );
ADD_SCREENSPACE_EFFECT( CEP2StunEffect, ep2_groggy );

// Water FX
ADD_SCREENSPACE_EFFECT( CWaterEffects, c17_waterfx );

// Episodic
ADD_SCREENSPACE_EFFECT( CEP1IntroEffect, episodic_intro );

// L4D Glow
ADD_SCREENSPACE_EFFECT( CEntGlowEffect, c17_l4dglow );

// Sun Shafts
ADD_SCREENSPACE_EFFECT( CSunShaftEffect, c17_sunshaft );

// SSAO
ADD_SCREENSPACE_EFFECT( CSSAOEffect, c17_ssao );

// Color Correction Extensions
ADD_SCREENSPACE_EFFECT( CColorCorrectionEffect, c17_colorcorrection );