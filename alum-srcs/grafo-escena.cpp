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
   cv.pilaMateriales.push();      // push de la pila de materiales
   glMatrixMode( GL_MODELVIEW );  // operaremos sobre la modelview
   glPushMatrix();                // guarda modelview actual

   // recorrer todas las entradas del array que hay en el nodo:
   for (unsigned i = 0; i < entradas.size(); i++) {
     switch (entradas[i].tipo) {
       case TipoEntNGE::objeto:
        entradas[i].objeto->visualizarGL(cv);    // visualizarlo
       break;

       case TipoEntNGE::transformacion:
        glMatrixMode( GL_MODELVIEW );  // modo modelview
        glMultMatrixf( *(entradas[i].matriz ) ); // componerla
       break;

       case TipoEntNGE::material:
        cv.pilaMateriales.activarMaterial( entradas[i].material );
       break;
     }
   }

   glMatrixMode( GL_MODELVIEW );  // operaremos sobre la modelview
   glPopMatrix();                 // restaura modelview guardada
   cv.pilaMateriales.pop();
}
// -----------------------------------------------------------------------------

NodoGrafoEscena::NodoGrafoEscena()
{
   ponerNombre("Escena anónima");

}
// -----------------------------------------------------------------------------

void NodoGrafoEscena::fijarColorNodo( const Tupla3f & nuevo_color )
{
   if (nuevo_color != color) {
     color = nuevo_color;
     for (unsigned i = 0; i < entradas.size(); i++)
       if (entradas[i].tipo == TipoEntNGE:: objeto)
         entradas[i].objeto->fijarColorNodo(color);
   }
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
  int ind_p1, ind_p2, ind_p3;

  agregar( new Base() );
  agregar( MAT_Traslacion(0.0, 0.5, 0.0) );
  ind_p1 = agregar(id);                // Grado de libertad 1
  agregar( new ParteInf() );
  agregar( MAT_Traslacion(2.0, 3.7, -0.5) );
  ind_p2 = agregar(id);                // Grado de libertad 2
  agregar( MAT_Rotacion(-30, 0, 0, 1) );
  agregar( new Brazo() );
  agregar( MAT_Traslacion(-4.2, 0.0, 0.0) );
  Cabezal * cabezal = new Cabezal();
  agregar( cabezal );

  Parametro p1("rotación del cuerpo", leerPtrMatriz(ind_p1),
               [=](float v) {return MAT_Rotacion(v, 0, 1, 0);},
               false, 0, 5, 0);
  parametros.push_back(p1);

  Parametro p2("rotación de la articulación inferior", leerPtrMatriz(ind_p2),
               [=](float v) {return MAT_Rotacion(v, 0, 0, 1);},
               true, 0, 60, 0.02);
  parametros.push_back(p2);

  Parametro p3("rotación del cabezal", cabezal->getPtrP3(),
               [=](float v) {return MAT_Rotacion(v, 0, 0, 1);},
               true, -80, 120, 0.05);
  parametros.push_back(p3);

  Parametro p4("movimiento pinzas", cabezal->getPtrP4(),
               [=](float v) {return MAT_Traslacion(0.0, 0.0, v);},
               true, 0, 0.2, 0.1);
  parametros.push_back(p4);

  Parametro p5("movimiento pinza der", cabezal->getPtrP5(),
               [=](float v) {return MAT_Traslacion(0.0, 0.0, -v);},
               true, 0, 0.2, 0.1);
  parametros.push_back(p5);

}

// -----------------------------------------------------------------------------
Base::Base()
{
  agregar( MAT_Escalado(1.2, 0.7, 1.2) );
  Cilindro * base = new Cilindro(2, 20, true, true, false);
  base->fijarColorNodo( {0.125, 0.125, 0.125} );  // ejemplo de fijarColorNodo
  agregar( base );
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
Cabezal::Cabezal()
{
  Matriz4f id = MAT_Ident();
  ind_p3 = agregar( id );  // Grado de libertad 3
  agregar( MAT_Rotacion(90, 0, 0, 1) );
  agregar( new Articulacion() );
  agregar( MAT_Rotacion(180, 0, 0, 1) );
  agregar( MAT_Traslacion(0.5, -0.2, 0.4) );
  ind_p4 = agregar(id);   // Grado de libertad 4
  agregar( new Pinza() );
  agregar( MAT_Traslacion(0.0, 0.0, 0.3) );
  ind_p5 = agregar(id);   // Grado de libertad 5
  agregar( new Pinza() );
}

Matriz4f * Cabezal::getPtrP3()
{
  return leerPtrMatriz( ind_p3 );
}

Matriz4f * Cabezal::getPtrP4()
{
  return leerPtrMatriz( ind_p4 );
}

Matriz4f * Cabezal::getPtrP5()
{
  return leerPtrMatriz( ind_p5 );
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
  Cilindro * articulacion = new Cilindro(2, 20, true, true, false);
  articulacion->fijarColorNodo( {0.125, 0.125, 0.125} );   // ejemplo de fijarColorNodo
  agregar( articulacion );
}

// -----------------------------------------------------------------------------
Pinza::Pinza()
{
  agregar( MAT_Escalado(0.5, 0.3, 0.1) );
  Cubo * pinza = new Cubo();
  std::vector<Tupla3f> colores;
  for (unsigned i = 0; i < pinza->getNumVer(); i++)
    colores.push_back( {0.0, (float)(i+1)/ pinza->getNumVer(), 0.0} );
  pinza->setColorVertices( &colores );   // ejemplo de setColorVertices
  agregar( pinza );
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
EscenaP4::EscenaP4()
{
  ponerNombre( "escena para la P4" );
  //agregar( new MallaRevol("../plys/peon.ply", 20, true, false, false) );

  agregar( MAT_Escalado(2.5, 2.5, 2.5) );
  agregar( new Lata() );
  agregar( MAT_Escalado(0.2, 0.2, 0.2) );
  agregar( MAT_Traslacion(0.0, 1.4, 5.0) );
  agregar( new PeonMadera() );
  agregar( MAT_Traslacion(2.5, 0.0, 0.0) );
  agregar( new PeonBlanco() );
  agregar( MAT_Traslacion(2.5, 0.0, 0.0) );
  agregar( new PeonNegro() );

}

// -----------------------------------------------------------------------------
Lata::Lata()
{
  ponerNombre( "lata" );

  agregar( new MaterialLata() );
  agregar( new MallaRevol("../plys/lata-pcue.ply", 20, false, false, true) );
  agregar( new MaterialTapasLata() );
  agregar( new MallaRevol("../plys/lata-pinf.ply", 20, false, false, true) );
  agregar( new MaterialTapasLata() );
  agregar( new MallaRevol("../plys/lata-psup.ply", 20, false, false, true) );

}

// -----------------------------------------------------------------------------
PeonMadera::PeonMadera()
{
  ponerNombre( "peón de madera" );

  agregar( new MaterialPeonMadera() );
  agregar( new MallaRevol("../plys/peon.ply", 20, true, false, false) );
}

// -----------------------------------------------------------------------------
PeonBlanco::PeonBlanco()
{
  ponerNombre( "peón blanco" );

  agregar( new MaterialPeonBlanco() );
  agregar( new MallaRevol("../plys/peon.ply", 20, true, false, false) );
}

// -----------------------------------------------------------------------------
PeonNegro::PeonNegro()
{
  ponerNombre( "peón negro" );

  agregar( new MaterialPeonNegro() );
  agregar( new MallaRevol("../plys/peon.ply", 20, true, false, false) );
}
