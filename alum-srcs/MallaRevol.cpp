// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Implementación de la clase 'MallaRevol'
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include <file_ply_stl.hpp>
#include <matrices-tr.hpp>
#include "MallaRevol.hpp"

using namespace std ;

// *****************************************************************************

MallaRevol::MallaRevol( const std::string & nombre )
{
  ponerNombre( nombre );
}

// *****************************************************************************

MallaRevol::MallaRevol( const std::string & nombre_arch,
                        const unsigned nperfiles,
                        const bool     crear_tapas,
                        const bool     cerrar_malla  )
{
   ponerNombre( std::string("malla por revolución del perfil en '"+ nombre_arch + "'" ));

   vector<float> vertices;
   vector<Tupla3f> perfil_original;

   ply::read_vertices( nombre_arch.c_str(), vertices );

   nper = nperfiles;
   nvp = vertices.size() / 3;
   num_ver = nper * nvp;    // dato de la clase MallaInd

   for (unsigned i = 0; i < vertices.size()-2; i+=3) {
     perfil_original.push_back( {vertices[i], vertices[i+1], vertices[i+2]} );
   }
   crearMallaRevol( perfil_original, crear_tapas, cerrar_malla );
}

// *****************************************************************************

void MallaRevol::crearMallaRevol ( const std::vector<Tupla3f> & perfil_original,
                                   const bool crear_tapas,
                                   const bool cerrar_malla   )
{
    float alpha = cerrar_malla ? 360.0 / (float)nper : 360.0 / (float)(nper-1); // angulo de giro
    Matriz4f matriz_giro = MAT_Rotacion(alpha, 0, 1, 0);
    vector<Tupla3f> perfil_girado = perfil_original;
    vector<Tupla3f> aux;

    // Vértices
    for (unsigned i = 0; i < nvp; i++)
      tabla_ver.push_back( perfil_original[i] );

    for (unsigned i = 1; i < nper; i++) {
      aux = perfil_girado;
      perfil_girado.clear();
      for (unsigned j = 0; j < nvp; j++) {
        perfil_girado.push_back( matriz_giro * aux[j] );
      }
      tabla_ver.insert( tabla_ver.end(), perfil_girado.begin(), perfil_girado.end() );
    }

    // Caras
    unsigned n = cerrar_malla ? nper : nper-1;
    for (unsigned i = 0; i < n; i++) {
      for (unsigned j = 0; j < nvp-1; j++) {
        if (cerrar_malla) {
          tabla_tri.push_back( { i*nvp+j, ((i+1)%nper)*nvp+j+1, i*nvp+j+1 } );
          tabla_tri.push_back( { i*nvp+j, ((i+1)%nper)*nvp+j, ((i+1)%nper)*nvp+j+1 } );
        }
        else {
          tabla_tri.push_back( { i*nvp+j, (i+1)*nvp+j+1, i*nvp+j+1 } );
          tabla_tri.push_back( { i*nvp+j, (i+1)*nvp+j, (i+1)*nvp+j+1 } );
        }
      }
    }

    // Tapas
    if (crear_tapas) {
        // Cara inferior
        Tupla3f vertice_inf = perfil_original[0];
        if (vertice_inf(X) != 0) {
          tabla_ver.push_back( {0.0, vertice_inf(Y), 0.0} );
          num_ver++;

          for (unsigned i = 0; i < nper; i++)
            tabla_tri.push_back( {num_ver-1, i*nvp, ((i+1)%nper)*nvp} );

          if ( cerrar_malla )
            tabla_tri.push_back( {(nper-1)*nvp, num_ver-1, 0} ); // Añadir ultima cara
        }

        // Cara superior
        Tupla3f vertice_sup = perfil_original[nvp-1];
        if (vertice_sup(X) != 0) {
          tabla_ver.push_back( {0.0, vertice_sup(Y), 0.0} );
          num_ver++;

          for (unsigned i = 0; i < nper; i++)
            tabla_tri.push_back( {num_ver-1, i*nvp+(nvp-1), (((i+1)%nper)*nvp)+(nvp-1)} );

          if ( cerrar_malla )
            tabla_tri.push_back( {nper*nvp-1, num_ver-1, nvp-1} ); // Añadir ultima cara
        }
    }

   // calcular la tabla de normales
   calcular_normales();

   // colores
   setColorVertices();
}

// *****************************************************************************

Cilindro::Cilindro ( const int num_verts_per,
                     const unsigned nperfiles,
                     const bool crear_tapas,
                     const bool cerrar_malla )
    : MallaRevol( "malla por revolución de un cilindro" )
{
    vector<Tupla3f> perfil_original;
    perfil_original.push_back( {1, 0, 0} );
    perfil_original.push_back( {1, 1, 0} );
    nvp = num_verts_per;
    nper = nperfiles;
    num_ver = nvp * nper;

    crearMallaRevol( perfil_original, crear_tapas, cerrar_malla );
}

// *****************************************************************************

Cono::Cono ( const int num_verts_per,
             const unsigned nperfiles,
             const bool crear_tapas,
             const bool cerrar_malla )
    : MallaRevol( "malla por revolución de un cono" )
{
    vector<Tupla3f> perfil_original;
    perfil_original.push_back( {1, 0, 0} );
    perfil_original.push_back( {0, 1, 0} );
    nvp = num_verts_per;
    nper = nperfiles;
    num_ver = nvp * nper;

    crearMallaRevol( perfil_original, crear_tapas, cerrar_malla );
}

// *****************************************************************************

Esfera::Esfera ( const int num_verts_per,
                 const unsigned nperfiles,
                 const bool crear_tapas,
                 const bool cerrar_malla )
    : MallaRevol( "malla por revolución de una esfera" )
{
    vector<Tupla3f> perfil_original;
    nvp = num_verts_per;
    nper = nperfiles;
    num_ver = nvp * nper;

    float dist = 2.0/(nvp-1);
    for (int i = 0; i < nvp; i++) {
      float y = -1.0 + i*dist;
      perfil_original.push_back( {sqrt(1-y*y), y, 0} );
    }

    crearMallaRevol( perfil_original, crear_tapas, cerrar_malla );
}
