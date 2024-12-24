cmake_minimum_required(VERSION 3.26)
project(lab2)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab2 main.cpp
        LabBallot.cpp
        LabBallot.h
        LabCentralElectionCommission.cpp
        LabCentralElectionCommission.h
        LabVoter.h
        LabCandidate.h
        LabRSA.cpp
        LabRSA.h
        LabCorruptCentralElectionCommission.cpp
        LabCorruptCentralElectionCommission.h
        LabMaskedBallotSetArray.cpp
        LabMaskedBallotSetArray.h
        LabLocalVoter.h
)
