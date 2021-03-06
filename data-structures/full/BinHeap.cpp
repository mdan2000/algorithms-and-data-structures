#pragma once
#include <memory>
#include <cassert>
#include <functional>
#include <iostream>
#include <algorithm>
#include <list>
#include <stdexcept>
#include <unordered_set>
#include <queue>

template<typename T>
class BinHeap {
private:
    struct BinHeapNode {
        T key;
        std::list<std::shared_ptr<BinHeapNode>> childs;
        size_t degree = 0;
    };

    std::shared_ptr<BinHeapNode> MergeEqualHeap(std::shared_ptr<BinHeapNode> l, std::shared_ptr<BinHeapNode> r) const {
        if (!cmp(l->key, r->key)) {
            swap(l, r);
        }
        l->childs.insert(l->childs.end(), r);
        l->degree++;
        return l;
    }

public:
    BinHeap() : size_(0) {};

    BinHeap(const std::function<bool(T, T)>& cmpFunc) :
        size_(0),
        cmp(cmpFunc) {};

    void Add(const T& newKey) {

        std::shared_ptr<BinHeapNode> node = std::make_shared<BinHeapNode>();
        node->key = newKey;
        auto it = Heap_.begin();
        while (it != Heap_.end() && (*it)->degree == node->degree) {
            if (cmp((*it)->key, node->key)) {
                (*it)->childs.insert((*it)->childs.end(), node);
                (*it)->degree++;
                node = *it;
            }
            else {
                node->childs.insert(node->childs.end(), *it);
                node->degree++;
            }
            if (it == MinNode_) {
                MinNode_ = Heap_.end();
            }
            it = Heap_.erase(it);
        }
        if (MinNode_ == Heap_.end() || cmp(node->key, (*MinNode_)->key)) {
            MinNode_ = Heap_.insert(it, node);
        }
        else {
            Heap_.insert(it, node);
        }
        ++size_;
    }

    void Merge(std::list<std::shared_ptr<BinHeapNode>>& r) {
        auto itl = Heap_.begin();
        auto itr = r.begin();
        typename std::list<std::shared_ptr<BinHeapNode>>::iterator node;
        while (itl != Heap_.end() && itr != r.end()) {
            if ((*itl)->degree == (*itr)->degree) {
                Heap_.insert(itl, MergeEqualHeap(*itl, *itr)); // ?????????????????? ???????????? itl
                itl = Heap_.erase(itl); // ?????????????????? ???? ????, ?????? ????????????????
                ++itr;
            }
            else if (itl != Heap_.begin() && (*itl)->degree == (*prev(itl))->degree) {
                node = prev(itl); // ???????? ?????????? ????????
                Heap_.insert(node, MergeEqualHeap(*node, *itl)); // ?????????????????? ???? itl
                Heap_.erase(node); // ?????????????? ???????????????????? ??????????????
                itl = Heap_.erase(itl); //
            }
            else if (itl != Heap_.begin() && (*itr)->degree == (*prev(itl))->degree) {
                node = prev(itl);
                Heap_.insert(node, MergeEqualHeap(*node, *itr));
                Heap_.erase(node);
                ++itr;
            }
            else {
                if (cmp((*itl)->key, (*itr)->key)) {
                    ++itl;
                }
                else {
                    Heap_.insert(itl, *itr);
                    ++itr;
                }
            }
        }

        Heap_.insert(Heap_.end(), itr, r.end());
        r.clear();
    }

    size_t size() const {
        return size_;
    }

    T Get() const {
        if (Heap_.empty()) {
            return {}; // throw???
        }
        return (*MinNode_)->key;
    }

    void Erase() {
        if (Heap_.empty()) {
            throw std::runtime_error("Erase from empty BinHeap");
        }
        std::shared_ptr<BinHeapNode> mn_ptr = *MinNode_;
        Heap_.erase(MinNode_);
        this->Merge(mn_ptr->childs);

        MinNode_ = Heap_.begin();
        for (auto it = Heap_.begin(); it != Heap_.end(); ++it) {
            if (cmp((*it)->key, (*MinNode_)->key)) {
                MinNode_ = it;
            }
        }
        size_--;
    }

    bool empty() const {
        return size_ == 0;
    }

private:
    size_t size_;
    std::list<std::shared_ptr<BinHeapNode>> Heap_;
    typename std::list<std::shared_ptr<BinHeapNode>>::iterator MinNode_ = Heap_.end();
    std::function<bool(T, T)> cmp = [](const T& l, const T& r) {
        return l < r;
    };
};


void TestAdd() {
    {
        BinHeap<size_t> heap;
        assert(heap.size() == 0);
    }
    {
        BinHeap<size_t> heap;
        for (size_t i = 0u; i < 10u; ++i) {
            heap.Add(i);
        }
        assert(heap.size() == 10);
    }
    {
        BinHeap<size_t> heap;
        heap.Add(1);
        heap.Add(2);
        assert(heap.size() == 2);
    }
    std::cout << "Add OK" << std::endl;
}

void TestGet() {
    {
        BinHeap<size_t> heap;
        for (size_t i = 0; i < 10; ++i) {
            heap.Add(i);
            assert(0 == heap.Get());
        }
    }
    {
        BinHeap<size_t> heap;
        for (size_t i = 10; i > 1; --i) {
            heap.Add(i - 1);
            assert(i - 1 == heap.Get());
        }
    }
    std::cout << "Get OK" << std::endl;
}

void TestErase() {
    {
        BinHeap<size_t> heap;
        for (size_t i = 0; i < 10; ++i) {
            heap.Add(i);
        }
        for (size_t i = 0; i < 9; ++i) {
            heap.Erase();
            assert(heap.Get() == i + 1);
        }
        std::cout << "Erase OK" << std::endl;
    }
}

void TestElseCMP() {
    {
        BinHeap<size_t> heap([](const size_t& l, const size_t& r) { return l > r; });

        for (size_t i = 0; i < 10; ++i) {
            heap.Add(i);
            assert(heap.Get() == i);
        }
    }

    std::cout << "TestElseCMP OK" << std::endl;
}

void TestMem() {
    {
        BinHeap<size_t> heap;
        for (size_t i = 0; i < 10000; ++i) {
            heap.Add(i);
        }
    }
    {
        BinHeap<size_t> heap;
        for (size_t i = 0; i < 100000; ++i) {
            heap.Add(i);
        }
    }
}

void TestBinHeap() {
    TestAdd();
    TestGet();
    TestErase();
    TestElseCMP();
    TestMem();
}
