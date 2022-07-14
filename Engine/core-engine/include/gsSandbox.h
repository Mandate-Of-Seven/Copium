#pragma once
#include "StateManager.h"

extern int gsCurrent, gsNext, gsPrevious;

void loadSandBox();
void initSandBox();
void updateSandBox();
void drawSandBox();
void freeSandBox();
void unloadSandBox();
