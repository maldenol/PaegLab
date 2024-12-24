#include "LabCrypto.h"

void LabCrypto::GenerateKeysRSA(LabPublicKeyRSA& PublicKey, LabPrivateKeyRSA& PrivateKey)
{
//    const LabBigInt P = LabBigInt::RandomPrime(256);
//    const LabBigInt Q = LabBigInt::RandomPrime(256);

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

std::vector<LabBigInt> LabCrypto::EncryptRSA(const std::vector<LabBigInt>& Message, const LabPublicKeyRSA& PublicKey)
{
    std::vector<LabBigInt> EncryptedMessage;
    EncryptedMessage.reserve(Message.size());
    for (const LabBigInt& BigInt : Message)
    {
        const LabBigInt EncryptedBigInt = EncryptRSA(BigInt, PublicKey);
        EncryptedMessage.push_back(EncryptedBigInt);
    }
    return EncryptedMessage;
}

LabBigInt LabCrypto::DecryptRSA(const LabBigInt& EncryptedMessage, const LabPrivateKeyRSA& PrivateKey)
{
    const LabBigInt& D = PrivateKey.D;
    const LabBigInt& N = PrivateKey.N;

    const LabBigInt Message = ModExp(EncryptedMessage, D, N);
    return Message;
}

std::vector<LabBigInt> LabCrypto::DecryptRSA(const std::vector<LabBigInt>& EncryptedMessage, const LabPrivateKeyRSA& PrivateKey)
{
    std::vector<LabBigInt> Message;
    Message.reserve(EncryptedMessage.size());
    for (const LabBigInt& EncryptedBigInt : EncryptedMessage)
    {
        const LabBigInt BigInt = DecryptRSA(EncryptedBigInt, PrivateKey);
        Message.push_back(BigInt);
    }
    return Message;
}

void LabCrypto::GenerateKeysEG(LabPublicKeyEG& PublicKey, LabPrivateKeyEG& PrivateKey)
{
//    LabBigInt P;
//    do
//    {
//        P = LabBigInt::RandomPrime(1000);
//    }
//    while (P < 6);
//
//    const LabBigInt G = LabBigInt::RandomPrime(P - 1);

    const LabBigInt P = 50021;
    const LabBigInt G = 20011;

    const LabBigInt X = LabBigInt::RandomRange(2, P - 3);
    const LabBigInt Y = ModExp(G, X, P);

    PublicKey.P = P;
    PublicKey.G = G;
    PublicKey.Y = Y;

    PrivateKey.P = P;
    PrivateKey.G = G;
    PrivateKey.X = X;
}

LabCrypto::LabSignatureEG LabCrypto::SignEG(const std::vector<LabBigInt>& Message, const LabPrivateKeyEG& PrivateKey)
{
    const LabBigInt& P = PrivateKey.P;
    const LabBigInt& G = PrivateKey.G;
    const LabBigInt& X = PrivateKey.X;

    const LabBigInt MessageHash = Hash(Message, P);

    LabBigInt R;
    LabBigInt S;

    while (true)
    {
        LabBigInt K;
        do
        {
            K = LabBigInt::RandomRange(2, P - 2);
        }
        while (Gcd(K, P - 1) != 1);

        R = ModExp(G, K, P);

        if (R == 0)
        {
            continue;
        }

        const LabBigInt KInverse = ModInverse(K, P - 1);
        S = (MessageHash - X * R) * KInverse % (P - 1);

        if (S <= 0)
        {
            continue;
        }

        break;
    }

    LabSignatureEG Signature;
    Signature.R = R;
    Signature.S = S;
    return Signature;
}

bool LabCrypto::VerifyEG(const std::vector<LabBigInt>& Message, const LabSignatureEG& Signature, const LabPublicKeyEG& PublicKey)
{
    const LabBigInt& P = PublicKey.P;
    const LabBigInt& G = PublicKey.G;
    const LabBigInt& Y = PublicKey.Y;

    const LabBigInt& R = Signature.R;
    const LabBigInt& S = Signature.S;

    if ((R <= 0) || (R >= P) || (S <= 0) || (S >= P - 1))
    {
        return false;
    }

    const LabBigInt MessageHash = Hash(Message, P);

    const LabBigInt V1 = ModExp(Y, R, P) * ModExp(R, S, P) % P;
    const LabBigInt V2 = ModExp(G, MessageHash, P);
    return V1 == V2;
}

LabBigInt LabCrypto::Hash(const std::vector<LabBigInt>& Message, const LabBigInt& P)
{
    const LabBigInt P2 = P * P;
    const LabBigInt P4 = P2 * P2;

    LabBigInt MessageHash = 0;
    while (MessageHash < P2)
    {
        for (const LabBigInt& BigInt : Message)
        {
            MessageHash += (MessageHash + BigInt * 3) * (BigInt + 1) + BigInt * 2;
            MessageHash %= P4;
        }
    }
    return MessageHash;
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
