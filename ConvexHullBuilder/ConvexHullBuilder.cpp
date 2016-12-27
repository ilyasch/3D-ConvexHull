#include "ConvexHullBuilder/ConvexHullBuilder.h"
#include "ConvexHullBuilder/ConflictGraph.h"
#include <QMatrix4x4>
#include <algorithm>
#include <random>
#include <QObject>
//---------------------------------@Ilyas CHAOUA : 65073---------------------ilyaschaoua@gmail.com--------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------------------------------------//
//This class represent the constructor class for the convex hull used to be created from a given dcel in GUI/managers/convexhullmanager.cpp,
//Our main aim is to create the multipe methods that help us to build our convex hull.There are two big steps to follow :
//--------------------------------------------------------------------------------------------------------------------------------------------------//
/*
    1 - Initial phase:
            - Create initial tetrahedron and initialize G
    2 - Itirate phase:
            - Determine visible facets for p(r) by checking G
            - Find horizon and add new facets to CH and G
            - Remove F_conflict(p(r)) from CH
            - Update G for new facets by testing the points in existing conflict lists for facets in CH(P(r-1))    incident to e on the new facets
            - Delete p(r) and F_onflict(p(r)) from G
*/
//--------------------------------------------------------------------------------------------------------------------------------------------------//
//mG is a Map<list of P_conflict,list of F_conflict> present the conflit graph.
//--------------------------------------------------------------------------------------------------------------------------------------------------//

/*!
 * \brief ConvexHullBuilder::ConvexHullBuilder
 * \param dcel
 */

ConvexHullBuilder::ConvexHullBuilder(DrawableDcel* dcel){
    mDcel = dcel;
}
/*!
 * \brief ConvexHullBuilder::build
 * \param showphases
 * \param mainWindow
 * \return
 */
bool ConvexHullBuilder::build(bool showphases,MainWindow* mainWindow){
    //Initialization Phase
    init();
    unsigned int index_point_4 = randomPermutation();
    createTetrahedron(index_point_4);
    mG = new ConflictGraph(mDcel,mVertices);
    //Iteration Phase
    for(unsigned int i=index_point_4+1;i<mVertices.size();i++){
        Dcel::Vertex* p_r = mVertices.at(i);
        //- Determine visible facets for p(r) by checking G
        std::vector<Dcel::Face*> visibleFaces = mG->getVisibleFacesFromVertex(p_r);
        if(visibleFaces.size() > 0){
           Dcel::Vertex* mRemainingVertex = this->mDcel->addVertex(p_r->getCoordinate());
        //- Find horizon and add new facets to CH and G
           QList<Dcel::HalfEdge*> mHorizon = findHorizon(visibleFaces);
           QMap<Dcel::HalfEdge*,std::vector<Dcel::Vertex*>> mVisibleVetices = getVisibleVerticesFromHorizon(mHorizon);


        //- Remove F_conflict(p(r)) from CH
           mG->deleteFaces(visibleFaces);
           deleteFacesFromCH(visibleFaces);
           //add new facets to CH and G from horizon
           std::vector<Dcel::Face*> newFaces;
           for(QList<Dcel::HalfEdge*>::iterator ith = mHorizon.begin(); ith != mHorizon.end(); ith++){
               Dcel::HalfEdge* halfEdge = *ith;
               //create a new face and push it in the vector of new faces
               Dcel::Face* newFace = addFace(mRemainingVertex, halfEdge);
               newFaces.push_back(newFace);
               //Update G for new facets by testing the points in existing conflict lists for facets in CH(P(r-1))    incident to e on the new facets
               mG->updateConflictGraph(newFace, mVisibleVetices.value(halfEdge));
           }
           //set the twins using the ordered new faces vector
           setTwins(newFaces);





           if(showphases){
               mDcel->update();
               mainWindow->updateGlCanvas();
           }
        }
        //- Delete p(r) and F_onflict(p(r)) from G
        mG->deleteVertex(mVertices.at(i));
    }
    return true;
}
/*!
 * \brief ConvexHullBuilder::init
 */
void ConvexHullBuilder::init(){
    Dcel::VertexIterator vit;
    for (vit = mDcel->vertexBegin(); vit != mDcel->vertexEnd(); vit++){
         Dcel::Vertex* v = *vit;
         mVertices.push_back(v);
    }
    mDcel->reset();
}


