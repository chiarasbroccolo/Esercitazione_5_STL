#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace PolygonalLibrary
{
	
/* Funzione principale che importa la mesh completa(celle 0D, 1D e 2D).
   Restituisce false anche se solo una delle tre importazioni fallisce. */
bool ImportMesh(PolygonalMesh& mesh)
{

    if(!ImportCell0Ds(mesh))
        return false;

    if(!ImportCell1Ds(mesh))
        return false;

    if(!ImportCell2Ds(mesh))
        return false;

    return true;

}

/* Funzione per importare le celle 0D (punti) dal file "Cell0Ds.csv" */
bool ImportCell0Ds(PolygonalMesh& mesh)
{
    ifstream inputFile("Cell0Ds.csv");		// Apro il file .csv

    if(inputFile.fail())					// Verifica di buona apertura del file
        return false;

    list<string> fileLines;
    string line;
	
	/* Leggo tutte le righe del file e le salvo in una lista */
    while (getline(inputFile, line))
        fileLines.push_back(line);

    inputFile.close();

    /* Rimuovo l'header, l'intestazione */
    fileLines.pop_front();

    /* Assegno la lunghezza della lista, cioè il numero di punti, alla variabile */
    mesh.NumCell0Ds = fileLines.size();		//NumCell0Ds è quella in PolygonalMesh

    if (mesh.NumCell0Ds == 0)
    {
        cerr << "ERROR: there is no cell 0D" << endl;
        return false;
    }

    /* Riservo spazio per gli Id dei punti (vettore dinamico) */
    mesh.Cell0DsId.reserve(mesh.NumCell0Ds);
	
    /* Inizializzo la matrice delle coordinate con zeri */
    mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3, mesh.NumCell0Ds);		//3 coordinate

    for (const string& line : fileLines)
    {
		/* Conversione della stringa in uno stream */
        istringstream converter(line);		 

        unsigned int id;		// Id del punto
        unsigned int marker;	// Marker associato 
        char separator; 		// Il carattere separatore in questo caso è ";"
        Vector2d coordinates;	// Coordinate del punto (x, y)

        /* Lettura dei dati: Id;Marker;x;y */
        converter >> id >> separator 
		          >> marker >> separator 
		          >> mesh.Cell0DsCoordinates(0, id) >> separator 
				  >> mesh.Cell0DsCoordinates(1, id);

        mesh.Cell0DsId.push_back(id);		// memorizzazione dell'Id

        /* Memorizzo i marker in un dizionario dove: chiave = marker - valore = lista vuota */
        map<unsigned int, list<unsigned int>>& m = mesh.Cell0Ds_markers;
		
        /* try_emplace(marker) cerca nella mappa se esiste già una chiave marker. 
		   Se NON esiste, la aggiunge e inizializza il valore con una lista vuota.
           In ogni caso restituisce un iteratore relativo alla posizione della coppia chiave - valore e 
		   un booleano che indica se la chiave è stata effettivamente inserita*/
        auto[itor, inserted] = m.try_emplace(marker);
		
        /* Aggiungo l'Id del punto alla lista(valore) relativo al marker(chiave) in posizione i */
        itor -> second.push_back(id);

    }

    return true;
}


/* STESSO PROCEDIMENTO
   Funzione per importare le celle 1D (segmenti) dal file "Cell1Ds.csv" */
