#ifndef FRULE_H
#define FRULE_H

enum GAMEBUTTONS {
	UP_R,
	UP_G,
	UP_B,
	UP_V,

	DOWN_R,
	DOWN_G,
	DOWN_B,
	DOWN_V,

	EVB_DESTROY,
	EVB_BREED,
	EVB_PAINT,
	EVB_STOP,

	PAUSE,
	SPEEDUP,
	SPEEDDOWN,

	MAXGBUTTON,
};


struct fieldRule {
	int type1;
	// +
	int type2;
	// = 
	int fevent;
};

#endif