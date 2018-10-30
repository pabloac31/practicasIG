// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Implementación de la clase 'MallaPLY'
// **
// *********************************************************************


#include <cassert>
#include <aux.hpp>
#include <tuplasg.hpp>
#include <file_ply_stl.hpp>
#include "MallaPLY.hpp"

using namespace std ;

// *****************************************************************************

MallaPLY::MallaPLY( const std::string & nombre_arch )
{
   ponerNombre(string("malla leída del archivo '") + nombre_arch + "'" );
   // COMPLETAR: práctica 2: leer archivo PLY e inicializar la malla
   // ..........................
   vector<float> vertices;
   vector<int> caras;

   // lee la malla PLY, incluyendo vértices y caras
   ply::read( nombre_arch.c_str(), vertices, caras );

   num_ver = vertices.size() / 3;

   for (unsigned i = 0; i < vertices.size()-2; i+=3) {
     tabla_ver.push_back( {vertices[i], vertices[i+1], vertices[i+2]} );
   }

   for (unsigned i = 0; i < caras.size()-2; i+=3) {
     tabla_tri.push_back( {caras[i], caras[i+1], caras[i+2]} );
   }

   // calcular la tabla de normales
   calcular_normales();

   // color de los vértices
   setColorVertices();

}

// *****************************************************************************
