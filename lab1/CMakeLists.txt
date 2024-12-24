cmake_minimum_required(VERSION 3.26)
project(lab1)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab1 main.cpp
        LabBallotPaper.cpp
        LabBallotPaper.h
        LabCentralElectionCommission.cpp
        LabCentralElectionCommission.h
        LabVoter.h
        LabCandidate.h
        LabRSA.cpp
        LabRSA.h
        LabCorruptCentralElectionCommission.cpp
        LabCorruptCentralElectionCommission.h
)
