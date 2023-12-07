#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/core.h>
#include <openssl/core_dispatch.h>

struct provctx_st {
    int dummy;
};

static OSSL_FUNC_provider_teardown_fn teardown;
void teardown(void *provctx)
{
    free(provctx);
}

const OSSL_DISPATCH provfns[] = {
    { OSSL_FUNC_PROVIDER_TEARDOWN, (void (*)(void))teardown },
    { 0, NULL }
};

OSSL_provider_init_fn OSSL_provider_init; /* Check function signature */
int OSSL_provider_init(const OSSL_CORE_HANDLE *handle,
                       const OSSL_DISPATCH *in,
                       const OSSL_DISPATCH **out,
                       void **provctx)
{
    fprintf(stderr, "nonsense: Hello, world!\n");

    struct provctx_st *pctx = malloc(sizeof(struct provctx_st));

    memset(pctx, 0, sizeof(*pctx));
    *provctx = pctx;
    *out = provfns;
    return 1;
}
