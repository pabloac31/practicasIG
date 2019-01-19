// *********************************************************************
// **
// ** Informática Gráfica
// ** Práctica 5  (implementaciones)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica5.hpp"
#include "grafo-escena.hpp"
#include "materiales.hpp"

#include "CamaraInter.hpp"

using namespace std ;

// COMPLETAR: práctica 5: declarar variables de la práctica 5 (static)
//    (escena, viewport, el vector de camaras, y las variables
//      que sirven para gestionar el modo arrastrar)
// ......
static const float INC = 5.0;   // deplazamiento en el eje Z de la camara actual

// escena
static constexpr int numObjetos5 = 1 ;
static unsigned objetoActivo5 = 0 ;
static NodoGrafoEscena * objetos5[numObjetos5] = { nullptr };
static ColFuentesLuzP4 * luzP5 = nullptr;

// camaras
static constexpr int numCamaras =  4;
static int camaraActiva = 0;
static CamaraInteractiva * camaras[numCamaras] = {nullptr, nullptr, nullptr, nullptr};

// posiciones anteriores del raton
static int x_anterior, y_anterior;

// viewport actual (se recalcula al inicializar y al fijar las matrices)
Viewport viewport ;
// true si se está en modo arrastrar, false si no
static bool modo_arrastrar = false ;


// ---------------------------------------------------------------------

void P5_Inicializar(  int vp_ancho, int vp_alto )
{
   cout << "Creando objetos de la práctica 5 .... " << flush ;
   // COMPLETAR: práctica 5: inicializar las variables de la práctica 5 (incluyendo el viewport)
   // .......
   viewport = Viewport(0, 0, vp_ancho, vp_alto);
   luzP5 = new ColFuentesLuzP4();
   objetos5[0] = new EscenaP4();

   float ratio = (float)vp_alto / vp_ancho;

   // Alzado
   camaras[0] = new CamaraInteractiva (true, ratio, 0, 0, {0, 0, 0}, true, 80.0, 1.8);
   // Alzado ortográfica
   camaras[1] = new CamaraInteractiva (true, ratio, 0, 0, {0, 0, 0}, false, 80.0, 3.2);
   // Planta
   camaras[2] = new CamaraInteractiva (true, ratio, 0, 90, {0, 0, 0}, true, 80.0, 4.0);
   // Perfil
   camaras[3] = new CamaraInteractiva (true, ratio, 90, 0, {0, 0, 0}, true, 80.0, 2.0);

   cout << "hecho." << endl << flush ;
}
// ---------------------------------------------------------------------

void P5_FijarMVPOpenGL( int vp_ancho, int vp_alto )
{
   // COMPLETAR: práctica 5: actualizar viewport, actualizar y activar la camara actual
   //     (en base a las dimensiones del viewport)
   // .......
   viewport = Viewport(0, 0, vp_ancho, vp_alto);
   viewport.fijarViewport();

   camaras[camaraActiva]->ratio_yx_vp = viewport.ratio_yx;  // ratio
   camaras[camaraActiva]->calcularViewfrustum(); // recalcular m_proy !!
   camaras[camaraActiva]->activar();    // activamos la camara

}
// ---------------------------------------------------------------------

void P5_DibujarObjetos( ContextoVis & cv )
{

   // COMPLETAR: práctica 5: activar las fuentes de luz y visualizar la escena
   //      (se supone que la camara actual ya está activada)
   // .......
   luzP5->activar();
   objetos5[objetoActivo5]->visualizarGL(cv);
   glDisable(GL_LIGHTING);

}

// ---------------------------------------------------------------------

