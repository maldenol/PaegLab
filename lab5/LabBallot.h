#ifndef LAB5_LABBALLOT_H
#define LAB5_LABBALLOT_H

#include "LabCrypto.h"

class LabBallot final
{
public:
    explicit LabBallot(const unsigned InVoterId);

    void SetVoterId(const unsigned InVoterId);
    unsigned GetVoterId() const;
    void SetCandidateId(const LabBigInt& InCandidateId);
    const LabBigInt& GetCandidateId() const;

    LabBallot Encrypt(const LabCrypto::LabPublicKeyRSA& PublicKey) const;
    LabBallot Decrypt(const LabCrypto::LabPrivateKeyRSA& PrivateKey) const;

    LabBallot Sign(const LabCrypto::LabPrivateKeyDSA& PrivateKey) const;
    bool Verify(const LabCrypto::LabPublicKeyDSA& PublicKey) const;

    bool operator==(const LabBallot& Other) const;

private:
    unsigned VoterId;
    LabBigInt CandidateId;

    LabCrypto::LabSignatureDSA SignatureVoterId;
    LabCrypto::LabSignatureDSA SignatureCandidateId;
};

#endif //LAB5_LABBALLOT_H
