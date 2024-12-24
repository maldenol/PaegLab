#ifndef LAB4_LABCRYPTO_H
#define LAB4_LABCRYPTO_H

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

    struct LabPublicKeyEG
    {
        LabBigInt P;
        LabBigInt G;
        LabBigInt Y;
    };

    struct LabPrivateKeyEG
    {
        LabBigInt P;
        LabBigInt G;
        LabBigInt X;
    };

    struct LabSignatureEG
    {
        LabBigInt R;
        LabBigInt S;
    };

    void GenerateKeysRSA(LabPublicKeyRSA& PublicKey, LabPrivateKeyRSA& PrivateKey);
    LabBigInt EncryptRSA(const LabBigInt& Message, const LabPublicKeyRSA& PublicKey);
    std::vector<LabBigInt> EncryptRSA(const std::vector<LabBigInt>& Message, const LabPublicKeyRSA& PublicKey);
    LabBigInt DecryptRSA(const LabBigInt& EncryptedMessage, const LabPrivateKeyRSA& PrivateKey);
    std::vector<LabBigInt> DecryptRSA(const std::vector<LabBigInt>& EncryptedMessage, const LabPrivateKeyRSA& PrivateKey);

    void GenerateKeysEG(LabPublicKeyEG& PublicKey, LabPrivateKeyEG& PrivateKey);
    LabSignatureEG SignEG(const std::vector<LabBigInt>& Message, const LabPrivateKeyEG& PrivateKey);
    bool VerifyEG(const std::vector<LabBigInt>& Message, const LabSignatureEG& Signature, const LabPublicKeyEG& PublicKey);

    LabBigInt Hash(const std::vector<LabBigInt>& Message, const LabBigInt& P);

    LabBigInt Gcd(const LabBigInt& InA, const LabBigInt& InB);
    LabBigInt ModExp(const LabBigInt& InBase, const LabBigInt& InExp, const LabBigInt& Mod);
    LabBigInt ModInverse(const LabBigInt& E, const LabBigInt& Phi);
};

#endif //LAB4_LABCRYPTO_H
