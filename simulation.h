#include "objectToMesh.h"
#include <utility>
#include "orbiter.h"
#include "draw.h"
#include "mat.h"
#include "vec.h"

class simulation{
public:
	simulation(){};
	simulation(std::vector<std::string> n):gmlfiles(n){};
	
	~simulation(){};
	void init(std::string f);
	void addFile(std::string f){gmlfiles.push_back(std::move(f));};
	Mesh & getMesh(){return global;};
	Orbiter & getCamera(){return camera;};
	std::vector<Mesh> & getGeometriesMeshes(){return geometries;};
	int getGeometriesSize(){return geometries.size();};
	Mesh & getGeometry(int i){return geometries[i];};
	//void centerElem(Mesh & mesh);
	void meshTo2D();
	void geometriesTo2D();
	void run(int action, int mx, int my);
	
	std::vector<int> & getShownedMeshes(){return showned;};
	float computeShowned(Point p);
	void quit();

private:
	float distanceM(Point & a, Point & bpmin, Point & bpmax);
	bool inBox(Point & a, Point & bpmin, Point & bpmax);
	void init_user();

	objectToMesh obm;
	Orbiter camera;
	Mesh global;
	Mesh user;
	Point cursor;
	float ground;
	Transform userMovement;
	std::vector<Mesh> geometries;
	std::vector<int> showned;
	std::vector<std::string> gmlfiles;
	std::vector<std::pair<Point, Point>> boxes;
};