#include <vector>
#include <stdexcept>


template<typename T>
std::vector<int> prefix_function(const T& t) {
	if (t.size() == 0) {
		throw std::runtime_error("Container for prefix function is empty");
	}
	std::vector<int> pf(t.size() + 1);
	pf[0] = -1;
	for (size_t i = 1; i < pf.size(); ++i) {
		int p = pf[i - 1];
		while (p >= 0 && t[p] != t[i - 1]) {
			p = pf[p];
		}
		pf[i] = p + 1;
	}
	return pf;
}
