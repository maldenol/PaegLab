#include "LabCrypto.h"

#include <cstdlib>
#include <vector>

void LabCrypto::GenerateKeysDSA(LabPublicKeyDSA& PublicKey, LabPrivateKeyDSA& PrivateKey)
{
    unsigned Q;
    do
    {
        Q = RandomPrime(50);
    }
    while (Q < 10);

    unsigned P;
    do
    {
        P = RandomPrime(1000);
    }
    while (((P - 1) % Q != 0) || (P < 5));

    unsigned G;
    do
    {
        const unsigned H = RandomRange(2, P - 2);
        G = ModExp(H, (P - 1) / Q, P);
    }
    while (G == 1);

    const unsigned X = RandomRange(1, Q - 1);
    const unsigned Y = ModExp(G, X, P);

    PublicKey.P = P;
    PublicKey.G = G;
    PublicKey.Q = Q;
    PublicKey.Y = Y;

    PrivateKey.P = P;
    PrivateKey.G = G;
    PrivateKey.Q = Q;
    PrivateKey.X = X;
}

LabCrypto::LabSignatureDSA LabCrypto::SignDSA(const unsigned Message, const LabPrivateKeyDSA& PrivateKey)
{
    const unsigned P = PrivateKey.P;
    const unsigned G = PrivateKey.G;
    const unsigned Q = PrivateKey.Q;
    const unsigned X = PrivateKey.X;

    if (P == 0)
    {
        return {};
    }

    unsigned R;
    unsigned S;

    do
    {
        const unsigned K = RandomRange(1, Q - 1);

        R = ModExp(G, K, P) % Q;
        if (R == 0)
        {
            continue;
        }

        const unsigned KInverse = ModInverse(K, Q);
        S = (KInverse * (Message + X * R)) % Q;
        if (S == 0)
        {
            continue;
        }

        break;
    }
    while (true);

    LabSignatureDSA Signature;
    Signature.R = R;
    Signature.S = S;
    return Signature;
}

bool LabCrypto::VerifyDSA(const unsigned Message, const LabSignatureDSA& Signature, const LabPublicKeyDSA& PublicKey)
{
    const unsigned R = Signature.R;
    const unsigned S = Signature.S;

    const unsigned P = PublicKey.P;
    const unsigned G = PublicKey.G;
    const unsigned Q = PublicKey.Q;
    const unsigned Y = PublicKey.Y;

    if (R <= 0 || R >= Q || S <= 0 || S >= Q)
    {
        return false;
    }

    const unsigned W = ModInverse(S, Q);
    const unsigned U1 = (Message * W) % Q;
    const unsigned U2 = (R * W) % Q;
    const unsigned V = (ModExp(G, U1, P) * ModExp(Y, U2, P)) % P % Q;

    return V == R;
}

void LabCrypto::GenerateKeysEG(LabPublicKeyEG& PublicKey, LabPrivateKeyEG& PrivateKey)
{
    unsigned P;
    do
    {
        P = RandomPrime(1000);
    }
    while (P < 6);

    const unsigned G = RandomPrime(P - 1);

    const unsigned X = RandomRange(2, P - 3);
    const unsigned Y = ModExp(G, X, P);

    PublicKey.P = P;
    PublicKey.G = G;
    PublicKey.Y = Y;

    PrivateKey.P = P;
    PrivateKey.X = X;
}

LabCrypto::LabCipherEG LabCrypto::EncryptEG(const unsigned Message, const LabPublicKeyEG& PublicKey)
{
    const unsigned P = PublicKey.P;
    const unsigned G = PublicKey.G;
    const unsigned Y = PublicKey.Y;

    const unsigned K = RandomRange(2, P - 2);

    const unsigned A = ModExp(G, K, P);
    const unsigned B = (Message * ModExp(Y, K, P)) % P;

    LabCipherEG Cipher;
    Cipher.A = A;
    Cipher.B = B;
    return Cipher;
}

unsigned LabCrypto::DecryptEG(const LabCipherEG& Cipher, const LabPrivateKeyEG& PrivateKey)
{
    const unsigned A = Cipher.A;
    const unsigned B = Cipher.B;

    const unsigned P = PrivateKey.P;
    const unsigned X = PrivateKey.X;

    const unsigned S = ModExp(A, X, P);
    const unsigned SInverse = ModInverse(S, P);
    const unsigned Message = (B * SInverse) % P;
    return Message;
}

unsigned LabCrypto::RandomRange(const unsigned Min, const unsigned Max)
{
    return std::rand() % (Max - Min + 1) + Min;
}

unsigned LabCrypto::RandomPrime(const unsigned Max)
{
    std::vector<unsigned long long> EratosthenesSieve;
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

unsigned LabCrypto::Gcd(unsigned a, unsigned b) {
    while (b != 0)
    {
        const unsigned temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

unsigned LabCrypto::ModExp(unsigned base, unsigned exp, const unsigned mod) {
    unsigned result = 1;
    base = base % mod;
    while (exp > 0)
    {
        if (exp % 2 == 1)
        {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

int LabCrypto::ModInverse(int e, int phi) {
    int t = 0, new_t = 1;
    int r = phi, new_r = e;

    while (new_r != 0)
    {
        int quotient = r / new_r;
        int temp = t;
        t = new_t;
        new_t = temp - quotient * new_t;

        temp = r;
        r = new_r;
        new_r = temp - quotient * new_r;
    }

    if (r > 1)
    {
        return -1;
    }
    if (t < 0)
    {
        t += phi;
    }
    return t;
}
