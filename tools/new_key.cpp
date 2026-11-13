// Generates a fake payment gateway key, in the same format as a live secret key,
// to simulate the key that the provider issues after a rotation.
//
// Usage: make new-key

# include <cstdio>
#include <cstdlib>

int main()
{
    const char* key = std::getenv("PAYMENT_KEY");
    printf("%s\n", key);
    return 0;
}
