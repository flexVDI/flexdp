#include <stdio.h>
#define FLEXVDI_PROTO_IMPL
#include "flexdp.h"

#define CHECK(cond) \
    do if (!(cond)) { \
       fprintf(stderr, "%s:%d: assertion '" #cond "' failed\n", __FUNCTION__, __LINE__); \
       return 1; \
    } while(0)

int test_FlexVDICapabilitiesMsg() {
    FlexVDICapabilitiesMsg msg = {0};
    CHECK(!supportsCapability(&msg, FLEXVDI_CAP_PRINTING));
    setCapability(&msg, FLEXVDI_CAP_PRINTING);
    CHECK(supportsCapability(&msg, FLEXVDI_CAP_PRINTING));
    resetCapability(&msg, FLEXVDI_CAP_PRINTING);
    CHECK(!supportsCapability(&msg, FLEXVDI_CAP_PRINTING));
}

int main() {
    return test_FlexVDICapabilitiesMsg();
}

