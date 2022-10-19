#pragma once

#include <vector>
#include <string>

// Read/write utility to create network messages.
class NetworkMessageBuffer {
    public:
        explicit NetworkMessageBuffer() : buffer_{std::vector<uint8_t>{}} {}
        explicit NetworkMessageBuffer(std::vector<uint8_t> buffer) : buffer_{std::forward<std::vector<uint8_t>>(buffer)} {}

        [[nodiscard]]
        inline const auto& buffer() {
            return this->buffer_;
        }

        [[nodiscard]]
        inline size_t read_offset() const {
            return this->read_offset_;
        }

        inline size_t read_offset(size_t new_offset) const {
            return std::exchange(this->read_offset_, new_offset);
        }

        [[nodiscard]]
        inline bool has_read_error() const {
            return this->read_error_occurred;
        }

        [[nodiscard]]
        inline bool has_write_error() const {
            return this->read_error_occurred;
        }

        inline bool skip_bytes(size_t count) const {
            if(this->read_error_occurred) {
                return false;
            } else if(this->read_offset_ + count > this->buffer_.size()) {
                this->read_error_occurred = true;
                return false;
            }

            this->read_offset_ += count;
            return true;
        }

        [[nodiscard]]
        inline bool read_bytes(uint8_t* target, size_t length) const {
            if(this->read_error_occurred) {
                return false;
            } else if(this->read_offset_ + length > this->buffer_.size()) {
                this->read_error_occurred = true;
                return false;
            }

            std::memcpy(target, this->buffer_.data() + this->read_offset_, length);
            this->read_offset_ += length;
            return true;
        }

        [[nodiscard]]
        inline bool write_bytes(const uint8_t* bytes, size_t length) {
            if(this->write_error_occurred) {
                return false;
            }

            this->buffer_.resize(this->buffer_.size() + length);
            std::memcpy(this->buffer_.data() + this->buffer_.size() - length, bytes, length);
            return true;
        }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "NotImplementedFunctions"
        template<typename T>
        [[nodiscard]] inline bool read(T& target) const;

        template<typename T>
        [[nodiscard]] inline bool write(const T& target);
#pragma clang diagnostic pop

        template<typename T>
        [[nodiscard]] inline bool read(std::vector<T>& target) const {
            uint32_t length{};
            (void) this->read(length);
            for(size_t index{0}; index < length && !this->read_error_occurred; index++) {
                auto& value = target.emplace_back();
                (void) this->read(value);
            }

            return !this->read_error_occurred;
        }

        template<typename T, size_t N>
        [[nodiscard]] inline bool read(std::array<T, N>& target) const {
            for(size_t index{0}; index < N && !this->read_error_occurred; index++) {
                (void) this->read(target[index]);
            }

            return !this->read_error_occurred;
        }

        template<typename T, size_t N>
        [[nodiscard]] inline bool write(const std::array<T, N>& target) {
            for(size_t index{0}; index < N && !this->write_error_occurred; index++) {
                (void) this->write(target[index]);
            }

            return !this->write_error_occurred;
        }

        template<typename T>
        [[nodiscard]] inline bool write(const std::vector<T>& target) {
            (void) this->write(target.size());
            for(const auto& entry : target) {
                if(!this->write(entry)) {
                    return false;
                }
            }

            return !this->write_error_occurred;
        }
    private:
        std::vector<uint8_t> buffer_;

        mutable size_t read_offset_{0};
        mutable bool read_error_occurred{false};

        bool write_error_occurred{false};
};

#define DEFINE_READ_PRIMITIVE(primitive_type)                                       \
template<>                                                                          \
inline bool NetworkMessageBuffer::read(primitive_type& target) const {               \
    return this->read_bytes((uint8_t*) &target, sizeof(primitive_type));            \
}                                                                                   \
template<>                                                                          \
inline bool NetworkMessageBuffer::write(const primitive_type& target) {              \
    return this->write_bytes((const uint8_t*) &target, sizeof(primitive_type));     \
}

DEFINE_READ_PRIMITIVE(int8_t)
DEFINE_READ_PRIMITIVE(uint8_t)

DEFINE_READ_PRIMITIVE(int16_t)
DEFINE_READ_PRIMITIVE(uint16_t)

DEFINE_READ_PRIMITIVE(int32_t)
DEFINE_READ_PRIMITIVE(uint32_t)

DEFINE_READ_PRIMITIVE(int64_t)
DEFINE_READ_PRIMITIVE(uint64_t)

DEFINE_READ_PRIMITIVE(float)
DEFINE_READ_PRIMITIVE(double)
#undef DEFINE_READ_PRIMITIVE

template<>
[[nodiscard]] inline bool NetworkMessageBuffer::read(std::string& target) const {
    uint32_t length{};
    if(!this->read(length)) {
        return false;
    }
    if(length > 0x1000000) {
        this->read_error_occurred = true;
        return false;
    }

    target.resize(length);
    return this->read_bytes((uint8_t*) target.data(), length);
}


template<>
[[nodiscard]] inline bool NetworkMessageBuffer::write(const std::string& target) {
    if(target.length() > 0x1000000) {
        this->write_error_occurred = true;
        return false;
    }

    (void) this->write((uint32_t) target.length());
    (void) this->write_bytes((const uint8_t*) target.data(), target.length());
    return !this->write_error_occurred;
}