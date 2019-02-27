#include "sheddingCluster.hpp"

int main()
{
    SheddingClusterPartialOrder SCPO;
    SCPO.loadLableInfo("lableInfo.txt");
    SCPO.sortSheddingClusters();
    SCPO.print();

    return 0;
}
