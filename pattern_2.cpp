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

static std::string GenerateImplBase( int number_of_methods )
{
    std::string buf;
    buf += "struct ImplBase {\n";
    for ( int i = 0; i < number_of_methods; ++i ) {
        buf += "    static void ThrowLogicErrorAtM"
        + std::to_string(i)
        + "( const std::type_info& typeid_t );\n";
    }
    buf += "};\n";
    return buf;
}

static std::string GenerateImpl( int number_of_methods )
{
    std::string buf;
    buf += "template <typename T>\n"
            "struct Impl : Base, private ImplBase {\n";
    for ( int i = 0; i < number_of_methods; ++i ) {
        buf += "    void M"
            + std::to_string(i)
            + "( int v ) override {\n";
        buf += "        if( v == 0 )\n";
        buf += "            ThrowLogicErrorAtM"
            + std::to_string(i)
            + "( typeid(T) );\n";
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
    std::cout << GenerateImplBase(number_of_methods) << std::endl;
    std::cout << GenerateImpl(number_of_methods) << std::endl;
    std::cout << GenerateInstantiations(number_of_instantiations) << std::endl;
}
