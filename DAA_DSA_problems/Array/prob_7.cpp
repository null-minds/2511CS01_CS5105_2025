// Name: Samyaka
// Roll No.: 2511AI46

#include<bits/stdc++.h>
using namespace std;

// Time Complexity: O(n)
// Space Complexity: O(1)
int BruteForce(vector<int> arr, int target) {
    for(int i=0;i<arr.size();i++)
    {
        if(arr[i]==target)
        {
            return i;
        }
    }
    return -1;
}

// Time Complexity: O(n)
// Space Complexity: O(1)
int Optimal(vector<int> arr, int target) {
    for(int i=0;i<arr.size();i++)
    {
        if(arr[i]==target)
        {
            return i;
        }
    }
    return -1;
}

int main() {
    int n;
    cout << "Enter the size of the array: ";
    cin >> n;

    vector<int> data(n);
    cout << "Enter the elements: ";
    for (int i = 0; i < n; i++) {
        cin >> data[i];
    }
    int target;

    cout << "Enter the target: ";
    cin>>target;
    cout<<Optimal(data,target);

    return 0;
}
