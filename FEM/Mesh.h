#ifndef _MESH_H

#include<vector>
#include<string>
#include "Dense"
#include "Sparse"

class Mesh
{
private:
  // Coordonnées des sommets du maillage
  Eigen::Matrix<double, Eigen::Dynamic, 2> _vertices;
  // Liste de tous les triangles et leur référence
  Eigen::Matrix<int, Eigen::Dynamic, 4> _triangles;
  // Liste de toutes les arêtes, leur référence et leurs triangles associés
  Eigen::Matrix<int, Eigen::Dynamic, 5> _edges;
  // Liste de toutes les arêtes de bord et leur référence
  Eigen::Matrix<int, Eigen::Dynamic, 4> _edgesBoundary;
  // Liste de toutes les arêtes avec conditions de Dirichlet et leur référence
  Eigen::Matrix<int, Eigen::Dynamic, 3> _edgesDirichlet;
  // Liste des références de points concernés par Dirichlet
  // Liste des références des edges associées
  std::vector<std::pair<int,int>> _refVerticesDirichletWithRef;
  // Liste de toutes les arêtes avec conditions de Neumann, leur référence, leur triangle d'appartenance et le numéro de l'arête
  Eigen::Matrix<int, Eigen::Dynamic, 5> _edgesNeumann;

  // Possible de compléter avec les tétra pour du 3D ; avec les quadrangles pour du structuré etc ...

  // Vecteur contenant des références de bord pour le moment d'arêtes (à adapter pour le 3D)
  std::vector<int> _neumannReferences, _dirichletReferences;

public:
  Mesh(std::string nameMesh, std::vector<int> dirichletReferences, std::vector<int> neumannReferences);
  void readMeshAndBuildGeometricEntities(std::string nameMesh);

  const Eigen::Matrix<double, Eigen::Dynamic, 2> & getVertices() const {return _vertices;};
  const Eigen::Matrix<int, Eigen::Dynamic, 4> & getTriangles() const {return _triangles;};
  const std::vector<std::pair<int,int>> & getRefVerticesDirichlet() const {return _refVerticesDirichletWithRef;};
  const Eigen::Matrix<int, Eigen::Dynamic, 5> & getNeumannEdges() const {return _edgesNeumann;};

protected:
  void AddSingleEdge(Eigen::Vector3i edge, int ne, std::vector<int>& head_minv,
		     std::vector<int>& next_edge, int& nb_edges);
};

#define _MESH_H
#endif
