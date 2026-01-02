#include "Types.h"

namespace Kodiak {

    template<typename Derived>
    class BaseColumn {
        public:
            const size_t size(){
                return static_cast<const Derived*>(this)->sizeImpl();
            }
    };

    template<typename T>
    class Column;

    template<Kodiak::Numerical T>
    class Column<T> : BaseColumn<Column<T>> {
        public:
        auto sum();
        auto filter();
        auto getColumn();

        private:
        std::unique_ptr<T[]> data;
    };

    template<Kodiak::Categorical T>
    class Column<T> : BaseColumn<Column<T>> {
        public:
        auto sum();
        auto filter();
        auto getColumn();

        private:
        std::unique_ptr<T[]> data;
    };

    template<>
    class Column<bool> : BaseColumn<Column<bool>> {
        public:

        private:

    };

};
