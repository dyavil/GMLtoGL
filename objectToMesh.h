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
	void toMesh(Mesh & mesh, const citygml::ConstCityObjects & obj, std::string theme, std::vector<Mesh> & geometries);
	void centerM(Mesh & mesh, std::vector<Mesh> & geometries, Point pmin, Point pmax);
	void centerBase(Mesh & mesh, Point pmin, Point pmax);
	void recursiveCall(Mesh &mesh, const citygml::CityObject * obj, std::vector<Mesh> & geometries, float r, float r1, float r2, int deep);
	void recursiveGeometryCall(Mesh & mesh, citygml::Geometry gs, std::vector<Mesh> & geometries, float r, float r1, float r2);

	void fuseMeshes(Mesh & out, Mesh in);

	std::ofstream logfile;
	std::string theme;


};