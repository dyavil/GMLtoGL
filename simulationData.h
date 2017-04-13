#include <tuple>
#include <iostream>
#include <utility>
#include "vec.h"
#include "mat.h"
#include "mesh.h"

class simulationData{
public:
	simulationData(){};
	~simulationData(){};

	void extractPlanes(Mesh & mesh);
	std::vector<Mesh> getPosPlanesMeshes();
	std::vector<Mesh> getMinPlanesMeshes();
	std::vector<Mesh> splitMeshes(std::vector<Mesh> & m);

private:
	std::vector<std::tuple<Point, Point, Vector>> planesPos;
	std::vector<std::tuple<Point, Point, Vector>> planesMin;
	std::vector<std::vector<TriangleData>> cutTriangleDataVec(std::vector<std::pair<TriangleData, Vector>> & in, int precision);
	std::vector<TriangleData> recur(Point cmp, std::vector<std::pair<TriangleData, bool>> & togo, int precision, int on);

	std::vector<Mesh> tempMeshes;
};