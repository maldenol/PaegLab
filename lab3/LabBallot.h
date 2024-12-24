#ifndef LAB3_LABBALLOT_H
#define LAB3_LABBALLOT_H

#include "LabCrypto.h"

struct LabBallotCipher;

class LabBallot final
{
public:
    explicit LabBallot(const unsigned InRegistrationId);

    unsigned GetRegistrationId() const;

    void SetCandidateId(const unsigned InCandidateId);
    unsigned GetCandidateId() const;

    LabBallot Sign(const LabCrypto::LabPrivateKeyDSA& PrivateKey) const;
    bool Verify(const LabCrypto::LabPublicKeyDSA& PublicKey) const;

    LabBallotCipher Encrypt(const LabCrypto::LabPublicKeyEG& PublicKey) const;

private:
    unsigned RegistrationId = 0;
    unsigned CandidateId = 0;

    LabCrypto::LabSignatureDSA SignatureRegistrationId = {};
    LabCrypto::LabSignatureDSA SignatureCandidateId = {};

    friend struct LabBallotCipher;
};

struct LabBallotCipher final
{
    LabBallot Decrypt(const LabCrypto::LabPrivateKeyEG& PrivateKey) const;

    LabCrypto::LabCipherEG RegistrationId = {};
    LabCrypto::LabCipherEG CandidateId = {};

    LabCrypto::LabCipherEG SignatureRRegistrationId = {};
    LabCrypto::LabCipherEG SignatureSRegistrationId = {};
    LabCrypto::LabCipherEG SignatureRCandidateId = {};
    LabCrypto::LabCipherEG SignatureSCandidateId = {};
};

#endif //LAB3_LABBALLOT_H
