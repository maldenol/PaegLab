#include "LabCrypto.h"

void LabCrypto::GenerateKeysRSA(LabPublicKeyRSA& PublicKey, LabPrivateKeyRSA& PrivateKey)
{
    const LabBigInt P = 50021;
    const LabBigInt Q = 20011;

    const LabBigInt N = P * Q;
    const LabBigInt Phi = (P - 1) * (Q - 1);

    LabBigInt E = 3;
    while (Gcd(E, Phi) != 1)
    {
        E += 2;
    }

    const LabBigInt D = ModInverse(E, Phi);

    PublicKey.E = E;
    PublicKey.N = N;

    PrivateKey.D = D;
    PrivateKey.N = N;
}

LabBigInt LabCrypto::EncryptRSA(const LabBigInt& Message, const LabPublicKeyRSA& PublicKey)
{
    const LabBigInt& E = PublicKey.E;
    const LabBigInt& N = PublicKey.N;

    const LabBigInt EncryptedMessage = ModExp(Message, E, N);
    return EncryptedMessage;
}

LabBigInt LabCrypto::DecryptRSA(const LabBigInt& EncryptedMessage, const LabPrivateKeyRSA& PrivateKey)
{
    const LabBigInt& D = PrivateKey.D;
    const LabBigInt& N = PrivateKey.N;

    const LabBigInt Message = ModExp(EncryptedMessage, D, N);
    return Message;
}

void LabCrypto::GenerateKeysDSA(LabPublicKeyDSA& PublicKey, LabPrivateKeyDSA& PrivateKey)
{
    const LabBigInt Q = 71;
    const LabBigInt P = 7243;

    LabBigInt G;
    do
    {
        const LabBigInt H = LabBigInt::RandomRange(2, P - 2);
        G = ModExp(H, (P - 1) / Q, P);
    }
    while (G == 1);

    const LabBigInt X = LabBigInt::RandomRange(1, Q - 1);
    const LabBigInt Y = ModExp(G, X, P);

    PublicKey.P = P;
    PublicKey.G = G;
    PublicKey.Q = Q;
    PublicKey.Y = Y;

    PrivateKey.P = P;
    PrivateKey.G = G;
    PrivateKey.Q = Q;
    PrivateKey.X = X;
}

LabCrypto::LabSignatureDSA LabCrypto::SignDSA(const LabBigInt& Message, const LabPrivateKeyDSA& PrivateKey)
{
    const LabBigInt& P = PrivateKey.P;
    const LabBigInt& G = PrivateKey.G;
    const LabBigInt& Q = PrivateKey.Q;
    const LabBigInt& X = PrivateKey.X;

    LabBigInt R;
    LabBigInt S;

    do
    {
        const LabBigInt K = LabBigInt::RandomRange(1, Q - 1);

        R = ModExp(G, K, P) % Q;
        if (R == 0)
        {
            continue;
        }

        const LabBigInt KInverse = ModInverse(K, Q);
        S = (KInverse * (Message + X * R)) % Q;
        if (S <= 0)
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

bool LabCrypto::VerifyDSA(const LabBigInt& Message, const LabSignatureDSA& Signature, const LabPublicKeyDSA& PublicKey)
{
    const LabBigInt& R = Signature.R;
    const LabBigInt& S = Signature.S;

    const LabBigInt& P = PublicKey.P;
    const LabBigInt& G = PublicKey.G;
    const LabBigInt& Q = PublicKey.Q;
    const LabBigInt& Y = PublicKey.Y;

    if (R <= 0 || R >= Q || S <= 0 || S >= Q)
    {
        return false;
    }

    const LabBigInt W = ModInverse(S, Q);
    const LabBigInt U1 = (Message * W) % Q;
    const LabBigInt U2 = (R * W) % Q;
    const LabBigInt V = (ModExp(G, U1, P) * ModExp(Y, U2, P)) % P % Q;

    return V == R;
}

LabBigInt LabCrypto::Gcd(const LabBigInt& InA, const LabBigInt& InB)
{
    LabBigInt A = InA;
    LabBigInt B = InB;

    while (B != 0)
    {
        const LabBigInt Temp = B;
        B = A % B;
        A = Temp;
    }

    return A;
}

LabBigInt LabCrypto::ModExp(const LabBigInt& InBase, const LabBigInt& InExp, const LabBigInt& Mod)
{
    LabBigInt Result = 1;

    LabBigInt Base = InBase % Mod;
    LabBigInt Exp = InExp;

    if (Base == 0)
    {
        return 0;
    }

    while (Exp > 0)
    {
        if (Exp % 2 == 1)
        {
            Result = (Result * Base) % Mod;
        }
        Exp >>= 1;
        Base = (Base * Base) % Mod;
    }

    return Result;
}

LabBigInt LabCrypto::ModInverse(const LabBigInt& E, const LabBigInt& Phi)
{
    LabBigInt T = 0, NewT = 1;
    LabBigInt R = Phi, NewR = E;

    while (NewR != 0)
    {
        const LabBigInt Quotient = R / NewR;

        LabBigInt Temp = T;
        T = NewT;
        NewT = Temp - Quotient * NewT;

        Temp = R;
        R = NewR;
        NewR = Temp - Quotient * NewR;
    }

    if (R > 1)
    {
        return -1;
    }

    if (T < 0)
    {
        T += Phi;
    }

    return T;
}
