// #include <logic.h>
#include <unity.h>

void test_example(void)
{
    TEST_ASSERT_EQUAL(1, subtract(3, 2));
    TEST_ASSERT_EQUAL(0, subtract(5, 5));
}

/**
 * @brief funcion que se llama antes de cada test.
 *
 * podrias aqui setear variables.
 *
 * @param[in].
 * @param[out].
 * @return.
 */
void setUp(void)
{
}

/**
 * @brief funcion que se llama despues de cada test.
 *
 * .
 *
 * @param[in].
 * @param[out].
 * @return.
 */
void tearDown(void)
{
}

int main()
{
    // UNITY_BEGIN();
    // RUN_TEST(test_div);
    // RUN_TEST(test_mul);
    // RUN_TEST(test_subtract);
    return UNITY_END();
}