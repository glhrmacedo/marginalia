# include "payment.h"

# include <cstdio>
# include <cstring>
#include <cstdlib>

# define KEY_SIZE 128

using namespace std;

static const char* PAYMENT_KEY = getenv("PAYMENT_KEY");

/* Simula o gateway de pagamentos, que aceita o pagamento quando a chave foi informada e o valor é positivo. */
static bool gatewayAuthorize(const char *key, double amount)
{
    return key[0] != '\0' && amount > 0.0;
}

/* Autoriza o pagamento com a chave do gateway e apaga a chave da memória logo depois do uso. */
bool processPayment(double amount)
{
    char key[KEY_SIZE];
    bool authorized;

    snprintf(key, sizeof key, "%s", PAYMENT_KEY);
    authorized = gatewayAuthorize(key, amount);
    memset(key, 0, sizeof key);
    return authorized;
}
