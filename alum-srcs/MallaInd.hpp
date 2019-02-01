// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Implementación de la clase MallaInd
// **
// *********************************************************************

#ifndef IG_MALLAIND_HPP
#define IG_MALLAIND_HPP

#include <vector>          // usar std::vector

#include "Objeto3D.hpp"   // declaración de 'Objeto3D'

#include <matrices-tr.hpp>


// ---------------------------------------------------------------------
// funciones auxiliares

// calcula el centro del objeto
Tupla3f calcularCentroCajaEnglobante(const std::vector<Tupla3f> & ver);

// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaInd : public Objeto3D
{

protected:
  unsigned num_ver;                       // número de vértices

  std::vector<Tupla3f> tabla_ver;         // tabla de vértices
  std::vector<Tupla3i> tabla_tri;         // tabla de triángulos (caras)
  std::vector<Tupla3f> col_ver;           // colores de los vértices
  std::vector<Tupla3f> tabla_nor_caras;   // tabla de normales de caras
  std::vector<Tupla3f> tabla_nor_ver;     // tabla de normales de vértices
  std::vector<Tupla2f> coor_textura;      // coordenadas de textura

  GLuint id_vbo_ver;      // identificador del VBO con la tabla de vértices
  GLuint id_vbo_tri;      // identificador del VBO con la tabla de triángulos
  GLuint id_vbo_col_ver;  // identificador del VBO con los colores
  GLuint id_vbo_nor_ver;  // identificador del VBO con las normales de los vértices
  GLuint id_vbo_cctt;     // identificador del VBO con las coordenadas de textura

  unsigned tam_ver;       // tamaño en bytes de la tabla de vértices
  unsigned tam_tri;       // tamaño en bytes de la tabla de triángulos
  unsigned tam_cctt;      // tamaño en bytes de la tabla de coordenadas de textura

  bool usar_texturas;     // indica si se usan o no texturas
  bool vbos_creados;      // indica si los VBOs han sido creados
  bool normales_creadas;  // indica si las normales se ha creado

  // calculo de las normales de esta malla
  void calcular_normales();
  // visualizar con 'draw elements', en modo inmediato
  void visualizarDE_MI( ContextoVis & cv );
  // visualizar con 'draw elements', en modo diferido (con VBOS)
  void visualizarDE_VBOs( ContextoVis & cv );
  // Visualizar con glBegin/glEnd en modo inmediato
  void visualizarDE_MI_Plano(ContextoVis & cv);
  // Inicializar VBOs
  void crearVBOs();

public:
  // crea una malla vacía (nombre: "malla indexada nueva vacía")
  MallaInd();
  // crea una malla vacía con un nombre concreto:
  MallaInd(const std::string & nombreIni);
  // visualizar el objeto con OpenGL
  virtual void visualizarGL( ContextoVis & cv ) ;
  // número de vértices de la malla
  unsigned getNumVer() const;
  // Establecer colores de los vértices
  void setColorVertices(std::vector<Tupla3f> * colores = nullptr);
  // Fijar el mismo color para todos los vértices
  virtual void fijarColorNodo(const Tupla3f& color);
  // Calcular el centro del objeto
  virtual void calcularCentroOC();
} ;
// ---------------------------------------------------------------------

// ---------------------------------------------------------------------
// clase Cubo
class Cubo : public MallaInd
{
public:
  // crea un cubo de lado 1
  Cubo();
};

// ---------------------------------------------------------------------
// clase Tetraedro
class Tetraedro : public MallaInd
{
public:
  // crea un tetraedro de lado 1
  Tetraedro();
};

// ---------------------------------------------------------------------
// clase Dado
class Dado : public MallaInd
{
public:
  Dado();
};


#endif
