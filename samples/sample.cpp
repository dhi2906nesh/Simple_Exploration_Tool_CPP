// #include <iostream>
// using namespace std;

// int main() {
//     int n;

//     cout << "Enter a positive number: ";
//     cin >> n;

//     if (n <= 0) {
//         cout << "Please enter a number greater than 0." << endl;
//         return 0;
//     }

//     cout << "Analyzing numbers from 1 to " << n << "..." << endl;

//     for (int i = 1; i <= n; i++) {
//         // Conditional statements
//         if (i % 7 == 0) {
//             cout << i << " is a multiple of 7. Stopping early!" << endl;
//             break;
//         }
//         else if (i % 2 == 0) {
//             cout << i << " is Even." << endl;
//         }
//         else {
//             cout << i << " is Odd." << endl;
//         }
//     }

//     cout << "Program ended." << endl;

//     return 0;
// }
#include <iostream>
using namespace std;

// Function to check if a number is prime
bool isPrime(int num) {
    if (num <= 1) return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0)
            return false;
    }
    return true;
}

int main() {
    int n;
    cout << "Enter a positive number: ";
    cin >> n;

    if (n <= 0) {
        cout << "Please enter a number greater than 0." << endl;
        return 0;
    }

    cout << "\n=== Multiplication Table up to " << n << " ===\n";

    // Outer loop for rows
    for (int i = 1; i <= n; i++) {
        // Inner loop for columns
        for (int j = 1; j <= n; j++) {
            cout << i * j << "\t";
        }
        cout << endl;
    }

    cout << "\n=== Number Classification ===\n";

    int evenCount = 0, oddCount = 0, primeCount = 0;

    // Using a while loop to iterate through numbers
    int i = 1;
    while (i <= n) {
        if (i % 2 == 0) {
            cout << i << " is Even." << endl;
            evenCount++;
        } else {
            cout << i << " is Odd." << endl;
            oddCount++;
        }

        if (isPrime(i)) {
            cout << " --> " << i << " is also a Prime number!" << endl;
            primeCount++;
        }

        i++;
    }

    cout << "\n=== Summary ===" << endl;
    cout << "Even numbers: " << evenCount << endl;
    cout << "Odd numbers: " << oddCount << endl;
    cout << "Prime numbers: " << primeCount << endl;

    cout << "\n=== Countdown using do-while ===" << endl;

    int k = n;
    do {
        if (k % 5 == 0)
            cout << k << " is divisible by 5." << endl;
        else if (k % 3 == 0)
            cout << k << " is divisible by 3." << endl;
        else
            cout << k << " has no special property." << endl;
        k--;
    } while (k > 0);

    cout << "\nProgram finished successfully." << endl;

    return 0;
}
