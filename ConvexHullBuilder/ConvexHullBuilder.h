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
    std::vector<Pointd> mVertices;
    ConflictGraph* mG;

    void init();
    unsigned int randomPermutation();
    void createTetrahedron(unsigned int i);
    void createFaceFromVertex(Dcel::Vertex* p3,Dcel::HalfEdge* h);

    QList<Dcel::HalfEdge*> findHorizon(std::vector<Dcel::Face*> horizon);
    QMap<Dcel::HalfEdge*,std::vector<Pointd>> getVisibleVerticesFromHorizon(QList<Dcel::HalfEdge*> horizon);
    void deleteFacesFromCH(std::vector<Dcel::Face*> visiblefaces);
    Dcel::Face* addFace(Dcel::Vertex* otherVertex, Dcel::HalfEdge* existingHe);
    void setTwins(std::vector<Dcel::Face*> &faces);
};

#endif // CONVEXHULLBUILDER_H
