/*  
    GNU Gama -- adjustment of geodetic networks
    Copyright (C) 2004  Ales Cepek <cepek@fsv.cvut.cz>

    This file is part of the GNU Gama C++ library.
    
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
 *  $Id: g3_cluster.cpp,v 1.2 2004/01/05 19:07:12 cepek Exp $
 */


#include <gnu_gama/g3/g3_cluster.h>

using namespace GNU_gama::g3;


void ObsCluster::write_xml(std::ostream& out) const 
{
  out << "\n<obs>\n";

  List<ObservationType*>::const_iterator i = observation_list.begin();
  List<ObservationType*>::const_iterator e = observation_list.end();
  while (i != e)
    {
      const ObservationType* obs = (*i);

      if (const Distance* distance = dynamic_cast<const Distance*>(obs))
        {
          out << "<distance>\n  "
              << " <from>" << distance->from  << "</from>"
              << " <to>"   << distance->to    << "</to>"
              << " <val>"  << distance->obs() << "</val>\n"
              << "   </distance>\n";
        }

      ++i;
    }

  const int dim  = covariance_matrix.dim();
  const int band = covariance_matrix.bandWidth();
  out << "\n<cov> "
      << "<dim>"  << dim  << "</dim> "
      << "<band>" << band << "</band>\n";
  for (int i=1; i<=dim; i++)
    {
      for (int j=i; j<=i+band && j<=dim; j++)
        {
          out << "<flt>" << covariance_matrix(i,j) <<  "</flt> ";
        }
      out << "\n";
    }
  out << "</cov>\n";
  out << "</obs>\n";
}

