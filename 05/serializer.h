#pragma once
#include <iostream>
#include <exception>

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    static constexpr char Separator = ' ';
    std::ostream& out_;

    template <class T, class... Args>
    Error process(T&& val, Args&&... args)
    {
        if(process(val) == Error::NoError)
            return process(std::forward<Args>(args)...);
        return Error::CorruptedArchive;
    }

    Error process()
    {
        return Error::NoError;
    }

    Error process(bool val)
    {
        if(val)
            out_ << "true" << Separator;
        else
            out_ << "false" << Separator;
        return Error::NoError;
    }

    Error process(uint64_t val)
    {
        out_ << val << Separator;
        return Error::NoError;
    }

public:
    explicit Serializer(std::ostream& out)
        : out_(out)
    {
    }

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(std::forward<ArgsT>(args)...);
    }
};

class Deserializer
{
    std::istream& in_;

    template <class T, class... Args>
    Error process(T&& val, Args&&... args)
    {
        if(process(val) == Error::NoError)
            return process(std::forward<Args>(args)...);
        return Error::CorruptedArchive;
    }

    Error process()
    {
        return Error::NoError;
    }

    Error process(bool& val)
    {
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

    Error process(uint64_t& val)
    {
        std::string text;
        in_ >> text;

        if(text[0] == '-')
            return Error::CorruptedArchive;

        try{
            val = std::stoull(text.c_str());
        }
        catch(std::exception&){
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

public:
    explicit Deserializer(std::istream& in)
        : in_(in)
    {
    }

    template <class T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(std::forward<ArgsT>(args)...);
    }
      
};