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
#include <iostream>
#include <fstream>

class objectToMesh{
public:
	objectToMesh(){};
	~objectToMesh(){};
	Mesh & toMesh(const citygml::ConstCityObjects & obj, std::string theme);
	void center(Mesh & mesh, Point pmin, Point pmax);
	void recursiveCall(Mesh &mesh, const citygml::CityObject * obj, float r, float r1, float r2, int deep);
	void recursiveGeometryCall(Mesh & mesh, citygml::Geometry gs, float r, float r1, float r2);
	void meshTo2D();
	Mesh & getMesh(){return mesh;};
	std::vector<Mesh> & getGeometriesMeshes(){return geometries;};

	std::ofstream logfile;
	std::string theme;
private:
	Mesh mesh;
	std::vector<Mesh> geometries;
};