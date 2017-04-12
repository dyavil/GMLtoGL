#include <tuple>
#include <iostream>
#include "vec.h"

class simulationData{
public:
	simulationData(){};
	~simulationData(){};

private:
	std::tuple<Point, Point, Vector> planes;
};