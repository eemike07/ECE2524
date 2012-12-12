// Unix 2524: Process Object
// Michael Surratt

#include "Process.h"
#include <iostream>
#include <algorithm>

Process::Process(const std::vector<std::string> &args)
{
    if(pipe(readpipe) < 0)
		throw "Error: Reading Issue - pipe";	
	if(pipe(writepipe) < 0)
		throw "Error: Writing Issue - pipe";
	if(args.size() == 0)
		throw "Error: Invalid amount of Arguments";
	m_pid = fork();
	if(m_pid < 0){
		throw "Error: Fork issue";
	}
	else if(m_pid == 0){
		close(readpipe[1]);
		dup2(writepipe[0],0);
		if (dup2(writepipe[0], 0) < 0){
			close(writepipe[0]);	
			throw std::string("dup2 writepipe[0]");
		}
		close(writepipe[1]);
		close(readpipe[0]);
		dup2(readpipe[1],1);
		if (dup2(readpipe[1], 1) < 0){
			close(readpipe[1]);
			throw std::string("dup2 readpipe[1]");
		}
		close(writepipe[0]);
		std::vector<const char*> cargs;
		std::transform(args.begin(),args.end(), std::back_inserter(cargs), 
		[](std::string s) {
				return s.c_str();
			} );
		cargs.push_back( NULL );
		int erroCheck = execv(cargs[0], const_cast<char**>(&cargs[0]));
    		if(erroCheck < 0){
     			perror("Error: Running Generator");
      			exit(1);
		    }
	}
	else{
		std::cerr << "Process[" << pid()-1 <<"] Process constructor " << std::endl;
		close(writepipe[0]);
		close(readpipe[1]);
		m_pread = fdopen(readpipe[0], "r");
	}
}
Process::~Process()
{
	fclose(m_pread);
	close(writepipe[1]);
	close(readpipe[0]);
	kill(m_pid, SIGTERM);
	waitpid(m_pid, NULL, 0);
 }
void Process::write(const std::string& str)
{
	error = ::write(writepipe[1], str.c_str(), str.length());
	if(error < 0)
	    throw "Error: Write failure";
}
std::string Process::readline()
{
	char* buffer;
	size_t buffer_size, errorCheck;
	errorCheck = getline(&buffer, &buffer_size, m_pread);
	if(errorCheck < 0)
	    throw "Error: Readline Failure";
	return string(buffer);
}

