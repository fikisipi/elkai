#include "LKH.h"
#include "Segment.h"

GainType Penalty_CVRPTW()
{
    static Node *StartRoute = 0;
    Node *N, *NextN, *CurrentRoute;
    GainType CostSum, DemandSum, P = 0;
    int Forward = SUCC(Depot)->Id != Depot->Id + DimensionSaved;

    if (!StartRoute)
        StartRoute = Depot;
    if (StartRoute->Id > DimensionSaved)
        StartRoute -= DimensionSaved;
    N = StartRoute;
    do {
        int Size = 0;
        CurrentRoute = N;
        CostSum = DemandSum = 0;
        if (N->Earliest > 0)
            CostSum = N->Earliest;
        do {
            if (N->Id <= Dim && N != Depot) {
                if ((DemandSum += N->Demand) > Capacity)
                    P += DemandSum - Capacity;
                if (CostSum < N->Earliest)
                    CostSum = N->Earliest;
                if (CostSum > N->Latest)
                    P += CostSum - N->Latest;
                if (P > CurrentPenalty ||
                    (P == CurrentPenalty && CurrentGain <= 0)) {
                    StartRoute = CurrentRoute;
                    return CurrentPenalty + (CurrentGain > 0);
                }
                CostSum += N->ServiceTime;
                Size++;
            }
            NextN = Forward ? SUCC(N) : PREDD(N);
            CostSum += (C(N, NextN) - N->Pi - NextN->Pi) / Precision;
            N = Forward ? SUCC(NextN) : PREDD(NextN);
        } while (N->DepotId == 0);
        if (MTSPMinSize >= 1 && Size < MTSPMinSize)
            P += MTSPMinSize - Size;
        if (Size > MTSPMaxSize)
            P += Size - MTSPMaxSize;
        if (CostSum > Depot->Latest &&
            ((P += CostSum - Depot->Latest) > CurrentPenalty ||
             (P == CurrentPenalty && CurrentGain <= 0))) {
            StartRoute = CurrentRoute;
            return CurrentPenalty + (CurrentGain > 0);
        }
    } while (N != StartRoute);
    return P;
}
