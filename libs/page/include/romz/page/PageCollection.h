#pragma once

#include <romz/container/IntrusiveList.h>
#include <romz/page/Page.h>

namespace romz {

/*
 * The PageCollection class
 */
template<std::size_t ID>
class PageCollection {
public:
    // Destructor
    ~PageCollection() {
        clear();
    }

    // Returns the list's id
    int id() const {
        return ID;
    }

    bool is_empty() const {
        return list.is_empty();
    }

    std::size_t size() const {
        return list.size();
    }

    // Returns the head
    Page *head() const {
        return list.head();
    }

    // Returns the tail
    Page *tail() const {
        return list.tail();
    }

    // Atomically applies the |visitor()| to each page
    template<typename Visitor>
    void for_each(Visitor &visitor) {
        for (Page *p = head(); p != nullptr; p = p->next(ID)) {
            if (!visitor(p))
                break;
        }
    }

    // Atomically applies the |visitor()| to each page; starts at the tail
    template<typename Visitor>
    void for_each_reverse(Visitor &visitor) {
        for (Page *p = tail(); p != nullptr; p = p->previous(ID)) {
            if (!visitor(p))
                break;
        }
    }

    // Same as |for_each()|, but removes the page if |visitor()| returns true
    template<typename Visitor>
    void extract(Visitor &visitor) {
        Page *page = head();
        while (page) {
            Page *next = page->next(ID);
            if (visitor(page)) {
                list.del(page);
            }
            page = next;
        }
    }

    // Clears the collection.
    void clear() {
        Page *page = head();
        while (page) {
            Page *next = page->next(ID);
            list.del(page);
            page = next;
        }

        assert(is_empty() == true);
    }

    // Returns a page from the collection; this is expensive!
    Page *get(uint64_t address) const {
        for (Page *p = head(); p != nullptr; p = p->next(ID)) {
            if (p->address() == address)
                return (p);
        }
        return nullptr;
    }

    // Removes a page from the collection. Returns true if the page was removed,
    // otherwise false (if the page was not in the list)
    bool del(Page *page) {
        if (has(page)) {
            list.del(page);
            return true;
        }
        return false;
    }

    // Adds a new page at the head of the list. Returns true if the page was
    // added, otherwise false (that's the case if the page is already part of
    // the list)
    bool put(Page *page) {
        if (!list.has(page)) {
            list.put(page);
            return true;
        }
        return false;
    }

    // Returns true if a page with the |address| is already stored.
    // This is expensive!
    bool has(uint64_t address) const {
        return get(address) != 0;
    }

    // Returns true if the |page| is already stored. This is much faster
    // than has(uint64_t address).
    bool has(Page *page) const {
        return list.has(page);
    }

private:
    // The linked list
    IntrusiveList<Page, ID> list;
};

}
