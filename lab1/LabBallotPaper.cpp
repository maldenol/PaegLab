#include "LabBallotPaper.h"

LabBallotPaper::LabBallotPaper(const unsigned InVoterId)
    : VoterId{InVoterId}
{
}

unsigned LabBallotPaper::GetVoterId() const
{
    return VoterId;
}

void LabBallotPaper::SetCandidateId(const unsigned InCandidate)
{
    CandidateId = InCandidate;
}

unsigned LabBallotPaper::GetCandidateId() const
{
    return CandidateId;
}

void LabBallotPaper::Sign(const LabRSA::LabPrivateKey& PrivateKey)
{
    Signature = LabRSA::Sign(CandidateId, PrivateKey);
}

bool LabBallotPaper::Verify(const LabRSA::LabPublicKey& PublicKey) const
{
    return LabRSA::Verify(CandidateId, Signature, PublicKey);
}

LabBallotPaper LabBallotPaper::Encrypt(const unsigned Gamma) const
{
    LabBallotPaper EncryptedBallotPaper = *this;

    EncryptedBallotPaper.CandidateId = EncryptedBallotPaper.CandidateId ^ Gamma;
    EncryptedBallotPaper.Signature = EncryptedBallotPaper.Signature ^ Gamma;

    return EncryptedBallotPaper;
}

LabBallotPaper LabBallotPaper::Decrypt(const unsigned Gamma) const
{
    LabBallotPaper BallotPaper = *this;

    BallotPaper.CandidateId = BallotPaper.CandidateId ^ Gamma;
    BallotPaper.Signature = BallotPaper.Signature ^ Gamma;

    return BallotPaper;
}
