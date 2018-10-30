#include <cassert>
#include <cmath>
#include "Parametro.hpp"
#include "Objeto3D.hpp"


// -----------------------------------------------------------------------------

// crear grado de libertad no acotado

Parametro::Parametro(
   const std::string &  p_descripcion,
   Matriz4f *           p_ptr_mat,
   TFuncionCMF          p_fun_calculo_matriz,
   bool                 p_acotado,
   float                p_c,
   float                p_s,
   float                p_f
) :
   descripcion(p_descripcion),
   acotado(p_acotado),
   fun_calculo_matriz (p_fun_calculo_matriz),
   c (p_c),
   s (p_s),
   f (p_f),
   ptr_mat (p_ptr_mat)

{
   valor_norm = 0;
   velocidad = v_ini;
}
// -----------------------------------------------------------------------------

void Parametro::siguiente_cuadro()
{
   valor_norm += velocidad;
   *ptr_mat = fun_calculo_matriz( leer_valor_actual() );
}
// -----------------------------------------------------------------------------

void Parametro::reset()
{
   valor_norm = 0;
   velocidad = v_ini;
   *ptr_mat = fun_calculo_matriz( leer_valor_actual() );
}
// -----------------------------------------------------------------------------
void Parametro::incrementar()
{
   valor_norm += delta;
   *ptr_mat = fun_calculo_matriz( leer_valor_actual() );
}
// -----------------------------------------------------------------------------

void Parametro::decrementar()
{
   valor_norm -= delta;
   *ptr_mat = fun_calculo_matriz( leer_valor_actual() );
}
// -----------------------------------------------------------------------------
void Parametro::acelerar()
{
   velocidad += a;
}
// -----------------------------------------------------------------------------
void Parametro::decelerar()
{
   velocidad = (velocidad-a) < 0 ? 0 : velocidad-a;
}
// -----------------------------------------------------------------------------

float Parametro::leer_valor_actual()
{
   float valor;

   if (!acotado)
     valor = c + s * valor_norm;
   else
     valor = c + s * sin( f * 2 * M_PI * valor_norm );

   return valor;
}

// -----------------------------------------------------------------------------
float Parametro::leer_velocidad_actual()
{
   return velocidad;
}
// -----------------------------------------------------------------------------

std::string Parametro::leer_descripcion()
{
   return descripcion;
}
// -----------------------------------------------------------------------------
