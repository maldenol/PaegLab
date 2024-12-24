#ifndef LAB6_LABECTOKEN_H
#define LAB6_LABECTOKEN_H

#include "LabCrypto.h"

struct LabECToken final
{
    unsigned RegistrationId = 0;
    LabCrypto::LabPublicKeyBBS PublicKeyBBS;
    LabCrypto::LabPrivateKeyBBS PrivateKeyBBS;
};

#endif //LAB6_LABECTOKEN_H
