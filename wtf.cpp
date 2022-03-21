#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>
#include<array>
#include<memory>
#include<sstream>

std::string run_command(const std::string& cmd)
{
    std::array<char, 256> buff;
    std::string result;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);

    if (!pipe)
    {
        std::cout << "Couldn't run command: " << cmd << std::endl;
        exit(1);
    }

    while (fgets(buff.data(), buff.size(), pipe.get()) != nullptr)
    {
        result += buff.data();
    }

    return result;
}

std::vector<std::string> output_lines(const std::string& out)
{
    std::vector<std::string> result;
    
    std::stringstream ss(out);
    std::string line;

    while (std::getline(ss, line, '\n'))
    {
        result.push_back(line);
    }

    return result;

}

std::string echo_green(const std::string s)
{
    std::stringstream ss;
    ss << "echo " << "\u001b[32m" << "\"" << s << "\"" << "\u001b[0m";
    
    return ss.str();
}

std::string triml(const std::string& s)
{
    std::string result;
    size_t i = 0;
    for (auto& c : s) {
        if (c == ' ') {
            i += 1;
            continue;
        }
        break;
    }

    result = s.substr(i);
    return result;
}

std::string get_description(std::string& bin)
{
    std::string result;
    std::string man("man ");
    man += bin;
    
    std::stringstream out(run_command(man));
    std::string description = "DESCRIPTION";
    
    std::size_t found = out.str().find(description);
    if (found != std::string::npos)
    {
        std::size_t startpos = found + description.length() + 1;
        out.seekg(startpos);

        std::string line;
        while (std::getline(out, line, '\n'))
        {
            if (line.empty())
                break;
            result += triml(line);
            result += "\n";
        }
    }

    return result;

}

void print_usage()
{
    std::cout << "USAGE: wtf <bin name>" << std::endl;
}

int main(int argc, char *argv[])
{  
    if (argc < 2)
    {
        print_usage();
        exit(1);
    }

    //TODO: Add --help info
    //FIXME: errors with which and error appear wrong if
    // nothing with the name in path 
    std::string bin(argv[1]);
    
    std::string which("which ");
    std::string whereis("whereis ");
    std::string man("man ");

    std::stringstream cmd;
    cmd << echo_green(" - Which =>\n") << ";" << which << bin << ";" << echo_green("\n - Whereis =>\n") << ";" << whereis << bin << ";" << echo_green("\n - Description =>");

    std::cout << run_command(cmd.str()) << std::endl;
    
    std::cout << get_description(bin) << std::endl;
    
    return 0;
}
