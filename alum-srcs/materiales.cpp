// *********************************************************************
// **
// ** Gestión de materiales y texturas (implementación)
// **
// ** Copyright (C) 2014 Carlos Ureña
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


#include "matrices-tr.hpp"
#include "materiales.hpp"

using namespace std ;

const bool trazam = false ;

//*********************************************************************

PilaMateriales::PilaMateriales()
{
   actual = nullptr ;
}
// -----------------------------------------------------------------------------

void PilaMateriales::activarMaterial( Material * material )
{
   if ( material != actual )
   {
      actual = material ;
      if ( actual != nullptr )
         actual->activar();
   }
}
// -----------------------------------------------------------------------------

void PilaMateriales::activarActual()
{
   if ( actual != nullptr )
      actual->activar() ;
}
// -----------------------------------------------------------------------------

void PilaMateriales::push(  )
{
   pila.push_back( actual );
}
// -----------------------------------------------------------------------------

void PilaMateriales::pop(  )
{
   if ( pila.size() == 0 )
   {
      cout << "error: intento de hacer 'pop' de un material en una pila de materiales vacía." << endl << flush ;
      exit(1);
   }

   Material * anterior = pila[pila.size()-1] ;
   pila.pop_back();
   activarMaterial( anterior );  // cambia 'actual'
}

//**********************************************************************

Textura::Textura( const std::string & nombreArchivoJPG )
{
   enviada = false;
   modo_gen_ct = mgct_desactivada;  // por defecto
   imagen = new jpg::Imagen( nombreArchivoJPG );
   glGenTextures(1, &ident_textura); // asigna un nuevo identificador
}

//----------------------------------------------------------------------

void Textura::enviar()
{
   // COMPLETAR: práctica 4: enviar la imagen de textura a la GPU
   // .......

   glBindTexture(GL_TEXTURE_2D, ident_textura); // seleccionamos la textura

   gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, imagen->tamX(), imagen->tamY(),
   GL_RGB, GL_UNSIGNED_BYTE, imagen->leerPixels() );

   enviada = true;
}

//----------------------------------------------------------------------

Textura::~Textura( )
{
   using namespace std ;
   cout << "destruyendo textura...imagen ==" << imagen << endl ;
   if ( imagen != NULL )
      delete imagen ;

   imagen = NULL ;
   cout << "hecho (no hecho!)" << endl << flush ;
}

//----------------------------------------------------------------------
// por ahora, se asume la unidad de texturas #0

void Textura::activar(  )
{

   glEnable( GL_TEXTURE_2D ); // habilita texturas

   if (!enviada)
    enviar();

   else
    glBindTexture( GL_TEXTURE_2D, ident_textura );  // seleccionamos textura

   if (modo_gen_ct != mgct_desactivada) {
     // activamos generación procedural de coordenadas de textura
     glEnable( GL_TEXTURE_GEN_S );
     glEnable( GL_TEXTURE_GEN_T );

     if (modo_gen_ct == mgct_coords_objeto) {
       // modo coords. de objeto
       glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
       glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
       // especificamos los coef de las funciones lineales
       glTexGenfv( GL_S, GL_OBJECT_PLANE, coefs_s );
       glTexGenfv( GL_T, GL_OBJECT_PLANE, coefs_t );
     }

     else if (modo_gen_ct == mgct_coords_ojo) {
       // modo coords. del ojo
       glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );
       glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );
       // especificamos los coef de las funciones lineales
       glTexGenfv( GL_S, GL_EYE_PLANE, coefs_s );
       glTexGenfv( GL_T, GL_EYE_PLANE, coefs_t );
     }

   }

   else {
     glDisable( GL_TEXTURE_GEN_S );
     glDisable( GL_TEXTURE_GEN_T );
   }

   // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) // texel con centro más cercano
   // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) // interpolación (por defecto)

}

// *********************************************************************

TexturaXY::TexturaXY( const std::string & nom ) : Textura(nom)
{
  modo_gen_ct = mgct_coords_ojo;

  // proyecta al plano XY

  coefs_s[0] = 1.0;
  coefs_s[1] = 0.0;
  coefs_s[2] = 0.0;
  coefs_s[3] = 0.0;

  coefs_t[0] = 0.0;
  coefs_t[1] = 1.0;
  coefs_t[2] = 0.0;
  coefs_t[3] = 0.0;
}

// *********************************************************************

Material::Material()
{
   ponerNombre( "material sin textura ni iluminacion");
   iluminacion = false ;
   tex = NULL ;
   coloresCero() ;
}

// ---------------------------------------------------------------------

