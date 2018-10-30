// *********************************************************************
// **
// ** Gestión de una grafo de escena (implementación)
// ** Copyright (C) 2016 Carlos Ureña
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

#include "aux.hpp"
#include "matrices-tr.hpp"
#include "shaders.hpp"
#include "grafo-escena.hpp"
#include "MallaRevol.hpp"

using namespace std ;


// *********************************************************************
// Entrada del nodo del Grafo de Escena

// ---------------------------------------------------------------------
// Constructor para entrada de tipo sub-objeto

EntradaNGE::EntradaNGE( Objeto3D * pObjeto )
{
   assert( pObjeto != NULL );
   tipo   = TipoEntNGE::objeto ;
   objeto = pObjeto ;
}
// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( const Matriz4f & pMatriz )
{
   tipo    = TipoEntNGE::transformacion ;
   matriz  = new Matriz4f() ; // matriz en el heap, puntero propietario
   *matriz = pMatriz ;
}

// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( Material * pMaterial )
{
   assert( pMaterial != NULL );
   tipo     = TipoEntNGE::material ;
   material = pMaterial ;
}

// -----------------------------------------------------------------------------
// Destructor de una entrada

EntradaNGE::~EntradaNGE()
{
   /**  no fnciona debido a que se hacen copias (duplicados) de punteros
   if ( tipo == TipoEntNGE::transformacion )
   {
      assert( matriz != NULL );
      delete matriz ;
      matriz = NULL ;
   }
   * **/
}

// *****************************************************************************
// Nodo del grafo de escena: contiene una lista de entradas
// *****************************************************************************

// -----------------------------------------------------------------------------
// Visualiza usando OpenGL

void NodoGrafoEscena::visualizarGL( ContextoVis & cv )
{
   glMatrixMode( GL_MODELVIEW );  // operaremos sobre la modelview
   glPushMatrix();                // guarda modelview actual
   // recorrer todas las entradas del array que hay en el nodo:
   for (unsigned i = 0; i < entradas.size(); i++) {
     if (entradas[i].tipo == TipoEntNGE::objeto )   // si la entrada es sub-objeto
       entradas[i].objeto->visualizarGL( cv );    // visualizarlo
     else {
       glMatrixMode( GL_MODELVIEW );  // modo modelview
       glMultMatrixf( *(entradas[i].matriz ) ); // componerla
     }
   }
   glMatrixMode( GL_MODELVIEW );  // operaremos sobre la modelview
   glPopMatrix();                 // restaura modelvire guardada
}
// -----------------------------------------------------------------------------

NodoGrafoEscena::NodoGrafoEscena()
{
   // COMPLETAR: práctica 3: inicializar un nodo vacío (sin entradas)
   // ........

}
// -----------------------------------------------------------------------------

void NodoGrafoEscena::fijarColorNodo( const Tupla3f & nuevo_color )
{
   // COMPLETAR: práctica 3: asignarle un color plano al nodo, distinto del padre
   // ........

}

// -----------------------------------------------------------------------------
// Añadir una entrada (al final).
// genérica

unsigned NodoGrafoEscena::agregar( const EntradaNGE & entrada )
{
   entradas.push_back( entrada );
   return entradas.size()-1;
}
// -----------------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// objeto (copia solo puntero)

unsigned NodoGrafoEscena::agregar( Objeto3D * pObjeto )
{
   return agregar( EntradaNGE( pObjeto ) );
}
// ---------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// matriz (copia objeto)

unsigned NodoGrafoEscena::agregar( const Matriz4f & pMatriz )
{
   return agregar( EntradaNGE( pMatriz ) );
}
// ---------------------------------------------------------------------
// material (copia solo puntero)
unsigned NodoGrafoEscena::agregar( Material * pMaterial )
{
   return agregar( EntradaNGE( pMaterial ) );
}

// devuelve el puntero a la matriz en la i-ésima entrada
Matriz4f * NodoGrafoEscena::leerPtrMatriz( unsigned indice )
{
   if ( indice >= 0 && indice < entradas.size() )
     if ( entradas[indice].tipo == TipoEntNGE::transformacion && entradas[indice].matriz != nullptr )
        return entradas[indice].matriz;

  cout << "ERROR: práctica 3: no hay una matriz válida en esa entrada..." << endl;
  exit(1);

}
// -----------------------------------------------------------------------------
// si 'centro_calculado' es 'false', recalcula el centro usando los centros
// de los hijos (el punto medio de la caja englobante de los centros de hijos)

void NodoGrafoEscena::calcularCentroOC()
{

   // COMPLETAR: práctica 5: calcular y guardar el centro del nodo
   //    en coordenadas de objeto (hay que hacerlo recursivamente)
   //   (si el centro ya ha sido calculado, no volver a hacerlo)
   // ........

}
// -----------------------------------------------------------------------------
// método para buscar un objeto con un identificador y devolver un puntero al mismo

