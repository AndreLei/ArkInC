#include "arkapi.h"
#include "curl.h"
#include "cJSON.h"
#include "arkmodels.h"

#include <stdio.h>

//ARKNETWORKTYPE _networkType = DEVELOPMENT;


/* Used by some code below as an example datatype. */
struct record
{
    const char *precision;
    double lat;
    double lon;
    const char *address;
    const char *city;
    const char *state;
    const char *zip;
    const char *country;
};

int main();

static void create_objects(void);
static int print_preallocated(cJSON *root);

static int sum(int a);

