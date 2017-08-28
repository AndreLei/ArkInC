﻿#ifndef __arkmodels_h__
#define __arkmodels_h__

#include <stdio.h>

/// --------------------------------------------------
/// ENUMERATIONS
/// --------------------------------------------------

typedef enum { DEVELOPMENT, MAIN } ARKNETWORKTYPE;

typedef enum { OK, EUNAVAILABLE, ETIMEOUT } ARKPEERSTATUS;

typedef enum { SENDARK, SECONDSIGNATURE, CREATEDELEGATE, VOTE, MULTISIGNATURE } ARKTRANSACTIONTYPE;

/// --------------------------------------------------
/// STRUCTURES
/// --------------------------------------------------

typedef struct {
    char*   data;
    size_t  size;
} ArkRestResponse;

typedef struct {
    int             delay;
    int             height;
    char*           ip;
    char*           os;
    int             port;
    char*           version;
    ARKPEERSTATUS   status;
} ArkPeer;

typedef struct {
    int         length;
    ArkPeer*    data;
} ArkPeerArray;

typedef struct {
    long delegate;
    long multiSignature;
    long secondSignature;
    long send;
    long vote;
} ArkFee;

typedef struct {
    char*   id;
    int     height;
} ArkBlockHeight;

typedef struct {
    char*   address;
    double  approval;
    long    missedBlocks;
    long    producedBlocks;
    double  productivity;
    char*   publicKey;
    int     rate;
    char*   username;
    char*   vote;
} ArkDelegate;

typedef struct {
    int             length;
    ArkDelegate*    data;
} ArkDelegateArray;

typedef struct {
    char*   address;
    long    balance;
    char*   publicKey;
    char*   username;
} ArkVoter;

typedef struct {
    int         length;
    ArkVoter*   data;
} ArkVoterArray;

typedef struct {
    char*   explorer;
    char*   netHash;
    char*   symbol;
    char*   token;
    int     version;
} ArkNetwork;

typedef struct {
    double              amount;
    //                  ASSET
    char*               blockId;
    int                 confirmation;
    double              fee;
    int                 height;
    char*               id;
    char*               recipientId;
    char*               secondSenderPublicKey;
    char*               senderId;
    char*               senderPublicKey;
    char*               signature;
    char*               signSignature;
    time_t              timestamp;
    ARKTRANSACTIONTYPE  type;
    char*               vendorField;
} ArkTransaction;

typedef struct {
    int                 length;
    ArkTransaction*     data;
} ArkTransactionArray;

/// --------------------------------------------------
/// CONSTANTS
/// --------------------------------------------------

static const char* SeedArray[] =
{
    "5.39.9.240:4001",
    "5.39.9.241:4001",
    "5.39.9.242:4001",
    "5.39.9.243:4001",
    "5.39.9.244:4001",
    "5.39.9.250:4001",
    "5.39.9.251:4001",
    "5.39.9.252:4001",
    "5.39.9.253:4001",
    "5.39.9.254:4001",
    "5.39.9.255:4001",
    "5.39.53.48:4001",
    "5.39.53.49:4001",
    "5.39.53.50:4001",
    "5.39.53.51:4001",
    "5.39.53.52:4001",
    "5.39.53.53:4001",
    "5.39.53.54:4001",
    "5.39.53.55:4001",
    "37.59.129.160:4001",
    "37.59.129.161:4001",
    "37.59.129.162:4001",
    "37.59.129.163:4001",
    "37.59.129.164:4001",
    "37.59.129.165:4001",
    "37.59.129.166:4001",
    "37.59.129.167:4001",
    "37.59.129.168:4001",
    "37.59.129.169:4001",
    "37.59.129.170:4001",
    "37.59.129.171:4001",
    "37.59.129.172:4001",
    "37.59.129.173:4001",
    "37.59.129.174:4001",
    "37.59.129.175:4001",
    "193.70.72.80:4001",
    "193.70.72.81:4001",
    "193.70.72.82:4001",
    "193.70.72.83:4001",
    "193.70.72.84:4001",
    "193.70.72.85:4001",
    "193.70.72.86:4001",
    "193.70.72.87:4001",
    "193.70.72.88:4001",
    "193.70.72.89:4001",
    "193.70.72.90:4001"
};

static const char* SeedArrayTest[] =
{
    "164.8.251.179:4002",
    "164.8.251.172:4002",
    "164.8.251.91:4002",
    "167.114.43.48:4002",
    "167.114.29.49:4002",
    "167.114.43.43:4002",
    "167.114.29.54:4002",
    "167.114.29.45:4002",
    "167.114.29.40:4002",
    "167.114.29.56:4002",
    "167.114.43.35:4002",
    "167.114.29.51:4002",
    "167.114.29.59:4002",
    "167.114.43.42:4002",
    "167.114.29.34:4002",
    "167.114.29.62:4002",
    "167.114.43.49:4002",
    "167.114.29.44:4002",
    "167.114.43.37:4002",
    "167.114.29.63:4002",
    "167.114.29.42:4002",
    "167.114.29.48:4002",
    "167.114.29.61:4002",
    "167.114.43.36:4002",
    "167.114.29.57:4002",
    "167.114.43.33:4002",
    "167.114.29.52:4002",
    "167.114.29.50:4002",
    "167.114.43.47:4002",
    "167.114.29.47:4002",
    "167.114.29.36:4002",
    "167.114.29.35:4002",
    "167.114.43.39:4002",
    "167.114.43.45:4002",
    "167.114.29.46:4002",
    "167.114.29.41:4002",
    "167.114.43.34:4002",
    "167.114.29.43:4002",
    "167.114.43.41:4002",
    "167.114.29.60:4002",
    "167.114.43.32:4002",
    "167.114.29.55:4002",
    "167.114.29.53:4002",
    "167.114.29.38:4002",
    "167.114.43.40:4002",
    "167.114.29.32:4002",
    "167.114.43.46:4002",
    "167.114.43.38:4002",
    "167.114.29.33:4002",
    "167.114.43.44:4002",
    "167.114.43.50:4002",
    "167.114.29.37:4002",
    "167.114.29.58:4002",
    "167.114.29.39:4002"
};

#endif /* __arkmodels_h__ */
