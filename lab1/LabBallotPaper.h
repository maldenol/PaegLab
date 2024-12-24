#ifndef LAB1_LABBALLOTPAPER_H
#define LAB1_LABBALLOTPAPER_H

#include "LabRSA.h"

class LabBallotPaper final
{
public:
    explicit LabBallotPaper(const unsigned InVoterId);

    unsigned GetVoterId() const;

    void SetCandidateId(const unsigned InCandidateId);
    unsigned GetCandidateId() const;

    void Sign(const LabRSA::LabPrivateKey& PrivateKey);
    bool Verify(const LabRSA::LabPublicKey& PublicKey) const;

    LabBallotPaper Encrypt(const unsigned Gamma) const;
    LabBallotPaper Decrypt(const unsigned Gamma) const;

private:
    unsigned VoterId = 0;
    unsigned CandidateId = 0;
    unsigned Signature = 0;
};

#endif //LAB1_LABBALLOTPAPER_H
