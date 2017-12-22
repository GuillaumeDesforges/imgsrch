#pragma once

#include <vector>
#include <string>
using namespace std;

#include "kmeans.h"
#include "point.h"

template<typename T>
class KMeansTree {
public:
    KMeansTree(int n_clusters, int dimension, int depth, string prefix = "");

    // Add points as training set
    void addPoint(Point<T>* point);
    void addPoint(Point<T>& point);
    void addPoints(vector<Point<T>*>& data);
    void addPoints(vector<Point<T>>& data);

    // Set training set
    void setPoints(vector<Point<T>*>& data);
    void setPoints(vector<Point<T>>& data);

    // Starting point before first fit
    void init();

    // Train
    void fit();

    const string getWord(const Point<T> &point) const;
    const bool isTrained() const;
private:
    int n_clusters, depth;
    string prefix;
    KMeans<Point<T>, T> kmeans;
    vector<KMeansTree<T>> subtrees;
};

template<typename T>
KMeansTree<T>::KMeansTree(int n_clusters, int dimension, int depth, string prefix) :
    n_clusters(n_clusters),
    depth(depth),
    prefix(prefix),
    kmeans(KMeans<Point<T>, T>(n_clusters, dimension))
    {
        if(depth > 0) {
            for(int i = 0; i < n_clusters; i++) {
                char c = (char) (((int) 'a') + i);
                KMeansTree<T> subtree(n_clusters, dimension, depth-1, prefix+c);
                subtrees.push_back(subtree);
            }
        }
    }

template<typename T>
void KMeansTree<T>::addPoint(Point<T>* point) {
    kmeans.addPoint(point);
}

template<typename T>
void KMeansTree<T>::addPoint(Point<T>& point) {
    kmeans.addPoint(point);
}

template<typename T>
void KMeansTree<T>::addPoints(vector<Point<T>*>& data) {
    kmeans.addPoints(data);
}

template<typename T>
void KMeansTree<T>::addPoints(vector<Point<T>>& data) {
    kmeans.addPoints(data);
}

template<typename T>
void KMeansTree<T>::setPoints(vector<Point<T>*>& data) {
    kmeans.setPoints(data);
}

template<typename T>
void KMeansTree<T>::setPoints(vector<Point<T>>& data) {
    kmeans.setPoints(data);
}

template<typename T>
void KMeansTree<T>::init() {
    kmeans.init();
}

template<typename T>
void KMeansTree<T>::fit() {
    kmeans.fit();
    if(depth > 0) {
        for(int i = 0; i < n_clusters; i++) {
            if(kmeans.getPartitions()[i].size() >= n_clusters) {
                KMeansTree<T>& subtree = subtrees[i];
                subtree.setPoints(kmeans.getPartitions()[i]);
                subtree.init();
                subtree.fit();
            }
        }
    }

}

template<typename T>
const string KMeansTree<T>::getWord(const Point<T> &point) const {
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

template<typename T>
const bool KMeansTree<T>::isTrained() const {
    return kmeans.isTrained();
}
