/// \file
// Range v3 library
//
//  Copyright Eric Niebler 2013-2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//

#ifndef RANGES_V3_VIEW_MOVE_HPP
#define RANGES_V3_VIEW_MOVE_HPP

#include <utility>
#include <type_traits>
#include <range/v3/range_fwd.hpp>
#include <range/v3/size.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/view_adaptor.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/utility/functional.hpp>
#include <range/v3/utility/static_const.hpp>
#include <range/v3/view/view.hpp>
#include <range/v3/view/all.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \addtogroup group-views
        /// @{
        template<typename Rng>
        struct move_view
          : view_adaptor<move_view<Rng>, Rng>
        {
        private:
            friend range_access;
            struct adaptor : adaptor_base
            {
                using value_type = range_value_t<Rng>;
                range_rvalue_reference_t<Rng> get(range_iterator_t<Rng> it) const
                {
                    return iter_move(it);
                }
                range_rvalue_reference_t<Rng> indirect_move(range_iterator_t<Rng> it) const
                {
                    return iter_move(it);
                }
            };
            adaptor begin_adaptor() const
            {
                return {};
            }
            adaptor end_adaptor() const
            {
                return {};
            }
        public:
            move_view() = default;
            explicit move_view(Rng rng)
              : move_view::view_adaptor{std::move(rng)}
            {}
#ifdef RANGES_WORKAROUND_MSVC_SFINAE_CONSTEXPR
            CONCEPT_REQUIRES(SizedRange<Rng>::value)
#else
            CONCEPT_REQUIRES(SizedRange<Rng>())
#endif
            range_size_t<Rng> size() const
            {
                return ranges::size(this->base());
            }
        };

        namespace view
        {
            struct move_fn
            {
                template<typename Rng,
#ifdef RANGES_WORKAROUND_MSVC_SFINAE_CONSTEXPR
                    CONCEPT_REQUIRES_(InputRange<Rng>::value)>
#else
                    CONCEPT_REQUIRES_(InputRange<Rng>())>
#endif
                move_view<all_t<Rng>> operator()(Rng && rng) const
                {
                    return move_view<all_t<Rng>>{all(std::forward<Rng>(rng))};
                }
            #ifndef RANGES_DOXYGEN_INVOKED
                template<typename Rng,
#ifdef RANGES_WORKAROUND_MSVC_SFINAE_CONSTEXPR
                    CONCEPT_REQUIRES_(!InputRange<Rng>::value)>
#else
                    CONCEPT_REQUIRES_(!InputRange<Rng>())>
#endif
                void operator()(Rng &&) const
                {
                    CONCEPT_ASSERT_MSG(InputRange<Rng>(),
                        "The argument passed to view::move must be a model of the InputRange "
                        "concept.");
                }
            #endif
            };

            /// \relates move_fn
            /// \ingroup group-views
            namespace
            {
                constexpr auto&& move = static_const<view<move_fn>>::value;
            }
        }
        /// @}
    }
}

#endif
