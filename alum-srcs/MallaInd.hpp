// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#ifndef IG_MALLAIND_HPP
#define IG_MALLAIND_HPP

#include <vector>          // usar std::vector

#include "Objeto3D.hpp"   // declaración de 'Objeto3D'
// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaInd : public Objeto3D
{

   protected:
      unsigned num_ver;                 // número de vértices de la figura
      std::vector<Tupla3f> tabla_ver;   // tabla de vértices
      std::vector<Tupla3i> tabla_tri;   // tabla de triángulos (caras)
      std::vector<Tupla3f> col_ver;     // colores de los vértices

      GLuint id_vbo_ver;      // identificador del VBO con la tabla de vértices
      GLuint id_vbo_tri;      // identificador del VBO con la tabla de triángulos
      GLuint id_vbo_col_ver;  // identificador del VBO con los colores
      unsigned tam_ver;       // tamaño en bytes de la tabla de vértices
      unsigned tam_tri;       // tamaño en bytes de la tabla de triángulos
      bool vbos_creados;      // indica si los VBOs han sido creados

      // calculo de las normales de esta malla
      void calcular_normales();
      // visualizar con 'draw elements', en modo inmediato
      void visualizarDE_MI( ContextoVis & cv );
      // visualizar con 'draw elements', en modo diferido (con VBOS)
      void visualizarDE_VBOs( ContextoVis & cv );
      // creacion de VBOs
      void crearVBOs( );
      // asigna colores a los vértices
      virtual void setColorVertices();

   public:
      // crea una malla vacía (nombre: "malla indexada nueva vacía")
      MallaInd() ;
      // crea una malla vacía con un nombre concreto:
      MallaInd( const std::string & nombreIni );
      // visualizar el objeto con OpenGL
      virtual void visualizarGL( ContextoVis & cv ) ;
} ;
// ---------------------------------------------------------------------

class Cubo : public MallaInd
{
   public:
      // crea un cubo de lado 1
      Cubo();
      // crea un cubo de lado 'size'
      Cubo(float size);
};
// ---------------------------------------------------------------------

class Tetraedro : public MallaInd
{
   public:
      // crea un tetraedro de lado 1
      Tetraedro();
      // crea un tetraedro de lado 'size'
      Tetraedro(float size);
};

#endif
