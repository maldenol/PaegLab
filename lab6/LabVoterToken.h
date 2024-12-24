#ifndef LAB6_LABVOTERTOKEN_H
#define LAB6_LABVOTERTOKEN_H

#include "LabCrypto.h"

struct LabVoterToken final
{
    unsigned RegistrationId = 0;
    LabCrypto::LabPublicKeyBBS PublicKeyBBS;
};

#endif //LAB6_LABVOTERTOKEN_H
