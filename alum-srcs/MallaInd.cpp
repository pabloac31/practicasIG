// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Implementación de la clase MallaInd
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include "MallaInd.hpp"   // declaración de 'ContextoVis'

// #define TIPO_MI 1         // glBegin-End(0) - glDrawElements(1)
#define USAR_COLORES 0    // usar patron amarillo-negro

// *****************************************************************************
// funciones auxiliares

GLuint VBO_Crear( GLuint tipo, GLuint tamanio, GLvoid * puntero )
{
  assert( tipo == GL_ARRAY_BUFFER || tipo == GL_ELEMENT_ARRAY_BUFFER );
  GLuint id_vbo;                  // resultado: identificador de VBO
  glGenBuffers( 1, &id_vbo );     // crear nuevo VBO, obtener identificador
  glBindBuffer( tipo, id_vbo );   // activar el VBO usando su identificador
  glBufferData( tipo, tamanio, puntero, GL_STATIC_DRAW ); // transferencia RAM -> GPU
  glBindBuffer( tipo, 0 );        // desactivación del VBO (activar 0)
  return id_vbo;
}

// -----------------------------------------------------------------------------

Tupla3f calcularCentroCajaEnglobante(const std::vector<Tupla3f> & ver)
{
  Tupla3f max, min = ver[0];

  for(Tupla3f vertice : ver){
    max[0] = std::max(vertice(X), max(X));
    max[1] = std::max(vertice(Y), max(Y));
    max[2] = std::max(vertice(Z), max(Z));
    min[0] = std::min(vertice(X), min(X));
    min[1] = std::min(vertice(Y), min(Y));
    min[2] = std::min(vertice(Z), min(Z));
  }

  return (max+min) / 2;
}

// *****************************************************************************
// métodos de la clase MallaInd.

void MallaInd::crearVBOs()
{
  tam_ver = sizeof(float) * 3L * num_ver;
  tam_tri = sizeof(unsigned) * 3L * tabla_tri.size();
  tam_cctt = sizeof(float) * 2L * num_ver;

  // crear VBO conteniendo la tabla de vértices
  id_vbo_ver = VBO_Crear( GL_ARRAY_BUFFER, tam_ver, tabla_ver.data() );
  // crear VBO con la tabla de triángulos (índices de vértices)
  id_vbo_tri = VBO_Crear(  GL_ELEMENT_ARRAY_BUFFER, tam_tri, tabla_tri.data() );

  // crear VBO con los colores de los vértices
  if ( col_ver.size() > 0 )
  id_vbo_col_ver = VBO_Crear( GL_ARRAY_BUFFER, tam_ver, col_ver.data() );

  if ( tabla_nor_ver.size() > 0 )
  id_vbo_nor_ver = VBO_Crear( GL_ARRAY_BUFFER, tam_ver, tabla_nor_ver.data() );

  if ( coor_textura.size() > 0 )
  id_vbo_cctt = VBO_Crear( GL_ARRAY_BUFFER, tam_cctt, coor_textura.data() );

  vbos_creados = true;
}


// -----------------------------------------------------------------------------

MallaInd::MallaInd()
: MallaInd("malla indexada, anónima") { }

// -----------------------------------------------------------------------------

MallaInd::MallaInd(const std::string & nombreIni)
{
  // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
  ponerNombre(nombreIni) ;
  id_vbo_tri = id_vbo_ver = id_vbo_col_ver = id_vbo_nor_ver = id_vbo_cctt = 0;
  num_ver = 0;
  tam_ver = tam_tri = tam_cctt = 0;
  vbos_creados = normales_creadas = false;
  usar_texturas = false;

}

// -----------------------------------------------------------------------------

