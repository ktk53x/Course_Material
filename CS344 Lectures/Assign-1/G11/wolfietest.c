#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) 
{
void* buf= (char *)malloc(365);
wolfie(buf,365);
printf(1, "%s\n", buf);
    exit();
}