/*!
 * \brief ConvexHullBuilder::randomPermutation
 * \return the id of the fourth point in pyramid
 */
unsigned int ConvexHullBuilder::randomPermutation(){
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(mVertices.begin(), mVertices.end(), g);
    double det;
    int i = 3;
    do{
    det = QMatrix4x4(
               mVertices.at(0)->getCoordinate().x(),mVertices.at(0)->getCoordinate().y(),mVertices.at(0)->getCoordinate().z(),1.0,
               mVertices.at(1)->getCoordinate().x(),mVertices.at(1)->getCoordinate().y(),mVertices.at(1)->getCoordinate().z(),1.0,
               mVertices.at(2)->getCoordinate().x(),mVertices.at(2)->getCoordinate().y(),mVertices.at(2)->getCoordinate().z(),1.0,
               mVertices.at(i)->getCoordinate().x(),mVertices.at(i)->getCoordinate().y(),mVertices.at(i)->getCoordinate().z(),1.0
               ).determinant();
    ++i;
    }while(fabs(det) < std::numeric_limits<double>::epsilon());
    return i-1;
}
/*!
 * \brief ConvexHullBuilder::createTetrahedron
 * \param i
 */
void ConvexHullBuilder::createTetrahedron(unsigned int i){
    Dcel::Vertex* p0 = mDcel->addVertex(mVertices.at(0)->getCoordinate());
    Dcel::Vertex* p1 = mDcel->addVertex(mVertices.at(1)->getCoordinate());
    Dcel::Vertex* p2 = mDcel->addVertex(mVertices.at(2)->getCoordinate());

    Dcel::HalfEdge* h0_1 = mDcel->addHalfEdge();
    Dcel::HalfEdge* h1_2 = mDcel->addHalfEdge();
    Dcel::HalfEdge* h2_0 = mDcel->addHalfEdge();

    Dcel::Face* f0 = mDcel->addFace();

    h0_1->setFromVertex(p0);
    h0_1->setToVertex(p1);
    h0_1->setNext(h1_2);
    h0_1->setPrev(h2_0);

    h1_2->setFromVertex(p1);
    h1_2->setToVertex(p2);
    h1_2->setNext(h2_0);
    h1_2->setPrev(h0_1);

    h2_0->setFromVertex(p2);
    h2_0->setToVertex(p0);
    h2_0->setNext(h0_1);
    h2_0->setPrev(h1_2);

    p0->setCardinality(2);
    p1->setCardinality(2);
    p2->setCardinality(2);
    f0->setOuterHalfEdge(h0_1);
    h0_1->setFace(f0);
    h1_2->setFace(f0);
    h2_0->setFace(f0);

    Dcel::Vertex* p3 = mDcel->addVertex(mVertices.at(i)->getCoordinate());
    createFaceFromVertex(p3,h0_1);
    createFaceFromVertex(p3,h1_2);
    createFaceFromVertex(p3,h2_0);
}
/*!
 * \brief ConvexHullBuilder::createFaceFromVertex
 * \param p3
 * \param mHalfEdge
 */
void ConvexHullBuilder::createFaceFromVertex(Dcel::Vertex* p3,Dcel::HalfEdge* mHalfEdge){
       Dcel::HalfEdge* h1 = mDcel->addHalfEdge();
       Dcel::HalfEdge* h2 = mDcel->addHalfEdge();
       Dcel::HalfEdge* h3 = mDcel->addHalfEdge();

       Dcel::Vertex* startVertex =  mHalfEdge->getFromVertex();
       Dcel::Vertex* endVertex = mHalfEdge->getToVertex();

       h1->setFromVertex(endVertex);
       endVertex->setIncidentHalfEdge(h1);
       h1->setToVertex(startVertex);
       h1->setNext(h2);
       h1->setPrev(h3);
       h1->setTwin(mHalfEdge);
       mHalfEdge->setTwin(h1);
       endVertex->incrementCardinality();
       startVertex->incrementCardinality();

       h2->setFromVertex(startVertex);
       startVertex->setIncidentHalfEdge(h2);
       h2->setToVertex(p3);
       h2->setNext(h3);
       h2->setPrev(h1);
       startVertex->incrementCardinality();
       p3->incrementCardinality();

       if(mHalfEdge->getPrev()->getTwin() != nullptr){
           Dcel::HalfEdge* twin = mHalfEdge->getPrev()->getTwin()->getPrev();
           h2->setTwin(twin);
           twin->setTwin(h2);
       }
       h3->setFromVertex(p3);
       p3->setIncidentHalfEdge(h3);
       h3->setToVertex(endVertex);
       h3->setNext(h1);
       h3->setPrev(h2);
       endVertex->incrementCardinality();
       p3->incrementCardinality();

       if(mHalfEdge->getNext()->getTwin() != nullptr){
           Dcel::HalfEdge* twin = mHalfEdge->getNext()->getTwin()->getNext();
           h3->setTwin(twin);
           twin->setTwin(h3);
       }
       Dcel::Face* face = mDcel->addFace();
       face->setOuterHalfEdge(h1);
       h1->setFace(face);
       h2->setFace(face);
       h3->setFace(face);
}

