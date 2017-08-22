#include "arkapi.h"

int main()
{
    ArkPeer *peers;
    peers = ark_api_get_peers("164.8.251.179:4002/api/peers");
    printf("Found %d peers\n", sizeof(peers));

    ArkFee *fee;
    fee = ark_api_get_fee(peers[10]);

    return 0;
}
