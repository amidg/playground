#include <iostream>
#include <deque> 
using namespace std;

void printKMax(int arr[], int n, int k){
	//Write your code here.
    /*
    Number of sections where we need to derive max value is n-k+1
    5 - 2 + 1 = 4
    7 - 4 + 1 = 4
    */
    std::deque<int> result;
    int max_pairs = n-k+1;
    int i=0, j=0;
    int temp_max = 0;
    while (i < max_pairs) {
        j = 0; // reset counter every iteration
        temp_max = arr[i];
        while (j < k) {
            if (temp_max >= arr[i+j])
                j++;
            else {
                temp_max = arr[i+j];
            } 
        }
        result.push_back(temp_max);
        ++i;
    }
    
    for(; !result.empty(); result.pop_front())
        std::cout << result.front() << " ";
    std::cout << std::endl;
}

int main(){
  
	int t;
	cin >> t;
	while(t>0) {
		int n,k;
    	cin >> n >> k;
    	int i;
    	int arr[n];
    	for(i=0;i<n;i++)
      		cin >> arr[i];
    	printKMax(arr, n, k);
    	t--;
  	}
  	return 0;
}

