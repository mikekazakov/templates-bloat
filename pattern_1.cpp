#include <iostream>
#include <string>

static std::string GeneratePrologue()
{
    std::string buf;
    buf += "#include <string>\n";
    buf += "#include <typeinfo>\n";
    buf += "#include <stdexcept>\n";
    buf += "using namespace std::literals;\n";
    return buf;
}

static std::string GenerateBase( int number_of_methods )
{
    std::string buf;
    buf +=
        "struct Base {\n"
        "    virtual ~Base() = default;\n";
    for ( int i = 0; i < number_of_methods; ++i ) {
        buf += "    virtual void M"
            + std::to_string(i)
            + "(int v) = 0;\n";
    }
    buf += "};\n";
    return buf;
}

static std::string GenerateImpl( int number_of_methods )
{
    std::string buf;
    buf += "template <typename T>\n"
            "struct Impl : Base {\n";
    for ( int i = 0; i < number_of_methods; ++i ) {
        buf += "    void M"
            + std::to_string(i)
            + "( int v ) override {\n";
        buf += "        if( v == 0 )\n";
        buf += "            throw std::logic_error\n";
        buf += "            ( \"you\'re so unlucky with M"
            + std::to_string(i)
            + "() for \\\'\"s +\n"
            + "             typeid(T).name() + \"\\\'!\" );\n";
        buf += "    }\n";
    }
    buf += "};\n";
    return buf;
}

static std::string GenerateInstantiations( int number_of_types )
{
    std::string buf;
    for ( int i = 0; i < number_of_types; ++i ) {
        buf += "struct T"
            + std::to_string(i)
            + "{};\n";
    }
    buf += "\n";
    for ( int i = 0; i < number_of_types; ++i ) {
        buf += "Base *Spawn_T"
        + std::to_string(i)
        + "() { return new Impl<T"
        + std::to_string(i)
        + ">; }\n";
    }
    return buf;
}

int main( int argc, const char **argv )
{
    if( argc < 3 )
        return -1;

    const auto number_of_methods = std::atoi(argv[1]);
    const auto number_of_instantiations = std::atoi(argv[2]);
    
    std::cout << GeneratePrologue() << std::endl;
    std::cout << GenerateBase(number_of_methods) << std::endl;
    std::cout << GenerateImpl(number_of_methods) << std::endl;
    std::cout << GenerateInstantiations(number_of_instantiations) << std::endl;
}
