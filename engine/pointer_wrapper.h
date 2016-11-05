// Created 2016-11-02 by LimeThaw

#ifndef PTRW_H_INCLUDED
#define PTRW_H_INCLUDED

/**
 *  Wrapper class around std::shared_ptr. Made for convenience while typing.
 *  Meant to be instantiated with specific type in the header of that class
 *  (see node.h).
 */

template<class T>
class pointer_wrapper : public shared_ptr<T> {
	public:

		/**
		 *  Constructor creating a new object of class T. It is allocated by std::make_shared<T>()
		 *  and this object acts as the shared_ptr to it.
		 */
		pointer_wrapper() : shared_ptr<T>(make_shared<T>()) {}

		/**
		 *  nullptr - constructor. Used to assign a nullptr to the object. Creates an empty
		 *  instance of shared_ptr<T>.
		 */
		pointer_wrapper(nullptr_t other) : shared_ptr<T>() {}
		/** Assignment operator version of the nullptr - constructor. */
		auto operator=(nullptr_t other) { shared_ptr<T>::reset(); }

		/**
		 *  Constructor used to create a pointer_wrapper as/from a shared_pointer<T>.
		 *  This hopefully makes it easy to use shared_ptr and pointer_wrapper together.
		 */
		template<class S>
		pointer_wrapper(const shared_ptr<S> &other) : shared_ptr<T>(other) {}
		/** Same as the other, just without constant modifyer. */
		template<class S>
		pointer_wrapper(shared_ptr<S> &other) : shared_ptr<T>(other) {}

		/** Copy constructor to create a pointer_wrapper from another instance. */
		template<class S>
		pointer_wrapper(const pointer_wrapper<S> &other) : shared_ptr<T>(other) {}
		/** Same as the other, just without constant modifyer. */
		template<class S>
		pointer_wrapper(pointer_wrapper<S> &other) : shared_ptr<T>(other) {}

		/**
		 *  Constructor meant for direct object initialization. If the first argument is not
		 *  of class pointer_wrapper<T> or std::nullptr_t all arguments are forwarded to
		 *  std::make_shared<T> to create the new object pointed to by this.
		 */
		template<class... Args>
		explicit pointer_wrapper(Args&... args) : shared_ptr<T>() {
				*this = make_shared<T>(args...);
		}

		bool operator==(const nullptr_t &other) { return std::operator==(static_cast<shared_ptr<T>>(*this), other); }
		bool operator!=(const nullptr_t &other) { return std::operator!=(static_cast<shared_ptr<T>>(*this), other); }
};

#endif //PTRW_H_INCLUDED