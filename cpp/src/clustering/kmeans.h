#pragma once

#include <vector>
#include <map>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

template<typename P, typename T>
class KMeans {
public:
    // Constructor
    KMeans(int n_clusters, int dimension);
    KMeans(string s);

    // Get closest cluster
    int getCluster(const P &point) const;

    // Add points to kmeans (no fit)
    void addPoint(P *point);
    void addPoint(P &point);
    void addPoints(vector<P*> &points);
    void addPoints(vector<P> &points);

    // Define training set (no fit)
    void setPoints(vector<P*> &points);
    void setPoints(vector<P> &points);

    // Starting point before first fit
    void init();

    // Train
    void fit();

    // Get means of clusters
    const vector<P> getMeans() const;

    // Get training clusters
    vector<vector<P*>> getPartitions() const;

    // Serialize
    string serialize();

    // Get if cluster was trained
    const bool isTrained() const;
private:
    int n_clusters, dimension;
    vector<P*> data;
    vector<P> means;
    vector<vector<P*>> partitions;
    bool initiated = false, trained = false;

    bool update();
    void computeMeans();
};

template<typename P, typename T>
KMeans<P, T>::KMeans(int n_clusters, int dimension) {
    this->n_clusters = n_clusters;
    this->dimension = dimension;
    this->partitions = vector<vector<P*>>(n_clusters);
    this->means = vector<P>();
    for(int i = 0; i < n_clusters; i++) {
        means.push_back(P(dimension));
    }
}

template<typename P, typename T>
KMeans<P, T>::KMeans(string s) {
    stringstream ss(s);
    string s0;
    getline(ss, s0);
    stringstream ss0(s0);
    ss0 >> n_clusters >> dimension;
    means = vector<P>();
    string s_i;
    for(int i_cluster = 0; i_cluster < n_clusters; i_cluster++) {
        getline(ss, s_i);
        P p_i(s_i);
        assert(p_i.getDimension() == dimension);
        means.push_back(p_i);
    }
    initiated = true;
    trained = true;
}

template<typename P, typename T>
void KMeans<P, T>::addPoint(P *point) {
    data.push_back(point);
}


template<typename P, typename T>
void KMeans<P, T>::addPoint(P &point) {
    addPoint(&point);
}

template<typename P, typename T>
void KMeans<P, T>::addPoints(vector<P*> &points) {
    for(int k = 0; k < points.size(); k++) {
        P* p = points[k];
        addPoint(p);
    }
}

template<typename P, typename T>
void KMeans<P, T>::addPoints(vector<P> &points) {
    for(int k = 0; k < points.size(); k++) {
        P &p = points[k];
        addPoint(p);
    }
}

template<typename P, typename T>
void KMeans<P, T>::setPoints(vector<P*> &points) {
    data.clear();
    addPoints(points);
}

template<typename P, typename T>
void KMeans<P, T>::setPoints(vector<P> &points) {
    data.clear();
    addPoints(points);
}

template<typename P, typename T>
int KMeans<P, T>::getCluster(const P &point) const {
    assert(initiated);
    // Compute distances between points and clusters
    vector<T> distances(n_clusters);
    for(int k = 0; k < n_clusters; k++) {
        distances[k] = (point - means[k]).norm2();
    }
    // Get the index of the closest cluster
    int best_cluster = min_element(distances.begin(), distances.end()) - distances.begin();
    assert(0 <= best_cluster && best_cluster < n_clusters);
    return best_cluster;
}

template<typename P, typename T>
void KMeans<P, T>::init() {
    assert(data.size() >= n_clusters);
    // Select random points
    // TODO improve kmeans init to select relevant starting points
    vector<int> indexes(data.size());
    for(int k = 0; k < data.size(); k++) {
        indexes[k] = k;
    }
    std::random_device rd;
    std::mt19937 rng(rd());
    shuffle(indexes.begin(), indexes.end(), rng);
    for(int k = 0; k < n_clusters; k++) {
        P p(dimension);
        p = *(data[indexes[k]]);
        means[k] = p;
    }
    assert(means.size() == n_clusters);
    initiated = true;
}

template<typename P, typename T>
bool KMeans<P, T>::update() {
    // Store points movement between clusters
    map<P*, int> movement_buffer;
    for(int i = 0; i < n_clusters; i++) {
        vector<P*> &partition = partitions[i];
        for(int k = 0; k < partition.size(); k++) {
            P &point = *partition[k];
            int cluster = getCluster(point);
            if(cluster != i) {
                movement_buffer.emplace(&point, cluster);
                partition.erase(partition.begin() + k);
                k--;
                continue;
            }
        }
    }
    // If no mouvements, it is a stable clustering
    if(movement_buffer.empty()) {
        return false;
    }
    // Else, move points
    for(auto it = movement_buffer.begin(); it != movement_buffer.end(); it++) {
        partitions[it->second].push_back(it->first);
    }
    // Compute means
    computeMeans();
    return true;
}

template<typename P, typename T>
void KMeans<P, T>::fit() {
    assert(initiated);
    // Init clusters
    for(int k = 0; k < data.size(); k++) {
        int cluster = getCluster(*data[k]);
        partitions[cluster].push_back(data[k]);
    }
    // Compute first repartition
    computeMeans();
    bool changed = true;
    int iteration = 0;
    while (changed) {
        // cout << "Iteration " << iteration << endl;
        changed = update();
        iteration++;
    }
    trained = true;
}

template<typename P, typename T>
const vector<P> KMeans<P, T>::getMeans() const {
    return means;
}

template<typename P, typename T>
vector<vector<P*>> KMeans<P, T>::getPartitions() const {
    return partitions;
}

template<typename P, typename T>
void KMeans<P, T>::computeMeans() {
    for(int i = 0; i < n_clusters; i++) {
        P p(dimension);
        means[i] = p;
        for(int k = 0; k < partitions[i].size(); k++) {
            means[i] += *(partitions[i])[k];
        }
        means[i] /= partitions[i].size();
    }
}

template<typename P, typename T>
string KMeans<P, T>::serialize() {
    stringstream ss;
    ss << n_clusters << " " << dimension << endl;
    for(auto& mean : means) {
        ss << mean << endl;
    }
    string o = ss.str();
    return o;
}

template<typename P, typename T>
const bool KMeans<P, T>::isTrained() const {
    return trained;
}
