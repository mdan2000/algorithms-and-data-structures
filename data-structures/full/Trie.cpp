#pragma once
#include <vector>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <cassert>

template<typename T>
class Trie {
private:
	struct Node {
		std::vector<std::shared_ptr<Node>> next;
		bool isTerminated = false;
	};

public:
	Trie(const T& newMaxEl) :
		maxEl(newMaxEl) {
		blockSize = getId(maxEl) + 1;
		start = std::make_shared<Node>();
	}

	Trie(const T& newMaxEl, std::function<size_t(T)> newGetId) :
		maxEl(newMaxEl),
		getId(newGetId) {
		blockSize = getId(maxEl) + 1;
		start = std::make_shared();
	}

	void Insert(const std::vector<T>& s) {
		std::shared_ptr now = start;
		for (const auto& c : s) {
			if (now->next.size() == 0) {
				now->next.resize(blockSize);
			}
			if (!(now->next[getId(c)])) {
				now->next[getId(c)] = std::make_shared<Node> ();
			}
			now = now->next[getId(c)];
		}
		now->isTerminated = true;
	}

	bool InTrie(const std::vector<T>& s) {
		std::shared_ptr now = start;
		for (const auto& c : s) {
			if (now->next.size() == 0 || !(now->next[getId(c)])) {
				return false;
			}
			now = now->next[getId(c)];
		}
		return now->isTerminated;
	}

private:
	std::shared_ptr<Node> start;
	T maxEl;
	size_t blockSize;
	std::function<size_t(T)> getId = [](const T& c) {
		if (c >= 'A') {
			return 26 + c - 'A';
		}
		else {
			return c - 'a';
		}
	};
};



void TestTrie() {
	{
		Trie<char> t('z');
		t.Insert(std::vector<char>({ 'a', 'b', 'b', 'a' }));
		t.Insert(std::vector<char>({ 'a', 'b', 'b', 'b' }));
		assert(t.InTrie(std::vector<char>({ 'a', 'b', 'b', 'a' })));
		assert(!t.InTrie(std::vector<char>({ 'a', 'b', 'b', 'a', 'a' })));
		assert(!t.InTrie(std::vector<char>({ 'z' })));
		assert(!t.InTrie(std::vector<char>({ 'a', 'b' })));
		assert(!t.InTrie(std::vector<char>({ 'a' })));
		t.Insert(std::vector<char>({ 'a' }));
		assert(!t.InTrie(std::vector<char>({ 'a', 'b' })));
		assert(t.InTrie(std::vector<char>({ 'a' })));
		t.Insert(std::vector<char>({ 'a', 'b' }));
		assert(t.InTrie(std::vector<char>({ 'a', 'b', 'b', 'a' })));
		assert(!t.InTrie(std::vector<char>({ 'a', 'b', 'b', 'a', 'a' })));
		assert(!t.InTrie(std::vector<char>({ 'z' })));
		assert(t.InTrie(std::vector<char>({ 'a', 'b' })));
		assert(t.InTrie(std::vector<char>({ 'a' })));
		t.Insert(std::vector<char>({ 'z' }));
		assert(t.InTrie(std::vector<char>({ 'a', 'b', 'b', 'a' })));
		assert(!t.InTrie(std::vector<char>({ 'a', 'b', 'b', 'a', 'a' })));
		assert(t.InTrie(std::vector<char>({ 'z' })));
		assert(t.InTrie(std::vector<char>({ 'a', 'b' })));
		assert(t.InTrie(std::vector<char>({ 'a' })));
	}
}
