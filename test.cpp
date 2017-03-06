#include <stdio.h>
#include "objectToMesh.h"
#include <stdio.h>
#include "window.h"
#include "mesh.h"
#include "draw.h"
#include "orbiter.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <algorithm>    
#include <citygml/citygml.h>
#include <citygml/citymodel.h>
#include <citygml/cityobject.h>
#include <citygml/geometry.h>
#include <citygml/polygon.h>

// creation des objets openGL
Mesh mesh;
Orbiter camera;

int init( )
{
    objectToMesh obm;
	citygml::ParserParams params;
	std::shared_ptr<const citygml::CityModel> city = citygml::load("/home/dyavil/Documents/Master/TER/Part-2-Tunnel-Bridge-V3.gml", params );
	const citygml::ConstCityObjects obj =  city->getRootCityObjects();

	mesh = obm.toMesh(obj);
	std::cout << "yolo" << std::endl;
   
    Point pmin, pmax;
    mesh.bounds(pmin, pmax);
    //camera.lookat(Point(-13.f, -5.f, -9.f), Point(13.f, 5.f, 9.f));
    camera.lookat(pmin, pmax);

    glClearDepth(1.f);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    return 0;   // renvoyer 0 ras, pas d'erreur, sinon renvoyer -1
}

// affichage
int draw( )
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //draw(mesh, Identity(), Identity(), Identity());
    draw(mesh, camera);

    int mx, my;
    unsigned int mb= SDL_GetRelativeMouseState(&mx, &my);
  
   // deplace la camera
    if(mb & SDL_BUTTON(1))              // le bouton gauche est enfonce
       // tourne autour de l'objet
        camera.rotation(mx, my);
    else if(mb & SDL_BUTTON(3))         // le bouton droit est enfonce
           // approche / eloigne l'objet
           camera.move(mx);

    draw(mesh, camera);
    /*static float angle= 0;      // il faudrait declarer angle comme variable globale...
        if(key_state('j'))
            angle= angle + 3;
        if(key_state('k'))1
            angle= angle - 3;
        
        Transform T= RotationY( angle );
        draw(mesh2, T, Identity(), Identity());*/

    return 1;   // on continue, renvoyer 0 pour sortir de l'application
}

// destruction des objets openGL
int quit( )
{
    mesh.release();
    return 0;   // ras, pas d'erreur
}


int main( int argc, char **argv )
{
    // etape 1 : creer la fenetre
    Window window= create_window(1024, 640);
    if(window == NULL)
        return 1;       // erreur lors de la creation de la fenetre ou de l'init de sdl2

    // etape 2 : creer un contexte opengl pour pouvoir dessiner
    Context context= create_context(window);
    if(context == NULL)
        return 1;       // erreur lors de la creation du contexte opengl

    // etape 3 : creation des objets
    if(init() < 0)
    {
        printf("[error] init failed.\n");
        return 1;
    }

    // etape 4 : affichage de l'application, tant que la fenetre n'est pas fermee. ou que draw() ne renvoie pas 0
    run(window, draw);

    // etape 5 : nettoyage
    quit();
    release_context(context);
    release_window(window);
    return 0;
}

