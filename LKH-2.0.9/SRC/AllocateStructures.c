#include "Segment.h"
#include "LKH.h"
#include "Heap.h"
#include "Sequence.h"

/*      
 * The AllocateStructures function allocates all necessary 
 * structures except nodes and candidates.
 */

#define Free(s) { free(s); s = 0; }

void AllocateStructures()
{
    int i, K;

    Free(Heap);
    Free(BestTour);
    Free(BetterTour);
    Free(HTable);
    Free(Rand);
    Free(CacheSig);
    Free(CacheVal);
    Free(T);
    Free(G);
    Free(t);
    Free(p);
    Free(q);
    Free(SwapStack);
    Free(tSaved);

    MakeHeap(Dimension);
    assert(BestTour = (int *) calloc(1 + Dimension, sizeof(int)));
    assert(BetterTour = (int *) calloc(1 + Dimension, sizeof(int)));
    assert(HTable = (HashTable *) malloc(sizeof(HashTable)));
    HashInitialize((HashTable *) HTable);
    SRandom(Seed);
    assert(Rand = (unsigned *)
           malloc((Dimension + 1) * sizeof(unsigned)));
    for (i = 1; i <= Dimension; i++)
        Rand[i] = Random();
    SRandom(Seed);
    if (WeightType != EXPLICIT) {
        for (i = 0; (1 << i) < (Dimension << 1); i++);
        i = 1 << i;
        assert(CacheSig = (int *) calloc(i, sizeof(int)));
        assert(CacheVal = (int *) calloc(i, sizeof(int)));
        CacheMask = i - 1;
    }
    AllocateSegments();
    K = MoveType;
    if (SubsequentMoveType > K)
        K = SubsequentMoveType;
    assert(T = (Node **) malloc((1 + 2 * K) * sizeof(Node *)));
    assert(G = (GainType *) malloc(2 * K * sizeof(GainType)));
    assert(t = (Node **) malloc(6 * K * sizeof(Node *)));
    assert(tSaved = (Node **) malloc((1 + 2 * K) * sizeof(Node *)));
    assert(p = (int *) malloc(6 * K * sizeof(int)));
    assert(q = (int *) malloc(6 * K * sizeof(int)));
    assert(incl = (int *) malloc(6 * K * sizeof(int)));
    assert(cycle = (int *) malloc(6 * K * sizeof(int)));
    assert(SwapStack =
           (SwapRecord *) malloc((MaxSwaps + 6 * K) * sizeof(SwapRecord)));
}

/*      
 * The AllocateSegments function allocates the segments of the two-level tree.
 */

void AllocateSegments()
{
    Segment *S = 0, *SPrev;
    SSegment *SS = 0, *SSPrev;
    int i;

    FreeSegments();
#ifdef THREE_LEVEL_TREE
    GroupSize = (int) pow((double) Dimension, 1.0 / 3.0);
#elif defined TWO_LEVEL_TREE
    GroupSize = (int) sqrt((double) Dimension);
#else
    GroupSize = Dimension;
#endif
    Groups = 0;
    for (i = Dimension, SPrev = 0; i > 0; i -= GroupSize, SPrev = S) {
        assert(S = (Segment *) malloc(sizeof(Segment)));
        S->Rank = ++Groups;
        if (!SPrev)
            FirstSegment = S;
        else
            SLink(SPrev, S);
    }
    SLink(S, FirstSegment);
#ifdef THREE_LEVEL_TREE
    SGroupSize = sqrt((double) Groups);
#else
    SGroupSize = Dimension;
#endif
    SGroups = 0;
    for (i = Groups, SSPrev = 0; i > 0; i -= SGroupSize, SSPrev = SS) {
        SS = (SSegment *) malloc(sizeof(SSegment));
        SS->Rank = ++SGroups;
        if (!SSPrev)
            FirstSSegment = SS;
        else
            SLink(SSPrev, SS);
    }
    SLink(SS, FirstSSegment);
}
