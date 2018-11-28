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

#ifndef _FLEXVDIPROTO_H_
#define _FLEXVDIPROTO_H_

#include <stdint.h>
#include <spice/macros.h>

/**
 * How to add new message types and not break old clients:
 * - ALWAYS add new message type constants at the end of the list, just
 *   above FLEXVDI_MAX_MESSAGE_TYPE. Otherwise other constant values
 *   may change. Even better, assign them a specific value. Constants
 *   MUST be consecutive, some functions expect them that way.
 * - NEVER change a message structure that has been commited to master branch.
 *   Add a new message if more information is needed or to deprecate an old one.
 **/

#ifdef WORDS_BIGENDIAN
#define BYTESWAP32(val) (uint32_t)val = SPICE_BYTESWAP32((uint32_t)val)
#else
#define BYTESWAP32(val) (void)val
#endif

SPICE_BEGIN_DECLS

typedef struct FlexVDIMessageHeader {
    uint32_t type;
    uint32_t size;
} FlexVDIMessageHeader;

#define FLEXVDI_MAX_MESSAGE_LENGTH 65536

static inline void marshallHeader(FlexVDIMessageHeader * header) {
    BYTESWAP32(header->type);
    BYTESWAP32(header->size);
}

static inline void unmarshallHeader(FlexVDIMessageHeader * header) {
    BYTESWAP32(header->type);
    BYTESWAP32(header->size);
}

int marshallMessage(uint32_t type, uint8_t * data, size_t bytes);
int unmarshallMessage(uint32_t type, uint8_t * data, size_t bytes);
size_t messageSize(uint32_t type, const uint8_t * data);

enum {
    FLEXVDI_CREDENTIALS             = 0,
    FLEXVDI_ASKCREDENTIALS          = 1,
    FLEXVDI_PRINTJOB                = 2,
    FLEXVDI_PRINTJOBDATA            = 3,
    FLEXVDI_SHAREPRINTER            = 4,
    FLEXVDI_UNSHAREPRINTER          = 5,
    FLEXVDI_RESET                   = 6,
    FLEXVDI_CAPABILITIES            = 7,
    FLEXVDI_SESSIONEVENT            = 8,
    FLEXVDI_POWEREVENT              = 9,
    FLEXVDI_FWDLISTEN               = 10,
    FLEXVDI_FWDACCEPTED             = 11,
    FLEXVDI_FWDSHUTDOWN             = 12,
    FLEXVDI_FWDCONNECT              = 13,
    FLEXVDI_FWDCLOSE                = 14,
    FLEXVDI_FWDDATA                 = 15,
    FLEXVDI_FWDACK                  = 16,
    FLEXVDI_MAX_MESSAGE_TYPE // Must be the last one
};


typedef struct FlexVDICredentialsMsg {
    uint32_t userLength;
    uint32_t passLength;
    uint32_t domainLength;
    char strings[0]; // user + '\0' + password + '\0' + domain + '\0'
} FlexVDICredentialsMsg;

static inline const char * getCredentialsUser(const FlexVDICredentialsMsg * msg) {
    return &msg->strings[0];
}
static inline const char * getCredentialsPassword(const FlexVDICredentialsMsg * msg) {
    return &msg->strings[msg->userLength + 1];
}
static inline const char * getCredentialsDomain(const FlexVDICredentialsMsg * msg) {
    return &msg->strings[msg->userLength + msg->passLength + 2];
}


typedef struct FlexVDIAskCredentialsMsg {
    uint8_t dummy; // C99: Empty structure yields undefined behavior
} FlexVDIAskCredentialsMsg;


typedef struct FlexVDIPrintJobMsg {
    uint32_t id;
    uint32_t optionsLength;
    char options[0];
} FlexVDIPrintJobMsg;


typedef struct FlexVDIPrintJobDataMsg {
    uint32_t id;
    uint32_t dataLength;
    char data[0];
} FlexVDIPrintJobDataMsg;


typedef struct FlexVDISharePrinterMsg {
    uint32_t printerNameLength;
    uint32_t ppdLength;
    char data[0]; // printerName + '\0' + PPD
} FlexVDISharePrinterMsg;

static inline const char * getPrinterName(const FlexVDISharePrinterMsg * msg) {
    return &msg->data[0];
}
static inline const char * getPPD(const FlexVDISharePrinterMsg * msg) {
    return &msg->data[msg->printerNameLength + 1];
}


typedef struct FlexVDIUnsharePrinterMsg {
    uint32_t printerNameLength;
    char printerName[0]; // printerName + '\0'
} FlexVDIUnsharePrinterMsg;


