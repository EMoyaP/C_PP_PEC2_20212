#include "csv.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

// Initialize the tCSVData structure
void csv_init(tCSVData* data) {
    data->count = 0;
    data->isValid = false;
    data->entries = NULL;
}

// Initialize the tCSVEntry structure
void csv_initEntry(tCSVEntry* entry) {
    entry->numFields = 0;    
    entry->fields = NULL;
    entry->type = NULL;
}

// Parse the contents of a CSV file
void csv_parse(tCSVData* data, const char* input, const char* type) {
    const char *pStart, *pEnd;
    char *line;
    int len;
    
    assert(data->count == 0);
    assert(data->entries == NULL);
    assert(!data->isValid);
    
    pStart = input;
    pEnd = strchr(pStart, '\n');    
    while(pEnd != NULL && pEnd != pStart) {
        len = pEnd - pStart + 1;
        line = (char*) malloc(len * sizeof(char));
        memset(line, 0, len * sizeof(char));
        strncpy(line, pStart, pEnd - pStart);
        data->count++;
        if (data->count == 1) {
            data->entries = (tCSVEntry*) malloc(sizeof(tCSVEntry));
        } else {
            data->entries = (tCSVEntry*) realloc(data->entries, data->count * sizeof(tCSVEntry));
        }
        csv_initEntry(&(data->entries[data->count-1]));
        csv_parseEntry(&(data->entries[data->count-1]), line, type);
        free(line);
        pStart = pEnd + 1;
        pEnd = strchr(pStart, '\n');
    }
    pEnd = strchr(pStart, '\0');
    if (pEnd != NULL && pEnd != pStart) {
        len = pEnd - pStart + 1;
        line = (char*) malloc(len * sizeof(char));
        memset(line, 0, len * sizeof(char));
        strncpy(line, pStart, pEnd - pStart);
        data->count++;
        if (data->count == 1) {
            data->entries = (tCSVEntry*) malloc(sizeof(tCSVEntry));
        } else {
            data->entries = (tCSVEntry*) realloc(data->entries, data->count * sizeof(tCSVEntry));
        }
        csv_initEntry(&(data->entries[data->count-1]));
        csv_parseEntry(&(data->entries[data->count-1]), line, type);
        free(line);
    }
    data->isValid = true;
}

// Print the content of the CSV data structure
void csv_print(tCSVData data) {
    int i;
    tCSVEntry* entry = NULL;
    
    for (i = 0; i < csv_numEntries(data); i++) {
        entry = csv_getEntry(data, i);
        printf("===============\n");
        printf("Entry %d: %s\n", i, entry->type);
        printf("===============\n");
        csv_printEntry(*entry);
        printf("===============\n");
    }    
}

// Print the content of the CSV entry structure
void csv_printEntry(tCSVEntry entry) {
    int i;
    char buffer[512];    
    printf("\tNum Fields: %d\n", csv_numFields(entry));
    for (i = 0; i < csv_numFields(entry); i++) {
        csv_getAsString(entry, i, buffer, 512);
        printf("\tField %d: %s\n", i, buffer);
    }
}

// Parse the contents of a CSV line
void csv_parseEntry(tCSVEntry* entry, const char* input, const char* type) {
    const char *pStart, *pEnd;    
    int len;
    bool readType = true;
    
    assert(entry->numFields == 0);
    assert(entry->fields == NULL);
    
    // If the type of the entry is not provided, use the first field
    if(type != NULL) {
        len = strlen(type) + 1;
        entry->type = (char*) malloc(len * sizeof(char));
        memset(entry->type, 0, len * sizeof(char));
        strncpy(entry->type, type, len);
        readType = false;
    }        
    pStart = input;
    pEnd = strchr(pStart, ';');    
    while(pEnd != NULL && pEnd != pStart) {        
        // Get the length of the field
        len = pEnd - pStart + 1;
        
        if(readType) {
            entry->type = (char*) malloc(len * sizeof(char));
            memset(entry->type, 0, len * sizeof(char));
            strncpy(entry->type, pStart, pEnd - pStart);
            readType = false;
        } else {
            entry->numFields++;
            if (entry->numFields == 1) {
                entry->fields = (char**) malloc(sizeof(char*));
            } else {
                entry->fields = (char**) realloc(entry->fields, entry->numFields * sizeof(char*));
            }                
            entry->fields[entry->numFields - 1] = (char*) malloc(len * sizeof(char));
            memset(entry->fields[entry->numFields - 1], 0, len * sizeof(char));
            strncpy(entry->fields[entry->numFields - 1], pStart, pEnd - pStart);
        }
        
        pStart = pEnd + 1;
        pEnd = strchr(pStart, ';');
    }
    pEnd = strchr(pStart, '\0');
    if (pEnd != NULL && pEnd != pStart) {
        
        assert(!readType);
        
        entry->numFields++;
        if (entry->numFields == 1) {
            entry->fields = (char**) malloc(sizeof(char*));
        } else {
            entry->fields = (char**) realloc(entry->fields, entry->numFields * sizeof(char*));
        }        
        len = pEnd - pStart + 1;
        entry->fields[entry->numFields - 1] = (char*) malloc(len * sizeof(char));
        memset(entry->fields[entry->numFields - 1], 0, len * sizeof(char));
        strncpy(entry->fields[entry->numFields - 1], pStart, pEnd - pStart);
    }
}

// Get the number of entries
bool csv_isValid(tCSVData data) {
    return data.isValid;
}

// Remove all data from structure
void csv_free(tCSVData* data) {
    int i;
    
    for (i = 0; i < data->count; i++) {
        csv_freeEntry(&(data->entries[i]));
    }
    free(data->entries);
    csv_init(data);
}

// Remove all data from structure
void csv_freeEntry(tCSVEntry* entry) {
    int i;
    
    if(entry->fields != NULL) {
        for(i = 0; i < entry->numFields; i++) {
            free(entry->fields[i]);
            entry->fields[i] = NULL;
        }
        
        free(entry->fields);
    }
    if(entry->type != NULL) {
        free(entry->type);
    }
    csv_initEntry(entry);
}

// Get the number of entries
int csv_numEntries(tCSVData data) {
    return data.count;
}

// Get the type of information contained in the entry
const char* csv_getType(tCSVEntry* entry) {
    return (const char*)entry->type;
}

// Get an entry from the CSV data
tCSVEntry* csv_getEntry(tCSVData data, int position) {
    return &(data.entries[position]);
}

// Get the number of fields for a given entry
int csv_numFields(tCSVEntry entry) {
    return entry.numFields;
}

// Get a field from the given entry as integer
int csv_getAsInteger(tCSVEntry entry, int position) {
    return atoi(entry.fields[position]);
}

// Get a field from the given entry as string
void csv_getAsString(tCSVEntry entry, int position, char* buffer, int length) {
    memset(buffer, 0, length);
    strncpy(buffer, entry.fields[position], length - 1);
}

// Get a field from the given entry as integer
float csv_getAsReal(tCSVEntry entry, int position) {
    return atof(entry.fields[position]);
}