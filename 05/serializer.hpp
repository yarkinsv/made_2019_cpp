#include <string>
#include <iostream>

enum class Error {
    NoError,
    CorruptedArchive
};

class Serializer {
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out) : out_(out) {}

    template <class T>
    Error save(T& object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT... args) {
        return process(args...);
    }

private:
    template <typename T, typename... Args>
    Error process(T&& val, Args&&... args) {
        Error error = process(val);
        if (error != Error::NoError) {
            return error;
        }
        return process(std::forward<Args>(args)...);
    }

    template <>
    Error process<uint64_t&>(uint64_t& val) {
        out_ << val << Separator;
        return Error::NoError;
    }

    template <>
    Error process<bool&>(bool& val) {
        out_ << (val ? "true" : "false") << Separator;
        return Error::NoError;
    }

    std::ostream& out_;
};

class Deserializer {
public:
    explicit Deserializer(std::istream& in) : in_(in) {}

    template <class T>
    Error load(T& object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args) {
        return process(args...);
    }

private:
    template <typename T, typename... Args>
    Error process(T&& val, Args&&... args) {
        Error error = process(val);
        if (error != Error::NoError) {
            return error;
        }
        return process(std::forward<Args>(args)...);
    }

    template <>
    Error process<uint64_t&>(uint64_t& val) {
        std::string text;
        in_ >> text;

        for (char c : text) {
            if (c < '0' || c > '9') {
                return Error::CorruptedArchive;
            }
        }

        val = std::stoull(text);
        return Error::NoError;
    }

    template <>
    Error process<bool&>(bool& val) {
        std::string text;
        in_ >> text;

        if (text == "true")
            val = true;
        else if (text == "false")
            val = false;
        else
            return Error::CorruptedArchive;

        return Error::NoError;
    }

    std::istream& in_;
};
