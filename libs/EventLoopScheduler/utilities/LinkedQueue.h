/*
 * Copyright 2015 Joshua Rodriguez.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _LINKED_QUEUE
#define _LINKED_QUEUE

#ifndef NULL
  #define NULL 0 // Pointers NULL
#endif

template<typename T> class LinkedQueue {
  private:
    struct Node {
      Node* _next;
      Node* _last;
      T _type;
      Node(T type) {
        _type = type;
      };
    };
    Node* _head;
    Node* _tail;
    int _size = 0;
  public:
    /** Grab the size of the queue */
    inline int size() const {
      return _size;
    };
    /** Push a node to the tail of the queue */
    inline void push(const T &type) {
      Node* node = new Node(type);
      if (_size == 0) {
        _head = node;
        _tail = node;
      } else {
        node->_last = _tail;
        _tail->_next = node;
        _tail = node;
      }
      _size++;
    };
    /** Get and remove the first element of the queue */
    inline T pop() {
      if (_size != 0) {
        _size--;
        T type = _head->_type;
        // One node left delete
        if (_head->_next == NULL) {
          delete _head;
        } else { // More nodes remove first node relink others
          Node* link = _head->_next;
          delete _head;
          _head = link;
        }
        return type;
      }
    };
    /** Get the node that is in front */
    inline T front() const {
      if (_head != NULL) {
        return _head->_type;
      }
    };
    /** Get the element that is in the back */
    inline T back() const {
      if (_tail != NULL) {
        return _tail->_type;
      }
    };
};

#endif
