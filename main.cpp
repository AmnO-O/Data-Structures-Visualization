#include "UI.h"


int main() {
	try {
		Visualizer product; 
		product.Process();
	} catch (std :: exception& e) {
		std :: cout << "\nEXCEPTION: " << e.what() << '\n';
	}
}