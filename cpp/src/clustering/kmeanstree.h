#pragma once

#include <vector>
#include <string>
using namespace std;

#include "kmeans.h"
#include "point.h"

#include <boost/serialization/vector.hpp>

/**
    \class KMeansTree
    \brief Decision tree with KMeans as decision nodes
**/
template<typename P, typename T>
class KMeansTree {
public:
    KMeansTree() {}
    KMeansTree(int n_clusters, int dimension, int depth, string prefix = "");

    // Add points as training set
    void addPoint(P* point);
    void addPoint(P& point);
    void addPoints(vector<P*>& data);
    void addPoints(vector<P>& data);

    // Set training set
    void setPoints(vector<P*>& data);
    void setPoints(vector<P>& data);

    // Starting point before first fit
    void init();

    // Train
    void fit();

    const string getWord(const P &point) const;
    const bool isTrained() const;
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & BOOST_SERIALIZATION_NVP(n_clusters);
        ar & BOOST_SERIALIZATION_NVP(depth);
        ar & BOOST_SERIALIZATION_NVP(prefix);
        ar & BOOST_SERIALIZATION_NVP(kmeans);
        ar & BOOST_SERIALIZATION_NVP(subtrees);
    }
    int n_clusters, depth;
    string prefix;
    KMeans<P, T> kmeans;
    vector<KMeansTree<P, T>> subtrees;
};

template<typename P, typename T>
KMeansTree<P, T>::KMeansTree(int n_clusters, int dimension, int depth, string prefix) :
    n_clusters(n_clusters),
    depth(depth),
    prefix(prefix),
    kmeans(KMeans<P, T>(n_clusters, dimension)) {
    if(depth > 0) {
        for(int i = 0; i < n_clusters; i++) {
            char c = (char) (((int) 'a') + i);
            KMeansTree<P, T> subtree(n_clusters, dimension, depth-1, prefix+c);
            subtrees.push_back(subtree);
        }
    }
}

template<typename P, typename T>
void KMeansTree<P, T>::addPoint(P* point) {
    kmeans.addPoint(point);
}

template<typename P, typename T>
void KMeansTree<P, T>::addPoint(P& point) {
    kmeans.addPoint(point);
}

template<typename P, typename T>
void KMeansTree<P, T>::addPoints(vector<P*>& data) {
    kmeans.addPoints(data);
}

template<typename P, typename T>
void KMeansTree<P, T>::addPoints(vector<P>& data) {
    kmeans.addPoints(data);
}

template<typename P, typename T>
void KMeansTree<P, T>::setPoints(vector<P*>& data) {
    kmeans.setPoints(data);
}

template<typename P, typename T>
void KMeansTree<P, T>::setPoints(vector<P>& data) {
    kmeans.setPoints(data);
}

template<typename P, typename T>
void KMeansTree<P, T>::init() {
    kmeans.init();
}

template<typename P, typename T>
void KMeansTree<P, T>::fit() {
    cout << "Training level " << this->depth << endl;
    kmeans.fit();
    if(depth > 0) {
        for(int i = 0; i < n_clusters; i++) {
            if(kmeans.getPartitions()[i].size() >= n_clusters) {
                KMeansTree<P, T>& subtree = subtrees[i];
                subtree.setPoints(kmeans.getPartitions()[i]);
                subtree.init();
                subtree.fit();
            }
        }
    }
    cout << "Done training level " << this->depth << endl;
}

template<typename P, typename T>
const string KMeansTree<P, T>::getWord(const P &point) const {
    // recursive
    // BUT be careful not to go into subtrees not initiated !
    int cluster = kmeans.getCluster(point);
    if(depth > 0) {
        if(subtrees[cluster].isTrained()) {
            return subtrees[cluster].getWord(point);
        }
    }
    char c = (char) (((int) 'a') + cluster);
    return prefix+c;
}

template<typename P, typename T>
const bool KMeansTree<P, T>::isTrained() const {
    return kmeans.isTrained();
}
