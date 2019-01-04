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
                        const bool     cerrar_malla,
                        const bool     textura_auto )
{
   ponerNombre( std::string("malla por revolución del perfil en '"+ nombre_arch + "'" ));

   vector<float> vertices;
   vector<Tupla3f> perfil_original;

   ply::read_vertices( nombre_arch.c_str(), vertices );

   nper = nperfiles;
   nvp = vertices.size() / 3;
   num_ver = nper * nvp;    // dato de la clase MallaInd

   for (unsigned i = 0; i < vertices.size()-2; i+=3)
     perfil_original.push_back( {vertices[i], vertices[i+1], vertices[i+2]} );

   crearMallaRevol( perfil_original, crear_tapas, cerrar_malla, textura_auto );
}

// *****************************************************************************

void MallaRevol::crearMallaRevol ( const std::vector<Tupla3f> & perfil_original,
                                   const bool crear_tapas,
                                   const bool cerrar_malla,
                                   const bool textura_auto )
{
    // Vértices
    Matriz4f matriz_giro;
    for (unsigned i = 0; i < nper; i++) {
      matriz_giro = cerrar_malla ? MAT_Rotacion((float)(360*i)/nper, 0, 1, 0) : MAT_Rotacion((float)(360*i)/(nper-1), 0, 1, 0);
      for (unsigned j = 0; j < nvp; j++) {
        Tupla3f vertice = perfil_original[j];
        tabla_ver.push_back( matriz_giro * vertice );
      }
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

    // Coordenadas de textura

    if (textura_auto && !cerrar_malla) {
      float s_i, t_j;
      std::vector<float> d;
      d.push_back(0.0);
      for (unsigned i = 1; i < nvp; i++) {
        Tupla3f aux = perfil_original[i] - perfil_original[i-1];
        d.push_back(d[i-1] + sqrt(aux.lengthSq()));
      }

      for (unsigned i = 0; i < nper; i++) {
        s_i = (float)i / (nper-1);
        for (unsigned j = 0; j < nvp; j++) {
          t_j = 1 - (float)d[j] / d[nvp-1];
          coor_textura.push_back( {s_i, t_j} );
        }
      }
    }

    // Tapas
    if (crear_tapas) {

      // Cara inferior
      Tupla3f vertice_inf = perfil_original[0];
      tabla_ver.push_back( {0.0, vertice_inf(Y), 0.0} );
      num_ver++;
      for (unsigned i = 0; i < n; i++){
        if (cerrar_malla)
          tabla_tri.push_back( {num_ver-1, i*nvp, ((i+1)%nper)*nvp} );
        else
          tabla_tri.push_back( {num_ver-1, i*nvp, (i+1)*nvp} );
      }


      // Cara superior
      Tupla3f vertice_sup = perfil_original[nvp-1];
      tabla_ver.push_back( {0.0, vertice_sup(Y), 0.0} );
      num_ver++;
      for (unsigned i = 0; i < n; i++){
        if (cerrar_malla)
          tabla_tri.push_back( {num_ver-1, i*nvp+(nvp-1), ((i+1)%nper)*nvp+(nvp-1)} );
        else
          tabla_tri.push_back( {num_ver-1, i*nvp+(nvp-1), (i+1)*nvp+(nvp-1)} );
      }
    }

   // colores
   setColorVertices();
}

// *****************************************************************************

Cilindro::Cilindro ( const int num_verts_per,
                     const unsigned nperfiles,
                     const bool crear_tapas,
                     const bool cerrar_malla,
                     const bool textura_auto )
    : MallaRevol( "malla por revolución de un cilindro" )
{
    vector<Tupla3f> perfil_original;
    perfil_original.push_back( {1, 0, 0} );
    perfil_original.push_back( {1, 1, 0} );
    nvp = num_verts_per;
    nper = nperfiles;
    num_ver = nvp * nper;

    crearMallaRevol( perfil_original, crear_tapas, cerrar_malla, textura_auto );
}

// *****************************************************************************

Cono::Cono ( const int num_verts_per,
             const unsigned nperfiles,
             const bool crear_tapas,
             const bool cerrar_malla,
             const bool textura_auto )
    : MallaRevol( "malla por revolución de un cono" )
{
    vector<Tupla3f> perfil_original;
    perfil_original.push_back( {1, 0, 0} );
    perfil_original.push_back( {0, 1, 0} );
    nvp = 2;
    nper = nperfiles;
    num_ver = nvp * nper; // (nvp-1) * nper + 1

    crearMallaRevol( perfil_original, crear_tapas, cerrar_malla, textura_auto );
}

// *****************************************************************************

Esfera::Esfera ( const int num_verts_per,
                 const unsigned nperfiles,
                 const bool crear_tapas,
                 const bool cerrar_malla,
                 const bool textura_auto )
    : MallaRevol( "malla por revolución de una esfera" )
{
    vector<Tupla3f> perfil_original;
    nvp = num_verts_per;
    nper = nperfiles;
    num_ver = nvp * nper; // (nvp-2) * nper + 2

    float dist = 2.0/(nvp-1);
    for (int i = 0; i < nvp; i++) {
      float y = -1.0 + i*dist;
      perfil_original.push_back( {sqrt(1-y*y), y, 0} );
    }

    crearMallaRevol( perfil_original, crear_tapas, cerrar_malla, textura_auto );
}
