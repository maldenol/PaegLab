#ifndef LAB3_LABLOCALVOTER_H
#define LAB3_LABLOCALVOTER_H

#include "LabBallot.h"
#include "LabCrypto.h"

struct LabLocalVoter final
{
    unsigned VoterId = 0;
    unsigned RegistrationId = 0;
    LabCrypto::LabPrivateKeyDSA PrivateKey = {};
    LabBallot Ballot = LabBallot{0};
};

#endif //LAB3_LABLOCALVOTER_H
