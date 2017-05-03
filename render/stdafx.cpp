#include"stdafx.h"
int CMID(int x, int min, int max) { return(x < min) ? min : ((x > max) ? max : x); }