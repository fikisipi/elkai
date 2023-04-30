#include "LKH.h"
#include "Genetic.h"
#include "BIT.h"

static int *ReadableTour = 0;
static int ReadableTourSize = 0;
static int ReadableTourAllocated = 100;

static void ReadableTourPut(int x) {
    if(ReadableTour == 0) {
        ReadableTour = (int *) malloc(sizeof(int) * ReadableTourAllocated);
    }
    if(ReadableTourSize == ReadableTourAllocated) {
        ReadableTourAllocated *= 2;
        ReadableTour = (int *) realloc(ReadableTour, sizeof(int) * ReadableTourAllocated);
    }
    ReadableTour[ReadableTourSize] = x;
    ReadableTourSize++;
}

static void ReadableTourClear() {
    ReadableTourSize = 0;
    ReadableTourAllocated = 100;
    if(ReadableTour != 0) {
        free(ReadableTour);
        ReadableTour = 0;
    }
}

void TheTour(int *Tour, GainType Cost)
{
    int i, j, k, n, Forward, a, b;
    time_t Now;

    ReadableTourClear();

    if (CurrentPenalty != 0 && MTSPObjective == -1 &&
        ProblemType != CCVRP && ProblemType != TRP &&
        ProblemType != MLP)
        return;

    Now = time(&Now);

    n = DimensionSaved;
    for (i = 1; i < n && Tour[i] != MTSPDepot; i++);
    Forward = Asymmetric ||
        Tour[i < n ? i + 1 : 1] < Tour[i > 1 ? i - 1 : Dimension];
    for (j = 1; j <= n; j++) {
        if ((a = Tour[i]) <= n)
            ReadableTourPut(ProblemType != STTSP ? a : NodeSet[a].OriginalId);
        if (Forward) {
            if (++i > n)
                i = 1;
        } else if (--i < 1)
            i = n;
        if (ProblemType == STTSP) {
            b = Tour[i];
            for (k = 0; k < NodeSet[a].PathLength[b]; k++)
                ReadableTourPut(NodeSet[a].Path[b][k]);
        }
    }
}

#define GB_STRING_IMPLEMENTATION
#include "gb_string.h"

extern void ClearLines();
extern void WriteLine(gbString str);

