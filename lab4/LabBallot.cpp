#include "LabBallot.h"

void LabBallot::SetCandidateId(const unsigned InCandidateId)
{
    CandidateId = {LabBigInt{static_cast<int>(InCandidateId)}};
}

unsigned LabBallot::GetCandidateId() const
{
    if (CandidateId.size() != 1)
    {
        return 0;
    }

    return CandidateId[0].ToInt();
}

std::vector<LabBigInt>& LabBallot::GetCandidateIdMut()
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

LabBallot LabBallot::Sign(const LabCrypto::LabPrivateKeyEG& PrivateKey) const
{
    LabBallot SignedBallot = *this;

    SignedBallot.SignatureCandidateId = LabCrypto::SignEG(SignedBallot.CandidateId, PrivateKey);

    return SignedBallot;
}

bool LabBallot::Verify(const LabCrypto::LabPublicKeyEG& PublicKey) const
{
    const bool bIsValidSignatureCandidateId = LabCrypto::VerifyEG(CandidateId, SignatureCandidateId, PublicKey);
    return bIsValidSignatureCandidateId;
}

bool LabBallot::operator==(const LabBallot& Other) const
{
    return CandidateId == Other.CandidateId;
}
