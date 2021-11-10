#include <iostream>

const unsigned long cBitsInByte=8; // implementation-defined. It is 8 for most platforms.
unsigned long ror(unsigned long value,unsigned char rotation)
{
return (value<<rotation) | (value>>(sizeof(value)*cBitsInByte - rotation));
}

int main() {
    int nums[] = {5, 4, 3, 8, 9, 1};
    for (int x:nums)
        std::cout << ror(nums[x],7) << "\n";
    
}