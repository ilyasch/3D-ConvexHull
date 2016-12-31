#ifndef CONVEXHULLBUILDER_H
#define CONVEXHULLBUILDER_H

#include <QObject>
#include "GUI/mainwindow.h"
#include "lib/dcel/drawable_dcel.h"
#include "ConvexHullBuilder/ConflictGraph.h"



class ConvexHullBuilder{

public:
    ConvexHullBuilder(DrawableDcel* mDcel);
    bool build(bool showphases , MainWindow* mMainWindow);
private:
    DrawableDcel* mDcel ;
    QVector<Pointd> mVertices;
    ConflictGraph* mG;

    void init();
    unsigned int randomPermutation();
    void createTetrahedron(unsigned int i);
    void createFaceFromVertex(Dcel::Vertex* p3,Dcel::HalfEdge* h);

    QList<Dcel::HalfEdge*> findHorizon(QVector<Dcel::Face*> horizon);
    QMap<Dcel::HalfEdge*,QVector<Pointd>> getVisibleVerticesFromHorizon(QList<Dcel::HalfEdge*> horizon);
    void deleteFacesFromCH(QVector<Dcel::Face*> visiblefaces);
    Dcel::Face* addFace(Dcel::Vertex* otherVertex, Dcel::HalfEdge* existingHe);
    void setTwins(QVector<Dcel::Face*> &faces);
};

#endif // CONVEXHULLBUILDER_H
