#ifndef LAB3_LABCRYPTO_H
#define LAB3_LABCRYPTO_H

namespace LabCrypto
{
    struct LabPublicKeyDSA
    {
        unsigned P = 0;
        unsigned G = 0;
        unsigned Q = 0;
        unsigned Y = 0;
    };

    struct LabPrivateKeyDSA
    {
        unsigned P = 0;
        unsigned G = 0;
        unsigned Q = 0;
        unsigned X = 0;
    };

    struct LabSignatureDSA
    {
        unsigned R = 0;
        unsigned S = 0;
    };

    struct LabPublicKeyEG
    {
        unsigned P = 0;
        unsigned G = 0;
        unsigned Y = 0;
    };

    struct LabPrivateKeyEG
    {
        unsigned P = 0;
        unsigned X = 0;
    };

    struct LabCipherEG
    {
        unsigned A = 0;
        unsigned B = 0;
    };

    void GenerateKeysDSA(LabPublicKeyDSA& PublicKey, LabPrivateKeyDSA& PrivateKey);
    LabSignatureDSA SignDSA(const unsigned Message, const LabPrivateKeyDSA& PrivateKey);
    bool VerifyDSA(const unsigned Message, const LabSignatureDSA& Signature, const LabPublicKeyDSA& PublicKey);

    void GenerateKeysEG(LabPublicKeyEG& PublicKey, LabPrivateKeyEG& PrivateKey);
    LabCipherEG EncryptEG(const unsigned Message, const LabPublicKeyEG& PublicKey);
    unsigned DecryptEG(const LabCipherEG& Cipher, const LabPrivateKeyEG& PrivateKey);

    unsigned RandomRange(const unsigned Min, const unsigned Max);
    unsigned RandomPrime(const unsigned Max);

    unsigned Gcd(unsigned a, unsigned b);
    unsigned ModExp(unsigned base, unsigned exp, const unsigned mod);
    int ModInverse(int e, int phi);
};

#endif //LAB3_LABCRYPTO_H
