﻿#include <stdio.h>
#include <time.h>

#include "jsonresponse.h"
#include "arkmodels.h"
#include "cJSON.h"

ARKPEERSTATUS getArkPeerStatus(const char* string);

ArkPeer* ark_api_get_peers(const char *url);

ArkFee ark_api_get_fee(ArkPeer peer);

ArkPeer ark_api_peers_get(ArkPeer peer, int port, char *ip);

char* ark_api_blocks_getEpoch(ArkPeer peer);

ArkBlockHeight ark_api_blocks_getHeight(ArkPeer peer);

int ark_api_blocks_getFee(ArkPeer peer);

char *ark_api_blocks_getNethash(ArkPeer peer);


