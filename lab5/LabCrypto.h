#ifndef LAB5_LABCRYPTO_H
#define LAB5_LABCRYPTO_H

#include "LabBigInt.h"

#include <vector>

namespace LabCrypto
{
    struct LabPublicKeyRSA
    {
        LabBigInt E;
        LabBigInt N;
    };

    struct LabPrivateKeyRSA
    {
        LabBigInt D;
        LabBigInt N;
    };

    struct LabPublicKeyDSA
    {
        LabBigInt P;
        LabBigInt G;
        LabBigInt Q;
        LabBigInt Y;
    };

    struct LabPrivateKeyDSA
    {
        LabBigInt P;
        LabBigInt G;
        LabBigInt Q;
        LabBigInt X;
    };

    struct LabSignatureDSA
    {
        LabBigInt R;
        LabBigInt S;
    };

    void GenerateKeysRSA(LabPublicKeyRSA& PublicKey, LabPrivateKeyRSA& PrivateKey);
    LabBigInt EncryptRSA(const LabBigInt& Message, const LabPublicKeyRSA& PublicKey);
    LabBigInt DecryptRSA(const LabBigInt& EncryptedMessage, const LabPrivateKeyRSA& PrivateKey);

    void GenerateKeysDSA(LabPublicKeyDSA& PublicKey, LabPrivateKeyDSA& PrivateKey);
    LabSignatureDSA SignDSA(const LabBigInt& Message, const LabPrivateKeyDSA& PrivateKey);
    bool VerifyDSA(const LabBigInt& Message, const LabSignatureDSA& Signature, const LabPublicKeyDSA& PublicKey);

    LabBigInt Gcd(const LabBigInt& InA, const LabBigInt& InB);
    LabBigInt ModExp(const LabBigInt& InBase, const LabBigInt& InExp, const LabBigInt& Mod);
    LabBigInt ModInverse(const LabBigInt& E, const LabBigInt& Phi);
};

#endif //LAB5_LABCRYPTO_H
