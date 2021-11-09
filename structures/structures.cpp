#include <iostream>

int main() {
    struct animal {
        char name[32];
        char species[32];
    };

    animal pet1 = {"whiskers", "very large feline"};
    animal pet2 = {"tiny", "cat"};

    std::cout << "Pet no. 1's name is " << pet1.name << "\n";
    std::cout << "Your pets are named: \n";

    animal pets[] = {{"whiskers", "cat"}, {"bruce", "dog"}, {"echo", "velociraptor"}};

    for (animal pet : pets)
        std::cout<<"Pet name: " << pet.name << "\n";
    return 0;
}