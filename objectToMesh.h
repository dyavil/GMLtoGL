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
	Mesh toMesh(const citygml::ConstCityObjects & obj, std::string theme);
	void center(Mesh & mesh, Point pmin, Point pmax);
	void recursiveCall(Mesh &mesh, const citygml::CityObject * obj);
	void recursiveGeometryCall(Mesh & mesh, citygml::Geometry gs);

	std::ofstream logfile;
	std::string theme;
};