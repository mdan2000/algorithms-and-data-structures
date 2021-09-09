#pragma once

#include <vector>
#include <algorithm>
#include <string>

struct SufMasNode {
	int l;
	int r;
	int i;
};

void RadixSortSufMas(std::vector<SufMasNode>& sm) {
	std::vector<int> pos(sm.back().r + 1);
	pos[0] = 0;
	for (const auto& node : sm) {
		pos[node.r]++;
	}

	for (size_t i = 1; i < pos.size(); ++i) {
		pos[i] += pos[i - 1];
	}

	std::vector<SufMasNode> res(sm.size());

	for (size_t i = 0; i < sm.size(); ++i) {
		res[pos[sm[i].l - 1]++] = sm[i];
	}

	sm = res;
}

template<typename T>
std::vector<int> MakeSuffixMassive(const T& t) {
	std::vector<SufMasNode> SufMas(t.size());
	std::vector<int> p(t.size());

	int n = t.size();

	for (size_t i = 0; i < SufMas.size(); ++i) {
		SufMas[i].i = i;
	}

	sort(SufMas.begin(), SufMas.end(), [&t, n](const SufMasNode& lhs, const SufMasNode& rhs) {
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
		if (node.i + 1 == n) {
			p[0] = node.r;
		}
		else {
			p[node.i + 1] = node.r;
		}
	}

	size_t window = 1;

	while (window < SufMas.size()) {
		for (size_t i = 0; i < SufMas.size(); ++i) {
			SufMas[i].l = p[SufMas[i].i];
		}

		RadixSortSufMas(SufMas);

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
			if (window > node.i) {
				node.i = (n + node.i) - window;
			}
			else {
				node.i = node.i - window;
			}
		}
	}

	return p;
}