Material::Material( const std::string & nombreArchivoJPG )
{
  ponerNombre( "material con textura e iluminacion por defecto");
   iluminacion    = true ;     // true ??
   tex            = new Textura( nombreArchivoJPG ) ;

   coloresCero();

   del.emision   = VectorRGB(0.0,0.0,0.0,1.0);
   del.ambiente  = VectorRGB( 0.0, 0.0, 0.0, 1.0);
   del.difusa    = VectorRGB( 0.5, 0.5, 0.5, 1.0 );
   del.especular = VectorRGB( 1.0, 1.0, 1.0, 1.0 );

   del.emision   = VectorRGB(0.0,0.0,0.0,1.0);
   del.ambiente  = VectorRGB( 0.0, 0.0, 0.0, 1.0);
   tra.difusa    = VectorRGB( 0.2, 0.2, 0.2, 1.0 );
   tra.especular = VectorRGB( 0.2, 0.2, 0.2, 1.0 );

}

// ---------------------------------------------------------------------
// crea un material usando textura y coeficientes: ka,kd,ks y exponente
// (la textura puede ser NULL, la ilum. queda activada)

Material::Material( Textura * text, float ka, float kd, float ks, float exp )
:  Material()
{
   // COMPLETAR: práctica 4: inicializar material usando text,ka,kd,ks,exp
   // .....

   ponerNombre("material con textura e iluminación") ;

   iluminacion = true;
   tex = text;

   coloresCero();

   del.ambiente = tra.ambiente = VectorRGB( ka, ka, ka, 1.0 );
   del.difusa = tra.difusa = VectorRGB( kd, kd, kd, 1.0 );
   del.especular = tra.especular = VectorRGB( ks, ks, ks, 1.0 );
   del.exp_brillo = tra.exp_brillo = exp;

 }

// ---------------------------------------------------------------------
// crea un material con un color único para las componentes ambiental y difusa
// en el lugar de textura (textura == NULL)
Material::Material( const Tupla3f & colorAmbDif, float ka, float kd, float ks, float exp )
{
   // COMPLETAR: práctica 4: inicializar material usando colorAmbDif,ka,kd,ks,exp
   // .....

   ponerNombre("material color plano, ilum.") ;

   iluminacion = true;
   tex = NULL;

   coloresCero();

   color = VectorRGB( colorAmbDif(0), colorAmbDif(1), colorAmbDif(2), 1.0 );

   del.ambiente = tra.ambiente = VectorRGB(ka * colorAmbDif(R), ka * colorAmbDif(G), ka * colorAmbDif(B), 1.0);
   del.difusa = tra.difusa = VectorRGB(kd * colorAmbDif(R), kd * colorAmbDif(G), kd * colorAmbDif(B), 1.0);
   del.especular = tra.especular = VectorRGB( ks, ks, ks, 1.0 );
   del.exp_brillo = tra.exp_brillo = exp;
}
// ---------------------------------------------------------------------

Material::Material( const float r, const float g, const float b )
{
   ponerNombre("material color plano");

   iluminacion = false;
   tex = NULL;

   //coloresCero();

   color = VectorRGB( r, g, b, 1.0 );
}

//----------------------------------------------------------------------

void Material::coloresCero()
{
   const VectorRGB ceroRGBopaco(0.0,0.0,0.0,1.0);

   color         =

   del.emision   =
   del.ambiente  =
   del.difusa    =
   del.especular =

   tra.emision   =
   tra.ambiente  =
   tra.difusa    =
   tra.especular = ceroRGBopaco ;

   del.exp_brillo =
   tra.exp_brillo = 1.0 ;
}
//----------------------------------------------------------------------

Material::~Material()
{
   if ( tex != nullptr )
   {  delete tex ;
      tex = nullptr ;
   }
}
//----------------------------------------------------------------------

void Material::ponerNombre( const std::string & nuevo_nombre )
{
   nombre_mat = nuevo_nombre ;
}
//----------------------------------------------------------------------

std::string Material::nombre() const
{
   return nombre_mat ;
}
//----------------------------------------------------------------------

