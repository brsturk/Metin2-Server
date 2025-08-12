#ifndef INC_METIN_II_COMMON_NONCOPYABLE_TEMPLATE
#define INC_METIN_II_COMMON_NONCOPYABLE_TEMPLATE

class noncopyable
{
	protected:
		noncopyable() {}
		~noncopyable() {}

	private:
		noncopyable(const noncopyable &);
		noncopyable& operator = (const noncopyable &);
};

#endif
//martysama0134's cc449580f8a0ea79d66107125c7ee3d3
