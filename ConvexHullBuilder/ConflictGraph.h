#ifndef CONFLICTGRAPH_H
#define CONFLICTGRAPH_H

#include <QObject>
#include "lib/dcel/drawable_dcel.h"


class ConflictGraph
{
public:
    ConflictGraph(DrawableDcel* mDcel, QVector<Pointd> mVertives);
    QVector<Dcel::Face*>* getVisibleFacesFromVertex(Pointd vertex);
    QVector<Pointd>* getVisibleVerticesFromFace(Dcel::Face *face);
    void deleteFaces(QVector<Dcel::Face*> visiblefaces);
    void deleteVertex(Pointd vertex);
    void updateConflictGraph(Dcel::Face* face, QVector<Pointd> candidateVertices);
    bool IsVisibile(Dcel::Face* face, Pointd v);
    void insertInFconflict(Pointd v, Dcel::Face *f);
    void insertInPconflict(Pointd v, Dcel::Face *f);

private:
    QMap<Dcel::Face*,QVector<Pointd>> P_conflict; //Map associate each face to the ensemble of vertices in the conflicts graphs
    QMap<Pointd,QVector<Dcel::Face*>> F_conflict; //Map associate each vetrex to the ensemble of faces in the conflicts graphs

};

#endif // CONFLICTGRAPH_H
