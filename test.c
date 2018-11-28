/*
    Copyright (C) 2014-2018 Flexible Software Solutions S.L.U.

    This file is part of flexDP.

    flexDP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    flexDP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with flexDP. If not, see <https://www.gnu.org/licenses/>.
*/

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

