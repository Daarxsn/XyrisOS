#include <stdio.h>

int test_system(void);

int main(void)
{
    printf("=====================================\n");
    printf("       XyrisOS Test Runner\n");
    printf("=====================================\n");

    int result = 0;

    result |= test_system();

    printf("\n=====================================\n");

    if (result == 0)
    {
        printf("ALL TESTS PASSED\n");
    }
    else
    {
        printf("SOME TESTS FAILED\n");
    }

    printf("=====================================\n");

    return result;
}