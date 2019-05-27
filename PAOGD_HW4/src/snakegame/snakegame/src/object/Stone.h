#ifndef STONE
#define STONE
#include <GL/glut.h>
#include <string>

class Stone {
public:
	Stone(int pX, int pZ);
	~Stone();
	void draw();
	float getX();
	float getZ();
	void setX(float pX);
	void setZ(float pZ);
private:
	float x, z;
};


#endif