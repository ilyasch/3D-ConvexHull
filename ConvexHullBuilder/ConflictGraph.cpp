#include "ConvexHullBuilder/ConflictGraph.h"

/*!
 * \brief ConflictGraph::ConflictGraph : Constructor of the class ConflictGraph
 * \param Dcel
 * \param Vertices
 */
ConflictGraph::ConflictGraph(DrawableDcel* mDcel,std::vector<Dcel::Vertex*> mVertives){
    for(std::vector<Dcel::Vertex*>::iterator it = mVertives.begin(); it != mVertives.end();it++){
        Dcel::Vertex* v = *it;
        for(Dcel::FaceIterator fit= mDcel->faceBegin();fit != mDcel->faceEnd();fit++){
            Dcel::Face* f= *fit;
            if(IsVisibile(f,v)){
                insertInFconflict(v,f);
                insertInPconflict(v,f);
            }
        }
    }
}
/*!
 * \brief ConflictGraph::getVisibleFacesFromVertex
 * \param vertex
 * \return vector of faces
 */
std::vector<Dcel::Face*> ConflictGraph::getVisibleFacesFromVertex(Dcel::Vertex* vertex){
    if(F_conflict.contains(vertex))
        return F_conflict.value(vertex);
    else{
        std::vector<Dcel::Face*> qp;
        return qp;
    }
}
/*!
 * \brief ConflictGraph::getVisibleVerticesFromFace
 * \param f
 * \return vector of vertices
 */
std::vector<Dcel::Vertex*> ConflictGraph::getVisibleVerticesFromFace(Dcel::Face* f){
    if(P_conflict.contains(f))
        return P_conflict.value(f);
    else{
      std::vector<Dcel::Vertex*> qv;
      return qv;
    }
}
/*!
 * \brief ConflictGraph::deleteFaces
 * \param visiblefaces
 */
void ConflictGraph::deleteFaces(std::vector<Dcel::Face*> visiblefaces){
    for(std::vector<Dcel::Face*>::const_iterator it = visiblefaces.begin();it<visiblefaces.end();it++){
        Dcel::Face* face = *it;
        std::vector<Dcel::Vertex*> visiblevertices = P_conflict.value(face);
        if(visiblevertices.size() != 0){
            P_conflict.remove(face);
            for(std::vector<Dcel::Vertex*>::iterator itv = visiblevertices.begin();itv<visiblevertices.end();itv++){
                Dcel::Vertex* vertex = *itv;
                std::vector<Dcel::Face*> facesAssTovertex = F_conflict.value(vertex);
                if(facesAssTovertex.size() != 0){
                    facesAssTovertex.erase(std::remove(facesAssTovertex.begin(), facesAssTovertex.end(), face), facesAssTovertex.end());
                }
            }
        }
    }
}
/*!
 * \brief ConflictGraph::deleteVertex
 * \param vertex
 */
void ConflictGraph::deleteVertex(Dcel::Vertex* vertex){
    std::vector<Dcel::Face*> facesAssTovertex = F_conflict.value(vertex);
    if(facesAssTovertex.size() != 0){
        F_conflict.remove(vertex);
        for(std::vector<Dcel::Face*>::iterator itf = facesAssTovertex.begin();itf<facesAssTovertex.end();itf++){
            Dcel::Face* face = *itf;
            std::vector<Dcel::Vertex*> verticesASsToThisFace = P_conflict.value(face);
            verticesASsToThisFace.erase(std::remove(verticesASsToThisFace.begin(), verticesASsToThisFace.end(), vertex), verticesASsToThisFace.end());
        }
    }
}

//TODO : badal hatchi b mes commentaires bach yrja3 mtiitaz e chra7 had la phase f rapport
/*!
 * \brief ConflictGraph::IsVisibile
 * \param face
 * \param v
 * \return true/false
 */
bool ConflictGraph::IsVisibile(Dcel::Face* face, Dcel::Vertex* v){
       Dcel::Vertex *ve = *(face->incidentVertexBegin());
       //if the dot product betweet it and the face normal is positive => the vector lies in the same semi-space of the normal => implying that the vertex sees the face
       //I use this form instead the determinant of the 4x4 matrix for performance
       Pointd vertices[3], vec1, vec2, dir;
          //get the vertices of the face
          int i=0;
          for(Dcel::Face::IncidentVertexIterator vit = face->incidentVertexBegin(); vit != face->incidentVertexEnd(); vit++){
              vertices[i] = (*vit)->getCoordinate();
              i++;
          }
          //the normal vector is the cross product between two edge vectors of the face
          //I don't normalize because I only need the direction
          vec1 = vertices[1] - vertices[0];
          vec2 = vertices[2] - vertices[0];
          dir = vec1.cross(vec2);
          return (v->getCoordinate() - ve->getCoordinate()).dot(dir) > std::numeric_limits<double>::epsilon();
}
/*!
 * \brief ConflictGraph::insertInFconflict
 * \param vertex
 * \param face
 */
void ConflictGraph::insertInFconflict(Dcel::Vertex* v, Dcel::Face *f){
        std::vector<Dcel::Face*> listofFaces;
        listofFaces.push_back(f);
        F_conflict.insert(v,listofFaces);
}
/*!
 * \brief ConflictGraph::insertInPconflict
 * \param vertex
 * \param face
 */
void ConflictGraph::insertInPconflict(Dcel::Vertex* v, Dcel::Face *f){
        std::vector<Dcel::Vertex*> listofVertices;
        listofVertices.push_back(v);
        P_conflict.insert(f,listofVertices);

}
/*!
 * \brief ConflictGraph::updateConflictGraph
 * \param face
 * \param candidateVertices
 */
void ConflictGraph::updateConflictGraph(Dcel::Face* face, std::vector<Dcel::Vertex*> candidateVertices){

    for(std::vector<Dcel::Vertex*>::iterator itv = candidateVertices.begin();itv<candidateVertices.end();itv++){
        Dcel::Vertex* v = *itv;
        if(IsVisibile(face,v)){
            insertInFconflict(v,face);
            insertInPconflict(v,face);
        }
    }
}

