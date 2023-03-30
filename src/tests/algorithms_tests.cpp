#include <gtest/gtest.h>

#include "../algorithms/graph_algorithms.h"

struct SearchAlgorithmsTest : public testing::Test {
 protected:
  s21::Graph graph;
  void SetUp() { graph.loadGraphFromFile("../datasets/small_o_nw.txt"); }
  void TearDown() {}
};

TEST_F(SearchAlgorithmsTest, dfs_1) {
  const std::vector<int> dfs_answer = {1, 3, 6, 7, 4, 2, 5};
  std::vector<int> result = s21::GraphAlgorithms::depthFirstSearch(graph, 1);
  auto answer_it = dfs_answer.begin();
  for (auto &heir : result) {
    EXPECT_EQ(*answer_it, heir);
    answer_it++;
  }
}

TEST_F(SearchAlgorithmsTest, bfs_1) {
  const std::vector<int> bfs_answer = {1, 2, 3, 5, 4, 6, 7};
  std::vector<int> result = s21::GraphAlgorithms::breadthFirstSearch(graph, 1);
  auto answer_it = bfs_answer.begin();
  for (auto &heir : result) {
    EXPECT_EQ(*answer_it, heir);
    answer_it++;
  }
}

struct SearchAlgorithmsTest2 : public testing::Test {
 protected:
  s21::Graph graph;
  void SetUp() { graph.loadGraphFromFile("../datasets/no_nw_tree.txt"); }
  void TearDown() {}
};

TEST_F(SearchAlgorithmsTest2, dfs_2) {
  const std::vector<int> dfs_answer = {1, 3, 8, 2, 5, 7, 6, 4};
  std::vector<int> result = s21::GraphAlgorithms::depthFirstSearch(graph, 1);
  auto answer_it = dfs_answer.begin();
  for (auto &heir : result) {
    EXPECT_EQ(*answer_it, heir);
    answer_it++;
  }
}

TEST_F(SearchAlgorithmsTest2, bfs_2) {
  const std::vector<int> bfs_answer = {1, 2, 3, 4, 5, 8, 6, 7};
  std::vector<int> result = s21::GraphAlgorithms::breadthFirstSearch(graph, 1);
  auto answer_it = bfs_answer.begin();
  for (auto &heir : result) {
    EXPECT_EQ(*answer_it, heir);
    answer_it++;
  }
}

struct PathAlgorithmsTest : public testing::Test {
 protected:
  s21::Graph graph;
  void SetUp() { graph.loadGraphFromFile("../datasets/small_o_w_2.txt"); }
  void TearDown() {}
};

TEST_F(PathAlgorithmsTest, get_shortest_path_between_two_1) {
  double result =
      s21::GraphAlgorithms::getShortestPathBetweenVertices(graph, 1, 2);
  EXPECT_FLOAT_EQ(result, 10);
}

TEST_F(PathAlgorithmsTest, get_shortest_path_between_two_2) {
  double result =
      s21::GraphAlgorithms::getShortestPathBetweenVertices(graph, 1, 7);
  EXPECT_FLOAT_EQ(result, 30);
}

TEST_F(PathAlgorithmsTest, get_shortest_path_between_two_3) {
  double result =
      s21::GraphAlgorithms::getShortestPathBetweenVertices(graph, 6, 4);
  EXPECT_FLOAT_EQ(result, 12);
}

TEST_F(PathAlgorithmsTest, get_shortest_path_between_two_4) {
  double result =
      s21::GraphAlgorithms::getShortestPathBetweenVertices(graph, 1, 1);
  EXPECT_FLOAT_EQ(result, 0);
}

TEST_F(PathAlgorithmsTest, get_shortest_path_between_two_5) {
  EXPECT_ANY_THROW(
      s21::GraphAlgorithms::getShortestPathBetweenVertices(graph, 2, 3));
  EXPECT_ANY_THROW(
      s21::GraphAlgorithms::getShortestPathBetweenVertices(graph, 3, 1));
  EXPECT_ANY_THROW(
      s21::GraphAlgorithms::getShortestPathBetweenVertices(graph, 7, 3));
}

struct AllAlgorithmsTest : public testing::Test {
 protected:
  s21::Graph graph;
  void SetUp() {}
  void TearDown() {}
};

TEST_F(AllAlgorithmsTest, spanning_tree_search_1) {
  const std::vector<double> answer = {
      0, 20, 0, 0, 0, 0, 30, 0, 0, 0, 0, 12, 0, 0, 0, 0,
  };
  graph.loadGraphFromFile("../datasets/complete.txt");
  s21::Matrix<double> result =
      s21::GraphAlgorithms::getLeastSpanningTree(graph);
  s21::Graph result_graph;
  result_graph.matrixToGraph(result);
  auto answer_it = answer.begin();
  for (auto &weight : result_graph) {
    EXPECT_FLOAT_EQ(*answer_it, weight);
    answer_it++;
  }
}

