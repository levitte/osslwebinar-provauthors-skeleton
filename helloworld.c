#include <stdio.h>
#include <string.h>
#include <openssl/core.h>
#include <openssl/core_dispatch.h>
#ifdef OPENSSL_PARAMS
# include <openssl/core_names.h>
# include <openssl/params.h>
#endif

static OSSL_FUNC_provider_gettable_params_fn gettable_params;
static OSSL_FUNC_provider_get_params_fn get_params;
#ifdef OPENSSL_PARAMS
static const OSSL_PARAM *gettable_params(void *provctx)
{
    static const OSSL_PARAM params[] = {
        OSSL_PARAM_utf8_ptr(OSSL_PROV_PARAM_BUILDINFO, NULL, 0),
        OSSL_PARAM_END
    };
    return params;
}
static int get_params(void *provctx, OSSL_PARAM *params)
{
    OSSL_PARAM *p;

    if ((p = OSSL_PARAM_locate(params, OSSL_PROV_PARAM_BUILDINFO)) != NULL) {
        if (!OSSL_PARAM_set_utf8_ptr(p, "commit " COMMITID))
            return 0;
    }
    return 1;
}
#else
static const OSSL_PARAM *gettable_params(void *provctx)
{
    static const OSSL_PARAM params[] = {
        { "buildinfo", OSSL_PARAM_UTF8_PTR, NULL, 0, 0 },
        { NULL, 0, NULL, 0, 0 }
    };
    return params;
}
static int get_params(void *provctx, OSSL_PARAM *params)
{
    for (; params != NULL && params->key != NULL; params++) {
        if (strcasecmp(params->key, "buildinfo") == 0) {
            if (params->data_type == OSSL_PARAM_UTF8_PTR) {
                const char **r = params->data;
                *r = "commit " COMMITID;
                params->return_size = strlen(*r);
            } else {
                return 0;
            }
        }
    }
    return 1;
}
#endif

const OSSL_DISPATCH provfns[] = {
    { OSSL_FUNC_PROVIDER_GETTABLE_PARAMS, (void (*)(void))gettable_params },
    { OSSL_FUNC_PROVIDER_GET_PARAMS, (void (*)(void))get_params },
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
