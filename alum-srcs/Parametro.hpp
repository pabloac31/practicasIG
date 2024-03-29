#include <string>
#include <functional>
#include <matrizg.hpp>

#ifndef GRADO_LIBERTAD_HPP
#define GRADO_LIBERTAD_HPP

class Objeto3D ;


// tipo usado para las funciones que calculan una matriz a partir de un flotante
typedef std::function< Matriz4f( float )> TFuncionCMF  ;

// -----------------------------------------------------------------------------

class Parametro
{
   private:
   static constexpr float v_ini = 0.1;    // velocidad inicial
   static constexpr float delta = 0.1;    // incremento
   static constexpr float a = 0.05;        // aceleracion

   const std::string descripcion;   // descripcion del grado de libertad
   const bool acotado;              // true si el valor oscila entre dos valores, false si no
   const TFuncionCMF fun_calculo_matriz;  // función que produce una nueva matriz a partir de un float
   const float c,   // valor inicial (y central para acotados)
               s,   // semiamplitud (acotado) o factor de escala (no acotado)
               f;   // si acotado: frecuencia (ciclos por unidad del valor normalizado)
   Matriz4f * const ptr_mat;    // puntero a la matriz del modelo
   float valor_norm,    // valor actual, normalizado, no acotado (crece desde 0)
         velocidad;     // velocidad actual (se suma al valor_norm)

   public:
   // crear grado de libertad no acotado
   Parametro
   (  const std::string &  p_descripcion,
      Matriz4f *           p_ptr_mat,
      TFuncionCMF          p_fun_calculo_matriz,
      bool                 p_acotado,
      float                p_c,
      float                p_s,
      float                p_f
   );

   void  siguiente_cuadro();   // actualizar valor y matriz al siguiente frame
   void  reset();        // vuelve al estado inicial
   void  incrementar();  // incrementar el valor
   void  decrementar() ; // decrementar el valor
   void  acelerar();     // acelerar (aumentar velocidad normalizada)
   void  decelerar();    // decelerar (disminuir la velocidad normalizada)
   float leer_valor_actual(); // devuelve el valor actual (escalado, no normalizado)
   float leer_velocidad_actual();    // devuelve velocidad o frecuencia actual
   std::string
         leer_descripcion();
};

#endif
