// Basic types for a weighted digraph.
#pragma once

#define CHECK(x)                                                            \
    do { if (!(x)) {                                                        \
            std::cerr <<"CHECK failed: "<< #x <<"\n"                        \
                      <<" at: " << __FILE__ <<":" << __LINE__ << "\n"       \
                      << " in function: " << __func__ << "\n"               \
                      << std::flush;                                        \
            std::abort();                                                   \
        } } while (0)

#include <cassert>
#include <limits>
#include <string>
#include <iostream>

using node = uint_least32_t;        // a node index
extern node node_undefined;         // an invalid index

using edge_len = int_least32_t;     // length of an edge
using dist = int64_t;               // length of a path
extern dist dist_infinity;          // maximum value

struct edge_head {
    node dst;
    edge_len len;
    edge_head(node dst, edge_len len) : dst(dst), len(len) {}
    operator node() const { return dst; }
    node head() const { return dst; }
    edge_len length() const { return len; }
    friend std::ostream& operator<<(std::ostream & os, edge_head hd) {
        os << hd.dst << "," << hd.len;
        return os;
    }
};

struct edge : public edge_head {
    node src;
    edge(node src, edge_head hd) : edge_head(hd), src(src) {}
    edge(node src, node dst, edge_len len = 1u)
        : edge_head(dst, len), src(src) {}
    node tail() const { return src; }
    edge backward() const { return edge(dst, src, len); }
    friend std::ostream& operator<<(std::ostream& os, edge e) {
        os << "{" << e.src << ", " << e.dst << ", " << e.len << "}";
        return os;
    }
    bool operator<(const edge o) const {
        if (src != o.src) return src < o.src;
        if (dst != o.dst) return dst < o.dst;
        return len < o.len;
    }
    bool operator==(const edge o) const {
        return src == o.src && dst == o.dst && len == o.len;
    }
};
