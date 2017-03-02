#include <citygml/citygml.h>
#include <citygml/citymodel.h>
#include <citygml/cityobject.h>
#include <citygml/geometry.h>
#include <citygml/polygon.h>
#include "mesh.h"

class objectToMesh{
public:
	objectToMesh();
	~objectToMesh();
	Mesh toMesh(const citygml::ConstCityObjects & obj);

};