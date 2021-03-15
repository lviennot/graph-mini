// Author: Laurent Viennot, Inria, 2020.

#include "traversal.hh"

void traversal::init(std::size_t n) {
    // TODO: find the best thresholds for prefering linear fill
    // to sparse reinitialization.
        
    // Queue has penalty in scanning elements but some cache locality:
    const std::size_t n_last = visited_nodes.size() + 2 * queue.size();
    if (n_last > capacity / 10) {
        std::fill(distances.begin(), distances.end(), dist_infinity);
        std::fill(visited.begin(), visited.end(), false);
        queue = queue_t(node_dist_greater);
    } else {
        for(node u : visited_nodes) {
            distances[u] = dist_infinity;
            visited[u] = false;
        }
        for ( ; ! queue.empty() ; queue.pop()) {
            const node u = queue.top();
            distances[u] = dist_infinity;
            visited[u] = false;
        }
    }
    visited_nodes.clear();

    if (n > distances.size()) {
        distances.resize(n, dist_infinity);
        visited.resize(n, false);
    }
    capacity = n;
}

void traversal::dijkstra(const digraph & g, const node src,
              std::function<bool(node, dist)> filter) {
    init(g.nb_nodes());
    distances[src] = 0;
    queue.push(node_dist(src, 0));

    while ( ! queue.empty()) {
        node_dist ud = queue.top();
        queue.pop();
        node u = ud._node;
        if ( ! visited[u] ) {
            dist du = ud._dist;
            assert(du == distances[u]);
            visited[u] = true;
            visited_nodes.push_back(u);
            for (auto e : g.out_neighbors(u)) {
                node v = e.head();
                dist dv = du + dist(e.length());
                if (filter(v, dv) && dv < distances[v]) {
                    distances[v] = dv;
                    queue.push(node_dist(v, dv));
                }
            }
        }
    }
}


namespace unit {

    void test_traversal() {

        traversal trav;

        // test small graph

        digraph fwd = dg_small_ids;
        digraph bwd = fwd.reverse();
        
        for (node u : fwd.nodes()) {
            trav.dijkstra(fwd, u);
            std::vector<dist> u_dist = trav.copy_distances();
            for (node v : bwd.nodes()) {
                trav.dijkstra(bwd, v);
                dist d = trav.distance(u);
                std::cout << u <<" "<< v <<" "<< d <<"\n";
                CHECK(d == u_dist[v]);
            }
        }

    }
}


