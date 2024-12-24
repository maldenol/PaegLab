#include "LabCorruptElectionCommission.h"

#include "LabElectionCommission.h"

#include <iostream>

void LabCorruptElectionCommission::VoteInsteadOfAnotherVoter(LabElectionCommission& EC, const LabCrypto::LabPublicKeyRSA& EncryptKey, const size_t ECNum)
{
    const LabVoter& VictimVoter = EC.Voters[0];
    const unsigned VoterId = VictimVoter.GetVoterId();
    const unsigned CandidateId = EC.Candidates[0].CandidateId;

    std::cout << "EC " << EC.GetElectionCommissionId() << ": Voting instead of voter " << VoterId << " for candidate " << CandidateId << "\n";

    LabCrypto::LabPublicKeyDSA VerifyKey;
    LabCrypto::LabPrivateKeyDSA SignKey;
    LabCrypto::GenerateKeysDSA(VerifyKey, SignKey);
    const std::vector<LabBallot> Ballots = LabVoter::CreateBallotParts(VoterId, CandidateId, ECNum, EncryptKey, SignKey);

    EC.Vote(Ballots[0], true);
}

void LabCorruptElectionCommission::VoteInsteadOfAnotherVoterAll(std::vector<LabElectionCommission>& ECs, const LabCrypto::LabPublicKeyRSA& EncryptKey)
{
    const LabVoter& VictimVoter = ECs[0].Voters[0];
    const unsigned VoterId = VictimVoter.GetVoterId();
    const unsigned CandidateId = ECs[0].Candidates[0].CandidateId;

    std::cout << "ECs: Voting instead of voter " << VoterId << " for candidate " << CandidateId << "\n";

    LabCrypto::LabPublicKeyDSA VerifyKey;
    LabCrypto::LabPrivateKeyDSA SignKey;
    LabCrypto::GenerateKeysDSA(VerifyKey, SignKey);
    const std::vector<LabBallot> Ballots = LabVoter::CreateBallotParts(VoterId, CandidateId, ECs.size(), EncryptKey, SignKey);

    for (size_t Index = 0; Index < Ballots.size(); ++Index)
    {
        ECs[Index].Vote(Ballots[Index], true);
    }
}

void LabCorruptElectionCommission::DropVote(LabElectionCommission& EC)
{
    const auto Ballot = EC.Ballots.begin();
    std::cout << "EC " << EC.GetElectionCommissionId() << ": Dropping vote from voter " << Ballot->GetVoterId() << "\n";
    EC.Ballots.erase(Ballot);
}

void LabCorruptElectionCommission::DropVoteAll(std::vector<LabElectionCommission>& ECs)
{
    const auto Ballot = ECs[0].Ballots.begin();
    std::cout << "ECs: Dropping votes from voter " << Ballot->GetVoterId() << "\n";
    for (LabElectionCommission& EC : ECs)
    {
        EC.Ballots.erase(EC.Ballots.begin());
    }
}

void LabCorruptElectionCommission::DuplicateVote(LabElectionCommission& EC)
{
    const auto Ballot = EC.Ballots.begin();
    std::cout << "EC " << EC.GetElectionCommissionId() << ": Duplicating vote from voter " << Ballot->GetVoterId() << "\n";
    EC.Ballots.push_back(*Ballot);
}

void LabCorruptElectionCommission::DuplicateVoteAll(std::vector<LabElectionCommission>& ECs)
{
    const auto Ballot = ECs[0].Ballots.begin();
    std::cout << "ECs: Duplicating votes from voter " << Ballot->GetVoterId() << "\n";
    for (LabElectionCommission& EC : ECs)
    {
        EC.Ballots.push_back(*EC.Ballots.begin());
    }
}

void LabCorruptElectionCommission::SubstituteVote(LabElectionCommission& EC, const LabCrypto::LabPublicKeyRSA& EncryptKey, const size_t ECNum)
{
    const LabVoter& VictimVoter = EC.Voters[0];
    const unsigned VoterId = VictimVoter.GetVoterId();
    const unsigned CandidateId = EC.Candidates[0].CandidateId;

    std::cout << "EC " << EC.GetElectionCommissionId() << ": Substituting vote from " << VoterId << " ";
    std::cout << " to candidate " << CandidateId << "\n";

    LabCrypto::LabPublicKeyDSA VerifyKey;
    LabCrypto::LabPrivateKeyDSA SignKey;
    LabCrypto::GenerateKeysDSA(VerifyKey, SignKey);
    const std::vector<LabBallot> Ballots = LabVoter::CreateBallotParts(VoterId, CandidateId, ECNum, EncryptKey, SignKey);

    EC.Ballots.erase(EC.Ballots.begin());
    EC.Vote(Ballots[0], true);
}

void LabCorruptElectionCommission::SubstituteVoteAll(std::vector<LabElectionCommission>& ECs, const LabCrypto::LabPublicKeyRSA& EncryptKey)
{
    const LabVoter& VictimVoter = ECs[0].Voters[0];
    const unsigned VoterId = VictimVoter.GetVoterId();
    const unsigned CandidateId = ECs[0].Candidates[0].CandidateId;

    std::cout << "ECs: Substituting vote from " << VoterId << " ";
    std::cout << " to candidate " << CandidateId << "\n";

    LabCrypto::LabPublicKeyDSA VerifyKey;
    LabCrypto::LabPrivateKeyDSA SignKey;
    LabCrypto::GenerateKeysDSA(VerifyKey, SignKey);
    const std::vector<LabBallot> Ballots = LabVoter::CreateBallotParts(VoterId, CandidateId, ECs.size(), EncryptKey, SignKey);

    for (size_t Index = 0; Index < Ballots.size(); ++Index)
    {
        ECs[Index].Ballots.erase(ECs[Index].Ballots.begin());
        ECs[Index].Vote(Ballots[Index], true);
    }
}
