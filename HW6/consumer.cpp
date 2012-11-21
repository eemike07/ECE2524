// ECE 2524 Homework 6: Heard it on the Pipeline
// Michael Surratt

#include <iostream>

int main() {
    double num;
    double total=0;
    size_t lines=0;
    while(std::cin >> num) {
	total += num;
	++lines;
    }
    std::cout << total << "/" << lines << "=" << total/lines << std::endl;
}