void MallaInd::calcular_normales()
{
  tabla_nor_ver.insert(tabla_nor_ver.begin(), num_ver, {0.0, 0.0 , 0.0});

  // caras
  Tupla3f a, b, n;
  for (auto cara : tabla_tri) {
    a = tabla_ver[cara(Y)] - tabla_ver[cara(X)],
    b = tabla_ver[cara(Z)] - tabla_ver[cara(X)],
    n = a.cross(b);   // producto vectorial

    if (n(X)!=0 || n(Y)!=0 || n(Z)!=0)
    n = n.normalized();

    tabla_nor_caras.push_back(n);

    // vértices
    for (int i = 0; i < 3; i++)
    tabla_nor_ver[cara(i)] = tabla_nor_ver[cara(i)] + n;
  }

  // normalizamos los vértices
  for (auto &nor : tabla_nor_ver){
    if (nor(X)!=0 || nor(Y)!=0 || nor(Z)!=0)
    nor = nor.normalized();
  }

  normales_creadas = true;
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI( ContextoVis & cv )
{
  // #if TIPO_MI == 0  // glBegin-glEnd
  //
  // glBegin(GL_TRIANGLES);
  // for (unsigned i = 0; i < tabla_tri.size(); i++) {
  //   for (unsigned j = 0; j < 3; j++) {
  //     unsigned ind_ver = tabla_tri[i](j);
  //
  //     if (usar_texturas) {  // enviamos normales y cctt
  //       glNormal3fv(tabla_nor_ver[ind_ver]);
  //       glTexCoord2fv(coor_textura[ind_ver]);
  //     }
  //
  //     if (col_ver.size() > 0)
  //     glColor3fv(col_ver[ind_ver]); // enviamos colores
  //
  //     glVertex3fv(tabla_ver[ind_ver]);
  //   }
  // }
  //
  // glEnd();
  //
  // #else     // glDrawElements

  if (usar_texturas) {
    glNormalPointer( GL_FLOAT, 0, tabla_nor_ver.data() );
    glEnableClientState( GL_NORMAL_ARRAY );

    if (coor_textura.size() > 0) {
      glTexCoordPointer(2, GL_FLOAT, 0, coor_textura.data());
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }
  }

  if ( col_ver.size() > 0 ) {
    glEnableClientState( GL_COLOR_ARRAY );             // habilitar colores
    glColorPointer( 3, GL_FLOAT, 0, col_ver.data() );  // fija puntero a colores
  }

  glEnableClientState( GL_VERTEX_ARRAY );
  glVertexPointer( 3, GL_FLOAT, 0, tabla_ver.data() );

  glDrawElements( GL_TRIANGLES, 3L * tabla_tri.size(), GL_UNSIGNED_INT, tabla_tri.data() );

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);

  // #endif
}

// ----------------------------------------------------------------------------

void MallaInd::visualizarDE_VBOs( ContextoVis & cv )
{
  // Colores o normales
  if (usar_texturas) {
    // activar VBO de coordenadas de normales
    glBindBuffer( GL_ARRAY_BUFFER, id_vbo_nor_ver );
    glNormalPointer( GL_FLOAT, 0, 0 );  // (0 == offset en vbo)
    glEnableClientState( GL_NORMAL_ARRAY );

    if (coor_textura.size() > 0) {
      // activar VBO de coordenadas de textura
      glBindBuffer( GL_ARRAY_BUFFER, id_vbo_cctt );
      glTexCoordPointer( 2, GL_FLOAT, 0, 0);  // (0 == offset en vbo)
      glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    }
  }

  if (col_ver.size() > 0) {
    glBindBuffer(GL_ARRAY_BUFFER, id_vbo_col_ver);
    glColorPointer(3, GL_FLOAT, 0, 0);
    glEnableClientState(GL_COLOR_ARRAY);
  }

  // especificar formato de los vértices en su VBO (y offset)
  glBindBuffer( GL_ARRAY_BUFFER, id_vbo_ver ); // act. VBO
  glVertexPointer( 3, GL_FLOAT, 0, 0 );        // formato y offset (=0)
  glBindBuffer( GL_ARRAY_BUFFER, 0 );          // desactivar VBO
  glEnableClientState( GL_VERTEX_ARRAY );      // act. uso VA

  // visualizar con glDrawElements (puntero a tabla == NULL)
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id_vbo_tri );
  glDrawElements( GL_TRIANGLES, 3L*tabla_tri.size(), GL_UNSIGNED_INT, NULL );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI_Plano(ContextoVis & cv)
{
  glBegin(GL_TRIANGLES);

  for (unsigned i = 0; i < tabla_tri.size(); i++) {
    glNormal3fv(tabla_nor_caras[i]);
    for (unsigned j = 0; j < 3; j++) {
      unsigned ind_ver = tabla_tri[i](j);
      if (col_ver.size() > 0)
      glColor3fv(col_ver[ind_ver]);
      if (coor_textura.size() > 0)
        glTexCoord2fv(coor_textura[ind_ver]);

      glVertex3fv(tabla_ver[ind_ver]);
    }
  }

  glEnd();
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarGL( ContextoVis & cv )
{
  // usamos las texturas en los modos 'modoColorNodoPlano' y 'modoMateriales'
  usar_texturas = cv.modoVis == modoColorNodoPlano || cv.modoVis == modoMateriales;

  if (!usar_texturas) { // desactivamos iluminacion y texturas en los modos correspondientes
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
  }
  else if (!normales_creadas)
    calcular_normales();

  if (cv.modoSeleccionFBO) {  // modo seleccion P5
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_FLAT);  // sombreado plano necesario
  }

  else {
    switch (cv.modoVis) {
      case modoPuntos:
      glPointSize(3);
      glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );
      break;
      case modoAlambre:
      glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
      glShadeModel( GL_SMOOTH );
      break;
      case modoSolido:
      glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
      glShadeModel( GL_SMOOTH );
      break;
      case modoColorNodoPlano:
      glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
      glShadeModel( GL_FLAT ); // activa sombreado plano
      break;
      case modoMateriales: // modoColorNodoSuave (goround)
      glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
      glShadeModel( GL_SMOOTH );
      break;
      default:
      glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
      glShadeModel( GL_SMOOTH );
    }
  }

  // Visualizamos según el modo
  if (cv.usarVBOs && !cv.modoSeleccionFBO){
    if ( !vbos_creados )
      crearVBOs();
    visualizarDE_VBOs(cv);
  }
  else if (cv.modoVis == modoColorNodoPlano)
    visualizarDE_MI_Plano(cv);
  else
    visualizarDE_MI(cv);
}

