#include <stdio.h>
#include "simulation.h"
#include <stdio.h>
#include "window.h"
#include "mesh.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <algorithm>    
#include <citygml/citygml.h>
#include <citygml/citymodel.h>
#include <citygml/cityobject.h>
#include <citygml/geometry.h>
#include <citygml/polygon.h>


GLuint texture;
// objectToMesh obm;
simulation simu;



int init( )
{
    simu.init("/home/dyavil/Documents/TER/LYON_1ER_2012/LYON_1ER_BATI_2012.gml");
    ///home/dyavil/Documents/Master/TER/Part-2-Tunnel-Bridge-V3.gml
    ///home/dyavil/Documents/TER/LYON_1ER_2012/LYON_1ER_OBJET_REMARQUABLE_2012.gml
    ///home/dyavil/Downloads/CityGML_2.0_Test_Dataset_2012-04-23/Part-3-Railway-V2.gml
    ///home/dyavil/Downloads/citygml/examples/2.0/building/Building_LOD4-EPSG25832.gml


    /*std::shared_ptr<const citygml::CityModel> city2 = citygml::load("/home/dyavil/Documents/TER/LYON_1ER_2012/LYON_1ER_WATER_2012.gml", params );
    
    std::vector<std::string> thl2= city2->themes();
    std::string theme2 = "";

    for (int i = 0; i < thl2.size(); ++i)
    {
        theme2 = thl2[i];
    }

    const citygml::ConstCityObjects obj2 =  city2->getRootCityObjects();
    mesh2 = obm.toMesh(obj2, theme2);
    obm.center(mesh2, pmin, pmax);
	
   
    std::shared_ptr<const citygml::CityModel> city3 = citygml::load("/home/dyavil/Documents/TER/LYON_1ER_2012/LYON_1ER_BATI_REMARQUABLE_2012.gml", params );
    
    std::vector<std::string> thl3= city3->themes();
    std::string theme3 = "";

    for (int i = 0; i < thl3.size(); ++i)
    {
        theme3 = thl3[i];
    }

    const citygml::ConstCityObjects obj3 =  city3->getRootCityObjects();
    mesh3 = obm.toMesh(obj3, theme3);
    obm.center(mesh3, pmin, pmax);*/


    //glClearDepth(1.f);
    //glClearColor(1, 0, 1, 0.5);
    glClearDepth(1.f);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    return 0;   // renvoyer 0 ras, pas d'erreur, sinon renvoyer -1
}

// affichage
int draw( )
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    int mx, my;
    unsigned int mb= SDL_GetRelativeMouseState(&mx, &my);
    simu.run(mb, mx, my);
  
   // deplace la camera
    if(mb & SDL_BUTTON(1)) {             // le bouton gauche est enfonce
       // tourne autour de l'objet
        simu.getCamera().rotation(mx, my);}
    else if(mb & SDL_BUTTON(3))         // le bouton droit est enfonce
           // approche / eloigne l'objet
        simu.getCamera().move(mx);
    else if(mb & SDL_BUTTON(2))         // le bouton du milieu est enfonce
            // deplace le point de rotation
        simu.getCamera().translation((float) mx / (float) window_width(), (float) my / (float) window_height());

    

    return 1;   // on continue, renvoyer 0 pour sortir de l'application
}

// destruction des objets openGL
int quit( )
{

    simu.quit();

    return 0;   // ras, pas d'erreur
}


int main( int argc, char **argv )
{
    srand(time(NULL));
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

