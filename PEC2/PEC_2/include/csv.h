#ifndef __CSV_H__
#define __CSV_H__

#include <stdbool.h>
#define CSV_SEPARATOR_CHAR ;

// Store one entry from a CSV file
typedef struct _tCSVEntry {
    int numFields;
    char* type;
    char** fields;    
} tCSVEntry;

// Store the content of a CSV file
typedef struct _tCSVData {
    tCSVEntry *entries;
    int count;
    bool isValid;
} tCSVData;

// Initialize the tCSVData structure
void csv_init(tCSVData* data);

// Initialize the tCSVEntry structure
void csv_initEntry(tCSVEntry* entry);

// Parse the contents of a CSV file
void csv_parse(tCSVData* data, const char* input, const char* type);

// Print the content of the CSV data structure
void csv_print(tCSVData data);

// Print the content of the CSV entry structure
void csv_printEntry(tCSVEntry entry);

// Parse the contents of a CSV line   "f1;f2;f3" =>  field_0 = f1, field_1 = f2, field_2 = f3
void csv_parseEntry(tCSVEntry* entry, const char* input, const char* type);

// Get the number of entries
bool csv_isValid(tCSVData data);

// Remove all data from structure
void csv_free(tCSVData* data);

// Remove all data from structure
void csv_freeEntry(tCSVEntry* entry);

// Get the number of entries
int csv_numEntries(tCSVData data);

// Get the type of information contained in the entry
const char* csv_getType(tCSVEntry* entry);

// Get an entry from the CSV data
tCSVEntry* csv_getEntry(tCSVData data, int position);

// Get the number of fields for a given entry
int csv_numFields(tCSVEntry entry);

// Get a field from the given entry as integer
int csv_getAsInteger(tCSVEntry entry, int position);

// Get a field from the given entry as string. The value is copied to the provided buffer with provided maximum length
void csv_getAsString(tCSVEntry entry, int position, char* buffer, int length);

// Get a field from the given entry as integer
float csv_getAsReal(tCSVEntry entry, int position);

#endif
