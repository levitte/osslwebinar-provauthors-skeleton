#include <openssl/core.h>
#include <openssl/provider.h>

OSSL_provider_init_fn my_prov_init;
int main()
{
  OSSL_PROVIDER *prov = NULL;

  if (OSSL_PROVIDER_add_builtin(NULL, "nonsense", &my_prov_init)
      && (prov = OSSL_PROVIDER_load(NULL, "nonsense")) != NULL)
    OSSL_PROVIDER_unload(prov);
}
