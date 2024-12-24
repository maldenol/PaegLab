#ifndef LAB6_LABCRYPTO_H
#define LAB6_LABCRYPTO_H

#include "LabBigInt.h"

#include <vector>

namespace LabCrypto
{
    struct LabPublicKeyEG
    {
        LabBigInt P = 0;
        LabBigInt G = 0;
        LabBigInt Y = 0;
    };

    struct LabPrivateKeyEG
    {
        LabBigInt P = 0;
        LabBigInt X = 0;
    };

    struct LabCipherEG
    {
        LabBigInt A = 0;
        LabBigInt B = 0;
    };

    struct LabPublicKeyBBS
    {
        LabBigInt N = 0;
    };

    struct LabPrivateKeyBBS
    {
        LabBigInt P = 0;
        LabBigInt Q = 0;
    };

    void GenerateKeysEG(LabPublicKeyEG& PublicKey, LabPrivateKeyEG& PrivateKey);
    LabCipherEG EncryptEG(const LabBigInt& Message, const LabPublicKeyEG& PublicKey);
    LabBigInt DecryptEG(const LabCipherEG& Cipher, const LabPrivateKeyEG& PrivateKey);

    void GenerateKeysBBS(LabPublicKeyBBS& PublicKey, LabPrivateKeyBBS& PrivateKey);
    LabBigInt EncryptBBS(const LabBigInt& Message, const LabPublicKeyBBS& PublicKey, LabBigInt& OutX0);
    LabBigInt DecryptBBS(const LabBigInt& EncryptedMessage, const LabPrivateKeyBBS& PrivateKey, const LabBigInt& X0);

    LabBigInt Gcd(const LabBigInt& InA, const LabBigInt& InB);
    LabBigInt ModExp(const LabBigInt& InBase, const LabBigInt& InExp, const LabBigInt& Mod);
    LabBigInt ModInverse(const LabBigInt& E, const LabBigInt& Phi);
};

#endif //LAB6_LABCRYPTO_H
