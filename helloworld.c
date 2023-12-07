#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/core.h>
#include <openssl/core_dispatch.h>

struct provctx_st {
    const OSSL_CORE_HANDLE *handle;
    OSSL_FUNC_core_new_error_fn *up_new_error;
    OSSL_FUNC_core_set_error_debug_fn *up_set_error_debug;
    OSSL_FUNC_core_vset_error_fn *up_vset_error;
};

static void err(struct provctx_st *pctx, int reason,
                const char *file, unsigned int line, const char *func,
                const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  pctx->up_new_error(pctx->handle);
  pctx->up_set_error_debug(pctx->handle, file, line, func);
  pctx->up_vset_error(pctx->handle, reason, fmt, ap);
  va_end(ap);
}

#define ERR(pctx,reason,fmt,...) \
    err((pctx), (reason), __FILE__, __LINE__, __func__, fmt     \
        __VA_OPT__(,) __VA_ARGS__)

#define E_UNSUPPORTED           1

const OSSL_ITEM *get_reasons(void *provctx)
{
  static const OSSL_ITEM reasons[] = {
    { E_UNSUPPORTED, "Hello, world!  This is unsupported" },
    { 0, NULL }
  };
  return reasons;
}

static OSSL_FUNC_provider_query_operation_fn query;
static const OSSL_ALGORITHM *query(void *provctx, int operation_id,
                                   int *no_cache)
{
    ERR(provctx, E_UNSUPPORTED, "operation_id=%d", operation_id);
    return NULL;
}

static OSSL_FUNC_provider_teardown_fn teardown;
void teardown(void *provctx)
{
    free(provctx);
}

const OSSL_DISPATCH provfns[] = {
    { OSSL_FUNC_PROVIDER_GET_REASON_STRINGS, (void (*)(void))get_reasons },
    { OSSL_FUNC_PROVIDER_QUERY_OPERATION, (void (*)(void))query },
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

    OSSL_FUNC_core_new_error_fn *c_new_error;
    OSSL_FUNC_core_set_error_debug_fn *c_set_error_debug;
    OSSL_FUNC_core_vset_error_fn *c_vset_error;

    for (; in != NULL && in->function_id; in++) {
        switch (in->function_id) {
        case OSSL_FUNC_CORE_NEW_ERROR:
            c_new_error = OSSL_FUNC_core_new_error(in);
            break;
        case OSSL_FUNC_CORE_SET_ERROR_DEBUG:
            c_set_error_debug = OSSL_FUNC_core_set_error_debug(in);
            break;
        case OSSL_FUNC_CORE_VSET_ERROR:
            c_vset_error = OSSL_FUNC_core_vset_error(in);
            break;
        }
    }

    pctx->handle = handle;
    pctx->up_new_error = c_new_error;
    pctx->up_set_error_debug = c_set_error_debug;
    pctx->up_vset_error = c_vset_error;

    return 1;
}