bool P5_FGE_PulsarTeclaCaracter(  unsigned char tecla )
{

   bool result = true ;

   switch ( toupper( tecla ) )
   {
      case 'C':
         // COMPLETAR: práctica 5: activar siguiente camara
         // .....
         camaraActiva = (camaraActiva + 1) % numCamaras;
         cout << "práctica 5: nueva cámara activa es " << camaraActiva << endl;

         break ;

      case 'V':
         // COMPLETAR: práctica 5: conmutar la cámara actual entre modo examinar y el modo primera persona
         // .....
         if (camaras[camaraActiva]->examinar)
           camaras[camaraActiva]->modoPrimeraPersona();
         else
           camaras[camaraActiva]->modoExaminar();
           cout << "práctica 5: cámara número " << camaraActiva << ": Modo cambiado a " << (camaras[camaraActiva]->examinar ? "examinar" : "primera persona") << endl;

         break ;

      case '-':
         // COMPLETAR: práctica 5: desplazamiento en Z de la cámara actual (positivo) (desplaZ)
         // .....
         camaras[camaraActiva]->desplaZ(-INC);
         cout << "práctica 5: desplazamiento hacia atrás en el eje Z" << endl;

         break;

      case '+':
         // COMPLETAR: práctica 5: desplazamiento en Z de la cámara actual (negativo) (desplaZ)
         // .....
         camaras[camaraActiva]->desplaZ(INC);
         cout << "práctica 5: desplazamiento hacia delante en el eje Z" << endl;

         break;

      default:
         result = false ;
         break ;
	}
   return result ;
}
// ---------------------------------------------------------------------

bool P5_FGE_PulsarTeclaEspecial(  int tecla  )
{

   bool result = true ;


   switch ( tecla )
   {
      case GLFW_KEY_LEFT:
         // COMPLETAR: práctica 5: desplazamiento/rotacion hacia la izquierda (moverHV)
         // .....
         camaras[camaraActiva]->moverHV(-INC, 0);
         cout << "práctica 5: desplazamiento hacia la izquierda" << endl;

         break;
      case GLFW_KEY_RIGHT:
         // COMPLETAR: práctica 5: desplazamiento/rotación hacia la derecha (moverHV)
         // .....
         camaras[camaraActiva]->moverHV(INC, 0);
         cout << "práctica 5: desplazamiento hacia la derecha" << endl;

         break;
      case GLFW_KEY_UP:
         // COMPLETAR: práctica 5: desplazamiento/rotación hacia arriba (moverHV)
         // .....
         camaras[camaraActiva]->moverHV(0, INC);
         cout << "práctica 5: desplazamiento hacia arriba" << endl;

         break;
      case GLFW_KEY_DOWN:
         // COMPLETAR: práctica 5: desplazamiento/rotación hacia abajo (moverHV)
         // .....
         camaras[camaraActiva]->moverHV(0, -INC);
         cout << "práctica 5: desplazamiento hacia abajo" << endl;

         break;
      default:
         result = false ;
         break ;
	}

   return result ;
}
// ---------------------------------------------------------------------
// se llama al hacer click con el botón izquierdo

void P5_ClickIzquierdo( int x, int y )
{

   // COMPLETAR: práctica 5: visualizar escena en modo selección y leer el color del pixel en (x,y)


   // 1. crear un 'contextovis' apropiado
   // .....

   ContextoVis cv;
   cv.modoSeleccionFBO = true;

   glClearColor(0, 0, 0, 1); // color de fondo indicado en la practica
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // limpiar pantalla

   // 2. visualizar en modo selección (sobre el backbuffer)
   // ....

   objetos5[0]->visualizarGL(cv);

   // 3. leer el color del pixel, si es 0 no se hace nada
   // .....

   unsigned id = LeerIdentEnPixel(x, y);

   if (id == 0) {
     cout << "práctica 5: no se ha seleccionado ningún objeto" << endl;
     return;
   }

   // 4. buscar el objeto en el grafo de escena e informar del mismo
   // .....

   Objeto3D * obj = nullptr;
   Matriz4f m = MAT_Ident();
   Tupla3f centro_wc(0.0, 0.0, 0.0);

   if (objetos5[objetoActivo5]->buscarObjeto(id, m, &obj, centro_wc)) {
     camaras[camaraActiva]->modoExaminar(centro_wc);
     cout << "práctica 5: objeto seleccionado: " << obj->leerNombre() << ". Centro del objeto: " << centro_wc << endl;
   }

   else
     cout << "práctica 5: objeto seleccionado no encontrado..." << endl;

}
// ---------------------------------------------------------------------
// se llama al mover el botón en modo arrastrar