/*!
 * \brief ConvexHullBuilder::findHorizon
 * \param mVisibleFaces
 * \return
 */
QList<Dcel::HalfEdge*> ConvexHullBuilder::findHorizon(std::vector<Dcel::Face*> mVisibleFaces){
    QList<Dcel::HalfEdge*> mHorizon;
    bool searchForFirstHalfEdge = false;
    Dcel::HalfEdge* firstHalfEdge;
    for(std::vector<Dcel::Face*>::iterator it = mVisibleFaces.begin(); it < mVisibleFaces.end(); it++){
        Dcel::Face* visibleface = *it;
        for(Dcel::Face::IncidentHalfEdgeIterator itHalfEdge = visibleface->incidentHalfEdgeBegin(); !searchForFirstHalfEdge && itHalfEdge != visibleface->incidentHalfEdgeEnd();itHalfEdge++){
            Dcel::HalfEdge* visibleHalfEdge = *itHalfEdge;
            if(visibleHalfEdge->getTwin() != nullptr){
                 if(!(std::find(mVisibleFaces.begin(), mVisibleFaces.end(), visibleHalfEdge->getTwin()->getFace()) != mVisibleFaces.end())){
                     searchForFirstHalfEdge = true;
                     firstHalfEdge = visibleHalfEdge->getTwin();
                 }
            }
        }
    }
    if(searchForFirstHalfEdge){
        Dcel::HalfEdge *current, *next, *twinOfNext;
                Dcel::Face *incidentFace;
                current = firstHalfEdge;
                mHorizon.append(firstHalfEdge);
                //we iterate until we get back to the first horizon half edge
                do{
                    next = current->getNext();
                    twinOfNext = next->getTwin();
                    incidentFace = twinOfNext->getFace();
                    //if the incident face of the twin is visible, we have an horizon half edge
                    if(std::find(mVisibleFaces.begin(), mVisibleFaces.end(), incidentFace) != mVisibleFaces.end()){
                        mHorizon.insert(mHorizon.end(),next);
                        current = next;
                    }else {
                        current = twinOfNext;
                    }
        }while(firstHalfEdge != current && firstHalfEdge != current->getNext());
    }else{
       //qInfo("There is no visible Faces for founding the horizon");
    }
    return mHorizon;
}
/*!
 * \brief ConvexHullBuilder::getVisibleVerticesFromHorizon
 * \param horizon
 * \return
 */
QMap<Dcel::HalfEdge*,std::vector<Dcel::Vertex*>> ConvexHullBuilder::getVisibleVerticesFromHorizon(QList<Dcel::HalfEdge*> horizon){
        QMap<Dcel::HalfEdge*,std::vector<Dcel::Vertex*>>  mVisibleVerticesinHorizon;
        //for each half edge in the horizon
        QListIterator<Dcel::HalfEdge*> it(horizon);
        while (it.hasNext()){
            Dcel::HalfEdge* halfEdge = it.next();
            //get its and its twin incident faces
            Dcel::Face *face1 = halfEdge->getFace();
            Dcel::Face *face2 = halfEdge->getTwin()->getFace();
            //get the set of the vertices visible from the former faces
            std::vector<Dcel::Vertex*> conflict1, conflict2;
            conflict1 = mG->getVisibleVerticesFromFace(face1);
            conflict2 = mG->getVisibleVerticesFromFace(face2);
            //merge the sets
            std::vector<Dcel::Vertex*> AB;
            AB.reserve( conflict1.size() + conflict2.size() ); // preallocate memory
            AB.insert( AB.end(), conflict1.begin(), conflict1.end() );
            AB.insert( AB.end(), conflict2.begin(), conflict2.end() );
            //associate the merged set to the horizon half edge
            mVisibleVerticesinHorizon.insert(halfEdge,AB);
        }
    return mVisibleVerticesinHorizon;
}
/*!
 * \brief ConvexHullBuilder::deleteFacesFromCH
 * \param visiblefaces
 */
