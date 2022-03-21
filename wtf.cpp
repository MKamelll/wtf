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

std::vector<std::string> run_command_list(std::vector<std::string>& cmds, std::string& bin)
{
    std::vector<std::string> result;
    for (auto& cmd : cmds)
    {
        std::string out = run_command(cmd + " " + bin);
        result.push_back(out);
    }

    return result;
}

std::string greenify(const std::string s)
{
    std::stringstream ss;
    ss << "\u001b[32m"  << s << "\u001b[0m";
    
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
        std::size_t startpos = found + description.size() + 1;
        out.seekg(startpos);

        std::string line;
        while (std::getline(out, line, '\n'))
        {
            if (line.empty())
                break;
            result += "\t";
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

void pretty_print(std::vector<std::string>& cmds, std::vector<std::string>& outputs, std::string& bin)
{
    std::stringstream prettified;
    for (int i = 0; i < cmds.size(); ++i)
    {
        prettified << greenify("* " + cmds[i]) << " => \n"
            << "\t" << outputs[i];
    }

    prettified << greenify("* Description") << " => \n" << get_description(bin);

    std::cout << prettified.str() << std::endl;
}

int main(int argc, char *argv[])
{  
    if (argc < 2)
    {
        print_usage();
        exit(1);
    }

    std::string bin(argv[1]);
    
    std::vector<std::string> cmds({"which", "whereis"});
    std::vector<std::string> outputs = run_command_list(cmds, bin);

    pretty_print(cmds, outputs, bin);

    return 0;
}
