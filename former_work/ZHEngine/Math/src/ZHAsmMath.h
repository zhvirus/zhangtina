#pragma once
#include "ZHMathSTD.h"

const float pi = static_cast<float>(4.0 * atan( 1.0 ));
const float half_pi = static_cast<float>(0.5 * pi);


#define FORCEINLINE __forceinline




float asm_arccos( float r ) ;


float asm_arcsin( float r ) ;

float asm_arctan( float r ) ;

float asm_sin( float r ) ;

float asm_cos( float r ) ;

float asm_tan( float r ) ;

// returns a for a * a = r
float asm_sqrt( float r );

// returns 1 / a for a * a = r
// -- Use this for Vector normalisation!!!
float asm_rsq( float r );
float apx_rsq( float r );



/*__declspec(naked) */
float __fastcall InvSqrt(float fValue);



// returns log2( r ) / log2( e )
float asm_ln( float r ); 







