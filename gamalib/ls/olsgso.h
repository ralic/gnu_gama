/*  
    Geodesy and Mapping C++ Library (GNU GaMa / GaMaLib)
    Copyright (C) 1999  Ales Cepek <cepek@fsv.cvut.cz>

    This file is part of the GNU GaMa / GaMaLib C++ Library.
    
    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
 *  $Id: olsgso.h,v 1.1 2001/12/07 12:56:38 cepek Exp $
 */

#ifndef GaMa_OLS_gso_h
#define GaMa_OLS_gso_h

#include <gmatvec/gso.h>
#include <gamalib/ls/baseols.h>
#include <cmath>

namespace GaMaLib {
  
template <class Float, class Exc>
class OLSgso : public virtual BaseOLS<Float, Exc> {

public:

  OLSgso() {}
  OLSgso(const gMatVec::Mat<Float, Exc>& A, const gMatVec::Vec<Float, Exc>& b)
    : BaseOLS<Float, Exc>(A, b) {}
  OLSgso(const gMatVec::Mat<Float, Exc>& A, const gMatVec::Vec<Float, Exc>& b,
         const gMatVec::Vec<Float, Exc>& w) : BaseOLS<Float, Exc>(A, b, w) {}
  
  void reset(const gMatVec::Mat<Float, Exc>& A, 
             const gMatVec::Vec<Float, Exc>& b)
    {
      BaseOLS<Float, Exc>::reset(A, b);
    }
  void reset(const gMatVec::Mat<Float, Exc>& A, 
             const gMatVec::Vec<Float, Exc>& b,
             const gMatVec::Vec<Float, Exc>& w)
    {
      BaseOLS<Float, Exc>::reset(A, b, w);
    }
  
  const gMatVec::Vec<Float, Exc>& solve(gMatVec::Vec<Float, Exc>& x)
    {
      return x = BaseOLS<Float, Exc>::solve();
    }
  const gMatVec::Vec<Float, Exc>& solve() 
    { 
      return BaseOLS<Float, Exc>::solve(); 
    }
  
  gMatVec::Index defect() { return gso.defect(); }
  bool  lindep(gMatVec::Index i) { return gso.lindep(i); }
  
  void  q_xx(gMatVec::Mat<Float, Exc>& C) { BaseOLS<Float, Exc>::q_xx(C); }
  Float q_xx(gMatVec::Index i, gMatVec::Index j);
  Float q_bb(gMatVec::Index i, gMatVec::Index j);
  Float q_bx(gMatVec::Index i, gMatVec::Index j);
  
  void min_x() { gso.min_x(); }
  void min_x(gMatVec::Index n, gMatVec::Index x[]) { gso.min_x(n, x); }
  
  
protected:
  
  void solve_me();
   
private:

   gMatVec::Mat<Float, Exc> A_;
   gMatVec::GSO<Float, Exc> gso;

   void init_gso_();
};

// ...................................................................


template <class Float, class Exc>
void OLSgso<Float, Exc>::solve_me()
{
  if (is_solved) return;

  const gMatVec::Index M = pA->rows();
  const gMatVec::Index N = pA->cols();
  
  A_.reset(M+N, N+1);
  sqrt_w.reset(M);

  if (pw)
    {
      using namespace std;
      const gMatVec::Vec<Float, Exc>& w_ = *pw;
      for (gMatVec::Index i=1; i<=M; i++) sqrt_w(i) = sqrt(w_(i));
    }
  else
    {
      for (gMatVec::Index i=1; i<=M; i++) sqrt_w(i) = 1;
    }

  const gMatVec::Mat<Float, Exc>& A1 = *pA;
  const gMatVec::Vec<Float, Exc>& b1 = *pb;

  {  // redundant curly braces needed by MS VC++ 
  for (gMatVec::Index i=1; i<=M; i++)
    {
      A_(i, N+1) = -b1(i)*sqrt_w(i);
      for (gMatVec::Index j=1; j<=N; j++) A_(i, j) = A1(i, j)*sqrt_w(i);
    }
  }
  {
  for (gMatVec::Index i=1; i<=N; i++) 
    for (gMatVec::Index j=1; j<=N+1; j++)
      A_(M+i, j) = (i==j) ? 1 : 0;
  }

  gso.reset(A_, M, N);
  gso.gso1(); 
  gso.gso2(); 

  x.reset(N);
  for (gMatVec::Index i=1; i<=N; i++)
    x(i) = A_(M+i, N+1);

  r.reset(M);
  for (gMatVec::Index j=1; j<=M; j++)
    r(j) = A_(j, N+1)/sqrt_w(j);
  
  is_solved = true; 
}


template <class Float, class Exc>
Float OLSgso<Float, Exc>::q_xx(gMatVec::Index i, gMatVec::Index j)
  {
    if(!is_solved) solve_me();
    const gMatVec::Index M = pA->rows();
    const gMatVec::Index N = pA->cols();
    i += M;
    j += M;
    Float s = 0;                        
    for (gMatVec::Index k=1; k<=N; k++) 
      s += A_(i,k)*A_(j,k);              // cov x_i x_j
    return s;
  }


template <class Float, class Exc>
Float OLSgso<Float, Exc>::q_bb(gMatVec::Index i, gMatVec::Index j)
  {
    if(!is_solved) solve_me();

    const gMatVec::Index N = pA->cols();
    Float s = 0;                        
    for (gMatVec::Index k=1; k<=N; k++) 
      s += A_(i,k)*A_(j,k);              // cov b_i b_j
    return s/sqrt_w(i)/sqrt_w(j);
  }


template <class Float, class Exc>
Float OLSgso<Float, Exc>::q_bx(gMatVec::Index i, gMatVec::Index j)
  {
    if(!is_solved) solve_me();

    const gMatVec::Index M = pA->rows();
    const gMatVec::Index N = pA->cols();
    j += M;
    Float s = 0;                        
    for (gMatVec::Index k=1; k<=N; k++) 
      s += A_(i,k)*A_(j,k);              // cov b_i x_j
    return s/sqrt_w(i);
  }


}   // GaMaLib

#endif


















