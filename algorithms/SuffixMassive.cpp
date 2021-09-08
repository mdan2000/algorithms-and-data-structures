#pragma once

#include <vector>
#include <algorithm>
#include <string>

struct SufMasNode {
	size_t l;
	size_t r;
	size_t i;
};

void RadixSortSufMas(std::vector<SufMasNode>& sm, std::vector<std::vector<SufMasNode>>& s) {
	s.resize(sm.back().r + 1);
	
	for (size_t i = 0; i < sm.size(); ++i) {
		s[sm[i].l].push_back(sm[i]);
	}
	
	size_t insert = 0;
	for (auto& i : s) {
		for (const auto& j : i) {
			sm[insert++] = j;
		}
		i.clear();
	}

}

template<typename T>
std::vector<size_t> MakeSuffixMassive(const T& t) {
	std::vector<SufMasNode> SufMas(t.size());
	std::vector<size_t> p(t.size());
	
	size_t n = t.size();
	
	for (size_t i = 0; i < SufMas.size(); ++i) {
		SufMas[i].i = i;
	}
	
	sort(SufMas.begin(), SufMas.end(), [t, n](const SufMasNode& lhs, const SufMasNode& rhs) {
		return t[(lhs.i + 1) % n] < t[(rhs.i + 1) % n];
		});

	SufMas[0].r = 1;
	for (size_t i = 1; i < SufMas.size(); ++i) {
		if (t[(SufMas[i].i + 1) % n] == t[(SufMas[i - 1].i + 1) % n]) {
			SufMas[i].r = SufMas[i - 1].r;
		}
		else {
			SufMas[i].r = SufMas[i - 1].r + 1;
		}
	}

	for (auto& node : SufMas) {
		p[(node.i + 1) % n] = node.r;
	}

	size_t window = 1;

	std::vector<std::vector<SufMasNode>> s(SufMas.back().r + 1);

	while (window < SufMas.size()) {
		for (size_t i = 0; i < SufMas.size(); ++i) {
			SufMas[i].l = p[SufMas[i].i];
		}

		RadixSortSufMas(SufMas, s);
		
		p[SufMas.front().i] = 1;
		for (size_t i = 1; i < n; ++i) {
			if (SufMas[i].l == SufMas[i - 1].l && SufMas[i].r == SufMas[i - 1].r) {
				p[SufMas[i].i] = p[SufMas[i - 1].i];
			}
			else {
				p[SufMas[i].i] = p[SufMas[i - 1].i] + 1;
			}
		}

		if (p[SufMas.back().i] == n) {
			break;
		}

		window *= 2;

		for (auto& node : SufMas) {
			node.r = p[node.i];
			node.i = ((2 * n + node.i) - window) % n;
		}
		
		
		
	}

	return p;
}
