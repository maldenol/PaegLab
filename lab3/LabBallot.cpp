#include "LabBallot.h"

LabBallot::LabBallot(const unsigned InRegistrationId)
    : RegistrationId{InRegistrationId}
{
}

unsigned LabBallot::GetRegistrationId() const
{
    return RegistrationId;
}

void LabBallot::SetCandidateId(const unsigned InCandidate)
{
    CandidateId = InCandidate;
}

unsigned LabBallot::GetCandidateId() const
{
    return CandidateId;
}

LabBallot LabBallot::Sign(const LabCrypto::LabPrivateKeyDSA& PrivateKey) const
{
    LabBallot SignedBallot = *this;

    SignedBallot.SignatureRegistrationId = LabCrypto::SignDSA(SignedBallot.RegistrationId, PrivateKey);
    SignedBallot.SignatureCandidateId = LabCrypto::SignDSA(SignedBallot.CandidateId, PrivateKey);

    return SignedBallot;
}

bool LabBallot::Verify(const LabCrypto::LabPublicKeyDSA& PublicKey) const
{
    const bool bIsRegistrationIdValid = LabCrypto::VerifyDSA(RegistrationId, SignatureRegistrationId, PublicKey);
    const bool bIsCandidateIdValid = LabCrypto::VerifyDSA(CandidateId, SignatureCandidateId, PublicKey);
    return bIsRegistrationIdValid && bIsCandidateIdValid;
}

LabBallotCipher LabBallot::Encrypt(const LabCrypto::LabPublicKeyEG& PublicKey) const
{
    LabBallotCipher BallotCipher;

    BallotCipher.RegistrationId = LabCrypto::EncryptEG(RegistrationId, PublicKey);
    BallotCipher.CandidateId = LabCrypto::EncryptEG(CandidateId, PublicKey);

    BallotCipher.SignatureRRegistrationId = LabCrypto::EncryptEG(SignatureRegistrationId.R, PublicKey);
    BallotCipher.SignatureSRegistrationId = LabCrypto::EncryptEG(SignatureRegistrationId.S, PublicKey);
    BallotCipher.SignatureRCandidateId = LabCrypto::EncryptEG(SignatureCandidateId.R, PublicKey);
    BallotCipher.SignatureSCandidateId = LabCrypto::EncryptEG(SignatureCandidateId.S, PublicKey);

    return BallotCipher;
}

LabBallot LabBallotCipher::Decrypt(const LabCrypto::LabPrivateKeyEG& PrivateKey) const
{
    LabBallot Ballot{0};

    Ballot.RegistrationId = LabCrypto::DecryptEG(RegistrationId, PrivateKey);
    Ballot.CandidateId = LabCrypto::DecryptEG(CandidateId, PrivateKey);

    Ballot.SignatureRegistrationId.R = LabCrypto::DecryptEG(SignatureRRegistrationId, PrivateKey);
    Ballot.SignatureRegistrationId.S = LabCrypto::DecryptEG(SignatureSRegistrationId, PrivateKey);
    Ballot.SignatureCandidateId.R = LabCrypto::DecryptEG(SignatureRCandidateId, PrivateKey);
    Ballot.SignatureCandidateId.S = LabCrypto::DecryptEG(SignatureSCandidateId, PrivateKey);

    return Ballot;
}
