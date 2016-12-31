#include "ConvexHullBuilder/ConflictGraph.h"

/*!
 * \brief ConflictGraph::ConflictGraph : Constructor of the class ConflictGraph
 * \param Dcel
 * \param Vertices
 */
ConflictGraph::ConflictGraph(DrawableDcel* mDcel,QVector<Pointd> mVertives){
    for(QVector<Pointd>::iterator it = mVertives.begin(); it != mVertives.end();it++){
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
QVector<Dcel::Face*>* ConflictGraph::getVisibleFacesFromVertex(Pointd vertex){
    if(!F_conflict.contains(vertex)) {
        QVector<Dcel::Face*> faces;
        F_conflict.insert(vertex,faces);
    }
    return new QVector<Dcel::Face*>(F_conflict.value(vertex));
}
/*!
 * \brief ConflictGraph::getVisibleVerticesFromFace
 * \param f
 * \return vector of vertices
 */
QVector<Pointd>* ConflictGraph::getVisibleVerticesFromFace(Dcel::Face* f){
    if(!P_conflict.contains(f)){
      QVector<Pointd> qv;
      P_conflict.insert(f,qv);
    }
    return new QVector<Pointd>(P_conflict.value(f));
}
/*!
 * \brief ConflictGraph::deleteFaces
 * \param visiblefaces
 */
void ConflictGraph::deleteFaces(QVector<Dcel::Face*> visiblefaces){
    for(QVector<Dcel::Face*>::const_iterator it = visiblefaces.begin();it<visiblefaces.end();it++){
        Dcel::Face* face = *it;
        QVector<Pointd> visiblevertices = P_conflict.value(face);
        if(visiblevertices.size() != 0){
            P_conflict.remove(face);
            for(QVector<Pointd>::iterator itv = visiblevertices.begin();itv<visiblevertices.end();itv++){
                Pointd vertex = *itv;
                QVector<Dcel::Face*> facesAssTovertex = F_conflict.value(vertex);
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
    QVector<Dcel::Face*> facesAssTovertex = F_conflict.value(vertex);
    if(facesAssTovertex.size() != 0){
        F_conflict.remove(vertex);
        for(QVector<Dcel::Face*>::iterator itf = facesAssTovertex.begin();itf<facesAssTovertex.end();itf++){
            Dcel::Face* face = *itf;
            QVector<Pointd> verticesASsToThisFace = P_conflict.value(face);
            verticesASsToThisFace.erase(std::remove(verticesASsToThisFace.begin(), verticesASsToThisFace.end(), vertex), verticesASsToThisFace.end());
        }
    }
}

/*! Efficent Way to check the visibility
 * \brief ConflictGraph::IsVisibile
 * \param face
 * \param v
 * \return true/false
 */
bool ConflictGraph::IsVisibile(Dcel::Face* face, Pointd v){
       Dcel::Vertex* ve = *(face->incidentVertexBegin());
       //if the dot product betweet it and the face normal is positive => the vector lies in the same semi-space of the normal => implying that the vertex sees the face
       Pointd vertices[3], vec1, vec2, dir;
          //get the vertices of the face
          int i=0;
          for(Dcel::Face::IncidentVertexIterator vit = face->incidentVertexBegin(); vit != face->incidentVertexEnd(); ++vit){
              vertices[i] = (*vit)->getCoordinate();
              if(i<3)
                i++;
          }
          //the normal vector is the cross product between two edge vectors of the face
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
          QVector<Dcel::Face*> list = F_conflict.value(v);
          list.push_back(f);
        }else{
            QVector<Dcel::Face*> list;
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
          QVector<Pointd> list = P_conflict.value(f);
          list.push_back(v);
        }else{
            QVector<Pointd> list;
            list.push_back(v);
            P_conflict.insert(f,list);
        }
}
/*!
 * \brief ConflictGraph::updateConflictGraph
 * \param face
 * \param candidateVertices
 */
void ConflictGraph::updateConflictGraph(Dcel::Face* face, QVector<Pointd> candidateVertices){

    for(QVector<Pointd>::iterator itv = candidateVertices.begin();itv<candidateVertices.end();itv++){
        Pointd v = *itv;
        if(IsVisibile(face,v)){
            insertInFconflict(v,face);
            insertInPconflict(v,face);
        }
    }
}

