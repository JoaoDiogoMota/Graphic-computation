#ifndef ENGINE_LIGHT_H
#define ENGINE_LIGHT_H

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>

class light {
private:

	std::string tipo;

	float pointX;
	float pointY;
	float pointZ;

	float dirX;
	float dirY;
	float dirZ;

	float phi;
	float theta;

public:

	light(std::string tipo);

	std::string getTipo();

	float getPointX();
	float getPointY();
	float getPointZ();

	float getDirX();
	float getDirY();
	float getDirZ();

	float getPhi();
	float getTheta();

	void setTipo(std::string tp);

	void setPointX(float val);
	void setPointY(float val);
	void setPointZ(float val);

	void setDirX(float val);
	void setDirY(float val);
	void setDirZ(float val);

	void setTheta(float val);
	void setPhi(float val);
};
#endif //ENGINE_LIGHT_H