void P5_InicioModoArrastrar( int x, int y )
{
   // COMPLETAR: práctica 5: activar bool de modo arrastrar, registrar (x,y) de inicio del modo arrastrar
   // .....
   modo_arrastrar = true;
   x_anterior = x;
   y_anterior = y;

}
// ---------------------------------------------------------------------
// se llama al subir el botón derecho del ratón

void P5_FinModoArrastrar()
{
   // COMPLETAR: práctica 5: desactivar bool del modo arrastrar
   // .....
   modo_arrastrar = false;

}
// ---------------------------------------------------------------------
// se llama durante el modo arrastrar

void P5_RatonArrastradoHasta( int x, int y )
{
   // COMPLETAR: práctica 5: calcular desplazamiento desde inicio de modo arrastrar, actualizar la camara (moverHV)
   // .....
   camaras[camaraActiva]->moverHV(x-x_anterior, y-y_anterior);
   x_anterior = x;
   y_anterior = y;

}
// ---------------------------------------------------------------------
// pulsar/levantar botón del ratón, específico de la práctica 5

bool P5_FGE_ClickRaton( int boton, int estado, int x, int y )
{
   //cout << "P5_FGE_ClickRaton" << endl ;
   if ( estado == GLFW_PRESS && boton == GLFW_MOUSE_BUTTON_LEFT  )
      P5_ClickIzquierdo( x, viewport.alto-y );
   else if ( estado == GLFW_PRESS && boton == GLFW_MOUSE_BUTTON_RIGHT )
      P5_InicioModoArrastrar( x, viewport.alto-y );
   else if ( estado == GLFW_RELEASE && boton == GLFW_MOUSE_BUTTON_RIGHT )
      P5_FinModoArrastrar();
   else
      return false ;

   return true ;
}
// ---------------------------------------------------------------------
bool P5_FGE_RatonMovidoPulsado( int x, int y )
{
   if ( modo_arrastrar )
   {
      P5_RatonArrastradoHasta( x, viewport.alto-y );
      return true ;
   }
   else
      return false ;
}
// ---------------------------------------------------------------------

bool P5_FGE_Scroll( int direction )
{
   // COMPLETAR: práctica 5: acercar/alejar la camara (desplaZ)
   // .....
   camaras[camaraActiva]->desplaZ(direction * INC);
   return true ;
}
// ---------------------------------------------------------------------

void FijarColorIdent( const int ident )  // 0 ≤ ident < 2^24
{
   // COMPLETAR: práctica 5: fijar color actual de OpenGL usando 'ident' (glColor3ub)
   // .....
   // ver P.57
    assert(ident >= 0);

    const unsigned char
      byteR = (ident           ) % 0x100U,    // rojo = byte menos significativo
      byteG = (ident /   0x100U) % 0x100U,    // verde = byte intermedio
      byteB = (ident / 0x10000U) % 0x100U;    // azul = byte más significativo

    glColor3ub(byteR, byteG, byteB);    // cambio de colo en OpenGL


}
//---------------

int LeerIdentEnPixel( int xpix, int ypix )
{
   // COMPLETAR: práctica 5: leer el identificador codificado en el color del pixel (x,y)
   // .....
   // ver P.58
   unsigned char bytes[3];    // para guardar los 3 bytes
   // leer los 3 bytes del frame-buffer (pixel = 1x1)
   glReadPixels(xpix, ypix, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *)bytes);
   // reconstruir el indentificador y devolverlo:
   return bytes[0] + (0x100U * bytes[1]) + (0x10000U * bytes[2]) ;

}
//---------------
