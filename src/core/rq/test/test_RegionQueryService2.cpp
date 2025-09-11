#include "rq_ut_helper.hpp"

void performance_test1() {

  // n*n matrices, 50 means 50*50 total objects
  // std::vector<int> magnitudes = {50, 100, 200, 400, 800, 1600, 3200, 6400,
  // 12800, 25600, 51200};
  std::vector<int> magnitudes = {50,   100,  200,  400,  800,
                                 1600, 3200, 6400, 12800};
  // std::vector<int> magnitudes = {10, 20, 40, 80, 160, 320, 640, 1280, 2560,
  // 5120, 10240}; std::vector<int> magnitudes = {10, 20, 40, 80, 160, 320, 640,
  // 1280, 2560, 5120}; std::vector<int> magnitudes = {10, 20, 40, 80, 160, 320,
  // 640, 1280}; std::vector<int> magnitudes = {50, 150, 200, 250};
  // std::vector<int> magnitudes = {50, 100, 200, 400};
  // std::vector<int> magnitudes = {10, 100, 1000, 10000, 100000, 1000000};

  int repeat_factor = 5;
  bool squares = true;
  do_rq_perf_test(repeat_factor, magnitudes, squares);
}

void performance_test2() {

  std::vector<int> magnitudes;
  int max = 150000000;
  int step = 1000000;
  int seed = 0;
  for (int i = 1; i < 250000; i++) {
    int k = seed + step * i;
    if (k >= max)
      break;
    // std::cout << i << " will insert " << k << " objects" << std::endl;
    magnitudes.push_back(k);
  }
  // magnitudes.push_back(100000000);
  int repeat_factor = 7;
  do_rq_perf_test(repeat_factor, magnitudes);
}

void run_validations() {
  // v1
  validate_rq(0, 0, 10, 10);
  // v2
  validate_rq(0, 0, 500, 500);
  // v3
  validate_rq(0, 0, 1000, 1000);
  // v4
  validate_rq(88, 88, 101, 103);
  // v5
  validate_rq(400, 441, 100, 203);
  // v6
  validate_rq(123, 432, 10, 509);
  // v7
  validate_rq(123, 432, 99, 10);
  // v8
  validate_rq(111, 222, 333, 444);
  // v9
  validate_rq(111, 222, 33, 44);
  // v10
  validate_rq(500, 500, 900, 10);
  // v11
  validate_rq(500, 500, 10, 700);
  // v12
  validate_rq(100, 100, 200, 50);
  // v13
  validate_rq(100, 100, 50, 200);
  // v14
  validate_rq(400, 400, 500, 200);
  // v14
  validate_rq(400, 400, 200, 500);
  // v15
  validate_rq(0, 0, 492, 492);
  // v16
  validate_rq(432, 312, 122, 124);
}

void test1() {
  init();
  insert_nxn_matrix_of_objs<Rectangle>(10);
  run_validations();
  fini();
}

void test2() {
  init();
  insert_nxn_matrix_of_objs<Ellipse>(10);
  run_validations();
  fini();
}

void test3() {
  init();
  insert_nxn_matrix_of_objs<Line>(10);
  run_validations();
  fini();
}

void test4() {
  init();
  insert_nxn_matrix_of_objs<Polygon>(10);
  run_validations();
  fini();
}

void test5() {
  init();
  insert_nxn_matrix_of_objs<Rectangle>(10);
  insert_nxn_matrix_of_objs<Ellipse>(10);
  insert_nxn_matrix_of_objs<Polygon>(10);
  run_validations();
  fini();
}

void test6() {
  init();
  insert_nxn_matrix_of_objs<Rectangle>(10);
  insert_nxn_matrix_of_objs<Line>(10);
  insert_nxn_matrix_of_objs<Ellipse>(10);
  insert_nxn_matrix_of_objs<Polygon>(10);
  run_validations();
  fini();
}

int main_perf() {
  performance_test2();
  return 0;
}

int main(int argc, char **argv) {
  QGuiApplication app(argc, argv);

  // use following functions to debug
  //
  //   skip_all_tests_except(test_idx,validation_idy);
  // or
  //   skip_tests(test_idx,validation_idy);
  //
  // both functions can be used multiple times.

  test1();
  test2();
  // test3();
  test4();
  test5();
  // test6();

  print_results();

  return 0;
}
