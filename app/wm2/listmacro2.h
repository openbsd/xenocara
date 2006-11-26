
#ifndef _LISTMACRO_H_
#define _LISTMACRO_H_

#include <assert.h>

// assumes efficient realloc()

#define declarePList(List, T) \
  typedef T *T##_pointer; \
  declareList(List, T##_pointer);

#define implementPList(List, T) \
  typedef T *T##_pointer; \
  implementList(List, T##_pointer);

#define declareList(List, T) \
class List { \
public: \
    List(); \
    ~List(); \
\
    long count() const { return m_count; } \
    T &item(long index) const { \
	assert(index >= 0 && index < m_count); \
	return m_items[index]; \
    } \
    T *array(long index, long count) { \
	return m_items + index; \
    } \
\
    void append(const T &); \
    void remove(long index); \
    void remove_all(); \
    void move_to_start(long index); \
\
private: \
    T *m_items; \
    long m_count; \
};

#define implementList(List, T) \
\
List::List() : m_count(0), m_items(0) { } \
\
List::~List() { remove_all(); } \
\
void List::append(const T &item) { \
    if (m_items) { \
        m_items = (T *)realloc(m_items, (m_count + 1) * sizeof(T)); \
    } else { \
	m_items = (T *)malloc(sizeof(T)); \
    } \
    assert(m_items); \
    m_items[m_count++] = (T)item; \
} \
\
void List::remove(long index) { \
    assert(index >= 0 && index < m_count); \
    memcpy(m_items+index, m_items+index+1, (m_count-index-1) * sizeof(T)); \
    m_items = (T *)realloc(m_items, (m_count - 1) * sizeof(T)); \
    --m_count; \
} \
\
void List::remove_all() { \
    while (m_count > 0) remove(0); \
} \
\
void List::move_to_start(long index) { \
    assert(index >= 0 && index < m_count); \
    T temp = item(index); \
    if (index > 0) memcpy(m_items+1, m_items, index * sizeof(T)); \
    m_items[0] = temp; \
}

#endif
