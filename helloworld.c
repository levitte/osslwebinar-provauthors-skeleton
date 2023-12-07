#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/core.h>
#include <openssl/core_dispatch.h>
#include <openssl/crypto.h>

struct provctx_st {
    OSSL_LIB_CTX *libctx;
};

static OSSL_FUNC_provider_teardown_fn teardown;
void teardown(void *provctx)
{
    struct provctx_st *pctx = provctx;

    OSSL_LIB_CTX_free(pctx->libctx);
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
    struct provctx_st *pctx = malloc(sizeof(struct provctx_st));

    memset(pctx, 0, sizeof(*pctx));
    *provctx = pctx;
    *out = provfns;

    /* A library context that's independent of the calling application */
    pctx->libctx = OSSL_LIB_CTX_new_from_dispatch(handle, in);
    if (pctx->libctx == NULL) {
        teardown(*provctx);
        *provctx = NULL;
        return 0;
    }

    return 1;
}
