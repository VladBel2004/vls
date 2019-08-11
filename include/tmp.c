#include "stdio.h"

int main(void)
{
#ifdef ARG
        printf( "Some\n" );
#endif
        printf( "Other\n" );
        return 0;
}
