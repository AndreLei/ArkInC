typedef enum { OK, EUNAVAILABLE, ETIMEOUT } ARKPEERSTATUS;

typedef struct {
    char* ip;
    int port;
    char* version;
    char* os;
	int height;
    ARKPEERSTATUS status;
	int delay;
} ArkPeer;

typedef struct {
    long send;
    long vote;
    long delegate;
    long secondSignature;
    long multiSignature;
} ArkFee;
