#ifndef _MESH_CPP

#include "Mesh.h"
#include<iostream>
#include<fstream>
#include <assert.h>

Mesh::Mesh(std::string nameMesh, std::vector<int> dirichletReferences, std::vector<int> neumannReferences)
: _dirichletReferences(dirichletReferences), _neumannReferences(neumannReferences)
{
  readMeshAndBuildGeometricEntities(nameMesh);
}

void Mesh::readMeshAndBuildGeometricEntities(std::string nameMesh)
{
  std::ifstream mesh_file(nameMesh.data());
  if (!mesh_file.is_open())
  {
    std::cout << "Unable to open file " << nameMesh << std::endl;
    abort();
  }
  else
  {
    std::cout << "Reading mesh: " << nameMesh << std::endl;
  }

  std::string file_line;

  while (!mesh_file.eof())
  {
    getline(mesh_file, file_line);

    if (file_line.find("Vertices") != std::string::npos)
    {
      int nb_vertices(0);
      double z(0.);
      mesh_file >> nb_vertices;
      std::cout << "Number of vertices  (" << nb_vertices << ")" << std::endl;
      _vertices.resize(nb_vertices,2);
      int ref;
      for (int i = 0 ; i < nb_vertices ; ++i)
      {
        mesh_file >> _vertices(i,0) >> _vertices(i,1) >> z >> ref;
      }
    }
    else if (file_line.find("Edges") != std::string::npos)
    {
      int nb_edges(0);
      mesh_file >> nb_edges;
      std::cout << "Number of edges (" << nb_edges << ")" << std::endl;
      _edgesBoundary.resize(nb_edges,4);
      int n1, n2, ref;
      for (int i = 0 ; i < nb_edges ; ++i)
      {
        mesh_file >> n1 >> n2 >> ref;
        n1--; n2--;
        _edgesBoundary.row(i) << n1, n2, ref, -1;
      }
    }
    else if (file_line.find("Triangles") != std::string::npos)
    {
      int nb_triangles(0);
      mesh_file >> nb_triangles;
      std::cout << "Number of triangles (" << nb_triangles << ")" << std::endl;
      _triangles.resize(nb_triangles,4);
      int vertex1, vertex2, vertex3, ref;
      for (int i = 0 ; i < nb_triangles ; ++i)
      {
        mesh_file >> vertex1 >> vertex2 >> vertex3 >> ref;
        vertex1--; vertex2--; vertex3--;
        _triangles.row(i) << vertex1, vertex2, vertex3, ref;
      }
    }
  }

  // Toutes les aretes exterieures du maillage sont presentes
  int nb_edges = (3*_triangles.size() + _edgesBoundary.size())/2;
  _edges.resize(nb_edges,5);
  _edges.col(3) = Eigen::VectorXi::Ones(_edges.rows()); _edges.col(3) *= -1;
  _edges.col(4) = Eigen::VectorXi::Ones(_edges.rows()); _edges.col(4) *= -1;

  int nb_vertices = _vertices.size();
  std::vector<int> head_minv(nb_vertices, -1);
  std::vector<int> next_edge(nb_edges, -1);

  // On rajoute d'abord les arêtes du bord
  nb_edges = 0;
  for (int i = 0; i < _edgesBoundary.rows(); i++)
  {
    Eigen::Vector3i edge; edge << _edgesBoundary(i,0), _edgesBoundary(i,1), _edgesBoundary(i,2);
    this->AddSingleEdge(edge, -1, head_minv, next_edge, nb_edges);
  }

  // Ensuite les arêtes intérieures
  for (int i = 0; i < _triangles.rows(); i++)
  {
    Eigen::Vector3i nv; nv << _triangles(i,0), _triangles(i,1), _triangles(i,2);
    for (int j = 0; j < 3; j++)
    {
      Eigen::Vector3i edge; edge << nv(j), nv((j+1)%3), 1000;
      this->AddSingleEdge(edge, i, head_minv, next_edge, nb_edges);
    }
  }

  // Conditions aux bords
  std::vector<int>::iterator it;
  for (int i = 0 ; i < _edgesBoundary.rows() ; ++i)
  {
    _edgesBoundary(i,3) = _edges(i,3);
    int n1(_edgesBoundary(i,0)), n2(_edgesBoundary(i,1)), ref(_edgesBoundary(i,2)), t1(_edgesBoundary(i,3));
    it = find (_dirichletReferences.begin(), _dirichletReferences.end(), ref);

    if (it != _dirichletReferences.end())
    {
      _edgesDirichlet.conservativeResize(_edgesDirichlet.rows()+1, 3);
      _edgesDirichlet.row(_edgesDirichlet.rows()-1) << n1, n2, ref;
      _refVerticesDirichletWithRef.push_back(std::make_pair(n1,ref));
      _refVerticesDirichletWithRef.push_back(std::make_pair(n2,ref));
    }
    else
    {
      it = find (_neumannReferences.begin(), _neumannReferences.end(), ref);
      if (it != _neumannReferences.end())
      {
        int numedge(-1);
        if (_triangles(t1,0) == n1)
        {
          if (_triangles(t1,1) == n2)
            numedge = 1;
          else if (_triangles(t1,2) == n2)
            numedge = 3;
        }
        else if (_triangles(t1,1) == n1)
        {
          if (_triangles(t1,0) == n2)
            numedge = 1;
          else if (_triangles(t1,2) == n2)
            numedge = 2;
        }
        else if (_triangles(t1,2) == n1)
        {
          if (_triangles(t1,0) == n2)
            numedge = 3;
          else if (_triangles(t1,1) == n2)
            numedge = 2;
        }
        if (numedge < 0)
        {
          std::cout << "The triangle is not associated to this edge!" << std::endl;
        }
        _edgesNeumann.conservativeResize(_edgesNeumann.rows()+1, 5);
        _edgesNeumann.row(_edgesNeumann.rows()-1) << n1, n2, ref, t1, numedge;
      }
      else
      {
          std::cout << "There is no boundary condition for edge " << i << " (reference"  << ref << ")" << std::endl;
          abort();
      }
    }
  }
  if (_refVerticesDirichletWithRef.size() > 0)
  {
    // sort pair using the first argument of _refVerticesDirichletWithRef
    sort(_refVerticesDirichletWithRef.begin(), _refVerticesDirichletWithRef.end());
    _refVerticesDirichletWithRef.erase(std::unique(_refVerticesDirichletWithRef.begin(),
                  _refVerticesDirichletWithRef.end()), _refVerticesDirichletWithRef.end());
 }
}

