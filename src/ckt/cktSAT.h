/****************************************************************************
  FileName     [ cktSAT.h ]
  PackageName  [ ckt ]
  Synopsis     [ Functions for Word-level gate to Boolean CNF conversion. ]
  Author       [ Alex, Cheng-Yin Wu ]
  Copyright    [ Copyleft(c) 2010 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef _CKT_SAT_HEADER
#define _CKT_SAT_HEADER

#include "Solver.h"

inline void xor_2(Solver & SS, int a, int b, int y)
{
   vec<Lit> lits;
   lits.push( mkLit(a) );
   lits.push( mkLit(b) );
   lits.push(~mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push(~mkLit(a) );
   lits.push(~mkLit(b) );
   lits.push(~mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push( mkLit(a) );
   lits.push(~mkLit(b) );
   lits.push( mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push(~mkLit(a) );
   lits.push( mkLit(b) );
   lits.push( mkLit(y) );
   SS.addClause(lits);
   lits.clear();
//   printf("%d %d -%d\n",a,b,y);
//   printf("-%d -%d -%d\n",a,b,y);
//   printf("%d -%d %d\n",a,b,y);
//   printf("-%d %d %d\n",a,b,y);
}

inline void carry_2_woc(Solver & SS, int a, int b, int y)
{
   //c = 0;
   // carry(a,b)
   //  b' -> x'
   //  a' -> x'
   //  a & b -> x    a' & b' -> x'
   vec<Lit> lits;
   lits.push(~mkLit(a) );
   lits.push(~mkLit(b) );
   lits.push( mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push( mkLit(a) );
   lits.push(~mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push( mkLit(b) );
   lits.push(~mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push( mkLit(a) );
   lits.push( mkLit(b) );
   lits.push(~mkLit(y) );
   SS.addClause(lits);
   lits.clear();
}

inline void carry_2(Solver & SS, int a, int b, int c, int y)
{
   // carry(a,b,c)
   //  b & c -> x    b' & c' -> x'
   //  a & c -> x    a' & c' -> x'
   //  a & b -> x    a' & b' -> x'
   vec<Lit> lits;
   lits.push(~mkLit(a) );
   lits.push(~mkLit(c) );
   lits.push( mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push(~mkLit(b) );
   lits.push(~mkLit(c) );
   lits.push( mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push(~mkLit(a) );
   lits.push(~mkLit(b) );
   lits.push( mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push( mkLit(a) );
   lits.push( mkLit(c) );
   lits.push(~mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push( mkLit(b) );
   lits.push( mkLit(c) );
   lits.push(~mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push( mkLit(a) );
   lits.push( mkLit(b) );
   lits.push(~mkLit(y) );
   SS.addClause(lits);
   lits.clear();
}

inline void xor_3(Solver & SS, int a, int b, int c, int y )
{
   // sum(a,b,c)
   // a' & b' & c' -> x'   a  & b  & c  -> x
   // a' & b  & c  -> x'   a  & b' & c' -> x
   // a  & b' & c  -> x'   a' & b  & c' -> x
   // a  & b  & c' -> x'   a' & b' & c  -> x
   vec<Lit> lits;
   lits.push( mkLit(a) );
   lits.push( mkLit(b) );
   lits.push( mkLit(c) );
   lits.push(~mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push( mkLit(a) );
   lits.push(~mkLit(b) );
   lits.push(~mkLit(c) );
   lits.push(~mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push(~mkLit(a) );
   lits.push( mkLit(b) );
   lits.push(~mkLit(c) );
   lits.push(~mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push(~mkLit(a) );
   lits.push(~mkLit(b) );
   lits.push( mkLit(c) );
   lits.push(~mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push( mkLit(a) );
   lits.push(~mkLit(b) );
   lits.push( mkLit(c) );
   lits.push( mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push( mkLit(a) );
   lits.push( mkLit(b) );
   lits.push(~mkLit(c) );
   lits.push( mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push(~mkLit(a) );
   lits.push(~mkLit(b) );
   lits.push(~mkLit(c) );
   lits.push( mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push(~mkLit(a) );
   lits.push( mkLit(b) );
   lits.push( mkLit(c) );
   lits.push( mkLit(y) );
   SS.addClause(lits);
   lits.clear();
}

inline void borrow_2_woB(Solver & SS, int a, int b, int y)
{
   //B=0;
   // Borrow(a,b,B)
   // a' & b -> x    a  & b' -> x'
   // a' & B -> x    a  & B' -> x'
   // b  & B -> x    b' & B' -> x'
   vec<Lit> lits;
   lits.push( mkLit(a) );
   lits.push(~mkLit(b) );
   lits.push( mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push(~mkLit(a) );
   lits.push(~mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push( mkLit(b) );
   lits.push(~mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push(~mkLit(a) );
   lits.push( mkLit(b) );
   lits.push(~mkLit(y) );
   SS.addClause(lits);
   lits.clear();
}

inline void borrow_2(Solver & SS, int a, int b, int B, int y)
{
   // Borrow(a,b,B)
   // a' & b -> x    a  & b' -> x'
   // a' & B -> x    a  & B' -> x'
   // b  & B -> x    b' & B' -> x'
   vec<Lit> lits;
   lits.push( mkLit(a) );
   lits.push(~mkLit(B) );
   lits.push( mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push(~mkLit(b) );
   lits.push(~mkLit(B) );
   lits.push( mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push( mkLit(a) );
   lits.push(~mkLit(b) );
   lits.push( mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push(~mkLit(a) );
   lits.push( mkLit(B) );
   lits.push(~mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push( mkLit(b) );
   lits.push( mkLit(B) );
   lits.push(~mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push(~mkLit(a) );
   lits.push( mkLit(b) );
   lits.push(~mkLit(y) );
   SS.addClause(lits);
   lits.clear();   
}

inline void inv(Solver & SS, int a, int b)
{
   vec<Lit> lits;
   lits.push(~mkLit(a));
   lits.push(~mkLit(b));
   SS.addClause(lits);
   lits.clear();
   lits.push( mkLit(a) );
   lits.push( mkLit(b) );
   SS.addClause(lits);
   lits.clear();
//   printf("%d %d \n",a,b);
//   printf("-%d -%d\n",a,b);
}

inline void buf(Solver & SS, int a, int b)
{
   vec<Lit> lits;
   lits.push( ~mkLit(a) );
   lits.push(  mkLit(b) );
   SS.addClause(lits);
   lits.clear();
   lits.push( mkLit(a) );
   lits.push(~mkLit(b) );
   SS.addClause(lits);
   lits.clear();
//   printf("-%d %d \n",a,b);
//   printf("%d -%d\n",a,b);
}

inline void unit1(Solver & SS, int a)
{
   vec<Lit> lits;
   lits.push( mkLit(a));
   SS.addClause(lits);
   lits.clear();
//   printf("%d\n",a);
}

inline void unit0(Solver & SS, int a)
{
   vec<Lit> lits;
   lits.push(~mkLit(a));
   SS.addClause(lits);
   lits.clear();
//   printf("-%d\n",a);
}

inline void and_2(Solver & SS, int a, int b, int y)
{
   vec<Lit> lits;
   lits.push(~mkLit(a));
   lits.push(~mkLit(b));
   lits.push( mkLit(y));
   SS.addClause(lits);
   lits.clear();
   lits.push(mkLit(a));
   lits.push(~mkLit(y));
   SS.addClause(lits);
   lits.clear();
   lits.push(mkLit(b));
   lits.push(~mkLit(y));
   SS.addClause(lits);
   lits.clear();
 //  printf("-%d -%d %d\n",a,b,y);
///   printf("%d -%d\n",a,y);
//   printf("%d -%d\n",b,y);
}

inline void and_red(Solver & SS, int a, int y, int Width)
{
   vec<Lit> lits;
   for (int i = 0; i < Width; ++i) {
      lits.push( mkLit(a+i));
      lits.push(~mkLit(y));
      SS.addClause(lits);
      lits.clear();
     // printf("%d -%d\n",a+i,y);
   }
   for (int i = 0; i < Width; ++i) {
      lits.push(~mkLit(a+i));
     // printf("-%d ",a+i);
   }

   lits.push(mkLit(y));
   //printf("%d\n",y);
   SS.addClause(lits);
   lits.clear();
}

inline void or_2(Solver & SS, int a, int b, int y)
{
   vec<Lit> lits;
   lits.push( mkLit(a));
   lits.push( mkLit(b));
   lits.push(~mkLit(y));
   SS.addClause(lits);
   lits.clear();
   lits.push(~mkLit(a));
   lits.push( mkLit(y));
   SS.addClause(lits);
   lits.clear();
   lits.push(~mkLit(b));
   lits.push( mkLit(y));
   SS.addClause(lits);
   lits.clear();
//   printf("%d %d -%d\n",a,b,y);
//   printf("-%d %d\n",a,y);
//   printf("-%d %d\n",b,y);
}

inline void or_red(Solver & SS, int a, int y, int Width)
{
   vec<Lit> lits;
   for (int i = 0; i < Width; ++i) {
      lits.push(~mkLit(a+i));
      lits.push( mkLit(y));
      SS.addClause(lits);
      lits.clear();
   }
   for (int i = 0; i < Width; ++i)
      lits.push(mkLit(a+i));

   lits.push(~mkLit(y));
   SS.addClause(lits);
   lits.clear();
}

inline void nand_red(Solver & SS, int a, int y, int Width)
{
   vec<Lit> lits;
   for (int i = 0; i < Width; ++i) {
      lits.push( mkLit(a+i));
      lits.push( mkLit(y));
      SS.addClause(lits);
      lits.clear();
   }
   for (int i = 0; i < Width; ++i)
      lits.push(~mkLit(a+i));

   lits.push(~mkLit(y));
   SS.addClause(lits);
   lits.clear();
}

inline void nor_red(Solver & SS, int a, int y, int Width)
{
   vec<Lit> lits;
   for (int i = 0; i < Width; ++i) {
      lits.push(~mkLit(a+i));
      lits.push(~mkLit(y));
      SS.addClause(lits);
      lits.clear();
   }
   for (int i = 0; i < Width; ++i)
      lits.push(mkLit(a+i));

   lits.push(mkLit(y));
   SS.addClause(lits);
   lits.clear();
}

inline void xnor_2(Solver & SS, int a, int b, int y)
{
   vec<Lit> lits;
   lits.push(~mkLit(a) );
   lits.push(~mkLit(b) );
   lits.push( mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push(mkLit(a) );
   lits.push(mkLit(b) );
   lits.push(mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push( mkLit(a) );
   lits.push(~mkLit(b) );
   lits.push(~mkLit(y) );
   SS.addClause(lits);
   lits.clear();
   lits.push(~mkLit(a) );
   lits.push( mkLit(b) );
   lits.push(~mkLit(y) );
   SS.addClause(lits);
   lits.clear();
//   printf("%d %d -%d\n",a,b,y);
//   printf("-%d -%d -%d\n",a,b,y);
//   printf("%d -%d %d\n",a,b,y);
//   printf("-%d %d %d\n",a,b,y);
}

inline void mux_a(Solver & SS, int s, int a, int o)
{
   // s' -> ( a == o ) 
   vec<Lit> lits;
   lits.push( mkLit(s));
   lits.push( mkLit(a));
   lits.push(~mkLit(o));
   SS.addClause(lits);
   lits.clear();
   lits.push( mkLit(s));
   lits.push(~mkLit(a));
   lits.push( mkLit(o));
   SS.addClause(lits);
   lits.clear();
//   printf("%d %d -%d\n",s,a,o);
//   printf("%d -%d %d\n",s,a,o);
}

inline void mux_b(Solver & SS, int s, int b, int o)
{
   // s -> ( b == o )
   vec<Lit> lits;
   lits.push(~mkLit(s));
   lits.push( mkLit(b));
   lits.push(~mkLit(o));
   SS.addClause(lits);
   lits.clear();
   lits.push(~mkLit(s));
   lits.push(~mkLit(b));
   lits.push( mkLit(o));
   SS.addClause(lits);
   lits.clear();
//   printf("%d %d -%d\n",s,b,o);
//   printf("%d -%d %d\n",s,b,o);
}

inline void mux_abo(Solver & SS, int a, int b, int o)
{
   // a & b -> o
   // a' & b' -> o'
   vec<Lit> lits;
   lits.push(~mkLit(a));
   lits.push(~mkLit(b));
   lits.push( mkLit(o));
   SS.addClause(lits);
   lits.clear();
   lits.push( mkLit(a));
   lits.push( mkLit(b));
   lits.push(~mkLit(o));
   SS.addClause(lits);
   lits.clear();
//   printf("%d %d -%d\n",a,b,o);
//   printf("-%d -%d %d\n",a,b,o);
}

inline void geq(Solver & SS, int a, int b, int y)
{
   vec<Lit> lits;
   lits.push(mkLit(a));
   lits.push(~mkLit(b));
   lits.push(~mkLit(y));
   SS.addClause(lits);
   lits.clear();
   lits.push(~mkLit(a));
   lits.push(mkLit(y));
   SS.addClause(lits);
   lits.clear();
   lits.push(mkLit(b));
   lits.push(mkLit(y));
   SS.addClause(lits);
   lits.clear();
}

inline void greater_2(Solver & SS, int a, int b, int y)
{
   vec<Lit> lits;
   lits.push(~mkLit(a));
   lits.push(mkLit(b));
   lits.push(mkLit(y));
   SS.addClause(lits);
   lits.clear();
   lits.push(mkLit(a));
   lits.push(~mkLit(y));
   SS.addClause(lits);
   lits.clear();
   lits.push(~mkLit(b));
   lits.push(~mkLit(y));
   SS.addClause(lits);
   lits.clear();
}

inline void geqInt(Solver & SS, int g, int e, int y, int Width)
{
   vec<Lit> lits;
   for (int i = 0; i < Width; i++) {
      lits.push(mkLit(g+i));
      lits.push(~mkLit(y+i));
      SS.addClause(lits);
      lits.clear();
      for (int j = i+1; j < Width; j++) {
         lits.push(mkLit(e+j));
	 lits.push(~mkLit(y+i));  // Fixed by Chengyin
	 SS.addClause(lits);
	 lits.clear();
      }
      for (int j = i+1; j < Width; j++)
	 lits.push(~mkLit(e+j));  // Fixed by Chengyin

      lits.push(~mkLit(g+i));
      lits.push( mkLit(y+i));
      SS.addClause(lits);  // Fixed by Chengyin
      lits.clear();
   }
}

inline void leq(Solver & SS, int a, int b, int y)
{
   vec<Lit> lits;
   lits.push(~mkLit(a));
   lits.push(mkLit(b));
   lits.push(~mkLit(y));
   SS.addClause(lits);
   lits.clear();
   lits.push(mkLit(a));
   lits.push(mkLit(y));
   SS.addClause(lits);
   lits.clear();
   lits.push(~mkLit(b));
   lits.push(mkLit(y));
   SS.addClause(lits);
   lits.clear();
}

inline void less_2(Solver & SS, int a, int b, int y)
{
   vec<Lit> lits;
   lits.push(mkLit(a));
   lits.push(~mkLit(b));
   lits.push(mkLit(y));
   SS.addClause(lits);
   lits.clear();
   lits.push(~mkLit(a));
   lits.push(~mkLit(y));
   SS.addClause(lits);
   lits.clear();
   lits.push(mkLit(b));
   lits.push(~mkLit(y));
   SS.addClause(lits);
   lits.clear();
}

inline int genXorCNF(Solver& SS, int yLit, int& iLit, int& aLit, int size)
{
   if ( size == 1 ) {
      int a = aLit;
      aLit++;
      return a;
   }
   else if ( size == 2 ) {
      xor_2(SS, aLit, aLit+1, yLit);
      aLit += 2;
      return yLit;
   }
   else {
      int i1, i2, i1_t;
      iLit++;
      i1 = iLit;
      i1_t = genXorCNF(SS, i1, iLit, aLit, size/2);
      if ( i1_t != i1 ) { // size == 1
         iLit--;
         i1 = i1_t;
      }
      iLit++;
      i2 = iLit;
      genXorCNF(SS, i2, iLit, aLit, size-size/2); // size == 1 is impossible here
      xor_2(SS, yLit, i1, i2);
      return yLit;
   } 
}

#endif

