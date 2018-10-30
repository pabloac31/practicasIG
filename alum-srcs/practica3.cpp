// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica3.hpp"
#include "grafo-escena.hpp" // NodoGrafoEscena



using namespace std ;

// COMPLETAR: práctica 3: declarar variables de la práctica 3 (static)
// .................
static constexpr int numObjetos3 = 1 ;
static unsigned objetoActivo3 = 0 ;
static NodoGrafoEscenaParam * objetos3[numObjetos3] = { nullptr };
static bool animaciones = false;
static unsigned parametro_activo = 0;


// ---------------------------------------------------------------------
// Función para implementar en la práctica 1 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P3_Inicializar(  )
{
   cout << "Creando objetos de la práctica 3 .... " << flush ;

   objetos3[0] = new BrazoRob();

   cout << "hecho." << endl << flush ;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 1 activa:
// (si la tecla no se procesa en el 'main').
//
//  - devuelve 'true' si la tecla se usa en esta práctica para cambiar
//    entre el grado de libertad o incrementar/decrementar valores
//  - devuelve 'false' si la tecla no se usa en esta práctica (no ha
//    cambiado nada)

bool P3_FGE_PulsarTeclaCaracter( unsigned char tecla )
{
   bool res = false  ; // valor devuelto (true si se ha procesado tecla)

   switch ( toupper( tecla ) )
   {
      case 'O' :
         objetoActivo3 = (objetoActivo3+1) % numObjetos3 ;
         cout << "práctica 3: nuevo objeto activo es: " << objetoActivo3 ;
         if ( objetos3[objetoActivo3] != nullptr )
            cout << " (" << objetos3[objetoActivo3]->leerNombre() << ")." << endl ;
         else
            cout << " (objeto no creado)" << endl ;
         res = true ;
         break ;

      case 'A' :
         animaciones = !animaciones;
         if (animaciones) {
           FijarFuncDesocupado( FGE_Desocupado );
         }
         cout << "práctica 3: animaciones " << ( animaciones ? "activadas" : "desactivadas" ) << endl;
         res = true;
         break ;

      case 'G' :
         if (objetos3[objetoActivo3] != nullptr && objetos3[objetoActivo3]->numParametros() > 0) {
           parametro_activo = (parametro_activo + 1) % objetos3[objetoActivo3]->numParametros();
           cout << "práctica 3: nuevo parámetro activo es: ("
                << objetos3[objetoActivo3]->leerPtrParametro(parametro_activo)->leer_descripcion()
                << ")" << endl;
           res = true;
         }

         break ;

      case 'R' :
         animaciones = false;
         cout << "práctica 3: animaciones desactivadas" << endl;

         if (objetos3[objetoActivo3] != nullptr) {
           for (unsigned i = 0; i < objetos3[objetoActivo3]->numParametros(); i++)
             objetos3[objetoActivo3]->leerPtrParametro(i)->reset();

           cout << "práctica 3: parámetros reseteados" << endl;
         }
         res = true;

         break ;

      case '>' :
         if (objetos3[objetoActivo3] != nullptr) {
           if (animaciones) {
             objetos3[objetoActivo3]->leerPtrParametro(parametro_activo)->acelerar();
             cout << "práctica 3: velocidad del parámetro " << parametro_activo << " = "
                  << objetos3[objetoActivo3]->leerPtrParametro(parametro_activo)->leer_velocidad_actual()
                  << endl;
           }
           else {
             objetos3[objetoActivo3]->leerPtrParametro(parametro_activo)->incrementar();
             cout << "práctica 3: valor del parámetro " << parametro_activo << " = "
                  << objetos3[objetoActivo3]->leerPtrParametro(parametro_activo)->leer_valor_actual()
                  << endl;
           }
           res = true;
         }

         break ;

      case '<' :
         if (objetos3[objetoActivo3] != nullptr) {
           if (animaciones) {
             objetos3[objetoActivo3]->leerPtrParametro(parametro_activo)->decelerar();
             cout << "práctica 3: velocidad del parámetro " << parametro_activo << " = "
                  << objetos3[objetoActivo3]->leerPtrParametro(parametro_activo)->leer_velocidad_actual()
                  << endl;
           }
           else {
             objetos3[objetoActivo3]->leerPtrParametro(parametro_activo)->decrementar();
             cout << "práctica 3: valor del parámetro " << parametro_activo << " = "
                  << objetos3[objetoActivo3]->leerPtrParametro(parametro_activo)->leer_valor_actual()
                  << endl;
           }
           res = true;
         }

         break ;
      default :
         break ;

   }
   return res ;
}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 3  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P3_DibujarObjetos( ContextoVis & cv )
{
   objetos3[objetoActivo3]->visualizarGL(cv);
}

//--------------------------------------------------------------------------

bool P3_FGE_Desocupado()
{
   // no hacer nada si no es necesario, y desactivar
   if (!animaciones)
     return false;    // provoca que el bucle principal espere nuevos eventos

   // modificar los parámetros animables según sus velocidades actuales
   if (objetos3[objetoActivo3] != nullptr)
     objetos3[objetoActivo3]->siguienteCuadro();

   // forzar llamada a VisualizarFrame en la próxima iteración del bucle
   redibujar_ventana = true;

   // terminar, manteniendo activa la gestión del evento
   return true;
}
