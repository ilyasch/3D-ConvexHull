#ifndef CONFLICTGRAPH_H
#define CONFLICTGRAPH_H

#include <QObject>
#include "lib/dcel/drawable_dcel.h"


class ConflictGraph
{
public:
    ConflictGraph(DrawableDcel* mDcel, std::vector<Pointd> mVertives);
    std::vector<Dcel::Face*>* getVisibleFacesFromVertex(Pointd vertex);
    std::vector<Pointd>* getVisibleVerticesFromFace(Dcel::Face *face);
    void deleteFaces(std::vector<Dcel::Face*> visiblefaces);
    void deleteVertex(Pointd vertex);
    void updateConflictGraph(Dcel::Face* face, std::vector<Pointd> candidateVertices);
    bool IsVisibile(Dcel::Face* face, Pointd v);
    void insertInFconflict(Pointd v, Dcel::Face *f);
    void insertInPconflict(Pointd v, Dcel::Face *f);

    //TODO : change name or delete it if possible
    Pointd getFaceNormalDirection(Dcel::Face *face);
private:
    QMap<Dcel::Face*,std::vector<Pointd>> P_conflict; //Map associate each face to the ensemble of vertices in the conflicts graphs
    QMap<Pointd,std::vector<Dcel::Face*>> F_conflict; //Map associate each vetrex to the ensemble of faces in the conflicts graphs

};

#endif // CONFLICTGRAPH_H
