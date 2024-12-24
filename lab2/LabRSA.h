#ifndef LAB2_LABRSA_H
#define LAB2_LABRSA_H

#include <vector>

class LabRSA final
{
public:
    struct LabPublicKey
    {
        unsigned E = 0;
        unsigned N = 0;
    };

    struct LabPrivateKey
    {
        unsigned D = 0;
        unsigned N = 0;
    };

    static void GenerateKeys(LabPublicKey& PublicKey, LabPrivateKey& PrivateKey);
    static unsigned GenerateR(const LabPublicKey& PublicKey);

    static unsigned Mask(const unsigned Message, const LabPublicKey& PublicKey, const unsigned R);
    static unsigned Unmask(const unsigned MaskedMessage, const LabPublicKey& PublicKey, const unsigned R);

    static unsigned SignMasked(const unsigned MaskedMessage, const LabPrivateKey& PrivateKey);
    static unsigned UnmaskSignature(const unsigned MaskedSignature, const LabPublicKey& PublicKey, const unsigned R);

    static unsigned Sign(const unsigned Message, const LabPrivateKey& PrivateKey);
    static unsigned Verify(const unsigned Message, const unsigned Signature, const LabPublicKey& PublicKey);

    static unsigned Encrypt(const unsigned Message, const LabPublicKey& PublicKey);
    static unsigned Decrypt(const unsigned EncryptedMessage, const LabPrivateKey& PrivateKey);
};

#endif //LAB2_LABRSA_H
