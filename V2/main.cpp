#include "test.cpp"

using namespace std;

int main (int argc, char **argv) {
    parser_init ();

    

    option_divider_test::TEST_divider ();

    noun_test::TEST_case ();
    noun_test::TEST_gender ();
    noun_test::TEST_plurality ();
    noun_test::TEST_definiteness ();
    noun_test::TEST_polarity ();
    noun_test::TEST_derivation ();
    noun_test::TEST_multiple_options ();
    
    return 0;
}
