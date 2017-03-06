#include <citygml/citygml.h>
#include <citygml/citymodel.h>
#include <citygml/cityobject.h>
#include <citygml/geometry.h>
#include <citygml/implictgeometry.h>
#include <citygml/polygon.h>
#include <citygml/linearring.h>
#include "mesh.h"

class objectToMesh{
public:
	objectToMesh(){};
	~objectToMesh(){};
	Mesh toMesh(const citygml::ConstCityObjects & obj);
	void recursiveCall(Mesh &mesh, const citygml::CityObject * obj);
	void recursiveGeometryCall(Mesh & mesh, citygml::Geometry gs);

};