// CPP code for Dynamic Programming based 
// solution for 0-1 Knapsack problem 
#include <bits/stdc++.h> 
#include <iostream>
#include <set>

using namespace std;

// A utility function that returns maximum of two integers 
int max(int a, int b) { return (a > b) ? a : b; } 
int min(int a, int b) { return (a < b) ? a : b; } 

// Prints the items which are put in a knapsack of capacity W 
void printknapSack(int W, int wt[], int val[], int name[], int n, set<int> &keepingPMSet) 
{ 
	int i, w; 
	int K[n + 1][W + 1]; 
    for(int i=0; i<=n; ++i)
        for(int j=W; j<=W; ++j)
            K[i][j] = 6000;

	// Build table K[][] in bottom up manner 
	for (i = 0; i <= n; i++) { 
		for (w = 0; w <= W; w++) { 
			if (i == 0 || w == 0) 
				K[i][w] = 0; 
			else if (wt[i - 1] <= w) 
				K[i][w] = max(val[i - 1] + 
					K[i - 1][w - wt[i - 1]], K[i - 1][w]); 
			else
				K[i][w] = K[i - 1][w]; 
		} 
	} 

	// stores the result of Knapsack 
	int res = K[n][W];	 
	printf("%d\n", res); 

	
	w = W; 
	for (i = n; i > 0 && res > 0; i--) { 
		
		// either the result comes from the top 
		// (K[i-1][w]) or from (val[i-1] + K[i-1] 
		// [w-wt[i-1]]) as in Knapsack table. If 
		// it comes from the latter one/ it means 
		// the item is included. 
		if (res == K[i - 1][w]) 
			continue;		 
		else { 

			// This item is included. 
			//printf("%d ", wt[i - 1]); 
			printf("%d ", name[i-1]); 
            keepingPMSet.insert(name[i-1]);
			
			// Since this weight is included its 
			// value is deducted 
			res = res - val[i - 1]; 
			w = w - wt[i - 1]; 
		} 
	} 

} 

// Driver code 
int main() 
{ 
	int val[19]; 
	int wt[19]; 
    int name[19];
    bool PMBookArray[21];
    for(int i = 0; i<=20; ++i)
        PMBookArray[i] = false;
    for(int i=0; i<19; ++i)
        name[i] = i+2;

    for(int i=0; i<19; ++i)
    {
        int pc = i+2;
        if(pc <11)
            val[i] = pc - 1;
        if(pc >=11)
            val[i] = 1;
        if(pc <= 11)
            wt[i] = pc -1;
        if(pc > 11)
            wt[i] = 20-pc+1;
    }

    
	int n = sizeof(val) / sizeof(val[0]); 
	
    set<int> PMBooking;
    for(int W = 1; W<=40; ++W)
    {
        cout << "keeping ratio: " << W << endl;
        printknapSack(W, wt, val, name,n, PMBooking); 
        cout << endl;

    //for(auto m: PMBooking)
    //    cout << m << " " << flush;
    //cout << endl;

    //for(int i=2; i<=20; ++i)
    //    cout << i << "--" <<  PMBooking.count(i) << endl;

    cout << "min " << *PMBooking.begin() << endl;
    cout << "max " << *PMBooking.rbegin() << endl;

    for(auto m: PMBooking)
        PMBookArray[m] = true;

    cout << "in PMBookArray " << endl;
    for(int i = 0; i<=20; ++i)
    {
        if(PMBookArray[i])
            cout << i  << " " << flush;
    }
    cout << endl;

    cout << "==============================" << endl;
    PMBooking.clear();
    for(auto &m: PMBookArray)
        m=false;
        

    }
	return 0; 
} 

