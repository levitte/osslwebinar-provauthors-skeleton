#include <stdio.h>
#include <openssl/core.h>
#include <openssl/core_dispatch.h>

const OSSL_DISPATCH provfns[] = {
    { 0, NULL }
};

OSSL_provider_init_fn OSSL_provider_init; /* Check function signature */
int OSSL_provider_init(const OSSL_CORE_HANDLE *handle,
                       const OSSL_DISPATCH *in,
                       const OSSL_DISPATCH **out,
                       void **provctx)
{
    fprintf(stderr, "nonsense: Hello, world!\n");
    *out = provfns;
    return 1;
}
