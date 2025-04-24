#pragma once

#include <iostream>
#include "PolygonalMesh.hpp" 

using namespace std;

namespace PolygonalLibrary
{
/* Funzione principale per importare una mesh.
   Richiama le funzioni per leggere punti, segmenti e poligoni.
   Ritorna true se l'importazione ha successo, altrimenti false. */
bool ImportMesh(PolygonalMesh& mesh);

/* Importa i dati delle celle 0D (punti) da "Cell0Ds.csv".
   Ritorna true se la letture va a buon fine, false altrimenti. */
bool ImportCell0Ds(PolygonalMesh& mesh);

/* Importa i dati delle celle 1D (segmenti) da "Cell1Ds.csv".
   Ritorna true se la letture va a buon fine, false altrimenti. */
bool ImportCell1Ds(PolygonalMesh& mesh);

/* Importa i dati delle celle 2D (poligoni) da "Cell2Ds.csv".
   Ritorna true se la letture va a buon fine, false altrimenti. */
bool ImportCell2Ds(PolygonalMesh& mesh);

}
