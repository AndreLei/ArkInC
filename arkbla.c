#include "arkbla.h"

ArkTransaction ark_bla_createVote(char* secret, ArkDelegateArray delegates, int secondSecret)
{
    //var keys = crypto.getKeys(secret);

    ArkTransaction t = {0};
    t.type = VOTE;
    t.amount = 0;
    t.fee = ark_defaults_feeVote;
    t.recipientId = "";     //crypto.getAddress(keys.publicKey)
    t.senderPublicKey = ""; //keys.publicKey
    //t.timestamp = 
}