void Material::activar(  )
{
   // COMPLETAR: práctica 4: activar un material
   // .....
   // cout << "activando material..." << endl;
   // cout << "textura = " << tex << endl;


   if (tex == nullptr)
     glDisable(GL_TEXTURE_2D);

   else
    tex->activar();

  if (iluminacion) {
    glEnable( GL_LIGHTING );

    // el color de la textura se usa en luegar de M_A y M_D, pero no en M_S
    glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );

    glMaterialfv( GL_FRONT, GL_EMISSION, del.emision );
    glMaterialfv( GL_FRONT, GL_AMBIENT, del.ambiente );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, del.difusa );
    glMaterialfv( GL_FRONT, GL_SPECULAR, del.especular );
    glMaterialf( GL_FRONT, GL_SHININESS, del.exp_brillo );

    glMaterialfv( GL_BACK, GL_EMISSION, tra.emision );
    glMaterialfv( GL_BACK, GL_AMBIENT, tra.ambiente );
    glMaterialfv( GL_BACK, GL_DIFFUSE, tra.difusa );
    glMaterialfv( GL_BACK, GL_SPECULAR, tra.especular );
    glMaterialf( GL_BACK, GL_SHININESS, tra.exp_brillo );
  }

  else {
    glDisable( GL_LIGHTING );
    glColor4fv( color );
  }

  // glLightModelfv(  GL_LIGHT_MODEL_AMBIENT, VectorRGB(color(X), color(Y), color(Z), 1.0) ); // hace A_G = color, inicialmente es (0.2,0.2,0.2,1.0)
  // glDisable( GL_COLOR_MATERIAL ); // pag 109

}

//**********************************************************************

FuenteLuz::FuenteLuz( const VectorRGB & p_color )
{
   //CError();

   if ( trazam )
      cout << "creando fuente de luz." <<  endl << flush ;

   // inicializar parámetros de la fuente de luz
   col_ambiente  = p_color ;
   col_difuso    = p_color ;
   col_especular = p_color ;

   ind_fuente = -1 ; // la marca como no activable hasta que no se le asigne indice

   //CError();
}

//----------------------------------------------------------------------

FuenteLuz::~FuenteLuz()
{

}

//----------------------------------------------------------------------

FuenteDireccional::FuenteDireccional( GLfloat alpha_inicial, GLfloat beta_inicial, const VectorRGB & p_color )
: FuenteLuz(p_color)
{
  longi_ini = longi = alpha_inicial;
  lati_ini = lati = beta_inicial;
}

//----------------------------------------------------------------------

void FuenteDireccional::activar()
{
  if ( ind_fuente != -1 ) {
    // Activar fuente
    glEnable(GL_LIGHT0+ind_fuente);
    // Configuramos los colores
    glLightfv(GL_LIGHT0+ind_fuente, GL_AMBIENT, col_ambiente);
    glLightfv(GL_LIGHT0+ind_fuente, GL_DIFFUSE, col_difuso);
    glLightfv(GL_LIGHT0+ind_fuente, GL_SPECULAR, col_especular);

    const Tupla4f ejeZ = {0.0, 0.0, 1.0, 0.0};  // el último valor determina el tipo (direccional)
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix() ;
    //glLoadIdentity();                   // M = Identidad
    glRotatef( longi, 0.0, 1.0, 0.0 );  // rotación alpha grados (eje Y)
    glRotatef( lati, -1.0, 0.0, 0.0 );  // rotación beta grados (eje -X)
    glLightfv( GL_LIGHT0+ind_fuente, GL_POSITION, ejeZ);   // paralela eje Z (0,0,1)
    glPopMatrix();
  }

  else
    std::cout << "ERROR: intento de activar una luz no válida" << std::endl;
}

//----------------------------------------------------------------------

bool FuenteDireccional::gestionarEventoTeclaEspecial( int key )
{
   bool actualizar = true ;
   const float incr = 3.0f ;

   switch( key )
   {
      case GLFW_KEY_RIGHT:
         longi = longi+incr ;
         break ;
      case GLFW_KEY_LEFT:
         longi = longi-incr ;
         break ;
      case GLFW_KEY_UP:
         lati = std::min( lati+incr, 90.0f) ;
         break ;
      case GLFW_KEY_DOWN:
         lati = std::max( lati-incr, -90.0f ) ;
         break ;
      case GLFW_KEY_HOME:
         lati  = lati_ini ;
         longi = longi_ini ;
         break ;
      default :
         actualizar = false ;
         cout << "tecla no usable para la fuente de luz." << endl << flush ;
   }

   if ( actualizar )
      cout << "fuente de luz cambiada: longi == " << longi << ", lati == " << lati << endl << flush ;
   return actualizar ;
}

//----------------------------------------------------------------------

void FuenteDireccional::variarAngulo(unsigned angulo, float incremento)
{
  if(angulo == 0)
    longi+=incremento;
  else
    lati+=incremento;
}
//----------------------------------------------------------------------

