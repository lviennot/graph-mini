// Author: Laurent Viennot, Inria, 2020.

// Necessary data structures for performing a Dijkstra like graph traversal.

#pragma once

#include <queue>
#include <vector>

#include "basics.hh"
#include "digraph.hh"


class traversal {

protected:

    // For the priority queue:
    struct node_dist {
        node _node;
        dist _dist;
        node_dist(node _node, dist _dist) : _node(_node), _dist(_dist) {}
        node_dist() : _node(-1), _dist(dist_infinity) {}
        operator node() const { return _node; }
    };
    static bool node_dist_greater(node_dist a, node_dist b) {
        return b._dist < a._dist; // priority_queue::top() returns max element
    }
    
    std::vector<dist> distances;
    using queue_t = std::priority_queue <node_dist,
                                         std::vector<node_dist>,
                                   std::function<bool(node_dist, node_dist)>>;
    queue_t queue;
    std::vector<bool> visited;
    std::vector<node> visited_nodes;
    std::size_t capacity;

public:
    
    traversal() : queue(node_dist_greater), capacity(0) {}

    dist distance(node u) const { return distances[u]; }

    std::vector<dist> copy_distances() const {
        return std::vector<dist>(distances.begin(), distances.begin()+capacity);
    }
    
    void init(std::size_t n);

    void dijkstra(const digraph & g, const node src,
                  std::function<bool(node, dist)> filter
                                     = [](node v, dist d) { return true; }
                  );

};


namespace unit {

    void test_traversal();
}

