#ifndef LAB5_LabVoter_H
#define LAB5_LabVoter_H

#include "LabBallot.h"
#include "LabCrypto.h"

#include <array>
#include <vector>

class LabVoter final
{
public:
    explicit LabVoter(const unsigned InVoterId);

    std::vector<LabBallot> CreateBallots(const unsigned CandidateId, const size_t BallotNum, const LabCrypto::LabPublicKeyRSA& EncryptKey);

    void VerifyOwnBallotPresence(const std::vector<LabBallot>& Ballots, const bool bVerifySignature) const;

    unsigned GetVoterId() const;
    const LabCrypto::LabPublicKeyDSA& GetVerifyKey() const;

    bool operator==(const LabVoter& Other) const;

    static LabBallot CreateWholeBallot(const unsigned VoterId, const unsigned CandidateId);
    static std::vector<LabBallot> CreateBallotParts(const unsigned VoterId, const unsigned CandidateId, const size_t BallotNum, const LabCrypto::LabPublicKeyRSA& EncryptKey, const LabCrypto::LabPrivateKeyDSA& SignKey);

    std::vector<LabBallot> CreateBallotsForVoter(const unsigned CandidateId, const size_t BallotNum, const LabCrypto::LabPublicKeyRSA& EncryptKey, const unsigned InVoterId);

private:
    unsigned VoterId;

    LabCrypto::LabPublicKeyDSA VerifyKey;
    LabCrypto::LabPrivateKeyDSA SignKey;

    std::vector<LabBallot> Ballots;

    friend class LabCorruptCentralElectionCommission;
};

#endif //LAB5_LabVoter_H
