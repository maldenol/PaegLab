#ifndef LAB1_LABRSA_H
#define LAB1_LABRSA_H

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

    static unsigned Sign(const unsigned Message, const LabPrivateKey& PrivateKey);
    static bool Verify(const unsigned Message, const unsigned Signature, const LabPublicKey& PublicKey);
};

#endif //LAB1_LABRSA_H