typedef struct FlexVDIResetMsg {
    uint8_t dummy;
} FlexVDIResetMsg;


enum {
    FLEXVDI_CAP_PRINTING   = 0,
    FLEXVDI_CAP_POWEREVENT = 1,
    FLEXVDI_CAP_FORWARD    = 2,
};

typedef struct FlexVDICapabilitiesMsg {
    uint32_t caps[4];
} FlexVDICapabilitiesMsg;

static inline int supportsCapability(const FlexVDICapabilitiesMsg * msg, unsigned int cap) {
    return cap < 128 && (msg->caps[cap >> 5] & (1 << (cap % 32)));
}
static inline void setCapability(FlexVDICapabilitiesMsg * msg, unsigned int cap) {
    if (cap < 128) msg->caps[cap >> 5] |= (1 << (cap % 32));
}
static inline void resetCapability(FlexVDICapabilitiesMsg * msg, unsigned int cap) {
    if (cap < 128) msg->caps[cap >> 5] &= ~(1 << (cap % 32));
}


enum {
    FLEXVDI_SESSION_LOGIN  = 0,
    FLEXVDI_SESSION_LOGOUT,
    FLEXVDI_SESSION_LAST
};

typedef struct FlexVDISessionEventMsg {
    uint32_t eventType;
    uint32_t sessionId;
} FlexVDISessionEventMsg;


enum {
    FLEXVDI_POWER_EVENT_RESET  = 0,
    FLEXVDI_POWER_EVENT_POWEROFF,
    FLEXVDI_POWER_EVENT_SHUTDOWN,
    FLEXVDI_POWER_EVENT_LAST
};

typedef struct FlexVDIPowerEventMsg {
    uint32_t event;
} FlexVDIPowerEventMsg;


enum {
    FLEXVDI_FWDPROTO_TCP  = 0,
    FLEXVDI_FWDPROTO_UDP  = 1,
    FLEXVDI_FWDPROTO_UNIX  = 2,
};

typedef struct FlexVDIForwardListenMsg {
    uint32_t id;
    uint32_t proto;
    uint32_t port;
    uint32_t addressLength;
    char address[0]; // address + '\0'
} FlexVDIForwardListenMsg;

typedef struct FlexVDIForwardAcceptedMsg {
    uint32_t listenId;
    uint32_t id;
    uint32_t winSize;
} FlexVDIForwardAcceptedMsg;

typedef struct FlexVDIForwardShutdownMsg {
    uint32_t listenId;
} FlexVDIForwardShutdownMsg;

typedef struct FlexVDIForwardConnectMsg {
    uint32_t id;
    uint32_t winSize;
    uint32_t proto;
    uint32_t port;
    uint32_t addressLength;
    char address[0]; // address + '\0'
} FlexVDIForwardConnectMsg;

typedef struct FlexVDIForwardCloseMsg {
    uint32_t id;
    uint32_t error; // Errno. 0 means no error.
} FlexVDIForwardCloseMsg;

typedef struct FlexVDIForwardDataMsg {
    uint32_t id;
    uint32_t size;
    uint8_t data[0];
} FlexVDIForwardDataMsg;

typedef struct FlexVDIForwardAckMsg {
    uint32_t id;
    uint32_t size;
    uint32_t winSize;
} FlexVDIForwardAckMsg;


#ifdef FLEXVDI_PROTO_IMPL

enum {
    OP_SIZE,
    OP_MARSHALL,
    OP_UNMARSHALL
};

#define MSG_OPERATIONS(Msg, id, extra, commands) \
case id: do { \
    Msg * msg = (Msg *)data; \
    if (op == OP_SIZE) return sizeof(*msg) + extra; \
    else { \
        if (bytes < sizeof(*msg)) return 0; \
        size_t size = 0; \
        if (op == OP_MARSHALL) size = sizeof(*msg) + extra; \
        commands \
        if (op == OP_UNMARSHALL) size = sizeof(*msg) + extra; \
        return bytes >= size; \
    } \
} while (0)

#define MSG_OPERATIONS_EMPTY(Msg, id) \
case id: return op == OP_SIZE ? sizeof(Msg) : TRUE

