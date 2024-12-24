#ifndef LAB3_LABPUBLICVOTER_H
#define LAB3_LABPUBLICVOTER_H

#include "LabCrypto.h"

struct LabPublicVoter final
{
    unsigned RegistrationId = 0;
    LabCrypto::LabPublicKeyDSA PublicKey = {};
    bool bHaveVoted = false;
};

#endif //LAB3_LABPUBLICVOTER_H
