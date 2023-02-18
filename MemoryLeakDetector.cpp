#include <iostream>
#include <map>
#include <string>

std::map<void*, std::pair<std::string, int>> alokacije;
size_t ukupno_alocirano = 0, ukupno_dealocirano = 0;

void* operator new(size_t vel, const char* file, int linije) {
	void* pok = malloc(vel);
	alokacije[pok] = std::make_pair(file, linije);
	ukupno_alocirano += vel;
	std::cout << "Alocirano " << vel << " bajta na adresi " << pok << " u fajlu " << file << " na liniji " << linije << std::endl;
	return pok;
}

int main() {
	return 0;
}