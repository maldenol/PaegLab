#include "LabBallot.h"

void LabBallot::SetCandidateId(const unsigned InCandidateId)
{
    CandidateId = InCandidateId;
}

unsigned LabBallot::GetCandidateId() const
{
    return CandidateId.ToInt();
}

void LabBallot::SetRegistrationId(const unsigned InRegistrationId)
{
    RegistrationId = InRegistrationId;
}

unsigned LabBallot::GetRegistrationId() const
{
    return RegistrationId.ToInt();
}

std::vector<LabBigInt> LabBallot::Encrypt(const LabCrypto::LabPublicKeyBBS& PublicKeyBBS, const LabCrypto::LabPublicKeyEG& PublicKeyEG) const
{
    LabBigInt X0;
    const LabBigInt CandidateIdBBS = LabCrypto::EncryptBBS(CandidateId, PublicKeyBBS, X0);

    const LabCrypto::LabCipherEG CandidateIdCipher = LabCrypto::EncryptEG(CandidateIdBBS, PublicKeyEG);
    const LabCrypto::LabCipherEG X0Cipher = LabCrypto::EncryptEG(X0, PublicKeyEG);
    const LabCrypto::LabCipherEG RegistrationIdCipher = LabCrypto::EncryptEG(RegistrationId, PublicKeyEG);

    return
    {
            CandidateIdCipher.A,
            CandidateIdCipher.B,
            X0Cipher.A,
            X0Cipher.B,
            RegistrationIdCipher.A,
            RegistrationIdCipher.B,
    };
}

LabBallot LabBallot::Decrypt(const std::vector<LabBigInt>& BallotCipher, const LabGetPrivateKeyBBSCallback& GetPrivateKeyBBSCallback, const LabCrypto::LabPrivateKeyEG& PrivateKeyEG)
{
    const LabCrypto::LabCipherEG CandidateIdCipher = {BallotCipher[0], BallotCipher[1]};
    const LabCrypto::LabCipherEG X0Cipher = {BallotCipher[2], BallotCipher[3]};
    const LabCrypto::LabCipherEG RegistrationIdCipher = {BallotCipher[4], BallotCipher[5]};

    const LabBigInt CandidateIdBBS = LabCrypto::DecryptEG(CandidateIdCipher, PrivateKeyEG);
    const LabBigInt X0 = LabCrypto::DecryptEG(X0Cipher, PrivateKeyEG);
    const LabBigInt RegistrationId = LabCrypto::DecryptEG(RegistrationIdCipher, PrivateKeyEG);

    const LabCrypto::LabPrivateKeyBBS& PrivateKeyBBS = GetPrivateKeyBBSCallback(RegistrationId.ToInt());
    const LabBigInt CandidateId = LabCrypto::DecryptBBS(CandidateIdBBS, PrivateKeyBBS, X0);

    LabBallot Ballot;
    Ballot.CandidateId = CandidateId;
    Ballot.RegistrationId = RegistrationId;
    return Ballot;
}
