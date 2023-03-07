#include <stdio.h>

#define DEBUG

#ifdef DEBUG
#define debug_printf printf
#else
#define debug_printf 1 ? (void)0 : printf
#endif

int fffffffff()
{
    return 1;
}

int main(void)
{
    int i = fffffffff();
    debug_printf("%d", 3);

    return i;
}