// Méthode interne qui rajoute une arete
void Mesh::AddSingleEdge(Eigen::Vector3i edge, int ne, std::vector<int>& head_minv, std::vector<int>& next_edge, int& nb_edges)
{
  int n1 = edge(0);
  int n2 = edge(1);
  int ref = edge(2);

  bool exist = false;
  // we look at the list of edges leaving from n1
  // if we find the same edge than n1->n2 we add the edge
  for (int e = head_minv[n1]; e != -1; e = next_edge[e])
  {
    if (_edges(e,1) == n2)
    {
      if (ne >= 0)
      {
        if (_edges(e,3)<0)
          _edges(e,3) = ne;
        else if (_edges(e,4)<0)
          _edges(e,4) = ne;
      }
      exist = true;
    }
  }

  // if the edge has not been found, we create it
  if (!exist)
  {
    // we initialize the edge
    _edges(nb_edges,0) = n1; _edges(nb_edges,1) = n2; _edges(nb_edges,2) = ref;
    if (ne >= 0)
    {
      if (_edges(nb_edges,3)<0)
        _edges(nb_edges,3) = ne;
      else if (_edges(nb_edges,4)<0)
        _edges(nb_edges,4) = ne;
    }
    // we update the arrays next_edge and head_minv
    next_edge[nb_edges] = head_minv[n1];
    head_minv[n1] = nb_edges;
    nb_edges++;
  }
}

#define _MESH_CPP
#endif