bool NodoGrafoEscena::buscarObjeto
(
   const int         ident_busc, // identificador a buscar
   const Matriz4f &  mmodelado,  // matriz de modelado
   Objeto3D       ** objeto,     // (salida) puntero al puntero al objeto
   Tupla3f &         centro_wc   // (salida) centro del objeto en coordenadas del mundo
)
{
   // COMPLETAR: práctica 5: buscar un sub-objeto con un identificador
   // ........

}

// *****************************************************************************
// Nodo del grafo de escena, con una lista añadida de parámetros
// *****************************************************************************


// -----------------------------------------------------------------------------
// devuelve el numero de grados de libertad
int NodoGrafoEscenaParam::numParametros()
{
   return parametros.size();
}
// -----------------------------------------------------------------------------

// devuelve un puntero al i-ésimo grado de libertad
Parametro * NodoGrafoEscenaParam::leerPtrParametro( unsigned i )
{
   assert( i >= 0 && i < parametros.size() );
   return &(parametros[i]);

}
// -----------------------------------------------------------------------------

void NodoGrafoEscenaParam::siguienteCuadro()
{
   for (unsigned i = 0; i < parametros.size(); i++)
     parametros[i].siguiente_cuadro();
}

// *****************************************************************************
// Modelo
// *****************************************************************************


// -----------------------------------------------------------------------------
BrazoRob::BrazoRob()
{
  ponerNombre("nodo raíz del modelo");
  Matriz4f id = MAT_Ident();

  agregar( new Base() );
  agregar( MAT_Traslacion(0.0, 0.5, 0.0) );
  Matriz4f * ptr_p1 = leerPtrMatriz( agregar(id) );   // Grado de libertad 1
  agregar( new ParteInf() );
  agregar( MAT_Traslacion(2.0, 3.7, -0.5) );
  Matriz4f * ptr_p2 = leerPtrMatriz( agregar(id) );   // Grado de libertad 2
  agregar( MAT_Rotacion(-30, 0, 0, 1) );
  agregar( new Brazo() );
  agregar( MAT_Traslacion(-4.2, 0.0, 0.0) );
  Matriz4f * ptr_p3 = leerPtrMatriz( agregar(id) );   // Grado de libertad 3
  agregar( MAT_Rotacion(90, 0, 0, 1) );
  agregar( new Articulacion() );
  agregar( MAT_Rotacion(180, 0, 0, 1) );
  agregar( MAT_Traslacion(0.5, -0.2, 0.4) );
  Matriz4f * ptr_p4 = leerPtrMatriz( agregar(id) );   // Grado de libertad 4
  agregar( new Pinza() );
  agregar( MAT_Traslacion(0.0, 0.0, 0.3) );
  Matriz4f * ptr_p5 = leerPtrMatriz( agregar(id) );   // Grado de libertad 5
  agregar( new Pinza() );

  Parametro p1("rotación del cuerpo", ptr_p1,
               [=](float v) {return MAT_Rotacion(v, 0, 1, 0);},
               false, 0, 2, 0);
  parametros.push_back(p1);

  Parametro p2("rotación de la articulación inferior", ptr_p2,
               [=](float v) {return MAT_Rotacion(v, 0, 0, 1);},
               true, 0, 60, 0.02);
  parametros.push_back(p2);

  Parametro p3("rotación del cabezal", ptr_p3,
               [=](float v) {return MAT_Rotacion(v, 0, 0, 1);},
               true, -80, 120, 0.05);
  parametros.push_back(p3);

  Parametro p4("movimiento dientes", ptr_p4,
               [=](float v) {return MAT_Traslacion(0.0, 0.0, v);},
               true, 0, 0.2, 0.1);
  parametros.push_back(p4);

  Parametro p5("movimiento diente der", ptr_p5,
               [=](float v) {return MAT_Traslacion(0.0, 0.0, -v);},
               true, 0, 0.2, 0.1);
  parametros.push_back(p5);

}

// -----------------------------------------------------------------------------
Base::Base()
{
  agregar( MAT_Escalado(1.2, 0.7, 1.2) );
  agregar( new Cilindro(2, 20, true, true) );
}

// -----------------------------------------------------------------------------
ParteInf::ParteInf()
{
  agregar( MAT_Rotacion(-30, 0, 0, 1) );
  agregar( new Viga() );
}

// -----------------------------------------------------------------------------
Brazo::Brazo()
{
  agregar( new Articulacion() );
  agregar( MAT_Rotacion(90, 0, 0, 1) );
  agregar( MAT_Traslacion(0.0, 0.0, 0.5) );
  agregar( new Viga() );
}

// -----------------------------------------------------------------------------
Viga::Viga()
{
  agregar( MAT_Escalado(0.4, 4.0, 0.7) );
  agregar( new Cubo() );
}

// -----------------------------------------------------------------------------
Articulacion::Articulacion()
{
  agregar( MAT_Rotacion(90, 1, 0, 0) );
  agregar( MAT_Escalado(0.4, 1.0, 0.4) );
  agregar( new Cilindro(2, 20, true, true) );
}

// -----------------------------------------------------------------------------
Pinza::Pinza()
{
  agregar( MAT_Escalado(0.5, 0.3, 0.1) );
  agregar( new Cubo() );
}
