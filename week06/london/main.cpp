#include <algorithm>
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>
    traits;
typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
                    boost::property<boost::edge_residual_capacity_t, long,
                                    boost::property<boost::edge_reverse_t,
                                                    traits::edge_descriptor>>>>
    graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
  graph &G;

public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void testcase() {
  int h, w;
  std::cin >> h >> w;

  std::string note;
  std::cin >> note;

  // one vertex per possible letter combination, one vertex per potentially
  // required letter and two for source and sink
  graph G(26 * 26 + 26 + 2);
  edge_adder adder(G);
  int source = 26 * 26 + 26;
  int sink = 26 * 26 + 27;

  std::vector<int> charCount(26);

  // count how often every character appears in the note
  for (char c : note) {
    int index = c - 'A';
    charCount[index]++;
  }

  // add edges from every character to the sink, this allows us to control the
  // flow
  for (int i = 0; i < 26; i++) {
    adder.add_edge(26 * 26 + i, sink, charCount[i]);
  }

  std::vector<std::string> front(h);
  std::vector<std::string> back(h);

  for (int i = 0; i < 2 * h; i++) {
    std::string line;
    std::cin >> line;
    if (i < h) {
      front[i] = line;
    } else {
      back[i - h] = line;
    }
  }

  // go through all appearing pairs of letters and count how often each
  // combination appears
  std::vector<int> pairCount(26 * 26);
  for (int i = 0; i < h; i++) {
    std::string line_front = front[i];
    std::string line_back = back[i];
    for (int j = 0; j < w; j++) {
      int frontIndex = line_front[j] - 'A';
      int backIndex = line_back[w - j - 1] - 'A';

      pairCount[frontIndex * 26 + backIndex]++;
    }
  }

  for (int i = 0; i < 26 * 26; i++) {
    adder.add_edge(source, i, pairCount[i]);
    int firstLetter = 26 * 26 + i / 26;
    int secondLetter = 26 * 26 + i % 26;
    adder.add_edge(i, firstLetter, pairCount[i]);
    adder.add_edge(i, secondLetter, pairCount[i]);
  }

  long flow = boost::push_relabel_max_flow(G, source, sink);

  if (flow == (int)note.length()) {
    std::cout << "Yes\n";
  } else {
    std::cout << "No\n";
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;

  for (int i = 0; i < t; i++) {
    testcase();
  }
  return 0;
}