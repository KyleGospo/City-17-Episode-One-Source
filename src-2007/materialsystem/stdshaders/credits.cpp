//------------------------------------------------------------------------------
//We're glad you find our code interesting and possibly useful, if you decide to use
//it in your own code, then it would be nice of you to leave these credits here, though you are not required to.
//------------------------------------------------------------------------------
#include "convar.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

void C17_GearDev( void )
{
	Msg( "---------------------------------------------------------------\n" );
	Msg( "This mod happily uses and possibly extends shader technology graciously originally made by Gear Development:\n" );
	Msg( "You can find the complete source code for our shaders at http://geardev.wordpress.com/\n" );
	Msg( "---------------------------------------------------------------\n" );
	Msg( "Cory de La Torre 'MrTwoVideoCards' - Mod Leader, Head Mapper (gear.dev@gmail.com)\n" );
	Msg( "Kyle Martin Gospodnetich '1/4 Life' - Head Programmer (onequarterlife2@gmail.com)\n" );
	Msg( "Adam Coltman 'Mess' - Mapper (adamcoltman@hotmail.co.uk)\n" );
	Msg( "Ian 'mm3guy' - Modeler and Animator (mm3guy@gmail.com)\n" );
	Msg( "Kilian Groﬂ 'diwako' - Texture Artist (lagging@gmx.de)\n" );
	Msg( "Kyle Bloss 'PelPix' - Texture Artist (kylebloss@verizon.net)\n" );
	Msg( "Hugh Russell 'fps_Breadkiller' - Music (fpsnoobkiller@gmail.com)\n" );
	Msg( "Zacharrias Quinten 'Da Big Man'- Sound Design (freemangordon1337@gmail.com)\n" );
	Msg( "Nathan Vess 'VESS' - Voice Acting and Concept Art\n" );
	Msg( "David Michael Zander 'Edli' - Voice Acting (bahamutshead@gmail.com)\n" );
	Msg( "Leonard Polous 'Leo Leonidas' - Voice Acting (covent21@hotmail.com)\n" );
	Msg( "---------------------------------------------------------------\n" );
	Msg( "\n" );
	Msg( "---------------------------------------------------------------\n" );
	Msg( "Special thanks to:\n" );
	Msg( "---------------------------------------------------------------\n" );
	Msg( "'Sortie' - Use of his Maxsi Distribution system (http://www.maxsi.dk/)\n" );
	Msg( "---------------------------------------------------------------\n" );
	Msg( "\n" );
	Msg( "---------------------------------------------------------------\n" );
	Msg( "Past Team Members:\n" );
	Msg( "---------------------------------------------------------------\n" );
	Msg( "'Mr. Happy' - Senior Mapper/Tester\n" );
	Msg( "'No.00 Dylan' - Particles/Tester\n" );
	Msg( "Sam Pavlovic 'Sam za Nemesis' - Modeler and Animator (samuelgames@gmail.com)\n" );
	Msg( "---------------------------------------------------------------\n" );
}

static ConCommand gear("gear", C17_GearDev, "Team Gear, the most awesome people on Earth. No exceptions." );