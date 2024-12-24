#include "LabCrypto.h"

void LabCrypto::GenerateKeysEG(LabPublicKeyEG& PublicKey, LabPrivateKeyEG& PrivateKey)
{
    const LabBigInt P = 9998603;
    const LabBigInt G = 4999301;

    const LabBigInt X = LabBigInt::RandomRange(2, P - 3);
    const LabBigInt Y = ModExp(G, X, P);

    PublicKey.P = P;
    PublicKey.G = G;
    PublicKey.Y = Y;

    PrivateKey.P = P;
    PrivateKey.X = X;
}

LabCrypto::LabCipherEG LabCrypto::EncryptEG(const LabBigInt& Message, const LabPublicKeyEG& PublicKey)
{
    const LabBigInt& P = PublicKey.P;
    const LabBigInt& G = PublicKey.G;
    const LabBigInt& Y = PublicKey.Y;

    const LabBigInt K = LabBigInt::RandomRange(2, P - 2);

    const LabBigInt A = ModExp(G, K, P);
    const LabBigInt B = (Message * ModExp(Y, K, P)) % P;

    LabCipherEG Cipher;
    Cipher.A = A;
    Cipher.B = B;
    return Cipher;
}

LabBigInt LabCrypto::DecryptEG(const LabCipherEG& Cipher, const LabPrivateKeyEG& PrivateKey)
{
    const LabBigInt& A = Cipher.A;
    const LabBigInt& B = Cipher.B;

    const LabBigInt& P = PrivateKey.P;
    const LabBigInt& X = PrivateKey.X;

    const LabBigInt S = ModExp(A, X, P);
    const LabBigInt SInverse = ModInverse(S, P);
    const LabBigInt Message = (B * SInverse) % P;
    return Message;
}

void LabCrypto::GenerateKeysBBS(LabPublicKeyBBS& PublicKey, LabPrivateKeyBBS& PrivateKey)
{
    const LabBigInt P = 11;
    const LabBigInt Q = 23;

    const LabBigInt N = P * Q;

    PublicKey.N = N;

    PrivateKey.P = P;
    PrivateKey.Q = Q;
}

LabBigInt LabCrypto::EncryptBBS(const LabBigInt& Message, const LabPublicKeyBBS& PublicKey, LabBigInt& OutX0)
{
    const LabBigInt& N = PublicKey.N;

    if (N == 0)
    {
        return 0;
    }

    LabBigInt X;
    do
    {
        X = LabBigInt::RandomRange(2, 1000000);
    }
    while (Gcd(X, N) != 1);

    const LabBigInt X0 = ModExp(X, 2, N);

    std::vector<std::byte> MessageBytes = Message.ToBytes();
    LabBigInt XI = X0;
    for (std::byte& Byte : MessageBytes)
    {
        Byte ^= XI.ToBytes().back();
        XI = ModExp(XI, 2, N);
    }

    OutX0 = X0;
    return LabBigInt::FromBytes(MessageBytes);
}

LabBigInt LabCrypto::DecryptBBS(const LabBigInt& EncryptedMessage, const LabPrivateKeyBBS& PrivateKey, const LabBigInt& X0)
{
    const LabBigInt& P = PrivateKey.P;
    const LabBigInt& Q = PrivateKey.Q;

    const LabBigInt N = P * Q;

    if (N == 0)
    {
        return 0;
    }

    std::vector<std::byte> MessageBytes = EncryptedMessage.ToBytes();
    LabBigInt XI = X0;
    for (std::byte& Byte : MessageBytes)
    {
        Byte ^= XI.ToBytes().back();
        XI = ModExp(XI, 2, N);
    }

    return LabBigInt::FromBytes(MessageBytes);
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
