#include "main.h"

int main()
{
    int arkBlockchainSetupFailed = ark_core_setEnvrionment(DEVELOPMENT);
    if (arkBlockchainSetupFailed == 0)
    {
        printf("[ARK] Unable to connect to blockchain\n");
        return 0;
    }


    int pirs = ark_api_peers_getStatus(global_selectedPeer.ip, global_selectedPeer.port);

    return 0;

    ArkPeer initPeer = ark_core_get_randomPeer();

    ArkPeerArray peers = ark_api_peers(initPeer.ip, initPeer.port);
    ArkPeer randomPeer = peers.data[0];

    /// get one Peer fees
    ArkFee fee = ark_api_fees_get(randomPeer.ip, randomPeer.port);

    /// api/peers/get TEST
    ArkPeer arkpeer = ark_api_peers_get(randomPeer, "164.8.251.179", 4002);

    /// api/blocks/getEpoch TEST
    char* epoch = ark_api_blocks_getEpoch(randomPeer.ip, randomPeer.port);

    /// api/blocks/getHeight TEST
    ArkBlockHeight arkblockheight = ark_api_blocks_getHeight(randomPeer.ip, randomPeer.port);

    /// api/blocks/getFee TEST
    int arkfee = ark_api_blocks_getFee(randomPeer.ip, randomPeer.port);

    /// api/blocks/getNethash TEST
    char* netHash = ark_api_blocks_getNethash(randomPeer.ip, randomPeer.port);

    /// api/blocks/getMilestone
    //int milestone =

    /// get Delegates from one Peer
    ArkDelegateArray delegates = ark_api_delegates(randomPeer.ip, randomPeer.port);

    ArkDelegate randomDelegate = delegates.data[0];

    /// get one Delegate by username
    ArkDelegate delegate = ark_api_delegates_getByUsername(randomPeer.ip, randomPeer.port, "darkjarunik");

    /// get Voters from one Delegate by publicKey
    ArkVoterArray voters = ark_api_voters_getByDelegate(randomPeer.ip, randomPeer.port, randomDelegate.publicKey);

    return 0;
}
