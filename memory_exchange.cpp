
#include <string>   /* string */
#include <iostream> /* cout */
#include <fstream>  /* ifstream */
#include <regex>    /* regular expr */

std::string get_file_contents(const char *filename);
void set_file_contents(std::string &content, const char *filename);
void memory_exchange(std::string &content);

int main(int arg, char *args[])
    {
    
    std::string content;
    
    content = get_file_contents(args[1]);
    memory_access(content);
    set_file_contents(content, "tmp2.pal");
    
    return 0;
    }

std::string get_file_contents(const char *filename)
	{   
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
		{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
		}
	throw(errno);
};

void set_file_contents(std::string &content, const char *filename)
    {
    std::ofstream outf;
    outf.open(filename);

    outf << content.c_str();
    outf.close();
    }

void memory_exchange(std::string &content)
    {
    std::regex e (
        "ADD(.*)(\\$[1-9])(.*)(\n)(.*)ADDI(.*)([1-9])(\n)(.*)EXCH(.*)((\\$)[1-9])(.*)(\n)(.*)ADDI(.*)(\n)(.*)SUB(.*)");

    content = std::regex_replace(
        content,
        e,
        "EXCHI $10 $2 $7"
    );
    }