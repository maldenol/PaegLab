#include "LabRSA.h"

#include <vector>

unsigned RandomPrime(const unsigned Max);
int Gcd(int a, int b);
int ModExp(int base, int exp, int mod);
int ModInverse(int e, int phi);

void LabRSA::GenerateKeys(LabPublicKey& PublicKey, LabPrivateKey& PrivateKey)
{
    const unsigned P = RandomPrime(256);
    const unsigned Q = RandomPrime(256);

    const unsigned N = P * Q;
    const unsigned Phi = (P - 1) * (Q - 1);

    unsigned E = 3;
    while (Gcd(E, Phi) != 1) {
        E += 2;
    }

    const unsigned D = ModInverse(E, Phi);

    PublicKey.E = E;
    PublicKey.N = N;

    PrivateKey.D = D;
    PrivateKey.N = N;
}

unsigned LabRSA::Sign(const unsigned Message, const LabPrivateKey& PrivateKey)
{
    const unsigned Signature = ModExp(Message, PrivateKey.D, PrivateKey.N);
    return Signature;
}

bool LabRSA::Verify(const unsigned Message, const unsigned Signature, const LabPublicKey& PublicKey)
{
    const unsigned RecoveredMessage = ModExp(Signature, PublicKey.E, PublicKey.N);
    return RecoveredMessage == Message;
}

unsigned RandomPrime(const unsigned Max)
{
    std::vector<unsigned> EratosthenesSieve;
    EratosthenesSieve.reserve(Max - 1);
    for (unsigned Value = 2; Value <= Max; ++Value)
    {
        EratosthenesSieve.push_back(Value);
    }

    for (unsigned Index = 0; Index < EratosthenesSieve.size(); ++Index)
    {
        const unsigned Value = EratosthenesSieve[Index];
        for (unsigned InnerIndex = Index + 1; InnerIndex < EratosthenesSieve.size(); ++InnerIndex)
        {
            const unsigned InnerValue = EratosthenesSieve[InnerIndex];
            if (InnerValue % Value == 0)
            {
                EratosthenesSieve.erase(EratosthenesSieve.begin() + InnerIndex);
                --InnerIndex;
            }
        }
    }

    const size_t Index = std::rand() % EratosthenesSieve.size();
    return EratosthenesSieve[Index];
}

int Gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int ModExp(int base, int exp, int mod) {
    int result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) { // If exp is odd, multiply base with result
            result = (result * base) % mod;
        }
        exp = exp >> 1; // Divide exp by 2
        base = (base * base) % mod;
    }
    return result;
}

int ModInverse(int e, int phi) {
    int t = 0, new_t = 1;
    int r = phi, new_r = e;

    while (new_r != 0) {
        int quotient = r / new_r;
        int temp = t;
        t = new_t;
        new_t = temp - quotient * new_t;

        temp = r;
        r = new_r;
        new_r = temp - quotient * new_r;
    }

    if (r > 1) {
        return -1;
    }
    if (t < 0) {
        t += phi;
    }
    return t;
}
