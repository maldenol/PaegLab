#include "LabVoter.h"

#include "LabBigInt.h"

#include <algorithm>
#include <iostream>

LabVoter::LabVoter(const unsigned InVoterId)
    : VoterId{InVoterId}
{
    LabCrypto::GenerateKeysDSA(VerifyKey, SignKey);
}

std::vector<LabBallot> LabVoter::CreateBallots(const unsigned CandidateId, const size_t BallotNum, const LabCrypto::LabPublicKeyRSA& EncryptKey)
{
    Ballots.clear();
    Ballots.reserve(1 + BallotNum);

    const LabBallot WholeBallot = CreateWholeBallot(VoterId, CandidateId);
    Ballots.push_back(WholeBallot);

    const std::vector<LabBallot> BallotParts = CreateBallotParts(VoterId, CandidateId, BallotNum, EncryptKey, SignKey);
    Ballots.insert(Ballots.end(), BallotParts.begin(), BallotParts.end());

    return BallotParts;
}

void LabVoter::VerifyOwnBallotPresence(const std::vector<LabBallot>& Ballots, const bool bVerifySignature) const
{
    const std::vector<LabBallot>& ThisBallots = this->Ballots;
    const auto OwnBallot = std::find_if(Ballots.begin(), Ballots.end(),
                                        [&ThisBallots](const LabBallot& Ballot) -> bool
                                        {
                                            return std::ranges::any_of(ThisBallots.begin(), ThisBallots.end(),
                                                                       [&Ballot](const LabBallot& ThisBallot) -> bool
                                                                       {
                                                                           return ThisBallot == Ballot;
                                                                       });
                                        });

    if (OwnBallot == Ballots.end())
    {
        std::cout << "Voter " << VoterId << ": Error: Own ballot is not present\n";
        return;
    }

    if (std::find_if(Ballots.begin(), Ballots.end(),
                     [OwnBallot](const LabBallot& Ballot) -> bool
                     {
                         return *OwnBallot == Ballot;
                     }) == Ballots.end())
    {
        std::cout << "Voter " << VoterId << ": Error: Own ballot has different CandidateId\n";
        return;
    }

    if (bVerifySignature && !OwnBallot->Verify(VerifyKey))
    {
        std::cout << "Voter " << VoterId << ": Error: Own ballot signature is not valid\n";
        return;
    }

    std::cout << "Voter " << VoterId << ": Own ballot is present";
    if (bVerifySignature)
    {
        std::cout << " and signature is valid";
    }
    std::cout << "\n";
}

unsigned LabVoter::GetVoterId() const
{
    return VoterId;
}

const LabCrypto::LabPublicKeyDSA& LabVoter::GetVerifyKey() const
{
    return VerifyKey;
}

bool LabVoter::operator==(const LabVoter& Other) const
{
    return VoterId == Other.VoterId;
}

LabBallot LabVoter::CreateWholeBallot(const unsigned VoterId, const unsigned CandidateId)
{
    LabBallot WholeBallot{VoterId};
    WholeBallot.SetCandidateId(CandidateId);
    return WholeBallot;
}

std::vector<LabBallot> LabVoter::CreateBallotParts(const unsigned VoterId, const unsigned CandidateId, const size_t BallotNum, const LabCrypto::LabPublicKeyRSA& EncryptKey, const LabCrypto::LabPrivateKeyDSA& SignKey)
{
    std::cout << "\tCreating ballot parts for voter " << VoterId << " for candidate " << CandidateId << "\n";

    std::vector<unsigned> CandidateIdDivisors;
    while (true)
    {
        CandidateIdDivisors.clear();
        CandidateIdDivisors.reserve(BallotNum);

        unsigned DividedCandidateId = CandidateId;
        while (DividedCandidateId > 1)
        {
            if (CandidateIdDivisors.size() == BallotNum - 1)
            {
                CandidateIdDivisors.push_back(DividedCandidateId);
                break;
            }

            for (size_t Index = 0; Index < CandidateId; ++Index)
            {
                const unsigned Divisor = LabBigInt::RandomRange(2, DividedCandidateId - 1);

                if (DividedCandidateId % Divisor == 0)
                {
                    DividedCandidateId /= Divisor;
                    CandidateIdDivisors.push_back(Divisor);
                    break;
                }
            }
        }

        if (CandidateIdDivisors.size() == BallotNum)
        {
            break;
        }
    }

    std::cout << "\tCandidateId divisors:";
    for (const unsigned CandidateIdDivisor : CandidateIdDivisors)
    {
        std::cout << " " << CandidateIdDivisor;
    }
    std::cout << "\n";

    std::vector<LabBallot> BallotParts;
    BallotParts.reserve(BallotNum);
    for (const unsigned CandidateIdDivisor : CandidateIdDivisors)
    {
        LabBallot Ballot{VoterId};
        Ballot.SetCandidateId(CandidateIdDivisor);

        const LabBallot EncryptedBallot = Ballot.Encrypt(EncryptKey);

        const LabBallot SignedEncryptedBallot = EncryptedBallot.Sign(SignKey);

        BallotParts.push_back(SignedEncryptedBallot);
    }
    return BallotParts;
}

std::vector<LabBallot> LabVoter::CreateBallotsForVoter(const unsigned CandidateId, const size_t BallotNum, const LabCrypto::LabPublicKeyRSA& EncryptKey, const unsigned InVoterId)
{
    std::cout << "Voter " << VoterId << ": Voting instead of voter " << InVoterId << "\n";
    const unsigned TempVoterId = VoterId;
    VoterId = InVoterId;
    const std::vector<LabBallot> Result = CreateBallots(CandidateId, BallotNum, EncryptKey);
    VoterId = TempVoterId;
    return Result;
}