void ElkaiSolveProblem(gbString params, gbString problem, int *tourSize, int **tourPtr) {
    ClearLines();
    WriteLine(params);

    ReadParameters();

    GainType Cost, OldOptimum;
    double Time, LastTime;
    StartTime = LastTime = GetTime();
    MaxMatrixDimension = 20000;
    MergeWithTour =
        Recombination == GPX2 ? MergeWithTourGPX2 :
        Recombination == CLARIST ? MergeWithTourCLARIST :
                                   MergeWithTourIPT;

    WriteLine(problem);

    ReadProblem();
    
    ClearLines();

    AllocateStructures();
    CreateCandidateSet();
    InitializeStatistics();

    if (Norm != 0 || Penalty) {
        Norm = 9999;
        BestCost = PLUS_INFINITY;
        BestPenalty = CurrentPenalty = PLUS_INFINITY;
    } else {
        /* The ascent has solved the problem! */
        Optimum = BestCost = (GainType) LowerBound;
        UpdateStatistics(Optimum, GetTime() - LastTime);
        RecordBetterTour();
        RecordBestTour();
        CurrentPenalty = PLUS_INFINITY;
        BestPenalty = CurrentPenalty = Penalty ? Penalty() : 0;
        TheTour(BestTour, BestCost);
        Runs = 0;
    }

    /* Find a specified number (Runs) of local optima */

    for (Run = 1; Run <= Runs; Run++) {
        LastTime = GetTime();
        if (LastTime - StartTime >= TotalTimeLimit) {
            if (TraceLevel >= 1)
                printff("*** Time limit exceeded ***\n");
            Run--;
            break;
        }
        Cost = FindTour();      /* using the Lin-Kernighan heuristic */
        if (MaxPopulationSize > 1 && !TSPTW_Makespan) {
            /* Genetic algorithm */
            int i;
            for (i = 0; i < PopulationSize; i++) {
                Cost = MergeTourWithIndividual(i);
            }
            if (!HasFitness(CurrentPenalty, Cost)) {
                if (PopulationSize < MaxPopulationSize) {
                    AddToPopulation(CurrentPenalty, Cost);
                    if (TraceLevel >= 1)
                        PrintPopulation();
                } else if (SmallerFitness(CurrentPenalty, Cost,
                                          PopulationSize - 1)) {
                    i = ReplacementIndividual(CurrentPenalty, Cost);
                    ReplaceIndividualWithTour(i, CurrentPenalty, Cost);
                    if (TraceLevel >= 1)
                        PrintPopulation();
                }
            }
        } else if (Run > 1 && !TSPTW_Makespan)
            Cost = MergeTourWithBestTour();
        if (CurrentPenalty < BestPenalty ||
            (CurrentPenalty == BestPenalty && Cost < BestCost)) {
            BestPenalty = CurrentPenalty;
            BestCost = Cost;
            RecordBetterTour();
            RecordBestTour();
            TheTour(BestTour, BestCost);
        }
        OldOptimum = Optimum;
        if (!Penalty ||
            (MTSPObjective != MINMAX && MTSPObjective != MINMAX_SIZE)) {
            if (CurrentPenalty == 0 && Cost < Optimum)
                Optimum = Cost;
        } else if (CurrentPenalty < Optimum)
            Optimum = CurrentPenalty;
        if (Optimum < OldOptimum) {
            if (FirstNode->InputSuc) {
                Node *N = FirstNode;
                while ((N = N->InputSuc = N->Suc) != FirstNode);
            }
        }
        Time = fabs(GetTime() - LastTime);
        UpdateStatistics(Cost, Time);
        if (StopAtOptimum && MaxPopulationSize >= 1) {
            if (ProblemType != CCVRP && ProblemType != TRP &&
                ProblemType != MLP &&
                MTSPObjective != MINMAX &&
                MTSPObjective != MINMAX_SIZE ?
                CurrentPenalty == 0 && Cost == Optimum :
                CurrentPenalty == Optimum) {
                Runs = Run;
                break;
            }
        }
        if (PopulationSize >= 2 &&
            (PopulationSize == MaxPopulationSize ||
             Run >= 2 * MaxPopulationSize) && Run < Runs) {
            Node *N;
            int Parent1, Parent2;
            Parent1 = LinearSelection(PopulationSize, 1.25);
            do
                Parent2 = LinearSelection(PopulationSize, 1.25);
            while (Parent2 == Parent1);
            ApplyCrossover(Parent1, Parent2);
            N = FirstNode;
            do {
                if (ProblemType != HCP && ProblemType != HPP) {
                    int d = C(N, N->Suc);
                    AddCandidate(N, N->Suc, d, INT_MAX);
                    AddCandidate(N->Suc, N, d, INT_MAX);
                }
                N = N->InitialSuc = N->Suc;
            } while (N != FirstNode);
        }
        SRandom(++Seed);
    }

    *tourSize = ReadableTourSize;

    if(tourPtr != 0) {
        *tourPtr = ReadableTour;
    }
}

