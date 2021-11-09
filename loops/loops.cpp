#include <cctype>
#include <iostream>
#include <cstring>
using namespace std;

int main() {
    char str[] = "Hello World! Oh what a wonderful morning.";
    char ch;

    cout << "The lowercase version of \"" << str << "\" is " << endl;

    for (int i = 0; i < strlen(str); i++) {

    // convert str[i] to lowercase
        ch = tolower(str[i]);

        cout << ch;
    }

    cout << "\n";

    int i = 1; 
    do {
        cout << i << " ";
        i++;
    }

    while (i <= 5);
  return 0;
}