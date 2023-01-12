// Copyright (c) 2009-2017 GeometryFactory (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL: https://github.com/CGAL/cgal/blob/v5.5.1/Mesh_3/include/CGAL/facets_in_complex_3_to_triangle_mesh.h $
// $Id: facets_in_complex_3_to_triangle_mesh.h ab03816 2022-04-20T10:26:26+02:00 Mael
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Maxime Gimeno,
//                 Mael Rouxel-Labbé

#ifndef CGAL_FACETS_IN_COMPLEX_3_TO_TRIANGLE_MESH_H
#define CGAL_FACETS_IN_COMPLEX_3_TO_TRIANGLE_MESH_H

#include <CGAL/license/Triangulation_3.h>

#include <CGAL/array.h>
#include <CGAL/boost/graph/Euler_operations.h>
#include <CGAL/Cartesian_converter.h>
#include <CGAL/Kernel_traits.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Time_stamper.h>

#include <boost/tuple/tuple.hpp>

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <unordered_map>
#include <vector>

namespace CGAL {

namespace Mesh_3 {

namespace internal {

template <class Polygon>
void resize(Polygon& p, std::size_t size)
{
  p.resize(size);
}

template <std::size_t N, class INT>
void resize(std::array<INT, N>&, std::size_t CGAL_assertion_code(size))
{
  CGAL_assertion(size == N);
}

template<class C3T3, class PointContainer, class FaceContainer>
void facets_in_complex_3_to_triangle_soup(const C3T3& c3t3,
                                          const typename C3T3::Subdomain_index sd_index,
                                          PointContainer& points,
                                          FaceContainer& faces,
                                          const bool normals_point_outside_of_the_subdomain = true,
                                          const bool export_all_facets = false)
{
  typedef typename PointContainer::value_type                            Point_3;
  typedef typename FaceContainer::value_type                             Face;

  typedef typename C3T3::Triangulation                                   Tr;

  typedef typename Tr::Cell_handle                                       Cell_handle;
  typedef typename Tr::Weighted_point                                    Weighted_point;

  typedef typename C3T3::Facets_in_complex_iterator                      Ficit;

  typedef std::unordered_map<Point_3, std::size_t>                       PIM;

  typedef typename C3T3::size_type                                       size_type;

  // triangulation point to range point
  CGAL::Cartesian_converter<typename CGAL::Kernel_traits<typename Tr::Geom_traits::Point_3>::type,
                            typename CGAL::Kernel_traits<Point_3>::type> t2r;

  size_type nf = c3t3.number_of_facets_in_complex();
  faces.reserve(faces.size() + nf);
  points.reserve(points.size() + nf/2); // approximating Euler

  PIM p_to_ids;
  std::size_t inum = 0;

  for(Ficit fit = c3t3.facets_in_complex_begin(),
            end = c3t3.facets_in_complex_end(); fit != end; ++fit)
  {
    Cell_handle c = fit->first;
    int s = fit->second;
    Face f;
    resize(f, 3);

    typename C3T3::Subdomain_index cell_sdi = c3t3.subdomain_index(c);
    typename C3T3::Subdomain_index opp_sdi = c3t3.subdomain_index(c->neighbor(s));

    if(!export_all_facets && cell_sdi != sd_index && opp_sdi != sd_index)
      continue;

    for(std::size_t i=1; i<4; ++i)
    {
      CGAL_assertion_code(typedef typename Tr::Vertex_handle Vertex_handle;)
      CGAL_assertion_code(Vertex_handle v = c->vertex((s+i)&3);)
      CGAL_assertion(v != Vertex_handle() && !c3t3.triangulation().is_infinite(v));

      const Weighted_point& wp = c3t3.triangulation().point(c, (s+i)&3);
      const Point_3& bp = t2r(c3t3.triangulation().geom_traits().construct_point_3_object()(wp));

      auto insertion_res = p_to_ids.emplace(bp, inum);
      if(insertion_res.second) // new point
      {
        points.push_back(bp);
        ++inum;
      }

      f[i-1] = insertion_res.first->second;
    }

    if(export_all_facets)
    {
      if((cell_sdi > opp_sdi) == (s%2 == 1))
        std::swap(f[0], f[1]);
    }
    else
    {
      if(((cell_sdi == sd_index) == (s%2 == 1)) == normals_point_outside_of_the_subdomain)
        std::swap(f[0], f[1]);
    }

    faces.push_back(f);
  }
}

template<class C3T3, class PointContainer, class FaceContainer>
void facets_in_complex_3_to_triangle_soup(const C3T3& c3t3,
                                          PointContainer& points,
                                          FaceContainer& faces)
{
  typedef typename C3T3::Subdomain_index              Subdomain_index;
  Subdomain_index useless = Subdomain_index();
  facets_in_complex_3_to_triangle_soup(c3t3, useless, points, faces,
                                       true/*point outward*/, true /*extract all facets*/);
}

} // end namespace internal

} // end namespace Mesh_3

//! \ingroup PkgMesh3Functions
//!
//! \brief builds a `TriangleMesh` from the surface facets, with a consistent orientation
//!        at the interface of two subdomains.
//!
//! This function exports the surface as a `TriangleMesh` and appends it to `graph`, using
//! `orient_polygon_soup()`.
//!
//! @tparam C3T3 a model of `MeshComplexWithFeatures_3InTriangulation_3`.
//! @tparam TriangleMesh a model of `MutableFaceGraph` with an internal point property map.
//!         The point type should be compatible with the one used in `C3T3`.
//!
//! @param c3t3 an instance of `C3T3`.
//! @param graph an instance of `TriangleMesh`.
template<class C3T3, class TriangleMesh>
void facets_in_complex_3_to_triangle_mesh(const C3T3& c3t3, TriangleMesh& graph)
{
  namespace PMP = CGAL::Polygon_mesh_processing;

  typedef typename boost::property_map<TriangleMesh, boost::vertex_point_t>::type  VertexPointMap;
  typedef typename boost::property_traits<VertexPointMap>::value_type              Point_3;

  typedef std::array<std::size_t, 3>                                               Face;

  std::vector<Face> faces;
  std::vector<Point_3> points;

  Mesh_3::internal::facets_in_complex_3_to_triangle_soup(c3t3, points, faces);

  if(!PMP::is_polygon_soup_a_polygon_mesh(faces))
    PMP::orient_polygon_soup(points, faces);
  CGAL_postcondition(PMP::is_polygon_soup_a_polygon_mesh(faces));

  PMP::polygon_soup_to_polygon_mesh(points, faces, graph);
}

} // namespace CGAL

#endif // CGAL_FACETS_IN_COMPLEX_3_TO_TRIANGLE_MESH_H
