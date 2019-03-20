#ifndef ZPP_ASSIGN_PTR_HPP
#define ZPP_ASSIGN_PTR_HPP

#include <type_traits>

namespace zpp
{
    namespace detail
    {
        template <typename SmartPtr>
        class assign_ptr_impl
        {
        public:
            using element_type = std::remove_extent_t<typename SmartPtr::element_type>;

            // No default construction
            assign_ptr_impl() = delete;

            explicit assign_ptr_impl(SmartPtr& p) noexcept :
                pp_(&p)
            {
            }

            // No copy semantics
            assign_ptr_impl(assign_ptr_impl const&) = delete;
            assign_ptr_impl& operator=(assign_ptr_impl const&) = delete;

            // NOTE
            // Ideally, no move semantics, but for now...
            //
            // TODO  C++17
            // When guaranteed copy elision is implemented from P0135R1,
            // delete the move constructor
            assign_ptr_impl(assign_ptr_impl&&) = default;
            assign_ptr_impl& operator=(assign_ptr_impl&&) = delete;

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
