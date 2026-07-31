#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "platform/stubs.h"
#include "UCraft.h"

static uint8_t cleanup = 0;
void app(void)
{
    UCraftStart(&cleanup);
    //failure
    while(1){
        yield_task(100);
    }
}
