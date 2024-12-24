#ifndef LAB1_LABVOTER_H
#define LAB1_LABVOTER_H

#include "LabRSA.h"

struct LabVoter final
{
    unsigned VoterId = 0;
    LabRSA::LabPublicKey PublicKey;
    LabRSA::LabPrivateKey PrivateKey;
    unsigned Gamma;
};

#endif //LAB1_LABVOTER_H
