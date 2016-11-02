// Created 2016-11-02 by LimeThaw

#ifndef PTRW_H_INCLUDED
#define PTRW_H_INCLUDED

namespace intern {

/**
 *  Wrapper class around std::shared_ptr. Made for convenience while typing.
 *  Meant to be instantiated with specific type in the header of that class
 *  (see node.h).
 */

template<class T>
class pointer_wrapper { //TODO: Fix documentation
	public:
		/**
		 *  Constructor which creates a shared pointer to an object of class
		 *  T and calls the constructor with the given arguments. The created shared
		 *  pointer is represented by the instance of pointer_wrapper.
		 */
		template<class... Args>
		void make(Args&&... args) { ptr = std::make_shared<T>(args...); }

		pointer_wrapper(std::shared_ptr<T> other) { ptr = other; }
		pointer_wrapper(std::nullptr_t other) {}
		pointer_wrapper() { make(); }

		const shared_ptr<T> &get() const { return ptr; } // IDEA: make get return T* to avoid .get().get()
		shared_ptr<T> operator->() const { return ptr; }
		bool operator==(nullptr_t other) const { return other==ptr; }
		bool operator!=(nullptr_t other) const { return other!=ptr; }
		template<class S>
		bool operator==(pointer_wrapper<S> other) const { return other.get()==ptr; }
		template<class S>
		bool operator!=(pointer_wrapper<S> other) const { return other.get()!=ptr; }
	private:
		shared_ptr<T> ptr;
};

}

#endif //PTRW_H_INCLUDED