bool ImportCell1Ds(PolygonalMesh& mesh)
{
    ifstream inputFile("Cell1Ds.csv");

    if(inputFile.fail())
        return false;

    list<string> fileLines;
    string line;
    while (getline(inputFile, line))
        fileLines.push_back(line);

    inputFile.close();

    fileLines.pop_front();

    mesh.NumCell1Ds = fileLines.size();

    if (mesh.NumCell1Ds == 0)
    {
        cerr << "ERROR: there is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DsId.reserve(mesh.NumCell1Ds);
    
	/* Memorizzo gli estremi del segmento */
    mesh.Cell1DsExtrema = Eigen::MatrixXi(2, mesh.NumCell1Ds);

    for (const string& line : fileLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        char separator;
        Vector2i extremes;
		
		/* Lettura dei dati: Id;Marker;punto0;punto1 */
        converter >> id >> separator 
		          >> marker >> separator 
				  >> mesh.Cell1DsExtrema(0, id) >> separator 
				  >> mesh.Cell1DsExtrema(1, id);
				  
        mesh.Cell1DsId.push_back(id);

        map<unsigned int, list<unsigned int>>& m = mesh.Cell1Ds_markers;
        auto [itor, inserted] = m.try_emplace(marker);
        itor -> second.push_back(id);
	
		/* TEST2 per verificare che uno segmento abbia lunghezza zero */
        int& origin = mesh.Cell1DsExtrema(0, id);
        int& end = mesh.Cell1DsExtrema(1, id);
		
        if(origin == end)		// Se i due punti coincidono
        {
            cerr<<"TEST NOT PASSED: the edge " << id << " has length equal to zero";
            return false;
        }
		
    }

    return true;
}

/* STESSO PROCEDIMENTO
   Funzione per importare le celle 2D (poligoni) dal file "Cell2Ds.csv" */
bool ImportCell2Ds(PolygonalMesh& mesh)
{
    ifstream inputFile("Cell2Ds.csv");

    if(inputFile.fail())
        return false;

    list<string> fileLines;
    string line;
    while (getline(inputFile, line))
        fileLines.push_back(line);

    inputFile.close();

    fileLines.pop_front();

    mesh.NumCell2Ds = fileLines.size();

    if (mesh.NumCell2Ds == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DsId.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsVertices.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsEdges.reserve(mesh.NumCell2Ds);

    for (const string& line : fileLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        unsigned int NumVertices;
        unsigned int NumEdges;
        char separator;
        double tmp;
        vector<unsigned int> vertices;
        vector<unsigned int> edges;

        converter >> id >> separator 
				  >> marker >> separator 
				  >> NumVertices;
				  
        vertices.reserve(NumVertices);  // In questo caso il numero di vertici dipende dalla variabile letta nel file
		
        for(unsigned int i = 0; i < NumVertices; i++)	// Il ciclo itera in base al numero di vertici 
		{  
            converter >> separator >> tmp;				
            vertices.push_back(tmp);	 	// I vertici, letti a uno a uno, vengono salvati nel vettore vertices
        }

		/* Stesso procedimento per gli spigoli */
        converter >> separator >> NumEdges;
        edges.reserve(NumEdges);
        for(unsigned int i = 0; i < NumEdges; i++)
        {
            converter >> separator >> tmp;
            edges.push_back(tmp);
	    }

        mesh.Cell2DsId.push_back(id);
        mesh.Cell2DsVertices.push_back(vertices);
        mesh.Cell2DsEdges.push_back(edges);

        map<unsigned int, list<unsigned int>>& m = mesh.Cell2Ds_markers;
        auto [itor, bool_val] = m.try_emplace(marker);
        itor -> second.push_back(id);


		/* TEST per verificare che il poligono non abbai area nulla */
        vector<unsigned int>& vec_vert = mesh.Cell2DsVertices[id];
        const unsigned int n = vec_vert.size();

        double area = 0.0;
        for(size_t i = 0; i < n ; i++)
        {   
            const unsigned int vi_id = vec_vert[i];
            const unsigned int vj_id = vec_vert[(i+1)%n]; // Procedimento per chiedere il poligono, connettere il primo vertice con l'ultimo 
			
            const MatrixXd coord = mesh.Cell0DsCoordinates;
            const double X_vi = coord(0, vi_id);
            const double Y_vi = coord(1, vi_id);
            const double X_vj = coord(0, vj_id);
            const double Y_vj = coord(1, vj_id);

            area += (X_vi * Y_vj) - (X_vj * Y_vi);
        }
        area = abs(area / 2.0);

        if(area <= 1e-16)
        {
            cerr<<"TEST NOT PASSED: the polygon " << id << " has area equal to zero";
            return false;
        }
    }

    return true;
}

}