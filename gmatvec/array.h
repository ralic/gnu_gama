/*  
    C++ Matrix/Vector templates (GNU Gama / gMatVec 0.9.23)
    Copyright (C) 1999  Ales Cepek <cepek@gnu.org>

    This file is part of the gMatVec C++ Matrix/Vector template library.
    
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
 *  $Id: array.h,v 1.12 2004/08/29 18:01:52 cepek Exp $
 *  http://www.gnu.org/software/gama/
 */

#ifndef gMatVec__Array__h_
#define gMatVec__Array__h_

#include <gmatvec/memrep.h>

namespace gMatVec {

template <typename Type, typename Exc=Exception>  
class Array : public MemRep<Type, Exc> {

public:

  Array(Index dim) : MemRep<Type, Exc>(dim) {}
  Type  operator[](Index i) const { return this->begin()[i]; }
  Type& entry(Index i) { return this->begin()[i]; }
  void swap(Index i, Index j) 
    {
      Type *ind = this->begin();
      Type t = ind[i]; ind[i] = ind[j]; ind[j] = t; 
    }

};


}   // namespace gMatVec

#endif




