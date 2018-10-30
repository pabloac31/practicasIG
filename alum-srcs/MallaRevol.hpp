// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase MallaRevol
// **
// *********************************************************************

#ifndef IG_MALLAREVOL_HPP
#define IG_MALLAREVOL_HPP

#include <vector>          // usar std::vector
#include <string>

#include "MallaInd.hpp"   // declaración de 'Objeto3D'
// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaRevol : public MallaInd
{
protected:
      unsigned
      nper , // numero de perfiles
      nvp  ; // numero de vertices por perfil

   // Método que crea las tablas vertices y triangulos
   void crearMallaRevol
   ( const std::vector<Tupla3f> & perfil_original,  // vértices del perfil original
     const bool crear_tapas,    // true para crear tapas
     const bool cerrar_malla    // true para cerrar la malla
   ) ;

   public:
      MallaRevol() {}  // Constructor vacío
      // crea una malla de revolucion
      MallaRevol( const std::string & nombre_arch,
                  const unsigned nperfiles,
                  const bool     crear_tapas,
                  const bool     cerrar_malla ) ;


} ;


// ---------------------------------------------------------------------
// clase Cilindro

class Cilindro : public MallaRevol
{
  public:
  // Constructor: crea el perfil original y llama a crearMalla
  // la base tiene el centro en el origen, el radio y la altura son 1
  Cilindro
  ( const int num_verts_per,    // número de vértices del perfil original (M )
    const unsigned nperfiles,   // número de perfiles (N )
    const bool crear_tapas,     // true para crear tapas
    const bool cerrar_malla     // true para cerrar la malla
  ) ;

} ;


// ---------------------------------------------------------------------
// clase Cono

class Cono : public MallaRevol
{
  public:
  // Constructor: crea el perfil original y llama a crearMalla
  // la base tiene el centro en el origen, el radio y la altura son 1
  Cono
  ( const int num_verts_per,    // número de vértices del perfil original (M )
    const unsigned nperfiles,   // número de perfiles (N )
    const bool crear_tapas,     // true para crear tapas
    const bool cerrar_malla     // true para cerrar la malla
  ) ;

} ;


// ---------------------------------------------------------------------
// clase Esfera

class Esfera : public MallaRevol
{
  public:
  // Constructor: crea el perfil original y llama a crearMalla
  // La esfera tiene el centro en el origen, el radio es la unidad
  Esfera
  ( const int num_verts_per,    // número de vértices del perfil original (M )
    const unsigned nperfiles,   // número de perfiles (N )
    const bool crear_tapas,     // true para crear tapas
    const bool cerrar_malla     // true para cerrar la malla
  ) ;

} ;

#endif
