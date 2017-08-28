#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "jsonresponse.h"
#include "arkmodels.h"
#include "cJSON.h"

ARKPEERSTATUS ark_helpers_getArkPeerStatus_fromString(const char* string);

ARKTRANSACTIONTYPE ark_helpers_getArkTransactionType_fromString(const char* string);

ArkPeer ark_helpers_getArkPeer_fromJSON(const cJSON * const json);

ArkDelegate ark_helpers_getArkDelegate_fromJSON(const cJSON * const json);

ArkVoter ark_helpers_getArkVoter_fromJSON(const cJSON * const json);

ArkTransaction ark_helpers_getArkTransaction_fromJSON(const cJSON * const json);

int ark_helpers_isNetworkNull(ArkNetwork network);

int ark_helpers_isPeerNull(ArkPeer peer);

int ark_helpers_isFeeNull(ArkFee fee);

int ark_helpers_isResponseSuccess(const cJSON * const json);