FuentePosicional::FuentePosicional( const Tupla3f & pos, const VectorRGB & p_color )
: FuenteLuz(p_color)
{
  posicion = Tupla4f(pos(X), pos(Y), pos(Z), 1.0);  // el 1 indica que es posicional
}

//----------------------------------------------------------------------

void FuentePosicional::activar()
{
   if (ind_fuente != -1) {
     glEnable( GL_LIGHT0 + ind_fuente );

     // Configuramos los colores de ambiente
     glLightfv( GL_LIGHT0 + ind_fuente, GL_AMBIENT, col_ambiente );
     glLightfv( GL_LIGHT0 + ind_fuente, GL_DIFFUSE, col_difuso );
     glLightfv( GL_LIGHT0 + ind_fuente, GL_SPECULAR, col_especular );

     // Configuramos posicion
     glLightfv( GL_LIGHT0 + ind_fuente, GL_POSITION, posicion );
   }

   else
    std::cout << "ERROR: intento de activar una luz no válida" << std::endl;
}

//**********************************************************************

ColFuentesLuz::ColFuentesLuz()
{
   max_num_fuentes = 8  ;
}
//----------------------------------------------------------------------

void ColFuentesLuz::insertar( FuenteLuz * pf )  // inserta una nueva
{
   assert( pf != nullptr );

   pf->ind_fuente = vpf.size() ;
   vpf.push_back( pf ) ;
}
//----------------------------------------------------------------------
// activa una colección de fuentes de luz

void ColFuentesLuz::activar()
{

   glEnable( GL_LIGHTING );    // activar iluminacion
   glEnable( GL_NORMALIZE );  ////////////////////////////
   //glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE ); // pag 104 -> proy perspectiva

   for (int i = 0; i < max_num_fuentes; i++) {
     if (i < vpf.size() )
       ptrFuente(i)->activar();
     else
       glDisable( GL_LIGHT0 + i );
   }

}
//----------------------------------------------------------------------
FuenteLuz * ColFuentesLuz::ptrFuente( unsigned i )
{
   assert(i < vpf.size()) ;
   return vpf[i] ;
}
//----------------------------------------------------------------------
ColFuentesLuz::~ColFuentesLuz()
{
   for( unsigned i = 0 ; i < vpf.size() ; i++ )
   {
      assert( vpf[i] != NULL );
      delete vpf[i] ;
      vpf[i] = NULL ;
   }
}


//**********************************************************************

MaterialLata::MaterialLata()
  : Material(new Textura("../imgs/lata-coke.jpg"), 0.4, 0.4, 0.8, 15.0)
{}

MaterialLata2::MaterialLata2()
  : Material(new Textura("../imgs/lata-pepsi.jpg"), 0.4, 0.4, 0.8, 15.0)
{}

MaterialTapasLata::MaterialTapasLata()
  : Material({0.9, 0.9, 0.9}, 0.3, 0.3, 0.4, 1.0)
{}

MaterialPeonNegro::MaterialPeonNegro()
  : Material({0.1, 0.1, 0.1}, 0.0, 0.8, 0.5, 2.0)
{}

MaterialPeonBlanco::MaterialPeonBlanco()
  : Material({1.0, 1.0, 1.0}, 0.3, 0.7, 0.0, 8.0)
{}

MaterialPeonMadera::MaterialPeonMadera()
  : Material(new TexturaXY("../imgs/text-madera.jpg"), 0.1, 0.6, 0.3, 1.0)
{}

ColFuentesLuzP4::ColFuentesLuzP4()
{
  const VectorRGB col1 = VectorRGB(0.4, 0.4, 0.4, 0.0);
  const VectorRGB col2 = VectorRGB(0.5, 0.5, 0.5, 1.0);

  insertar( new FuenteDireccional( -30.0, 60.0, col1 ) );
  insertar( new FuentePosicional( {2.0, 3.0, 2.0}, col2 ) );
}

ColFuentesLuzP5::ColFuentesLuzP5()
{
  const VectorRGB col1 = VectorRGB(0.6, 0.6, 0.6, 0.0);
  const VectorRGB col2 = VectorRGB(0.5, 0.5, 0.5, 1.0);

  insertar( new FuenteDireccional( -10.0, 30.0, col1 ) );
  insertar( new FuentePosicional( {2.0, 3.0, 1.0}, col2 ) );
}

//**********************************************************************

MaterialBrazo::MaterialBrazo()
: Material(new Textura("../imgs/text-metal.jpg"), 0.2, 0.2, 0.6, 4.9)
{}

MaterialDado::MaterialDado()
: Material(new Textura("../imgs/textura-dado-nr.jpg"), 0.4, 0.6, 0.0, 0.0)
{}
