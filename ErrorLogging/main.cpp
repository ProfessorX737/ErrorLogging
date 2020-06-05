//#define NDEBUG
#include <iostream>
#include "logging.h"

using namespace std;

int main() {
	void* ptr = nullptr;
	DCHECK_NOT_NULL(ptr) << "ptr must not equal to null";
	int i;
	cin >> i;
	return 0;
}