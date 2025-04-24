#pragma once

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

/* Creo un namespace personalizzato che racchiude tutte le entità relative alla mesh poligonale */
namespace PolygonalLibrary {

/* Struttura che rappresenta una mesh poligonale in 2D */
struct PolygonalMesh	
{
	/* Numero di celle di dimensione 0(punti), 1(segmenti) e 2(poligoni) */
    unsigned int NumCell0Ds;
    unsigned int NumCell1Ds;
    unsigned int NumCell2Ds;

    /* Vettori contenti gli identificativi (Id) univoci delle celle */
    vector<unsigned int> Cell0DsId;
    vector<unsigned int> Cell1DsId;
    vector<unsigned int> Cell2DsId;
	
	/* Matrice contenente le coordinate dei punti(sono doubles) */
    MatrixXd Cell0DsCoordinates; 

	/* Matrice contente gli estremi di ogni segmento, di ogni lato (sono int) */
    MatrixXi Cell1DsExtrema;    

    /* Ogni poligono è descritto da una lista di vertici e da una lista di spigoli.
	   Hanno cardinalità variabile perchè non tutti i poligoni sono uguali */
    vector<vector<unsigned int>> Cell2DsVertices; 
    vector<vector<unsigned int>> Cell2DsEdges;

    /* Utilizzo i dizionari per associare ad ogni marker una lista a cui poi verranno aggiunti i dati */
    map<unsigned int, list<unsigned int>> Cell0Ds_markers;
    map<unsigned int, list<unsigned int>> Cell1Ds_markers;
    map<unsigned int, list<unsigned int>> Cell2Ds_markers;
};

}