TEST_F(AllAlgorithmsTest, spanning_tree_search_2) {
  const std::vector<double> answer = {
      0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0,
      0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  graph.loadGraphFromFile("../datasets/no_w.txt");
  s21::Matrix<double> result =
      s21::GraphAlgorithms::getLeastSpanningTree(graph);
  s21::Graph result_graph;
  result_graph.matrixToGraph(result);
  auto answer_it = answer.begin();
  for (auto &weight : result_graph) {
    EXPECT_FLOAT_EQ(*answer_it, weight);
    answer_it++;
  }
}

TEST_F(AllAlgorithmsTest, spanning_tree_search_3) {
  const std::vector<double> answer = {
      0, 10, 20, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0,
      3, 0,  9,  0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0,
      0, 0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  };
  graph.loadGraphFromFile("../datasets/small_o_w_2.txt");
  s21::Matrix<double> result =
      s21::GraphAlgorithms::getLeastSpanningTree(graph);
  s21::Graph result_graph;
  result_graph.matrixToGraph(result);
  auto answer_it = answer.begin();
  for (auto &weight : result_graph) {
    EXPECT_FLOAT_EQ(*answer_it, weight);
    answer_it++;
  }
}

TEST_F(AllAlgorithmsTest, simple_ant_algorithm_1) {
  graph.loadGraphFromFile("../datasets/complete.txt");
  const std::vector<int> answer = {1, 2, 3, 4, 1};
  auto answer_it = answer.begin();
  s21::TsmResult result =
      s21::GraphAlgorithms::solveTravelingSalesmanProblem(graph);
  for (auto &vertice : result.vertices) {
    EXPECT_EQ(*answer_it, vertice);
    answer_it++;
  }
  EXPECT_FLOAT_EQ(result.distance, 97.0);
}

TEST_F(AllAlgorithmsTest, simple_ant_algorithm_2) {
  graph.loadGraphFromFile("../datasets/complete_2.txt");
  const std::vector<int> answer = {1, 2, 3, 4, 5, 1};
  auto answer_it = answer.begin();
  s21::TsmResult result =
      s21::GraphAlgorithms::solveTravelingSalesmanProblem(graph);
  for (auto &vertice : result.vertices) {
    EXPECT_EQ(*answer_it, vertice);
    answer_it++;
  }
  EXPECT_FLOAT_EQ(result.distance, 40.0);
}

TEST_F(AllAlgorithmsTest, simple_ant_algorithm_fail) {
  graph.loadGraphFromFile("../datasets/ncomplete.txt");
  EXPECT_ANY_THROW(s21::GraphAlgorithms::solveTravelingSalesmanProblem(graph));
}

TEST_F(AllAlgorithmsTest, branch_and_bound_algorithm_test_1) {
  graph.loadGraphFromFile("../datasets/complete.txt");
  const std::vector<int> answer = {1, 2, 3, 4, 1};
  auto answer_it = answer.begin();
  s21::TsmResult result;
  result = s21::GraphAlgorithms::solveTravelingSalesmanProblemBB(graph);
  for (auto &vertice : result.vertices) {
    EXPECT_EQ(*answer_it, vertice);
    answer_it++;
  }
  EXPECT_FLOAT_EQ(result.distance, 97.0);
}

TEST_F(AllAlgorithmsTest, branch_and_bound_algorithm_test_2) {
  graph.loadGraphFromFile("../datasets/complete_2.txt");
  const std::vector<int> answer = {1, 2, 3, 4, 5, 1};
  auto answer_it = answer.begin();
  s21::TsmResult result =
      s21::GraphAlgorithms::solveTravelingSalesmanProblemBB(graph);
  for (auto &vertice : result.vertices) {
    EXPECT_EQ(*answer_it, vertice);
    answer_it++;
  }
  EXPECT_FLOAT_EQ(result.distance, 40.0);
}

TEST_F(AllAlgorithmsTest, brute_force_algorithm_test_1) {
  graph.loadGraphFromFile("../datasets/complete.txt");
  const std::vector<int> answer = {1, 2, 3, 4, 1};
  auto answer_it = answer.begin();
  s21::TsmResult result;
  result = s21::GraphAlgorithms::solveTravelingSalesmanProblemBF(graph);
  for (auto &vertice : result.vertices) {
    EXPECT_EQ(*answer_it, vertice);
    answer_it++;
  }
  EXPECT_FLOAT_EQ(result.distance, 97.0);
}

TEST_F(AllAlgorithmsTest, brute_force_algorithm_test_2) {
  graph.loadGraphFromFile("../datasets/complete_2.txt");
  const std::vector<int> answer = {1, 2, 3, 4, 5, 1};
  auto answer_it = answer.begin();
  s21::TsmResult result =
      s21::GraphAlgorithms::solveTravelingSalesmanProblemBF(graph);
  for (auto &vertice : result.vertices) {
    EXPECT_EQ(*answer_it, vertice);
    answer_it++;
  }
  EXPECT_FLOAT_EQ(result.distance, 40.0);
}

TEST_F(AllAlgorithmsTest, path_between_all_vertices_test_1) {
  graph.loadGraphFromFile("../datasets/no_w.txt");
  const std::vector<double> answer = {
      0.00,  4.00,  3.00,  8.00,  10.00, 17.00, 17.00, 19.00, 22.00, 18.00,
      4.00,  0.00,  1.00,  4.00,  10.00, 13.00, 13.00, 15.00, 18.00, 18.00,
      3.00,  1.00,  0.00,  5.00,  9.00,  14.00, 14.00, 16.00, 19.00, 17.00,
      8.00,  4.00,  5.00,  0.00,  7.00,  9.00,  9.00,  11.00, 14.00, 15.00,
      10.00, 10.00, 9.00,  7.00,  0.00,  8.00,  10.00, 9.00,  12.00, 8.00,
      17.00, 13.00, 14.00, 9.00,  8.00,  0.00,  2.00,  2.00,  5.00,  11.00,
      17.00, 13.00, 14.00, 9.00,  10.00, 2.00,  0.00,  4.00,  6.00,  13.00,
      19.00, 15.00, 16.00, 11.00, 9.00,  2.00,  4.00,  0.00,  3.00,  9.00,
      22.00, 18.00, 19.00, 14.00, 12.00, 5.00,  6.00,  3.00,  0.00,  9.00,
      18.00, 18.00, 17.00, 15.00, 8.00,  11.00, 13.00, 9.00,  9.00,  0.00};
  s21::Matrix<double> result =
      s21::GraphAlgorithms::getShortestPathsBetweenAllVertices(graph);
  auto answer_it = answer.begin();
  for (int i = 0; i < result.GetRows(); i++) {
    for (int j = 0; j < result.GetCols(); j++) {
      EXPECT_DOUBLE_EQ(*answer_it, result.at(i, j));
      ++answer_it;
    }
  }
}

TEST_F(AllAlgorithmsTest, path_between_all_vertices_test_2) {
  graph.loadGraphFromFile("../datasets/o_w.txt");
  const std::vector<double> answer = {
      0.00,  29.00, 20.00, 20.00, 16.00,  31.00, 100.00, 12.00, 4.00,  31.00,
      18.00, 29.00, 0.00,  15.00, 29.00,  28.00, 40.00,  72.00, 21.00, 29.00,
      41.00, 12.00, 20.00, 15.00, 0.00,   15.00, 14.00,  25.00, 81.00, 9.00,
      23.00, 27.00, 13.00, 20.00, 29.00,  15.00, 0.00,   4.00,  12.00, 92.00,
      12.00, 24.00, 13.00, 25.00, 16.00,  28.00, 14.00,  4.00,  0.00,  16.00,
      94.00, 9.00,  20.00, 16.00, 22.00,  31.00, 40.00,  25.00, 12.00, 16.00,
      0.00,  95.00, 24.00, 35.00, 3.00,   37.00, 100.00, 72.00, 81.00, 92.00,
      94.00, 95.00, 0.00,  90.00, 101.00, 98.00, 84.00,  12.00, 21.00, 9.00,
      12.00, 9.00,  24.00, 90.00, 0.00,   15.00, 25.00,  13.00, 4.00,  29.00,
      23.00, 24.00, 20.00, 35.00, 101.00, 15.00, 0.00,   35.00, 18.00, 31.00,
      41.00, 27.00, 13.00, 16.00, 3.00,   98.00, 25.00,  35.00, 0.00,  38.00,
      18.00, 12.00, 13.00, 25.00, 22.00,  37.00, 84.00,  13.00, 18.00, 38.00,
      0.00};
  s21::Matrix<double> result =
      s21::GraphAlgorithms::getShortestPathsBetweenAllVertices(graph);
  auto answer_it = answer.begin();
  for (int i = 0; i < result.GetRows(); i++) {
    for (int j = 0; j < result.GetCols(); j++) {
      EXPECT_DOUBLE_EQ(*answer_it, result.at(i, j));
      ++answer_it;
    }
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
