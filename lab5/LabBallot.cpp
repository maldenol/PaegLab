#include "LabBallot.h"

LabBallot::LabBallot(const unsigned InVoterId)
    : VoterId{InVoterId}
{
}

void LabBallot::SetVoterId(const unsigned InVoterId)
{
    VoterId = InVoterId;
}

unsigned LabBallot::GetVoterId() const
{
    return VoterId;
}

void LabBallot::SetCandidateId(const LabBigInt& InCandidateId)
{
    CandidateId = InCandidateId;
}

const LabBigInt& LabBallot::GetCandidateId() const
{
    return CandidateId;
}

LabBallot LabBallot::Encrypt(const LabCrypto::LabPublicKeyRSA& PublicKey) const
{
    LabBallot EncryptedBallot = *this;

    EncryptedBallot.CandidateId = LabCrypto::EncryptRSA(EncryptedBallot.CandidateId, PublicKey);

    return EncryptedBallot;
}

LabBallot LabBallot::Decrypt(const LabCrypto::LabPrivateKeyRSA& PrivateKey) const
{
    LabBallot Ballot = *this;

    Ballot.CandidateId = LabCrypto::DecryptRSA(Ballot.CandidateId, PrivateKey);

    return Ballot;
}

LabBallot LabBallot::Sign(const LabCrypto::LabPrivateKeyDSA& PrivateKey) const
{
    LabBallot SignedBallot = *this;

    SignedBallot.SignatureVoterId = LabCrypto::SignDSA(SignedBallot.VoterId, PrivateKey);
    SignedBallot.SignatureCandidateId = LabCrypto::SignDSA(SignedBallot.CandidateId, PrivateKey);

    return SignedBallot;
}

bool LabBallot::Verify(const LabCrypto::LabPublicKeyDSA& PublicKey) const
{
    const bool bIsValidSignatureVoterId = LabCrypto::VerifyDSA(VoterId, SignatureVoterId, PublicKey);
    const bool bIsValidSignatureCandidateId = LabCrypto::VerifyDSA(CandidateId, SignatureCandidateId, PublicKey);
    return bIsValidSignatureVoterId && bIsValidSignatureCandidateId;
}

bool LabBallot::operator==(const LabBallot& Other) const
{
    return VoterId == Other.VoterId && CandidateId == Other.CandidateId;
}
