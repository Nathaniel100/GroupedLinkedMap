//
// Created by 吴凡 on 2018/7/19.
//

#ifndef GROUPEDLINKEDMAP_GROUPEDLINKEDMAP_H
#define GROUPEDLINKEDMAP_GROUPEDLINKEDMAP_H

// GroupedLinkedMap实现了LRU功能, 基于Glide库中GroupedLinkedMap实现
// 支持Get, Put, RemoteLast操作
// GroupedLinkedMap<Int, String> m;
// m.put(1, "a");
// m.put(2, "b");
// m.put(1, "c");
// auto s = m.get(1)
// auto s2 = m.get(2)

#include <vector>
#include <unordered_map>

namespace glm {

namespace internal {

// 循环链表节点
template<class K, class V>
class LinkedEntry {
 public:
  LinkedEntry() : prev(this), next(this) {}
  LinkedEntry(K key) : key(std::move(key)), prev(this), next(this) {}

  void Add(V item) {
    values_.push_back(std::move(item));
  }

  V RemoveLast() {
    V t = values_.back();
    values_.pop_back();
    return t;
  }

  size_t size() const {
    return values_.size();
  }

 public:
  K key;
  LinkedEntry<K, V> *prev; // 不需要释放
  LinkedEntry<K, V> *next; // 不需要释放
 private:
  std::vector<V> values_;
};

}

// GroupedLinkedMap包括循环链表以及哈希表, 哈希表的value为循环链表的节点
template<class K, class V>
class GroupedLinkedMap {
 private:
 public:
  using Entry = internal::LinkedEntry<K, V>;
 public:
  GroupedLinkedMap() : head_(new Entry) {}
  ~GroupedLinkedMap() {
    Entry *p = head_->next;
    while (p != head_) {
      Entry *remove = p;
      p = p->next;
      DeleteEntry(remove);
    }
    DeleteEntry(head_);
  }

  // TODO Copy & Move

  void Put(K key, V value) {
    Entry *entry = nullptr;
    if (entries_.count(key) == 0) {
      entry = new Entry(key);
      MakeTail(entry);
      entries_[key] = entry;
    } else {
      entry = entries_[key];
    }
    entry->Add(value);
  }

 private:
  void RemoveEntry(Entry *entry) {
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
  }

  void UpdateEntry(Entry *entry) {
    entry->prev->next = entry;
    entry->next->prev = entry;
  }

  void MakeTail(Entry *entry) {
    RemoveEntry(entry);
    entry->prev = head_->prev;
    entry->next = head_;
    UpdateEntry(entry);
  }

  void MakeHead(Entry *entry) {
    RemoveEntry(entry);
    entry->prev = head_;
    entry->next = head_->next;
    UpdateEntry(entry);
  }

  void DeleteEntry(Entry *entry) {
    entry->prev = nullptr;
    entry->next = nullptr;
    delete (entry);
  }

 private:
  Entry *head_;
  std::unordered_map<K, Entry *> entries_;

};

}

#endif //GROUPEDLINKEDMAP_GROUPEDLINKEDMAP_H