// -----------------------------------------------------------------------------

unsigned MallaInd::getNumVer() const {
  return num_ver;
}

// -----------------------------------------------------------------------------

void MallaInd::setColorVertices( std::vector<Tupla3f> * colores )
{
  if (colores != nullptr) {
    col_ver.clear();
    for (unsigned i = 0; i < num_ver; i++)
    col_ver.push_back( colores->at(i) );
  }

  #if USAR_COLORES == 1
  else if (col_ver.size() == 0) {
    for (unsigned i = 0; i < num_ver; i++){
      if( i%2 == 0 )
      col_ver.push_back( {0.0, 0.0, 0.0} );
      else
      col_ver.push_back( {214.0/256.0, 214.0/256.0, 66.0/256.0} );
    }
  }
  #endif
}

// -----------------------------------------------------------------------------

void MallaInd::fijarColorNodo( const Tupla3f & nuevo_color )
{
  col_ver.clear();
  for (unsigned i = 0; i < num_ver; i++)
  col_ver.push_back( nuevo_color );
}

// -----------------------------------------------------------------------------

void MallaInd::calcularCentroOC() {
  if (!centro_calculado) {
    ponerCentroOC(calcularCentroCajaEnglobante(tabla_ver));
    centro_calculado = true;
  }
}


// *****************************************************************************

Cubo::Cubo()
:  MallaInd( "malla cubo" )
{
  num_ver = 8;

  // Vertices
  tabla_ver = {
    Tupla3f(0.5,0,0.5),
    Tupla3f(-0.5,0,0.5),
    Tupla3f(-0.5,0,-0.5),
    Tupla3f(0.5,0,-0.5),
    Tupla3f(0.5,1,0.5),
    Tupla3f(-0.5,1,0.5),
    Tupla3f(-0.5,1,-0.5),
    Tupla3f(0.5,1,-0.5)
  };

  // Indices
  tabla_tri = {
    Tupla3i(0,3,1),
    Tupla3i(1,3,2),
    Tupla3i(0,7,4),
    Tupla3i(0,3,7),
    Tupla3i(2,3,7),
    Tupla3i(2,7,6),
    Tupla3i(1,2,6),
    Tupla3i(1,6,5),
    Tupla3i(1,0,4),
    Tupla3i(1,4,5),
    Tupla3i(5,4,7),
    Tupla3i(5,7,6)
  };

  // Color de los vértices
  setColorVertices();

}
// *****************************************************************************

