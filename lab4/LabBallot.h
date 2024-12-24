#ifndef LAB4_LABBALLOT_H
#define LAB4_LABBALLOT_H

#include "LabCrypto.h"

class LabBallot final
{
public:
    void SetCandidateId(const unsigned InCandidateId);
    unsigned GetCandidateId() const;
    std::vector<LabBigInt>& GetCandidateIdMut();

    LabBallot Encrypt(const LabCrypto::LabPublicKeyRSA& PublicKey) const;
    LabBallot Decrypt(const LabCrypto::LabPrivateKeyRSA& PrivateKey) const;

    LabBallot Sign(const LabCrypto::LabPrivateKeyEG& PrivateKey) const;
    bool Verify(const LabCrypto::LabPublicKeyEG& PublicKey) const;

    bool operator==(const LabBallot& Other) const;

private:
    std::vector<LabBigInt> CandidateId;

    LabCrypto::LabSignatureEG SignatureCandidateId;
};

#endif //LAB4_LABBALLOT_H
