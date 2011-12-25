#ifndef _POINTERS_H_
#define _POINTERS_H_

#include <cstdlib>

// A reference counted pointer to a type.
//
// Automatically disposes of the underlying object, if the
// reference count drops to zero.
//
// If you want to use multiple pointers to a single object
// and copy and assign them freely, use SafePtr
template <class T>
class RefPtr {
	public:
		// Creates the pointer and automatically grabs it
		RefPtr(T* ptr) {
			_ptr = ptr;
			_refcount = 0;
			grab();
		}

		// Disallow copying
		private: RefPtr(const RefPtr& other) {}
		private: RefPtr& operator=(const RefPtr& other) {}
	public:

		void grab() { _refcount++; }
		void drop() {
			_refcount--;
			if (_refcount == 0) {
				if (_ptr)
					delete _ptr;
				_ptr = NULL;
				delete this;
			}
		}

		T& get() { return *_ptr; }

	private:
		unsigned int _refcount;
		T* _ptr;
};

template <class T>
class SafePtr {
	public:
		SafePtr() { _ptr = new RefPtr<T>(NULL); }
		SafePtr(T* ptr) { _ptr = new RefPtr<T>(ptr); }
		SafePtr(const SafePtr<T>& other) {
			_ptr = other._ptr;
			_ptr->grab();
		}
		SafePtr<T>& operator=(const SafePtr<T>& other) {
			if (_ptr) _ptr->drop();

			_ptr = other._ptr;
			_ptr->grab();

			return *this;
		}
		~SafePtr() {
			_ptr->drop();
		}

		T& operator*() { return _ptr->get(); }
		T* operator->() { return &_ptr->get(); }
	private:
		RefPtr<T>* _ptr;
};

#endif

