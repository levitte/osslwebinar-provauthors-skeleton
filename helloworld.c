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
    *out = provfns;

    OSSL_FUNC_core_gettable_params_fn *c_gettable_params;
    OSSL_FUNC_core_get_params_fn *c_get_params;

    for (; in != NULL && in->function_id; in++) {
        switch (in->function_id) {
        case OSSL_FUNC_CORE_GETTABLE_PARAMS:
            c_gettable_params = OSSL_FUNC_core_gettable_params(in);
            break;
        case OSSL_FUNC_CORE_GET_PARAMS:
            c_get_params = OSSL_FUNC_core_get_params(in);
            break;
        }
    }

    const OSSL_PARAM *p = c_gettable_params(handle);
    for (; p != NULL && p->key != NULL; p++)
        printf("available libcrypto param: %s [type %u, size %zu]\n",
               p->key, p->data_type, p->data_size);

    const char *config_openssl_version = NULL;
    const char *config_something = NULL;
    OSSL_PARAM config_params[] = {
        { "openssl-version", OSSL_PARAM_UTF8_PTR,
          &config_openssl_version, 0, (size_t)-1 },
        { "something", OSSL_PARAM_UTF8_PTR,
          &config_something, 0, (size_t)-1 },
        { NULL, 0, NULL, 0, 0 }
    };
    if (!c_get_params(handle, config_params))
        return 0;
    if (config_params[0].return_size != (size_t)-1)
        printf("libcrypto param 'openssl-version': %s\n",
               config_openssl_version);
    if (config_params[1].return_size != (size_t)-1)
        printf("libcrypto param 'something': %s\n",
               config_something);

    return 1;
}
