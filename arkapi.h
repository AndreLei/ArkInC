#include <stdio.h>
#include <time.h>

#include "jsonresponse.h"
#include "arkmodels.h"
#include "cJSON.h"

ARKPEERSTATUS getArkPeerStatus(const char* string);

ArkPeer ark_helpers_get_ArkPeer(const cJSON * const json);

ArkDelegate ark_helpers_get_ArkDelegate(const cJSON * const json);

ArkVoter ark_helpers_get_ArkVoter(const cJSON * const json);

ArkPeer* ark_api_get_peers(char *ip, int port);

ArkFee ark_api_get_fee(char *ip, int port);

ArkPeer ark_api_peers_get(ArkPeer peer, int port, char *ip);

char* ark_api_blocks_getEpoch(char* ip, int port);

ArkBlockHeight ark_api_blocks_getHeight(char *ip, int port);

int ark_api_blocks_getFee(char* ip, int port);

char *ark_api_blocks_getNethash(char *ip, int port);

int ark_blocks_getMilestone(char* ip, int port);

ArkDelegate* ark_api_get_delegates(char* ip, int port);

ArkDelegate ark_api_get_delegate_by_username(char* ip, int port, char* username);

ArkVoter* ark_api_get_delegate_voters(char* ip, int port, char* publicKey);
