#include "rq_ut_helper.hpp"


void performance_test()
{

    // n*n matrices, 50 means 50*50 total objects
    //std::vector<int> magnitudes = {50, 100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600, 51200};
    std::vector<int> magnitudes = {50, 100, 200, 400, 800, 1600, 3200, 6400, 12800};
    //std::vector<int> magnitudes = {50, 150, 200, 250};
    //std::vector<int> magnitudes = {50, 100, 200, 400};
    //std::vector<int> magnitudes = {10, 100, 1000, 10000, 100000, 1000000};
    
    int repeat_factor = 1;
    do_perf_test(repeat_factor,magnitudes);
}

void run_validations()
{
    //v1
    validate_rq(0, 0, 10, 10);
    //v2
    validate_rq(0, 0, 500, 500);
    //v3
    validate_rq(0, 0, 1000, 1000);
    //v4
    validate_rq(88, 88, 101, 103);
    //v5
    validate_rq(400, 441, 100, 203);
    //v6
    validate_rq(123, 432, 10, 509);
    //v7
    validate_rq(123, 432, 99, 10);
    //v8
    validate_rq(111, 222, 333, 444);
    //v9
    validate_rq(111, 222, 33, 44);
    //v10
    validate_rq(500, 500, 900, 10);
    //v11
    validate_rq(500, 500, 10, 700);
    //v12
    validate_rq(100, 100, 200, 50);
    //v13
    validate_rq(100, 100, 50, 200);
    //v14
    validate_rq(400, 400, 500, 200);
    //v14
    validate_rq(400, 400, 200, 500);
    //v15
    validate_rq(0, 0, 492, 492);
    //v16
    validate_rq(432, 312, 122, 124);
}

void test1() 
{
    init();
    insert_nxn_matrix_of_objs<Rectangle>(10);
    run_validations();
    fini();
}

void test2() 
{
    init();
    insert_nxn_matrix_of_objs<Ellipse>(10);
    run_validations();    
    fini();
}

void test3() 
{
    init();
    insert_nxn_matrix_of_objs<Line>(10);
    run_validations();    
    fini();
}

void test4() 
{
    init();
    insert_nxn_matrix_of_objs<Polygon>(10);
    run_validations();
    fini();
}

void test5() 
{
    init();
    insert_nxn_matrix_of_objs<Rectangle>(10);
    insert_nxn_matrix_of_objs<Ellipse>(10);
    insert_nxn_matrix_of_objs<Polygon>(10);
    run_validations();
    fini();
}

void test6() 
{
    init();
    insert_nxn_matrix_of_objs<Rectangle>(10);
    insert_nxn_matrix_of_objs<Line>(10);
    insert_nxn_matrix_of_objs<Ellipse>(10);
    insert_nxn_matrix_of_objs<Polygon>(10);
    run_validations();
    fini();
}

int main2(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    performance_test();

    return 0;
}

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    
    //use following functions to debug
    //  skip_all_tests_except(test_idx,validation_idy);
    //or
    //  skip_tests(test_idx,validation_idy);
    //can be used multiple times.

    test1();
    test2();
    //test3();
    test4();
    test5();
    //test6();

    print_results();
    
    return 0;
}

