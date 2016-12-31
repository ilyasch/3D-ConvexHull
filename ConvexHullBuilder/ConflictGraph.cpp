#include "ConvexHullBuilder/ConflictGraph.h"

/*!
 * \brief ConflictGraph::ConflictGraph : Constructor of the class ConflictGraph
 * \param Dcel
 * \param Vertices
 */
ConflictGraph::ConflictGraph(DrawableDcel* mDcel,std::vector<Pointd> mVertives){
    for(std::vector<Pointd>::iterator it = mVertives.begin(); it != mVertives.end();it++){
        Pointd v = *it;
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
std::vector<Dcel::Face*>* ConflictGraph::getVisibleFacesFromVertex(Pointd vertex){
    if(!F_conflict.contains(vertex)) {
        std::vector<Dcel::Face*> faces;
        F_conflict.insert(vertex,faces);
    }
    return new std::vector<Dcel::Face*>(F_conflict.value(vertex));
}
/*!
 * \brief ConflictGraph::getVisibleVerticesFromFace
 * \param f
 * \return vector of vertices
 */
std::vector<Pointd>* ConflictGraph::getVisibleVerticesFromFace(Dcel::Face* f){
    if(!P_conflict.contains(f)){
      std::vector<Pointd> qv;
      P_conflict.insert(f,qv);
    }
    return new std::vector<Pointd>(P_conflict.value(f));
}
/*!
 * \brief ConflictGraph::deleteFaces
 * \param visiblefaces
 */
void ConflictGraph::deleteFaces(std::vector<Dcel::Face*> visiblefaces){
    for(std::vector<Dcel::Face*>::const_iterator it = visiblefaces.begin();it<visiblefaces.end();it++){
        Dcel::Face* face = *it;
        std::vector<Pointd> visiblevertices = P_conflict.value(face);
        if(visiblevertices.size() != 0){
            P_conflict.remove(face);
            for(std::vector<Pointd>::iterator itv = visiblevertices.begin();itv<visiblevertices.end();itv++){
                Pointd vertex = *itv;
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
void ConflictGraph::deleteVertex(Pointd vertex){
    std::vector<Dcel::Face*> facesAssTovertex = F_conflict.value(vertex);
    if(facesAssTovertex.size() != 0){
        F_conflict.remove(vertex);
        for(std::vector<Dcel::Face*>::iterator itf = facesAssTovertex.begin();itf<facesAssTovertex.end();itf++){
            Dcel::Face* face = *itf;
            std::vector<Pointd> verticesASsToThisFace = P_conflict.value(face);
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
bool ConflictGraph::IsVisibile(Dcel::Face* face, Pointd v){
       Dcel::Vertex* ve = *(face->incidentVertexBegin());
       //if the dot product betweet it and the face normal is positive => the vector lies in the same semi-space of the normal => implying that the vertex sees the face
       //I use this form instead the determinant of the 4x4 matrix for performance
       Pointd vertices[3], vec1, vec2, dir;
          //get the vertices of the face
          int i=0;
          for(Dcel::Face::IncidentVertexIterator vit = face->incidentVertexBegin(); vit != face->incidentVertexEnd(); ++vit){
              vertices[i] = (*vit)->getCoordinate();
              if(i<3)
                i++;
          }
          //the normal vector is the cross product between two edge vectors of the face
          //I don't normalize because I only need the direction
          vec1 = vertices[1] - vertices[0];
          vec2 = vertices[2] - vertices[0];
          dir = vec1.cross(vec2);
          return (v - ve->getCoordinate()).dot(dir) > std::numeric_limits<double>::epsilon();
}
/*!
 * \brief ConflictGraph::insertInFconflict
 * \param vertex
 * \param face
 */
void ConflictGraph::insertInFconflict(Pointd v, Dcel::Face *f){
        if(!F_conflict.value(v).empty()){
          std::vector<Dcel::Face*> list = F_conflict.value(v);
          list.push_back(f);
        }else{
            std::vector<Dcel::Face*> list;
            list.push_back(f);
            F_conflict.insert(v,list);
        }
}
/*!
 * \brief ConflictGraph::insertInPconflict
 * \param vertex
 * \param face
 */
void ConflictGraph::insertInPconflict(Pointd v, Dcel::Face *f){
        if(!P_conflict.value(f).empty()){
          std::vector<Pointd> list = P_conflict.value(f);
          list.push_back(v);
        }else{
            std::vector<Pointd> list;
            list.push_back(v);
            P_conflict.insert(f,list);
        }
}
/*!
 * \brief ConflictGraph::updateConflictGraph
 * \param face
 * \param candidateVertices
 */
void ConflictGraph::updateConflictGraph(Dcel::Face* face, std::vector<Pointd> candidateVertices){

    for(std::vector<Pointd>::iterator itv = candidateVertices.begin();itv<candidateVertices.end();itv++){
        Pointd v = *itv;
        if(IsVisibile(face,v)){
            insertInFconflict(v,face);
            insertInPconflict(v,face);
        }
    }
}

