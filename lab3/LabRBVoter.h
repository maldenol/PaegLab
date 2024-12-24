#ifndef LAB3_LABRBVOTER_H
#define LAB3_LABRBVOTER_H

#include "LabCrypto.h"

struct LabRBVoter final
{
    unsigned VoterId = 0;
    unsigned RegistrationId = 0;
    LabCrypto::LabPublicKeyDSA PublicKey = {};
    LabCrypto::LabPrivateKeyDSA PrivateKey = {};
};

#endif //LAB3_LABRBVOTER_H
