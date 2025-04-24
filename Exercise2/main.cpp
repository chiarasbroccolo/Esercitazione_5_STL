#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;

int main()
{
	/*Creo un oggetto di tipo PolygonalMesh che conterrà tutte le informazioni della mesh*/
    PolygonalMesh mesh;

    /* Chiamo la funzione ImportMesh per importare la mesh da file.
	   Se la funzione restituisce false significa che qualcosa è andato storto */
    if(!ImportMesh(mesh))
    {
        cerr << "file not found" << endl;		//Stampa messaggio di errore e terminazione del programma
        return 1;
    }

    /* Creo un oggetto della classe UCDUtilities che fornisce metodi per esportare i dati in formato ParaView */
    Gedim::UCDUtilities writer;
	
	/* Esporto i punti (celle 0D) in un file .inp leggibile da Paraview */
    writer.ExportPoints("./Cell0Ds.inp", mesh.Cell0DsCoordinates);

	/* Esporto i segmenti(celle 1D) in un file .inp 
	   Per farlo utilizzo le coordinate dei punti e gli indici degli estremi*/
    writer.ExportSegments("./Cell1Ds.inp", mesh.Cell0DsCoordinates, mesh.Cell1DsExtrema);

    return 0;
}