#include <iostream>
#include <string>

using namespace std;
// we define our byte as having 8 bits, but we compile for 32 bit
// this gives us some interesting results in IDA later
// a char is 1 byte in this case for reference

const unsigned long cBitsInByte=8;
unsigned long ror(unsigned long value,unsigned char rotation)
{
return (value<<rotation) | (value>>(sizeof(value)*cBitsInByte - rotation));
}
string names[] = {"sdf", "wef", "dave", "steve", "sam", "mark", "giles", "joe", "keiran", "tom", "chaz", "laura", "c", "fakename", "ford", "also jack", "jack", "paulo", "sasha", "tim", "jeremy"};
int main() {
    int nums[] = {5, 4, 3, 8, 9, 1};

    for (int x:nums)
        for (char c:names[nums[x]])
            cout << ror(c, 2) << "\n";
    
}