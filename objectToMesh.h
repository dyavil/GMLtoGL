#include <citygml/citygml.h>
#include <citygml/citymodel.h>
#include <citygml/cityobject.h>
#include <citygml/geometry.h>
#include <citygml/implictgeometry.h>
#include <citygml/linearring.h>
#include <citygml/polygon.h>
#include <citygml/texture.h>
#include "texture.h"
#include "mesh.h"
#include "math.h"
#include "vec.h"
#include <iostream>
#include <fstream>
#include <algorithm>

class objectToMesh{
public:
	objectToMesh(){};
	~objectToMesh(){};
	Mesh & toMesh(const citygml::ConstCityObjects & obj, std::string theme);
	void centerM(Mesh & mesh, Point pmin, Point pmax);
	void centerBase(Mesh & mesh, Point pmin, Point pmax);
	void recursiveCall(Mesh &mesh, const citygml::CityObject * obj, float r, float r1, float r2, int deep);
	void recursiveGeometryCall(Mesh & mesh, citygml::Geometry gs, float r, float r1, float r2);
	void meshTo2D();
	void colorMeshTo2D();
	Mesh & getMesh(){return mesh;};
	std::vector<Mesh> & getGeometriesMeshes(){return geometries;};
	std::vector<int> & getShownedMeshes(){return showned;};
	std::vector<Mesh> & splitGeometry(Mesh mesh);
	void computeShowned(Point p);


	std::ofstream logfile;
	std::string theme;
private:
	float distanceM(Point & a, Point & bpmin, Point & bpmax);
	bool inBox(Point & a, Point & bpmin, Point & bpmax);

	Mesh mesh;
	std::vector<Mesh> geometries;
	std::vector<int> showned;
};