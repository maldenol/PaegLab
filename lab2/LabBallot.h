#ifndef LAB2_LABBALLOT_H
#define LAB2_LABBALLOT_H

#include "LabRSA.h"

class LabBallot final
{
public:
    static constexpr unsigned MaxBallotId = 100;

    explicit LabBallot(const unsigned InBallotId);

    unsigned GetBallotId() const;

    void SetCandidateId(const unsigned InCandidateId);
    unsigned GetCandidateId() const;

    LabBallot Mask(const LabRSA::LabPublicKey& PublicKey, const unsigned R) const;
    LabBallot Unmask(const LabRSA::LabPublicKey& PublicKey, const unsigned R) const;

    LabBallot Sign(const LabRSA::LabPrivateKey& PrivateKey) const;
    LabBallot SignMasked(const LabRSA::LabPrivateKey& PrivateKey) const;
    bool Verify(const LabRSA::LabPublicKey& PublicKey) const;

    LabBallot Encrypt(const LabRSA::LabPublicKey& PublicKey) const;
    LabBallot Decrypt(const LabRSA::LabPrivateKey& PrivateKey) const;

private:
    unsigned BallotId = 0;
    unsigned CandidateId = 0;

    unsigned SignatureBallotId = 0;
    unsigned SignatureCandidateId = 0;
};

#endif //LAB2_LABBALLOT_H
