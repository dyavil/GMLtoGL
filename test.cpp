#include <stdio.h>
#include "objectToMesh.h"

int main( int argc, char **argv )
{
	objectToMesh obm;
	citygml::ParserParams params;
	std::shared_ptr<const citygml::CityModel> city = citygml::load("/home/dyavil/Documents/Master/TER/Part-2-Tunnel-Bridge-V3.gml", params );
	const citygml::ConstCityObjects obj =  city->getRootCityObjects();
	
	Mesh mesh = obm.toMesh(obj);
	std::cout << "yolo" << std::endl;
	return 0;
}