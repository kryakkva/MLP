#include <gtest/gtest.h>

#include <iostream>
#include <vector>

#include "../graph/graph.h"

const std::vector<double> answer = {0,  12, 0, 0, 0, 0, 0, 0, 20, 8, 11, 0, 0,
                                    17, 0,  0, 0, 0, 0, 0, 0, 0,  0, 0,  0};
const std::vector<double> answer_double = {0,      8.234, 5.65, 0, 0,
                                           3.2131, 0,     0,    0};
const std::vector<double> col_answer = {0, 0, 11, 0, 0};

struct GraphTest : public testing::Test {
 protected:
  s21::Graph graph;
  void SetUp() {}
  void TearDown() {}
};

TEST_F(GraphTest, export_to_dot1) {
  graph.loadGraphFromFile("../datasets/small_o_w_1.txt");
  graph.exportGraphToDot("small_o_w_1.dot");
  EXPECT_EQ(0,
            system("diff small_o_w_1.dot tests/data/small_o_w_1_answer.dot"));
  system("rm small_o_w_1.dot");
}

TEST_F(GraphTest, export_to_dot2) {
  graph.loadGraphFromFile("../datasets/o_w.txt");
  graph.exportGraphToDot("o_w.dot");
  EXPECT_EQ(0, system("diff o_w.dot tests/data/o_w_answer.dot"));
  system("rm o_w.dot");
}

TEST_F(GraphTest, loading_from_file) {
  graph.loadGraphFromFile("../datasets/small_o_w_1.txt");
  EXPECT_EQ(graph.getVerticesCount(), 5);
  auto graph_it = graph.begin();
  auto answer_it = answer.begin();
  for (int i = 0; i < graph.getVerticesCount(); i++) {
    EXPECT_EQ(*graph_it, *answer_it);
    ++graph_it;
    ++answer_it;
  }
}

TEST_F(GraphTest, loading_from_file_double) {
  graph.loadGraphFromFile("../datasets/o_w_double.txt");
  EXPECT_EQ(graph.getVerticesCount(), 3);
  auto graph_it = graph.begin();
  auto answer_it = answer_double.begin();
  for (int i = 0; i < graph.getVerticesCount(); i++) {
    EXPECT_FLOAT_EQ(*graph_it, *answer_it);
    ++graph_it;
    ++answer_it;
  }
}

TEST_F(GraphTest, broken_loading) {
  EXPECT_ANY_THROW(graph.loadGraphFromFile("../datasets/bad_files/empty.txt"));
  EXPECT_ANY_THROW(
      graph.loadGraphFromFile("../datasets/bad_files/no_matrix.txt"));
  EXPECT_ANY_THROW(
      graph.loadGraphFromFile("../datasets/bad_files/letters.txt"));
  EXPECT_ANY_THROW(
      graph.loadGraphFromFile("../datasets/bad_files/letters_2.txt"));
  EXPECT_ANY_THROW(
      graph.loadGraphFromFile("../datasets/bad_files/incorrect_matrix.txt"));
  EXPECT_ANY_THROW(
      graph.loadGraphFromFile("../datasets/bad_files/symbols.txt"));
}

// iterators

TEST_F(GraphTest, iterator_operator_plus) {
  graph.loadGraphFromFile("../datasets/small_o_w_1.txt");
  auto answer_it = answer.begin();
  for (auto it = graph.begin(); it != graph.end(); ++it) {
    EXPECT_EQ(*it, *answer_it);
    answer_it++;
  }
}

TEST_F(GraphTest, iterator_operator_minus) {
  graph.loadGraphFromFile("../datasets/small_o_w_1.txt");
  auto answer_it = answer.end() - 1;
  for (auto it = graph.end() - 1; it != graph.begin(); --it) {
    EXPECT_EQ(*it, *answer_it);
    answer_it--;
  }
}

TEST_F(GraphTest, row_iterator_operator_plus) {
  graph.loadGraphFromFile("../datasets/small_o_w_1.txt");
  auto answer_it = answer.begin();
  for (auto it = graph.rowBegin(0); it != graph.rowEnd(0); ++it) {
    EXPECT_EQ(*it, *answer_it);
    answer_it++;
  }
}

TEST_F(GraphTest, row_iterator_operator_minus) {
  graph.loadGraphFromFile("../datasets/small_o_w_1.txt");
  auto answer_it = answer.end() - 1;
  auto it = graph.rowEnd(4) - 1;
  for (; it != graph.rowBegin(4); --it) {
    EXPECT_EQ(*it, *answer_it);
    answer_it--;
  }
  EXPECT_EQ(*it, *answer_it);
}

TEST_F(GraphTest, col_iterator_operator_plus) {
  graph.loadGraphFromFile("../datasets/small_o_w_1.txt");
  auto answer_it = col_answer.begin();
  for (auto it = graph.colBegin(0); it != graph.colEnd(0); ++it) {
    EXPECT_EQ(*it, *answer_it);
    answer_it++;
  }
}

TEST_F(GraphTest, col_iterator_operator_minus) {
  graph.loadGraphFromFile("../datasets/small_o_w_1.txt");
  auto answer_it = col_answer.end() - 1;
  auto it = graph.colEnd(0) - 1;
  for (; it != graph.colBegin(0); --it) {
    EXPECT_EQ(*it, *answer_it);
    answer_it--;
  }
  EXPECT_EQ(*it, *answer_it);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