size_t msgOp(uint32_t type, int op, uint8_t * data, size_t bytes) {
    switch (type) {
        MSG_OPERATIONS(FlexVDICredentialsMsg, FLEXVDI_CREDENTIALS,
                       msg->userLength + msg->passLength + msg->domainLength + 3,
                       BYTESWAP32(msg->userLength);
                       BYTESWAP32(msg->passLength);
                       BYTESWAP32(msg->domainLength);
        );
        MSG_OPERATIONS_EMPTY(FlexVDIAskCredentialsMsg, FLEXVDI_ASKCREDENTIALS);
        MSG_OPERATIONS(FlexVDIPrintJobMsg, FLEXVDI_PRINTJOB,
                       msg->optionsLength,
                       BYTESWAP32(msg->id);
                       BYTESWAP32(msg->optionsLength);
        );
        MSG_OPERATIONS(FlexVDIPrintJobDataMsg, FLEXVDI_PRINTJOBDATA,
                       msg->dataLength,
                       BYTESWAP32(msg->id);
                       BYTESWAP32(msg->dataLength);
        );
        MSG_OPERATIONS(FlexVDISharePrinterMsg, FLEXVDI_SHAREPRINTER,
                       msg->printerNameLength + msg->ppdLength + 1,
                       BYTESWAP32(msg->printerNameLength);
                       BYTESWAP32(msg->ppdLength);
        );
        MSG_OPERATIONS(FlexVDIUnsharePrinterMsg, FLEXVDI_UNSHAREPRINTER,
                       msg->printerNameLength + 1,
                       BYTESWAP32(msg->printerNameLength);
        );
        MSG_OPERATIONS_EMPTY(FlexVDIResetMsg, FLEXVDI_RESET);
        MSG_OPERATIONS(FlexVDICapabilitiesMsg, FLEXVDI_CAPABILITIES, 0,
                       int i;
                       for (i = 0; i < 4; ++i)
                           BYTESWAP32(msg->caps[i]);
        );
        MSG_OPERATIONS(FlexVDISessionEventMsg, FLEXVDI_SESSIONEVENT, 0,
                       BYTESWAP32(msg->eventType);
                       BYTESWAP32(msg->sessionId);
        );
        MSG_OPERATIONS(FlexVDIPowerEventMsg, FLEXVDI_POWEREVENT, 0,
                       BYTESWAP32(msg->event);
        );
        MSG_OPERATIONS(FlexVDIForwardListenMsg, FLEXVDI_FWDLISTEN,
                       msg->addressLength + 1,
                       BYTESWAP32(msg->id);
                       BYTESWAP32(msg->proto);
                       BYTESWAP32(msg->port);
                       BYTESWAP32(msg->addressLength);
        );
        MSG_OPERATIONS(FlexVDIForwardAcceptedMsg, FLEXVDI_FWDACCEPTED, 0,
                       BYTESWAP32(msg->listenId);
                       BYTESWAP32(msg->id);
                       BYTESWAP32(msg->winSize);
        );
        MSG_OPERATIONS(FlexVDIForwardShutdownMsg, FLEXVDI_FWDSHUTDOWN, 0,
                       BYTESWAP32(msg->listenId);
        );
        MSG_OPERATIONS(FlexVDIForwardConnectMsg, FLEXVDI_FWDCONNECT,
                       msg->addressLength + 1,
                       BYTESWAP32(msg->id);
                       BYTESWAP32(msg->winSize);
                       BYTESWAP32(msg->proto);
                       BYTESWAP32(msg->port);
                       BYTESWAP32(msg->addressLength);
        );
        MSG_OPERATIONS(FlexVDIForwardCloseMsg, FLEXVDI_FWDCLOSE, 0,
                       BYTESWAP32(msg->id);
                       BYTESWAP32(msg->error);
        );
        MSG_OPERATIONS(FlexVDIForwardDataMsg, FLEXVDI_FWDDATA,
                       msg->size,
                       BYTESWAP32(msg->id);
                       BYTESWAP32(msg->size);
        );
        MSG_OPERATIONS(FlexVDIForwardAckMsg, FLEXVDI_FWDACK, 0,
                       BYTESWAP32(msg->id);
                       BYTESWAP32(msg->size);
                       BYTESWAP32(msg->winSize);
        );
        default: return 0;
    }
}


size_t messageSize(uint32_t type, const uint8_t * data) {
    return msgOp(type, OP_SIZE, (uint8_t *)data, 0);
}


int marshallMessage(uint32_t type, uint8_t * data, size_t bytes) {
    return msgOp(type, OP_MARSHALL, data, bytes);
}


int unmarshallMessage(uint32_t type, uint8_t * data, size_t bytes) {
    return msgOp(type, OP_UNMARSHALL, data, bytes);
}
#endif

SPICE_END_DECLS

#endif // _FLEXVDIPROTO_H_
