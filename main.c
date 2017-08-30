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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "btc/ecc_key.h"
#include "btc/utils.h"

void test_eckey()
{
    btc_key key;
    btc_privkey_init(&key);
    assert(btc_privkey_is_valid(&key) == 0);
    btc_privkey_gen(&key);
    assert(btc_privkey_is_valid(&key) == 1);

    btc_pubkey pubkey;
    btc_pubkey_init(&pubkey);
    assert(btc_pubkey_is_valid(&pubkey) == 0);
    btc_pubkey_from_key(&key, &pubkey);
    assert(btc_pubkey_is_valid(&pubkey) == 1);

    assert(btc_privkey_verify_pubkey(&key, &pubkey) == 1);

    unsigned int i;
    for (i = 33; i < BTC_ECKEY_UNCOMPRESSED_LENGTH; i++)
        assert(pubkey.pubkey[i] == 0);

    uint8_t* hash = utils_hex_to_uint8((const char*)"26db47a48a10b9b0b697b793f5c0231aa35fe192c9d063d7b03a55e3c302850a");
    unsigned char sig[74];
    size_t outlen = 74;
    btc_key_sign_hash(&key, hash, sig, &outlen);

    unsigned char sigcmp[64];
    size_t outlencmp = 64;
    btc_key_sign_hash_compact(&key, hash, sigcmp, &outlencmp);

    unsigned char sigcmp_rec[64];
    size_t outlencmp_rec = 64;
    int recid;
    btc_pubkey pubkey_rec;
    btc_pubkey_init(&pubkey_rec);
    btc_key_sign_hash_compact_recoverable(&key, hash, sigcmp_rec, &outlencmp_rec, &recid);
    btc_key_sign_recover_pubkey(sigcmp_rec, hash, recid, &pubkey_rec);
    u_assert_int_eq(btc_pubkey_verify_sig(&pubkey, hash, sig, outlen), true);
    u_assert_int_eq(btc_pubkey_verify_sig(&pubkey, hash, sig, outlen), true);
    int test = sizeof(pubkey.pubkey);
    u_assert_mem_eq(pubkey.pubkey, pubkey_rec.pubkey, sizeof(pubkey.pubkey));

/*
    const size_t size = 66;
    char str[size];
    int r = btc_pubkey_get_hex(&pubkey, str, &size);
    u_assert_int_eq(r, true);
    u_assert_int_eq(size, 66);

    const size_t size2 = 50;
    r = btc_pubkey_get_hex(&pubkey, str, &size2);
    u_assert_int_eq(r, false);
    btc_privkey_cleanse(&key);
    btc_pubkey_cleanse(&pubkey);*/
}