Tetraedro::Tetraedro()
:  MallaInd( "malla tetraedro")
{
  num_ver = 4;

  // Vertices
  tabla_ver = {
    Tupla3f(1,0,0),
    Tupla3f(-0.5,0,-0.866),
    Tupla3f(-0.5,0,0.866),
    Tupla3f(0,1,0)
  };

  // Indices
  tabla_tri = {
    Tupla3i(0,2,1),
    Tupla3i(3,2,1),
    Tupla3i(3,1,0),
    Tupla3i(3,0,2)
  };

  // Color de los vértices
  setColorVertices();

}
// *****************************************************************************



//---------------------------------------------------------------------------------
// EJERCICIOS RELACIÓN
// 33-.

void tronco()
{
  // glDisable(GL_DEPTH_TEST);
  // relleno (tres polígonos)
  glColor3f(0.5,0.8,1.0);
  glBegin(GL_POLYGON);
  glVertex2f(0.0,0.0);
  glVertex2f(1.0,0.0);
  glVertex2f(1.0,1.0);
  glVertex2f(0.5,1.5);
  glVertex2f(0.0,1.5);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex2f(0.5,1.5);
  glVertex2f(1.0,1.0);
  glVertex2f(2.0,2.0);
  glVertex2f(1.5,2.5);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex2f(0.5,1.5);
  glVertex2f(0.0,3.0);
  glVertex2f(-0.5,3.0);
  glVertex2f(0.0,1.5);
  glEnd();
  // aristas (tres polilíneas)
  glColor3f(0,0,1);
  glLineWidth(2);
  glBegin(GL_LINE_STRIP);
  glVertex2f(1.0,0.0); // 1
  glVertex2f(1.0,1.0); // 2
  glVertex2f(2.0,2.0); // 3
  glEnd() ;
  glBegin(GL_LINE_STRIP);
  glVertex2f(1.5,2.5); // 4
  glVertex2f(0.5,1.5); // 5
  glVertex2f(0.0,3.0); // 6
  glEnd() ;
  glBegin(GL_LINE_STRIP);
  glVertex2f(-0.5,3.0); // 7
  glVertex2f(0.0,1.5); // 8
  glVertex2f(0.0,0.0); // 0
  glEnd();
  // fin de 'tronco'
}

// 34-.
const GLfloat mr2 = 0.5*sqrtf(2.0);
void arbol(unsigned resto_niveles)
{
  tronco();
  if (resto_niveles > 0) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef( 1.5, 2.5, 0.0 );
    glScalef( mr2, mr2, 1.0 );
    glRotatef( -45.0, 0.0, 0.0, 1.0 );
    arbol( resto_niveles-1 ); // sub-árbol a la derecha
    glPopMatrix();
    glPushMatrix();
    glTranslatef( -0.5, 3.0, 0.0 );
    glScalef( 0.5, 0.5, 0.5 );
    arbol( resto_niveles-1 ); // sub-árbol la izquierda
    glPopMatrix();
  }
}

void poligono(int n, float radio)
{
  float alpha = 360.0 / n;

  Matriz4f m = MAT_Rotacion(alpha, 0,0,1);

  auto p = Tupla3f(1,0,0);

  std::vector<Tupla3f> v;
  for(unsigned i = 0; i<n; ++i){
    v.push_back(p);
    p = m*p;
  }

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glEnableClientState(GL_VERTEX_ARRAY);
  glColor3f(0,0,1);
  glVertexPointer(3, GL_FLOAT, 0, v.data());
  glDrawArrays(GL_POLYGON,0 ,n);
  glDisableClientState(GL_VERTEX_ARRAY);

  auto u = (1.0 - 0.1)/n;

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  for(unsigned i=0; i< n ; ++i){
    auto r = 1.0 - i*u;
    auto p0 = v[i%n] * r;
    auto p1 = v[(i+1)%n] * r;
    glColor3f(0.5,0.8,1.0);
    glBegin(GL_TRIANGLES);
      glVertex3fv(p0);
      glVertex3fv(p1);
      glVertex3fv(Tupla3f(0.0,0.0,0.0));
    glEnd();

    glColor3f(0,0,1);
    glBegin(GL_LINES);
      glVertex3fv(p0);
      glVertex3fv(p1);
    glEnd();
  }

  glDisable(GL_DEPTH_TEST);
}
