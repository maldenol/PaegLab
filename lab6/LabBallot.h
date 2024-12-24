#ifndef LAB6_LABBALLOT_H
#define LAB6_LABBALLOT_H

#include "LabCrypto.h"

#include <functional>

class LabBallot final
{
public:
    using LabGetPrivateKeyBBSCallback = std::function<LabCrypto::LabPrivateKeyBBS(const unsigned RegistrationId)>;

    void SetCandidateId(const unsigned InCandidateId);
    unsigned GetCandidateId() const;
    void SetRegistrationId(const unsigned InRegistrationId);
    unsigned GetRegistrationId() const;

    std::vector<LabBigInt> Encrypt(const LabCrypto::LabPublicKeyBBS& PublicKeyBBS, const LabCrypto::LabPublicKeyEG& PublicKeyEG) const;
    static LabBallot Decrypt(const std::vector<LabBigInt>& BallotCipher, const LabGetPrivateKeyBBSCallback& GetPrivateKeyBBSCallback, const LabCrypto::LabPrivateKeyEG& PrivateKeyEG);

private:
    LabBigInt CandidateId;
    LabBigInt RegistrationId;
};

#endif //LAB6_LABBALLOT_H