int ElkaiDeprecatedSolve(int dimension, float *weights, int *tour, int runs)
{
    GainType Cost, OldOptimum;
    double Time, LastTime;
    ClearLines();
    WriteLine(gb_make_string("TRACE_LEVEL = 0\nPROBLEM_FILE = :stdin:\n"));

    char runsStr[100];
    sprintf(runsStr, "RUNS = %d\n", runs);
    WriteLine(gb_make_string(runsStr));

    ReadParameters();

    StartTime = LastTime = GetTime();
    MaxMatrixDimension = 20000;
    MergeWithTour =
        Recombination == GPX2 ? MergeWithTourGPX2 :
        Recombination == CLARIST ? MergeWithTourCLARIST :
                                   MergeWithTourIPT;
    
    char dimensionStr[100];
    sprintf(dimensionStr, "DIMENSION : %d\n", dimension);
    WriteLine(gb_make_string("TYPE: ATSP\n"));
    WriteLine(gb_make_string(dimensionStr));
    WriteLine(gb_make_string("EDGE_WEIGHT_TYPE: EXPLICIT\nEDGE_WEIGHT_FORMAT: FULL_MATRIX\nEDGE_WEIGHT_SECTION\n"));

    for(int y = 0; y < dimension; y++) {
        for(int x = 0; x < dimension; x++) {
            char weightStr[64];
            sprintf(weightStr, "%f ", weights[y * dimension + x]);
            WriteLine(gb_make_string(weightStr));
        }
        WriteLine(gb_make_string("\n"));
    }

    ReadProblem();
    AllocateStructures();
    CreateCandidateSet();
    InitializeStatistics();

    if (Norm != 0 || Penalty) {
        Norm = 9999;
        BestCost = PLUS_INFINITY;
        BestPenalty = CurrentPenalty = PLUS_INFINITY;
    } else {
        /* The ascent has solved the problem! */
        Optimum = BestCost = (GainType) LowerBound;
        UpdateStatistics(Optimum, GetTime() - LastTime);
        RecordBetterTour();
        RecordBestTour();
        CurrentPenalty = PLUS_INFINITY;
        BestPenalty = CurrentPenalty = Penalty ? Penalty() : 0;
        TheTour(BestTour, BestCost);
        Runs = 0;
    }

    /* Find a specified number (Runs) of local optima */

    for (Run = 1; Run <= Runs; Run++) {
        LastTime = GetTime();
        if (LastTime - StartTime >= TotalTimeLimit) {
            if (TraceLevel >= 1)
                printff("*** Time limit exceeded ***\n");
            Run--;
            break;
        }
        Cost = FindTour();      /* using the Lin-Kernighan heuristic */
        if (MaxPopulationSize > 1 && !TSPTW_Makespan) {
            /* Genetic algorithm */
            int i;
            for (i = 0; i < PopulationSize; i++) {
                Cost = MergeTourWithIndividual(i);
            }
            if (!HasFitness(CurrentPenalty, Cost)) {
                if (PopulationSize < MaxPopulationSize) {
                    AddToPopulation(CurrentPenalty, Cost);
                    if (TraceLevel >= 1)
                        PrintPopulation();
                } else if (SmallerFitness(CurrentPenalty, Cost,
                                          PopulationSize - 1)) {
                    i = ReplacementIndividual(CurrentPenalty, Cost);
                    ReplaceIndividualWithTour(i, CurrentPenalty, Cost);
                    if (TraceLevel >= 1)
                        PrintPopulation();
                }
            }
        } else if (Run > 1 && !TSPTW_Makespan)
            Cost = MergeTourWithBestTour();
        if (CurrentPenalty < BestPenalty ||
            (CurrentPenalty == BestPenalty && Cost < BestCost)) {
            BestPenalty = CurrentPenalty;
            BestCost = Cost;
            RecordBetterTour();
            RecordBestTour();
            TheTour(BestTour, BestCost);
        }
        OldOptimum = Optimum;
        if (!Penalty ||
            (MTSPObjective != MINMAX && MTSPObjective != MINMAX_SIZE)) {
            if (CurrentPenalty == 0 && Cost < Optimum)
                Optimum = Cost;
        } else if (CurrentPenalty < Optimum)
            Optimum = CurrentPenalty;
        if (Optimum < OldOptimum) {
            if (FirstNode->InputSuc) {
                Node *N = FirstNode;
                while ((N = N->InputSuc = N->Suc) != FirstNode);
            }
        }
        Time = fabs(GetTime() - LastTime);
        UpdateStatistics(Cost, Time);
        if (StopAtOptimum && MaxPopulationSize >= 1) {
            if (ProblemType != CCVRP && ProblemType != TRP &&
                ProblemType != MLP &&
                MTSPObjective != MINMAX &&
                MTSPObjective != MINMAX_SIZE ?
                CurrentPenalty == 0 && Cost == Optimum :
                CurrentPenalty == Optimum) {
                Runs = Run;
                break;
            }
        }
        if (PopulationSize >= 2 &&
            (PopulationSize == MaxPopulationSize ||
             Run >= 2 * MaxPopulationSize) && Run < Runs) {
            Node *N;
            int Parent1, Parent2;
            Parent1 = LinearSelection(PopulationSize, 1.25);
            do
                Parent2 = LinearSelection(PopulationSize, 1.25);
            while (Parent2 == Parent1);
            ApplyCrossover(Parent1, Parent2);
            N = FirstNode;
            do {
                if (ProblemType != HCP && ProblemType != HPP) {
                    int d = C(N, N->Suc);
                    AddCandidate(N, N->Suc, d, INT_MAX);
                    AddCandidate(N->Suc, N, d, INT_MAX);
                }
                N = N->InitialSuc = N->Suc;
            } while (N != FirstNode);
        }
        SRandom(++Seed);
    }

    if(tour != 0) {
        for(int M = 0; M < ReadableTourSize; M++) {
            tour[M] = ReadableTour[M];
        }
    }

    return 0;
}


/*
 * This file contains the main function of the program.
 */

int main(int argc, char * argv[])
{
    printf("This is a test.");
    int tourSize;
    int *tour;
    gbString params = gb_make_string("RUNS = 1\nPROBLEM_FILE = :stdin:\n");
    gbString problem = gb_make_string(
        "TYPE : TSP\nDIMENSION : 4\nEDGE_WEIGHT_TYPE : EUC_2D\nNODE_COORD_SECTION\n"
        "1 0.0 0.0\n"
        "2 10.0 -5.3\n"
        "3 1.0 -2.1\n"
        "4 11.0 -3.0\n"
        "EOF\n"
        );
    ElkaiSolveProblem(params, problem, &tourSize, &tour);
    gb_free_string(params);
    gb_free_string(problem);
    for(int i = 0; i < tourSize; i++) {
        printf("%d ", tour[i]);
    }
}
