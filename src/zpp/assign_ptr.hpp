#ifndef ZPP_ASSIGN_PTR_HPP
#define ZPP_ASSIGN_PTR_HPP

#include <type_traits>

#define ZPP_TARGET_COMPILER(X) ZPP_TARGET_COMPILER_PRIV_DEF_##X()
#if defined(__clang__)
#  define ZPP_TARGET_COMPILER_PRIV_DEF_CLANG() 1
#  define ZPP_TARGET_COMPILER_PRIV_DEF_GCC() 0
#  define ZPP_TARGET_COMPILER_PRIV_DEF_MSVC() 0
#  define ZPP_TARGET_COMPILER_PRIV_DEF_UNKNOWN() 0
#elif defined(__GNUC__)
#  define ZPP_TARGET_COMPILER_PRIV_DEF_CLANG() 0
#  define ZPP_TARGET_COMPILER_PRIV_DEF_GCC() 1
#  define ZPP_TARGET_COMPILER_PRIV_DEF_MSVC() 0
#  define ZPP_TARGET_COMPILER_PRIV_DEF_UNKNOWN() 0
#elif defined(_MSC_VER)
#  define ZPP_TARGET_COMPILER_PRIV_DEF_CLANG() 0
#  define ZPP_TARGET_COMPILER_PRIV_DEF_GCC() 0
#  define ZPP_TARGET_COMPILER_PRIV_DEF_MSVC() 1
#  define ZPP_TARGET_COMPILER_PRIV_DEF_UNKNOWN() 0
#else
#  define ZPP_TARGET_COMPILER_PRIV_DEF_CLANG() 0
#  define ZPP_TARGET_COMPILER_PRIV_DEF_GCC() 0
#  define ZPP_TARGET_COMPILER_PRIV_DEF_MSVC() 0
#  define ZPP_TARGET_COMPILER_PRIV_DEF_UNKNOWN() 1
#endif

#define ZPP_CXX03(X) ZPP_CXX03_PRIV_DEF_##X()
#define ZPP_CXX11(X) ZPP_CXX11_PRIV_DEF_##X()
#define ZPP_CXX14(X) ZPP_CXX14_PRIV_DEF_##X()
#define ZPP_CXX17(X) ZPP_CXX17_PRIV_DEF_##X()

#if ZPP_TARGET_COMPILER(CLANG)
#  if __clang_major__ >= 4 && __cplusplus >= 201703L
#    define ZPP_CXX17_PRIV_DEF_COPY_ELISION() 1
#  else
#    define ZPP_CXX17_PRIV_DEF_COPY_ELISION() 0
#  endif
#elif ZPP_TARGET_COMPILER(GCC)
#  if __GNUC__  >= 7 && __cplusplus >= 201703L
#    define ZPP_CXX17_PRIV_DEF_COPY_ELISION() 1
#  else
#    define ZPP_CXX17_PRIV_DEF_COPY_ELISION() 0
#  endif
#elif ZPP_TARGET_COMPILER(MSVC)
#  if _MSC_VER >= 1913 && _MSVC_LANG >= 201703L
#    define ZPP_CXX17_PRIV_DEF_COPY_ELISION() 1
#  else
#    define ZPP_CXX17_PRIV_DEF_COPY_ELISION() 0
#  endif
#elif ZPP_TARGET_COMPILER(UNKNOWN)
#  define ZPP_COPY_ELISION() 0
#endif

namespace zpp
{
    namespace detail
    {
        template <typename SmartPtr>
        class assign_ptr_impl
        {
        public:
            using element_type = typename std::remove_extent<typename SmartPtr::element_type>::type;

            // No default construction
            assign_ptr_impl() = delete;

            explicit assign_ptr_impl(SmartPtr& p) noexcept :
                pp_(&p)
            {
            }

            // No copy semantics
            assign_ptr_impl(assign_ptr_impl const&) = delete;
            assign_ptr_impl& operator=(assign_ptr_impl const&) = delete;

#if ZPP_CXX17(COPY_ELISION)
            // No move semantics - use copy elision
            assign_ptr_impl(assign_ptr_impl&&) = delete;
            assign_ptr_impl& operator=(assign_ptr_impl&&) = delete;
#else
            assign_ptr_impl(assign_ptr_impl&&) = default;
            assign_ptr_impl& operator=(assign_ptr_impl&&) = default;
#endif

            ~assign_ptr_impl()
            {
                pp_->reset(p_);
            }

            operator element_type*&() && noexcept
            {
                return p_;
            }

            operator element_type**() && noexcept
            {
                return &p_;
            }

            element_type*& by_ref() && noexcept
            {
                return p_;
            }

            element_type** by_ptr() && noexcept
            {
                return &p_;
            }

        private:
            SmartPtr* pp_ = nullptr;
            element_type* p_ = nullptr;
        };
    }

    template <typename SmartPtr>
    detail::assign_ptr_impl<SmartPtr> assign_ptr(SmartPtr& p)
    {
        return detail::assign_ptr_impl<SmartPtr>(p);
    }
}

#endif
