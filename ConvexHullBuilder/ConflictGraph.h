#ifndef CONFLICTGRAPH_H
#define CONFLICTGRAPH_H

#include <QObject>
#include "lib/dcel/drawable_dcel.h"


class ConflictGraph
{
public:
    ConflictGraph(DrawableDcel* mDcel, std::vector<Dcel::Vertex*> mVertives);
    std::vector<Dcel::Face*> getVisibleFacesFromVertex(Dcel::Vertex *vertex);
    std::vector<Dcel::Vertex*> getVisibleVerticesFromFace(Dcel::Face *face);
    void deleteFaces(std::vector<Dcel::Face*> visiblefaces);
    void deleteVertex(Dcel::Vertex* vertex);
    void updateConflictGraph(Dcel::Face* face, std::vector<Dcel::Vertex*> candidateVertices);
    bool IsVisibile(Dcel::Face* face, Dcel::Vertex* v);
    void insertInFconflict(Dcel::Vertex* v, Dcel::Face *f);
    void insertInPconflict(Dcel::Vertex* v, Dcel::Face *f);

    //TODO : change name or delete it if possible
    Dcel::Vertex* getFaceNormalDirection(Dcel::Face *face);
private:
    QMap<Dcel::Face*,std::vector<Dcel::Vertex*>> P_conflict; //Map associate each face to the ensemble of vertices in the conflicts graphs
    QMap<Dcel::Vertex*,std::vector<Dcel::Face*>> F_conflict; //Map associate each vetrex to the ensemble of faces in the conflicts graphs

};

#endif // CONFLICTGRAPH_H
