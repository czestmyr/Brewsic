#ifndef _POINTERS_H_
#define _POINTERS_H_

#include <cstdlib>
#include <iostream>

#undef DUMP_SHIT

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
			#ifdef DUMP_SHIT
			std::cout << "RefPtr-ing " << _ptr << std::endl;
			#endif
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
				if (_ptr) {
					delete _ptr;
					#ifdef DUMP_SHIT
					std::cout << "Deleting " << _ptr << std::endl;
					#endif
				}
				_ptr = NULL;
				delete this;
			}
		}

		const T& get() const { return *_ptr; }
		T& get() { return *_ptr; }

                unsigned int getCount() const { return _refcount; }

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
			#ifdef DUMP_SHIT
			std::cout << "Initing SafePtr from SafePtr pointing at " << other.get() << std::endl;
			#endif
			_ptr = other._ptr;
			_ptr->grab();
		}
		SafePtr(RefPtr<T>* ref) {
			#ifdef DUMP_SHIT
			std::cout << "Initing SafePtr from RefPtr pointing at " << &ref->get() << std::endl;
			#endif
			_ptr = ref;
			_ptr->grab();
		}
		SafePtr<T>& operator=(const SafePtr<T>& other) {
			#ifdef DUMP_SHIT
			std::cout << "Copying SafePtr from other SafePtr pointing at " << other.get() << std::endl;
			#endif
			if (_ptr) _ptr->drop();

			_ptr = other._ptr;
			_ptr->grab();

			return *this;
		}
		~SafePtr() {
                        clear();
		}

		template <class U> SafePtr<U> cast() {
			RefPtr<U>* refPtr = (RefPtr<U>*)(_ptr);
			return SafePtr<U>(refPtr);
		}

		T& operator*() { return _ptr->get(); }
		T* operator->() { return &_ptr->get(); }
		operator bool() { return &_ptr->get(); }
		bool operator==(const SafePtr<T>& other) { return _ptr == other._ptr; }

                unsigned int getCount() { if (_ptr) return _ptr->getCount(); return 0; }

		T* get() const { return &_ptr->get(); }
                void clear() {
			#ifdef DUMP_SHIT
			std::cout << "Clearing SafePtr pointing at " << &_ptr->get() << std::endl;
			#endif
			if (_ptr) _ptr->drop();
                        _ptr = NULL;
                }
	private:
		RefPtr<T>* _ptr;
};

#endif