void ConvexHullBuilder::deleteFacesFromCH(std::vector<Dcel::Face*> visiblefaces){
    for(std::vector<Dcel::Face*>::iterator it = visiblefaces.begin();it<visiblefaces.end();it++){
        Dcel::Face* face = *it;
        for(Dcel::Face::IncidentHalfEdgeIterator itHalfEdge = face->incidentHalfEdgeBegin(); itHalfEdge != face->incidentHalfEdgeEnd();itHalfEdge++){
            Dcel::HalfEdge* halfedge = *itHalfEdge;
            Dcel::Vertex* start = halfedge->getFromVertex();
            Dcel::Vertex* end = halfedge->getToVertex();
            mDcel->deleteHalfEdge(halfedge);
            //Every time we delete an half edge => decrement the cardinality of each vertex
            start->decrementCardinality();
            end->decrementCardinality();
            //the vertex is disconnected when cardinality == 0 => delete it
            if(start->getCardinality() == 0){
                mDcel->deleteVertex(start);
             }
            if(end->getCardinality() == 0){
               mDcel->deleteVertex(end);
            }
        }
        mDcel->deleteFace(face);
    }
}
/*!
 * \brief ConvexHullBuilder::addFace
 * \param otherVertex
 * \param existingHe
 * \return
 */
Dcel::Face* ConvexHullBuilder::addFace(Dcel::Vertex* otherVertex, Dcel::HalfEdge* existingHe){
    Dcel::HalfEdge* he1 = mDcel->addHalfEdge();
    Dcel::HalfEdge* he2 = mDcel->addHalfEdge();
    Dcel::HalfEdge* he3 = mDcel->addHalfEdge();

    Dcel::Vertex* startVertex = existingHe->getFromVertex();
    Dcel::Vertex* endVertex = existingHe->getToVertex();

    he1->setFromVertex(endVertex);
    endVertex->setIncidentHalfEdge(he1);
    he1->setToVertex(startVertex);
    he1->setNext(he2);
    he1->setPrev(he3);
    he1->setTwin(existingHe);
    existingHe->setTwin(he1);
    endVertex->incrementCardinality();
    startVertex->incrementCardinality();

    he2->setFromVertex(startVertex);
    startVertex->setIncidentHalfEdge(he2);
    he2->setToVertex(otherVertex);
    he2->setNext(he3);
    he2->setPrev(he1);
    startVertex->incrementCardinality();
    otherVertex->incrementCardinality();

    he3->setFromVertex(otherVertex);
    otherVertex->setIncidentHalfEdge(he3);
    he3->setToVertex(endVertex);
    he3->setNext(he1);
    he3->setPrev(he2);
    endVertex->incrementCardinality();
    otherVertex->incrementCardinality();

    Dcel::Face* face = mDcel->addFace();
    face->setOuterHalfEdge(he1);
    he1->setFace(face);
    he2->setFace(face);
    he3->setFace(face);

    return face;
}
/*!
 * \brief ConvexHullBuilder::setTwins
 * \param faces
 */
void ConvexHullBuilder::setTwins(std::vector<Dcel::Face*> faces){
    std::vector<Dcel::HalfEdge*> he1Set(faces.size());
    std::vector<Dcel::HalfEdge*> he2Set(faces.size());
    std::vector<Dcel::HalfEdge*> he3Set(faces.size());

    for(unsigned int i=0; i<faces.size(); i++){
        he1Set[i] = faces.at(i)->getOuterHalfEdge();
        he2Set[i] = faces.at(i)->getOuterHalfEdge()->getNext();
        he3Set[i] = faces.at(i)->getOuterHalfEdge()->getNext()->getNext();
    }
    for(unsigned int i=1; i<=faces.size(); i++){
        he2Set[i%faces.size()]->setTwin(he3Set[i-1]);
        he3Set[i-1]->setTwin(he2Set[i%faces.size()]);
    }
}
