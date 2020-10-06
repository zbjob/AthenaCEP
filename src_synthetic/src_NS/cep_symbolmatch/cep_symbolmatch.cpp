#include "../Query.h"
#include "../PatternMatcher.h"
#include "../EventStream.h"
#include "BatchWorker.h"

#include "../freegetopt/getopt.h"

#include <thread>
#include <vector>
#include <memory>
#include <assert.h>

using namespace std;

int main(int _argc, char* _argv[])
{
	const char* deffile = "default.eql";
	const char* queryName = 0;

	int c;
	while ((c = getopt(_argc, _argv, "c:q:")) != -1)
	{
		switch (c)
		{
		case 'c':
			deffile = optarg;
			break;
		case 'q':
			queryName = optarg;
			break;
		default:
			abort();
		}
	}

	StreamEvent::setupStdIo();

	QueryLoader def;
	if (!def.loadFile(deffile))
	{
		fprintf(stderr, "failed to load definition file %s\n", deffile);
		return false;
	}

	const Query* query = !queryName ? def.query((size_t)0) : def.query(queryName);
	if (!query)
	{
		fprintf(stderr, "query not found");
		return false;
	}

	unsigned int numThreads = std::thread::hardware_concurrency() + 1;

	BatchWorker* worker = new BatchWorker[numThreads];
	for (unsigned int i = 0; i < numThreads; ++i)
	{
		QueryLoader::Callbacks cb;
		
		unique_ptr<PatternMatcher> matcher(new PatternMatcher);
		def.setupPatternMatcher(query, *matcher, cb);

		worker[i].setPatternMatcher(move(matcher));
		worker[i].start();
	}

	delete[] worker;
	return 0;
}
