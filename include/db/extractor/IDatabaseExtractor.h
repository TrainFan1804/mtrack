#pragma once

#include <string>
#include <memory>

struct sqlite3;

class DumpExtractor;

class IDatabaseExtractor
{
public:
    virtual ~IDatabaseExtractor() {}
    /**
     * This virtual method need to be overwritten by child classes.
     * 
     * @attention When enter `dump_file` as argument the file should contain NO
     * file extension. The child class will append the correct file extension by
     * it's own.
     */
    virtual void exportDatabase(sqlite3 *db, const std::string &dump_file) = 0;
};

std::unique_ptr<IDatabaseExtractor> createExtractor(const std::string &type);
