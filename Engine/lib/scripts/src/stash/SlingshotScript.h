#include <scripting.h>

class SlingshotScript: public Engine::Script
{
	void awake();
	int hello = 1;
	void hello();
	void start();
	void update();
	void yolo(float yessir);
}
