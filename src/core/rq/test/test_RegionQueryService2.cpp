#include "rq_ut_helper.hpp"


void performance_test()
{

    // n*n matrices, 50 means 50*50 total objects
    //std::vector<int> magnitudes = {50, 100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600, 51200};
    std::vector<int> magnitudes = {50, 100, 200, 400, 800, 1600, 3200, 6400, 12800};
    //std::vector<int> magnitudes = {50, 150, 200, 250};
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
    insert_nxn_matrix_of_objs<Line>(10);
    insert_nxn_matrix_of_objs<Ellipse>(10);
    insert_nxn_matrix_of_objs<Polygon>(10);
    run_validations();
    fini();
}

/*
int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    performance_test();

    return 0;

}
*/

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    // do NOT remove, ws query not working properly for those cases 
    /*
    skip_test(2,1);
    skip_test(3,5);
    skip_test(4,5);

    // remove after rq fix 
    //*
    skip_test(1,1);
    skip_test(3,1);
    skip_test(3,4);
    skip_test(3,6);
    skip_test(3,7);
    skip_test(4,1);
    skip_test(4,4);
    skip_test(4,6);
    skip_test(4,7);
    /**/

    //use to debug
    //skip_all_tests_except(test_id1,validation_id2);
    //...
    //skip_all_tests_except(test_idx,validation_idy);

    /*
    test1();
    test2();
    test3();
    test4();
    test5();

    print_results();
    */

    return 0;
}

