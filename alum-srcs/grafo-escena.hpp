// *********************************************************************
// **
// ** Gestión de una grafo de escena (declaraciones)
// ** Copyright (C) 2014-2017 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************


#ifndef GRAFO_ESCENA_HPP
#define GRAFO_ESCENA_HPP

//#include <obj-gra.hpp>
//#include <materiales.hpp>

#include "matrices-tr.hpp"
#include "materiales.hpp"
#include "Objeto3D.hpp"

// *********************************************************************
// declaración adelantada de estructura para un nodo del grafo de escena

class NodoGrafoEscena ;

// *********************************************************************
// tipo enumerado con los tipos de entradas del nodo del grafo de escena

enum class TipoEntNGE { objeto, transformacion, material, noInicializado } ;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

struct EntradaNGE
{
   TipoEntNGE tipo ;   // objeto, transformacion, material
   union
   {  Objeto3D * objeto ;  // ptr. a un objeto (no propietario)
      Matriz4f * matriz ;  // ptr. a matriz 4x4 transf. (propietario)
      Material * material ; // ptr. a material (no propietario)
   } ;
   // constructores (uno por tipo)
   EntradaNGE( Objeto3D * pObjeto ) ;      // (copia solo puntero)
   EntradaNGE( const Matriz4f & pMatriz ); // (crea copia en el heap)
   EntradaNGE( Material * pMaterial ) ;    // (copia solo puntero)
   ~EntradaNGE() ;
} ;

// *********************************************************************
// Nodo del grafo de escena: es un objeto 3D parametrizado, que contiene una lista de entradas

class NodoGrafoEscena : public Objeto3D
{
   protected:

   std::vector<EntradaNGE> entradas;    // vector de entradas
   Tupla3f color;                       // color asignado al nodo

   public:

   NodoGrafoEscena() ;

   // visualiza usando OpenGL
   virtual void visualizarGL( ContextoVis & cv ) ;
   void fijarColorNodo( const Tupla3f & nuevo_color ) ;

   // calcular y obtener la caja englobante
   //virtual CajaEngf cajaEnglobante() ;

   // añadir una entrada al final, hace copia de la entrada
   // devuelve indice de la entrada dentro del vector de entradas
   unsigned agregar( const EntradaNGE & entrada );

   // construir una entrada y añadirla (al final)
   unsigned agregar( Objeto3D * pObjeto ); // objeto (copia solo puntero)
   unsigned agregar( const Matriz4f & pMatriz ); // matriz (copia objeto)
   unsigned agregar( Material * pMaterial ); // material (copia solo puntero)

   // devuelve el puntero a la matriz en la i-ésima entrada
   Matriz4f * leerPtrMatriz( unsigned iEnt );

   // método para buscar un objeto con un identificador
   virtual bool buscarObjeto( const int ident_busc, const Matriz4f & mmodelado,
                    Objeto3D ** objeto, Tupla3f & centro_wc )  ;

   // si 'centro_calculado' es 'false', recalcula el centro usando los centros
   // de los hijos (el punto medio de la caja englobante de los centros de hijos)
   virtual void calcularCentroOC() ;

} ;

// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos que tienen además un vector de
// parámetros

class NodoGrafoEscenaParam : public NodoGrafoEscena
{
   protected:
      std::vector<Parametro> parametros;

   public:
      // devuelve el número de parámetros
      int numParametros();

      // devuelve un puntero al i-ésimo parámetro (i < numParametros())
      Parametro * leerPtrParametro( unsigned i ) ;

      // actualiza el objeto para ir al siguiente cuadro,
      // se usa cuando están activadas las animaciones, una vez antes de cada frame
      void siguienteCuadro();
} ;

// ---------------------------------------------------------------------
// clase para el objeto que es el nodo raíz del modelo
class BrazoRob : public NodoGrafoEscenaParam
{
public:
  BrazoRob();  // Constructor por defecto
};

// ---------------------------------------------------------------------
// clases para el modelo

class Base : public NodoGrafoEscena
{
public:
  Base();
};

class ParteInf : public NodoGrafoEscena
{
public:
  ParteInf();
};

class Brazo : public NodoGrafoEscena
{
public:
  Brazo();
};

class Cabezal : public NodoGrafoEscena
{
private:
  int ind_p3, ind_p4, ind_p5;   // indices para los parametros
public:
  Cabezal();
  // Metodos para obtener los punteros a las matrices
  Matriz4f * getPtrP3();
  Matriz4f * getPtrP4();
  Matriz4f * getPtrP5();
};

class Viga : public NodoGrafoEscena
{
public:
  Viga();
};

class Articulacion : public NodoGrafoEscena
{
public:
  Articulacion();
};

class Pinza : public NodoGrafoEscena
{
public:
  Pinza();
};


//----------------------------------------------------------------------
class EscenaP4 : public NodoGrafoEscena
{
public:
  EscenaP4();
};

class Lata : public NodoGrafoEscena
{
public:
  Lata();
};

class Lata2 : public NodoGrafoEscena
{
public:
  Lata2();
};

class PeonMadera : public NodoGrafoEscena
{
public:
  PeonMadera();
};

class PeonBlanco : public NodoGrafoEscena
{
public:
  PeonBlanco();
};

class PeonNegro : public NodoGrafoEscena
{
public:
  PeonNegro();
};

class DadoMat : public NodoGrafoEscena
{
public:
  DadoMat();
};


//----------------------------------------------------------------------
class EscenaP5 : public NodoGrafoEscena
{
public:
  EscenaP5();
};

#endif // GRAFO_ESCENA_HPP

// *********************************************************************
