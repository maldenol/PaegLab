#include "LabBallot.h"

LabBallot::LabBallot(const unsigned InBallotId)
    : BallotId{InBallotId}
{
}

unsigned LabBallot::GetBallotId() const
{
    return BallotId;
}

void LabBallot::SetCandidateId(const unsigned InCandidate)
{
    CandidateId = InCandidate;
}

unsigned LabBallot::GetCandidateId() const
{
    return CandidateId;
}

LabBallot LabBallot::Mask(const LabRSA::LabPublicKey& PublicKey, const unsigned R) const
{
    LabBallot MaskedBallot = *this;

    MaskedBallot.BallotId = LabRSA::Mask(MaskedBallot.BallotId, PublicKey, R);
    MaskedBallot.CandidateId = LabRSA::Mask(MaskedBallot.CandidateId, PublicKey, R);

    return MaskedBallot;
}

LabBallot LabBallot::Unmask(const LabRSA::LabPublicKey& PublicKey, const unsigned R) const
{
    LabBallot Ballot = *this;

    Ballot.BallotId = LabRSA::Unmask(Ballot.BallotId, PublicKey, R);
    Ballot.CandidateId = LabRSA::Unmask(Ballot.CandidateId, PublicKey, R);
    Ballot.SignatureBallotId = LabRSA::UnmaskSignature(Ballot.SignatureBallotId, PublicKey, R);
    Ballot.SignatureCandidateId = LabRSA::UnmaskSignature(Ballot.SignatureCandidateId, PublicKey, R);

    return Ballot;
}

LabBallot LabBallot::Sign(const LabRSA::LabPrivateKey& PrivateKey) const
{
    LabBallot SignedBallot = *this;

    SignedBallot.SignatureBallotId = LabRSA::Sign(SignedBallot.BallotId, PrivateKey);
    SignedBallot.SignatureCandidateId = LabRSA::Sign(SignedBallot.CandidateId, PrivateKey);

    return SignedBallot;
}

LabBallot LabBallot::SignMasked(const LabRSA::LabPrivateKey& PrivateKey) const
{
    LabBallot SignedMaskedBallot = *this;

    SignedMaskedBallot.SignatureBallotId = LabRSA::SignMasked(SignedMaskedBallot.BallotId, PrivateKey);
    SignedMaskedBallot.SignatureCandidateId = LabRSA::SignMasked(SignedMaskedBallot.CandidateId, PrivateKey);

    return SignedMaskedBallot;
}

bool LabBallot::Verify(const LabRSA::LabPublicKey& PublicKey) const
{
    const bool bIsBallotIdValid = LabRSA::Verify(BallotId, SignatureBallotId, PublicKey);
    const bool bIsCandidateIdValid = LabRSA::Verify(CandidateId, SignatureCandidateId, PublicKey);
    return bIsBallotIdValid && bIsCandidateIdValid;
}

LabBallot LabBallot::Encrypt(const LabRSA::LabPublicKey& PublicKey) const
{
    LabBallot EncryptedBallot = *this;

    EncryptedBallot.BallotId = LabRSA::Encrypt(EncryptedBallot.BallotId, PublicKey);
    EncryptedBallot.CandidateId = LabRSA::Encrypt(EncryptedBallot.CandidateId, PublicKey);
    EncryptedBallot.SignatureBallotId = LabRSA::Encrypt(EncryptedBallot.SignatureBallotId, PublicKey);
    EncryptedBallot.SignatureCandidateId = LabRSA::Encrypt(EncryptedBallot.SignatureCandidateId, PublicKey);

    return EncryptedBallot;
}

LabBallot LabBallot::Decrypt(const LabRSA::LabPrivateKey& PrivateKey) const
{
    LabBallot Ballot = *this;

    Ballot.BallotId = LabRSA::Decrypt(Ballot.BallotId, PrivateKey);
    Ballot.CandidateId = LabRSA::Decrypt(Ballot.CandidateId, PrivateKey);
    Ballot.SignatureBallotId = LabRSA::Decrypt(Ballot.SignatureBallotId, PrivateKey);
    Ballot.SignatureCandidateId = LabRSA::Decrypt(Ballot.SignatureCandidateId, PrivateKey);

    return Ballot;
}
