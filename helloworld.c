#include <stdio.h>
#include <string.h>
#include <openssl/core.h>
#include <openssl/core_dispatch.h>

static OSSL_FUNC_digest_newctx_fn blargh_h_newctx;
static void *blargh_h_newctx(void *provctx) { return NULL; }
static OSSL_FUNC_digest_freectx_fn blargh_h_freectx;
static void blargh_h_freectx(void *bctx) { }
static OSSL_FUNC_digest_init_fn blargh_h_init;
static int blargh_h_init(void *bctx, const OSSL_PARAM *params) { return 0; }
static OSSL_FUNC_digest_update_fn blargh_h_update;
static int blargh_h_update(void *bctx, const unsigned char *m, size_t s) { return 0; }
static OSSL_FUNC_digest_final_fn blargh_h_final;
static int blargh_h_final(void *bctx, unsigned char *d, size_t *l, size_t s) { return 0; }
static OSSL_FUNC_digest_get_params_fn blargh_h_get_params;
static int blargh_h_get_params(OSSL_PARAM *params)
{
    for (; params != NULL && params->key != NULL; params++) {
        if (strcasecmp(params->key, "blocksize") == 0) {
            if (params->data_type == OSSL_PARAM_UNSIGNED_INTEGER
                && params->data_size == sizeof(size_t)) {
                size_t *r = params->data;
                *r = 1;
            } else {
                return 0;
            }
        } else if (strcasecmp(params->key, "size") == 0) {
            if (params->data_type == OSSL_PARAM_UNSIGNED_INTEGER
                && params->data_size == sizeof(size_t)) {
                size_t *r = params->data;
                *r = 1;
            } else {
                return 0;
            }
        }
    }
    return 1;
}

static const OSSL_DISPATCH blargh_hash_impl[] = {
    { OSSL_FUNC_DIGEST_NEWCTX, (void (*)(void))blargh_h_newctx },
    { OSSL_FUNC_DIGEST_FREECTX, (void (*)(void))blargh_h_freectx },
    { OSSL_FUNC_DIGEST_INIT, (void (*)(void))blargh_h_init },
    { OSSL_FUNC_DIGEST_UPDATE, (void (*)(void))blargh_h_update },
    { OSSL_FUNC_DIGEST_FINAL, (void (*)(void))blargh_h_final },
    { OSSL_FUNC_DIGEST_GET_PARAMS, (void (*)(void))blargh_h_get_params },
    { 0, NULL }
};

static const OSSL_ALGORITHM hashes[] = {
    { "BLARG:id-blargh:1.2.3.4.5", "x.author=bleah", blargh_hash_impl,
       "BLARG is a fictious generic algorithm" },
    { NULL, NULL, NULL, NULL }
};

static OSSL_FUNC_provider_query_operation_fn query;
static const OSSL_ALGORITHM *query(void *provctx, int operation_id,
                                   int *no_cache)
{
    *no_cache = 0;
    switch (operation_id) {
    case OSSL_OP_DIGEST:
        return hashes;
    }
    return NULL;
}

const OSSL_DISPATCH provfns[] = {
    { OSSL_FUNC_PROVIDER_QUERY_OPERATION, (void (*)(void))query },
    { 0, NULL }
};

OSSL_provider_init_fn OSSL_provider_init; /* Check function signature */
int OSSL_provider_init(const OSSL_CORE_HANDLE *handle,
                       const OSSL_DISPATCH *in,
                       const OSSL_DISPATCH **out,
                       void **provctx)
{
    *out = provfns;
    return 1;
}
