#include <cstdint>
#include <cstddef>
#include <limits.h>

namespace maske {
    using Mask = std::uint64_t;

    template<std::size_t N>
    using Initials = std::size_t[N];

    namespace core {
        constexpr std::size_t BITS = sizeof(Mask) * CHAR_BIT;

        template <std::size_t N, bool InitialRun>
        struct Loop {
            template <typename F, std::size_t I>
            constexpr void operator()(F& f, Mask upper, Initials<I>& initials) {
                if constexpr (InitialRun)  {
                    Mask mask = (Mask(1) << initials[N - 1]++) | upper;
                    Loop<N - 1, true>()(f, mask, initials);
                }

                for (std::size_t i { initials[N - 1] }; i <= BITS - N; i++) {
                    Mask mask = (Mask(1) << i) | upper;
                    initials[N - 2] = i + 1;
                    Loop<N - 1, false>()(f, mask, initials);
                }

            }
        };

        template<bool InitialRun>
        struct Loop<1, InitialRun> {
            template <typename F, std::size_t I>
            constexpr void operator()(F& f, Mask upper, Initials<I>& initials) {
                for (std::size_t i { initials[0] }; i < BITS; i++) {
                    Mask mask = (Mask(1) << i) | upper;
                    f(mask);
                }
            }
        };
    };

    template<std::size_t N>
    struct Permutator {
        static_assert(0 < N && N <= core::BITS / 2);

        template <typename F, std::size_t I>
        constexpr void operator()(F& f, Initials<I>& initials) {
            core::Loop<N, true>()(f, 0, initials);
        }
    };
};
