/*
    GNU Gama -- adjustment of geodetic networks
    Copyright (C) 2003  Ales Cepek <cepek@gnu.org>

    This file is part of the GNU Gama C++ library.

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef GNU_gama__mathematical_model_h_gnugamamodel___gnu_gama_gmodel___h
#define GNU_gama__mathematical_model_h_gnugamamodel___gnu_gama_gmodel___h

#include <gnu_gama/obsdata.h>


namespace GNU_gama {

  // Three basic components of mathematical model (of geodetic
  // adjustment) are functional relations (class Model), unknown
  // parameters and constants (class Parameter) and observables (class
  // Observation).

  // Model, Parameter and Observation classes are logically
  // related. To brake the source code dependency we use the 'acyclic
  // visitor' pattern, where Model objects are visiting Observation
  // objects.

  // BaseVisitor is a completely degenerated class having only
  // the virtual destructor.


  /** BaseVisitor is a completely degenerated class having only
   *  the virtual destructor.
   */

  class BaseVisitor
  {
  public:
    virtual ~BaseVisitor() {}
  };


  /** Base observation class.
   */

  template <typename Cluster, typename Matrix, typename Index=std::size_t>
  class Observation
  {
  public:

    Cluster*        cluster;
    Index           cluster_index;
    typedef Matrix  CovarianceMatrix;

    Observation() : cluster(0), cluster_index(0), active_(true) {}
    virtual ~Observation() {}

    virtual int  dimension() const = 0;
    virtual void accept(BaseVisitor* visitor) = 0;

    bool active() const     { return  active_;      }
    bool set_active(bool b) { return (active_ = b); }


  private:

    bool active_;
  };


  // .....................................................................


  /** Abstract visitor class (design pattern 'acyclic visitor')
   */

  template <typename Element> class Visitor
  {
  public:
    virtual ~Visitor() {}
    virtual void visit(Element* element) = 0;
  };


  /** Helper intermediate template class Accept defines method
   *  accept() for derived element classes in acyclic visitor pattern.
   *
   * Example:
   *
   * \code
   *  //  g3 distance class
   *
   *  class Distance : public Accept<Distance, Observation>,
   *                   public FromTo, public Value {
   *  public:
   *
   *    Distance() {}
   *    Distance(double d) : Value(d) {}
   *
   *    int dimension() const { return 1; }
   *  };
   * \endcode
   */

  template <typename Derived, typename Base>
    class Accept : public Base {
  public:
    void accept(BaseVisitor* visitor)
    {
      if (Visitor<Derived>* v = dynamic_cast<Visitor<Derived>*>(visitor))
        {
          v->visit(static_cast<Derived*>(this));
        }
    }
  };


  /** Base template class Model (mathematical model of geodetic
   *  adjustment)
   */

  template <typename Observation>
  class Model
  {
  public:
    typedef typename
          ObservationData<Observation>::ClusterList   ClusterList;
    typedef Observation                               ObservationType;
    typedef ObservationData<Observation>              ObsData;

    ObsData  obsdata;
  };

}

#endif
